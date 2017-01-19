#ifndef ARKHORSCRIPTPARSER_H
#define ARKHORSCRIPTPARSER_H

#include "arkhorscriptlexer.h"

class QIODevice;
class QTextStream;
#include <QList>
#include <QStringList>
#include <QSharedPointer>

namespace AHS {

class ArkhorScriptParser
{
public:
    ArkhorScriptParser(QIODevice *input);
    ~ArkhorScriptParser();

    bool parse();
    QString error() const { return m_error; }

    struct ClassDef;

    enum AttributeType {
        Function,
        Literal,
        Complex,
        Primitive,
        EnumValue,
        IDRef,
        NestedObject,
        ArrayValues,
        None
    };

    typedef QPair<QString, QSharedPointer<ClassDef> > AttributeValue;
    typedef QPair<AttributeType, AttributeValue> AttributeArrayElem;
    typedef QList<AttributeArrayElem> AttributeValueArray;

    struct AttrDef {
        typedef AttributeType Type;
        QString name;
        Type type;

        AttributeValue content;
        AttributeValueArray array;
        AttrDef() : type(None) {}
        AttrDef(QString n, Type t, AttributeValue c, AttributeValueArray a = AttributeValueArray()) : name(n), type(t), content(c), array(a) {}
        AttrDef(QString n, Type t, QString c, AttributeValueArray a = AttributeValueArray()) : name(n), type(t), content(qMakePair(c, static_cast<ClassDef*>(NULL))), array(a) {}

        static AttrDef forArrayElement(const AttributeArrayElem &e) {
            return AttrDef("", e.first, e.second);
        }
    };

    struct ClassDef {
        QString elemType;
        QString elemName;
        int elemMult;
        bool hasElemMult;
        bool isAnonymous;
        bool isNested;
        ClassDef() : elemMult(1), hasElemMult(false), isAnonymous(false), isNested(false) {}
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

    bool NestedElement(AttributeValue &elem);

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
    ClassDef *m_curClass;
    QList<ClassDef> m_allClasses;
};
}

#endif // ARKHORSCRIPTPARSER_H
