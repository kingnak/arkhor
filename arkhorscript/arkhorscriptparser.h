#ifndef ARKHORSCRIPTPARSER_H
#define ARKHORSCRIPTPARSER_H

#include "arkhorscriptlexer.h"

class QIODevice;
class QTextStream;
#include <QList>
#include <QStringList>

namespace AHS {

class ArkhorScriptParser
{
public:
    ArkhorScriptParser(QIODevice *input);
    ~ArkhorScriptParser();

    bool parse();
    QString error() const { return m_error; }

    struct AttrDef {
        QString name;
        enum Type {
            Function,
            Literal,
            Complex,
            Primitive,
            EnumValue,
            IDRef,
            Array,
            None
        } type;
        QString content;
        QStringList array;
        AttrDef() : type(None) {}
        AttrDef(QString n, Type t, QString c, QStringList a = QStringList()) : name(n), type(t), content(c), array(a) {}
    };

    struct ClassDef {
        QString elemType;
        QString elemName;
        int elemMult;
        bool hasElemMult;
        ClassDef() : elemMult(1), hasElemMult(false) {}
        QList<AttrDef> attrs;
    };

    QList<ClassDef> getDefinedClasses() const {
        return m_allClasses;
    }

private:
    bool AHS();
    bool ElementDefinition();
    bool ElementClass();
    bool ElementID();
    bool ElementMultiplicity();
    bool ElementBlock();

    bool ElementAttributes();
    bool ElementAttribute();

    bool ComplexAttribute(QString &value);
    bool IDRefOrEnumValue(AttrDef &a);
    bool Array(AttrDef &a);
    bool ArrayContent(AttrDef &a);
    bool String(QString &value);

    bool setError(QString err);

    typedef ArkhorScriptLexer::Symbol Symbol;

    bool consumeToken(Symbol::Type type);

private:
    ArkhorScriptLexer *m_lexer;
    QString m_error;
    ClassDef m_curClass;
    QList<ClassDef> m_allClasses;
};
}

#endif // ARKHORSCRIPTPARSER_H
