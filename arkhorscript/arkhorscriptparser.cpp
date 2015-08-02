#include "arkhorscriptparser.h"
#include "arkhorscriptlexer.h"
#include <QTextStream>

namespace AHS {

ArkhorScriptParser::ArkhorScriptParser(QIODevice *input)
{
    m_lexer = new ArkhorScriptLexer(input);
}

ArkhorScriptParser::~ArkhorScriptParser()
{
    delete m_lexer;
}

bool ArkhorScriptParser::parse()
{
    m_lexer->nextSymbol();
    return AHS();
}

bool ArkhorScriptParser::AHS()
{
    while (m_lexer->currentSymbol().type != ArkhorScriptLexer::Symbol::EndSym) {
        if (!ElementDefinition()) {
            return false;
        }
    }

    return true;
}

bool ArkhorScriptParser::ElementDefinition()
{
    m_curClass = ClassDef();
    if (!ElementClass()) {
        return setError("Expected Class Type");
    }
    if (!ElementID()) {
        return setError("Expected Class Name");
    }
    if (!ElementMultiplicity()) {
        return setError("Invalid Multiplicity");
    }
    if (!ElementBlock()) {
        return setError("Expected Class Definition");
    }
    m_allClasses << m_curClass;
    return true;
}

bool ArkhorScriptParser::ElementClass()
{
    if (m_lexer->currentSymbol().type == Symbol::Identifier) {
        m_curClass.elemType = m_lexer->currentSymbol().image;
        consumeToken(Symbol::Identifier);
        return true;
    }
    return false;
}

bool ArkhorScriptParser::ElementID()
{
    if (m_lexer->currentSymbol().type == Symbol::Identifier) {
        m_curClass.elemName = m_lexer->currentSymbol().image;
        consumeToken(Symbol::Identifier);
        return true;
    }
    return false;
}

bool ArkhorScriptParser::ElementMultiplicity()
{
    if (m_lexer->currentSymbol().type == Symbol::Colon) {
        consumeToken(Symbol::Colon);
        Symbol s = m_lexer->currentSymbol();
        if (s.type == Symbol::Number) {
            m_curClass.elemMult = s.image.toInt();
            consumeToken(Symbol::Number);
            return true;
        }
        return setError("Expected number");
    }
    return true;
}

bool ArkhorScriptParser::ElementBlock()
{
    if (!consumeToken(Symbol::BraceOpen)) {
        return false;
    }
    ElementAttributes();
    if (!consumeToken(Symbol::BraceClose)) {
        return false;
    }
    return true;
}

bool ArkhorScriptParser::ElementAttributes()
{
    while (m_lexer->currentSymbol().type != Symbol::BraceClose) {
        if (!ElementAttribute()) {
            return false;
        }
    }
    return true;
}

bool ArkhorScriptParser::ElementAttribute()
{
    AttrDef a;
    if (m_lexer->currentSymbol().type == Symbol::Identifier) {
        a.name = m_lexer->currentSymbol().image;
        consumeToken(Symbol::Identifier);
        if (!consumeToken(Symbol::Colon)) {
            return setError("Expected ':'");
        }

        switch (m_lexer->currentSymbol().type) {
        case Symbol::String:
            a.type = AttrDef::Primitive;
            if (!String(a.content))
                return false;
            break;
        case Symbol::Number:
        case Symbol::True:
        case Symbol::False:
            a.type = AttrDef::Primitive;
            a.content = m_lexer->currentSymbol().image;
            consumeToken(m_lexer->currentSymbol().type);
            break;
        case Symbol::Literal:
            a.type = AttrDef::Literal;
            a.content = m_lexer->currentSymbol().image;
            consumeToken(m_lexer->currentSymbol().type);
            break;
        case Symbol::Identifier:
            if (!IDRefOrEnumValue(a)) {
                return false;
            }
            break;
        case Symbol::Function:
            a.type = AttrDef::Function;
            a.content = m_lexer->currentSymbol().image;
            consumeToken(m_lexer->currentSymbol().type);
            break;
        case Symbol::BracketOpen:
            if (!ComplexAttribute(a.content)) {
                return setError("Error in complex attribute");
            }
            a.type = AttrDef::Complex;
            break;
        case Symbol::Minus:
            a.content = "-";
        case Symbol::Plus:
            consumeToken(m_lexer->currentSymbol().type);
            a.content += m_lexer->currentSymbol().image;
            a.type = AttrDef::Primitive;
            consumeToken(Symbol::Number);
            break;
        case Symbol::ParenOpen:
            Array(a);
            a.type = AttrDef::Array;
            break;
        default:
            return setError("Expected Attribute Value");
        }
    }
    m_curClass.attrs << a;
    return consumeToken(Symbol::EndExpr);
}

bool ArkhorScriptParser::ComplexAttribute(QString &value)
{
    if (!consumeToken(Symbol::BracketOpen)) {
        return setError("Expected [");
    }

    value = "";
    while (m_lexer->currentSymbol().type != Symbol::BracketClose && !m_lexer->error()) {
        value += m_lexer->currentSymbol().image;
        if (m_lexer->currentSymbol().type == Symbol::Identifier) {
            value += ' ';
        }
        consumeToken(m_lexer->currentSymbol().type);
    }

    if (!consumeToken(Symbol::BracketClose)) {
        return setError("Expected ]");
    }
    return true;
}

bool ArkhorScriptParser::IDRefOrEnumValue(AttrDef &a)
{
    if (m_lexer->currentSymbol().type != Symbol::Identifier) {
        return setError("Expected Identifier");
    }
    a.content = m_lexer->currentSymbol().image;
    a.type = AttrDef::EnumValue;
    consumeToken(Symbol::Identifier);
    if (m_lexer->currentSymbol().type == Symbol::Dot) {
        consumeToken(Symbol::Dot);
        if (m_lexer->currentSymbol().type == Symbol::Identifier) {
            a.type = AttrDef::IDRef;
            a.content += QString(".%1").arg(m_lexer->currentSymbol().image);
            consumeToken(Symbol::Identifier);
        } else {
            return setError("Expected Identifier");
        }
    }
    return true;
}

bool ArkhorScriptParser::Array(ArkhorScriptParser::AttrDef &a)
{
    if (!consumeToken(Symbol::ParenOpen)) {
        return setError("Expected (");
    }

    if (!ArrayContent(a)) return false;

    if (!consumeToken(Symbol::ParenClose)) {
        return setError("Expected )");
    }
    return true;
}

bool ArkhorScriptParser::ArrayContent(ArkhorScriptParser::AttrDef &a)
{
    do {
        AttrDef tmp;
        if (!IDRefOrEnumValue(tmp)) return false;
        a.array << tmp.content;
    } while (m_lexer->currentSymbol().type == Symbol::Comma && consumeToken(Symbol::Comma));

    return true;
}

bool ArkhorScriptParser::String(QString &value)
{
    value = "";
    forever {
        if (m_lexer->currentSymbol().type != Symbol::String) {
            return setError("Expected String");
        }
        value += m_lexer->currentSymbol().image;
        consumeToken(Symbol::String);
        if (m_lexer->currentSymbol().type == Symbol::Plus) {
            value += "+\n\t\t";
            consumeToken(Symbol::Plus);
        } else {
            return true;
        }
    }
}

bool ArkhorScriptParser::setError(QString err)
{
    m_error += QString("%3 @ %1,%2").arg(m_lexer->currentSymbol().line).arg(m_lexer->currentSymbol().pos).arg(err);
    if (m_lexer->currentSymbol().type == Symbol::Error) {
        m_error += QString(" (Cause: %1)").arg(m_lexer->currentSymbol().image);
    }
    m_error += "\n";
    return false;
}

bool ArkhorScriptParser::consumeToken(ArkhorScriptLexer::Symbol::Type type)
{
    if (m_lexer->currentSymbol().type != type) {
        return setError(QString("Unexpected Symbol '%1'").arg(m_lexer->currentSymbol().image));
    }
    m_lexer->nextSymbol();
    return true;
}



}
