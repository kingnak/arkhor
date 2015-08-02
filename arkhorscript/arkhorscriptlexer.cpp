#include "arkhorscriptlexer.h"
#include <QIODevice>
#include <QTextStream>

namespace AHS {


ArkhorScriptLexer::ArkhorScriptLexer(QIODevice *input)
:   m_input(input),
    m_symbol(makeSymbol(Symbol::NoSymbol, -1, -1, "\0")),
    m_lineCount(1),
    m_linePos(0),
    m_hasPeek(false)
{
    if (m_input) {
        m_stream = new QTextStream(m_input);
    } else {
        m_stream = NULL;
    }
}

ArkhorScriptLexer::~ArkhorScriptLexer()
{
    delete m_stream;
}

const ArkhorScriptLexer::Symbol ArkhorScriptLexer::nextSymbol()
{
    parse();
    return currentSymbol();
}

const ArkhorScriptLexer::Symbol ArkhorScriptLexer::currentSymbol() const
{
    return m_symbol;
}

bool ArkhorScriptLexer::parse()
{
    QString img;
    if (!m_stream || m_stream->atEnd()) {
        if (m_symbol.type == Symbol::EndSym) {
            m_symbol = makeSymbol(Symbol::Error, m_lineCount, m_linePos, "Read past EOF");
            return false;
        }
        m_symbol = makeSymbol(Symbol::EndSym, m_lineCount, m_linePos, "\0");
        return true;
    }
    while (!m_stream->atEnd()) {
        int sLine = m_lineCount;
        int sPos = m_linePos;

        QChar c = parseChar();
        if (c.isSpace()) {
            continue;
        }

        switch (c.unicode()) {
        case '}':
            m_symbol = makeSymbol(Symbol::BraceClose, sLine, sPos, c);
            return true;
        case '[':
            m_symbol = makeSymbol(Symbol::BracketOpen, sLine, sPos, c);
            return true;
        case ']':
            m_symbol = makeSymbol(Symbol::BracketClose, sLine, sPos, c);
            return true;
            /*
        case '<':
            m_symbol = makeSymbol(Symbol::AngleOpen, sLine, sPos, c);
            return true;
        case '>':
            m_symbol = makeSymbol(Symbol::AngleClose, sLine, sPos, c);
            return true;
            */
        case '(':
            m_symbol = makeSymbol(Symbol::ParenOpen, sLine, sPos, c);
            return true;
        case ')':
            m_symbol = makeSymbol(Symbol::ParenClose, sLine, sPos, c);
            return true;
            /*
        case '=':
            m_symbol = makeSymbol(Symbol::Equals, sLine, sPos, c);
            return true;
            */
        case '.':
            m_symbol = makeSymbol(Symbol::Dot, sLine, sPos, c);
            return true;
        case ':':
            m_symbol = makeSymbol(Symbol::Colon, sLine, sPos, c);
            return true;
        case '+':
            m_symbol = makeSymbol(Symbol::Plus, sLine, sPos, c);
            return true;
        case '-':
            m_symbol = makeSymbol(Symbol::Minus, sLine, sPos, c);
            return true;
        case ';':
            m_symbol = makeSymbol(Symbol::EndExpr, sLine, sPos, c);
            return true;
        case '"':
            return parseString('"');
        case '\'':
            return parseString('\'');
        case '<':
            c = parseChar();
            pushBack(c);
            if (c == '<')
                return parseBlock('<', '>', Symbol::Literal);
            m_symbol = makeSymbol(Symbol::Operator, sLine, sPos, "<");
            /*
        case '[':
            return parseBlock('[', ']', Symbol::Complex);
            */
        case '{':
            c = parseChar();
            pushBack(c);
            if (c == '{')
                return parseBlock('{', '}', Symbol::Function);
            m_symbol = makeSymbol(Symbol::BraceOpen, sLine, sPos, "{");
            return true;
        case '/':
            c = parseChar();
            pushBack(c);
            if (c == '/' || c == '*') {
                if (!parseComment())
                    return false;
            } else {
                m_symbol = makeSymbol(Symbol::Operator, sLine, sPos, "/");
                return true;
            }
            break;
        case ',':
            m_symbol = makeSymbol(Symbol::Comma, sLine, sPos, c);
            return true;
        case '*':
        case '&':
        case '|':
        case '^':
        case '=':
        case '>':
            m_symbol = makeSymbol(Symbol::Operator, sLine, sPos, c);
            return true;

        default:
            if (c >= '0' && c <= '9') {
                pushBack(c);
                return parseNumber();
            }

            pushBack(c);
            if (!parseIdentifier(img)) {
                return false;
            }
            if (img.isEmpty()) {
                m_symbol = makeSymbol(Symbol::Error, m_lineCount, m_linePos, "Unexpected character");
                return false;
            }
            if (img == "true") {
                m_symbol = makeSymbol(Symbol::True, m_lineCount, m_linePos, "true");
                return true;
            }
            if (img == "false") {
                m_symbol = makeSymbol(Symbol::False, m_lineCount, m_linePos, "true");
                return true;
            }
            m_symbol = makeSymbol(Symbol::Identifier, m_lineCount, m_linePos, img);
            return true;
        }
    }

    m_symbol = makeSymbol(Symbol::EndSym, m_lineCount, m_linePos, "\0");
    return true;
}

bool ArkhorScriptLexer::parseComment()
{
    QChar c = parseChar();
    if (c == '/') {
        //parseToEOL;
        while (!m_stream->atEnd()) {
            c = parseChar();
            if (c == '\n') return true;
        }
        return true;
    } else if (c == '*') {
        //parseTo*/
        QChar l;
        while (!m_stream->atEnd()) {
            l = c;
            c = parseChar();
            if (c == '/' && l == '*') {
                return true;
            }
        }
        m_symbol = makeSymbol(Symbol::Error, m_lineCount, m_linePos, "EOF in comment");
        return false;
    } else {
        m_symbol = makeSymbol(Symbol::Error, m_lineCount, m_linePos, "Expected / or * for comment");
        return false;
    }
}

bool ArkhorScriptLexer::parseString(QChar match)
{
    QString out;
    out += match;
    QChar c = '\0';
    bool hasEscape = false;
    while (c != match) {
        if (m_stream->atEnd()) {
            m_symbol = makeSymbol(Symbol::Error, m_lineCount, m_linePos, "EOF in String");
            return false;
        }
        c = parseChar();
        if (c == '\n') {
            m_symbol = makeSymbol(Symbol::Error, m_lineCount, m_linePos, "Newline in String");
            return false;
        }
        out += c;
        if (c == '\\') {
            hasEscape = !hasEscape;
            continue;
        }
        if (c == match) {
            if (hasEscape)
                c = '\0'; // Escaped => trick to not break
        }
        hasEscape = false;


        /*
        if (c == '\\') {
            if (hasEscape) out += "\\\\";
            hasEscape = !hasEscape;
            continue;
        }
        if (c == match) {
            if (hasEscape) {
                out += '\\'+c;
                c = '\0';
            }
        }
        out += c;
        hasEscape = false;
        */
    }
    m_symbol = makeSymbol(Symbol::String, m_lineCount, m_linePos, out);
    return true;
}

bool ArkhorScriptLexer::parseBlock(QChar delimStart, QChar delimEnd, Symbol::Type type)
{
    QChar c1, c2, c3;
    c1 = delimStart;
    c2 = parseChar();
    c3 = parseChar();


    // Need "<<<"
    if (c1 != delimStart || c2 != delimStart || c3 != delimStart) {
        m_symbol = makeSymbol(Symbol::Error, m_lineCount, m_linePos, QString("Expected %1%1%1").arg(delimStart));
        return false;
    }

    const int sl = m_lineCount, sp = m_linePos;
    QString delimEndStr(3, delimEnd);
    QString img;
    while (!m_stream->atEnd()) {
        QChar c = parseChar();
        img += c;
        if (img.endsWith(delimEndStr)) {
            img.chop(3);
            m_symbol = makeSymbol(type, m_lineCount, m_linePos, img);
            return true;
        }
    }
    m_symbol = makeSymbol(Symbol::Error, sl, sp, "EOF in Literal Block");
    return false;
}

bool ArkhorScriptLexer::parseNumber()
{
    QString nr;
    while (!m_stream->atEnd()) {
        QChar c = parseChar();
        if (c.isDigit()) {
            nr += c;
        } else {
            pushBack(c);
            m_symbol = makeSymbol(Symbol::Number, m_lineCount, m_linePos, nr);
            return true;
        }
    }
    return true;
}

bool ArkhorScriptLexer::parseIdentifier(QString &out)
{
    out = "";
    while (!m_stream->atEnd()) {
        QChar c = parseChar();
        if (c.isLetterOrNumber() || c == '_') {
            out += c;
        } else {
            pushBack(c);
            return true;
        }
    }
    return true;
}

QChar ArkhorScriptLexer::parseChar()
{
    if (m_hasPeek) {
        m_hasPeek = false;
        return m_peekChar;
    }
    if (!m_stream || m_stream->atEnd()) {
        return '\0';
    }
    QChar c;
    *m_stream >> c;
    m_linePos++;
    if (c == '\n') {
        m_lineCount++;
        m_linePos = 0;
    }
    return c;
}

void ArkhorScriptLexer::pushBack(QChar c)
{
    Q_ASSERT(!m_hasPeek);
    m_hasPeek = true;
    m_peekChar = c;
}

}
