#ifndef CLASSGENERATOR_H
#define CLASSGENERATOR_H

class QTextStream;

#include "arkhorscriptparser.h"

namespace AHS {
class ClassGenerator
{
public:
    typedef ArkhorScriptParser::ClassDef ClassDef;
    typedef ArkhorScriptParser::AttrDef AttrDef;

    ClassGenerator(QTextStream &out);
    virtual ~ClassGenerator() {}
    virtual bool generate(const ClassDef &cls) = 0;
    QString error() const { return m_err; }
    QString warning() const { return m_warn; }

protected:
    struct AttributeDesc {
        QString name;
        enum ReqType {
            R_INVALID, R_Required, R_Optional, R_Default, R_Predefined
        } reqType;
        enum HandleType {
            H_Simple, H_Name, H_Special, H_TID, H_ID, H_IDRef, H_Enum
        } handleType;
        enum ValueType {
            V_Primitive =     0x0001,
            V_Function =      0x0002,
            V_Complex =       0x0004,
            V_Array =         0x0008
        };
        Q_DECLARE_FLAGS(ValueTypes, ValueType)
        ValueTypes valTypes;
        AttributeDesc(QString n, ReqType r, HandleType h, int v)
            : name(n), reqType(r), handleType(h), valTypes(ValueTypes(v)) {}
        AttributeDesc() : reqType(R_INVALID) {}
    };

    bool setError(QString err);
    bool setError(QString err, const ClassDef &cls);
    bool setWarning(QString warn);
    bool setWarning(QString warn, const ClassDef &cls);

    virtual QList<AttributeDesc> getAttributes() = 0;

    QString idPrefixForClass(QString classType);
    QString generateName(QString clsName);

    virtual bool outputSpecialAttribute(AttributeDesc desc, const ClassDef &cls, const AttrDef &attr) = 0;
    virtual bool outputDefaultAttribute(AttributeDesc desc, const ClassDef &cls);

    void outputClassComment(const ClassDef &cls);
    void outputCreateObjectStart(const ClassDef &cls);
    bool outputAttributes(const ClassDef &cls);
    bool outputAttribute(const ClassDef &cls, const AttrDef &attr, bool inPredefined);
    void outputCreateObjectEnd(const ClassDef &cls);
    void outputRegisterObject(const ClassDef &cls);

    bool outputModifications(QString mod);
    bool outputIDRef(const AttrDef &attr, const ClassDef &cls);
    bool outputIDRefArray(const AttrDef &attr, const ClassDef &cls);
    bool outputEnumValue(QString prefix, const AttrDef &attr, const ClassDef &cls);
    bool outputEnumValueArray(QString prefix, const AttrDef &attr, const ClassDef &cls);

    bool outputFunction(const AttrDef &attr, const ClassGenerator::ClassDef &cls, QString params = QString::null);

private:
    bool doOutputIDRef(QString ref);

protected:
    QTextStream &m_out;

private:
    QString m_err;
    QString m_warn;
};
}

#endif // CLASSGENERATOR_H
