#ifndef ARKHORSCRIPTLEXER_H
#define ARKHORSCRIPTLEXER_H

class QIODevice;
class QTextStream;
#include <QString>

namespace AHS {

class ArkhorScriptLexer
{
public:
    struct Symbol {
        enum Type {
            NoSymbol,
            BraceOpen, BraceClose,
            BracketOpen, BracketClose,
            //AngleOpen, AngleClose,
            ParenOpen, ParenClose,
            /*Equals,*/ Colon, Dot, Plus, Minus, Operator,
            Comma,
            EndExpr,
            Number, String, True, False,
            Identifier,
            Literal, Complex, Function,
            EndSym,
            Error
        };
        Type type;
        QString image;
        int line, pos;
    };

    ArkhorScriptLexer(QIODevice *input);
    ~ArkhorScriptLexer();

    const Symbol nextSymbol();
    const Symbol currentSymbol() const;
    bool error() const { return m_symbol.type == Symbol::Error; }

private:
    bool parse();
    bool parseComment();
    bool parseString( QChar match);
    bool parseBlock(QChar delimStart, QChar delimEnd, Symbol::Type type);
    bool parseNumber();
    bool parseIdentifier(QString &out);

    QChar parseChar();
    void pushBack(QChar c);

private:
    QIODevice *m_input;
    QTextStream *m_stream;
    Symbol m_symbol;
    int m_lineCount;
    int m_linePos;

    bool m_hasPeek;
    QChar m_peekChar;

    static inline Symbol makeSymbol(Symbol::Type type, int line, int pos, QString img);
};


ArkhorScriptLexer::Symbol ArkhorScriptLexer::makeSymbol(Symbol::Type type, int line, int pos, QString img)
{
    Symbol s;
    s.type = type;
    s.line = line;
    s.pos = pos;
    s.image = img;
    return s;
}

}

#endif // ARKHORSCRIPTLEXER_H
