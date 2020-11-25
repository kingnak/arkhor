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
    typedef ArkhorScriptParser::AttributeValue AttributeValue;
    typedef ArkhorScriptParser::AttributeArrayElem AttributeArrayElem;

    ClassGenerator(QTextStream &out);
    virtual ~ClassGenerator() = default;
    virtual bool generate(const ClassDef &cls) = 0;
    QString error() const { return m_err; }
    QString warning() const { return m_warn; }

    bool fixClass(ClassDef &cls);
    static QString idPrefixForClass(const QString &classType);
    static QString constantScopeForClass(const QString &classType);

    static const int INFINITE_MULT = -1;

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
        AttributeDesc() : reqType(R_INVALID), handleType(H_Simple) {}
    };

    virtual bool allowInfinite(const ClassDef &cls) const { Q_UNUSED(cls); return false; }
    virtual bool allowAnonymous(const ClassDef &cls) const { return cls.isNested; }
    virtual bool allowNested(const ClassDef &cls) const { Q_UNUSED(cls); return false; }

    bool setError(const QString &err);
    bool setError(const QString &err, const ClassDef &cls);
    bool setWarning(const QString &warn);
    bool setWarning(const QString &warn, const ClassDef &cls);

    virtual QList<AttributeDesc> getAttributes() = 0;
    virtual bool outputSpecialAttribute(const AttributeDesc &desc, const ClassDef &cls, const AttrDef &attr) = 0;
    virtual bool outputDefaultAttribute(const AttributeDesc &desc, const ClassDef &cls);

    QString generateName(const QString &clsName);

    void outputClassComment(const ClassDef &cls);
    void outputCreateStart(const QString &type, const ClassDef &cls);
    bool outputAttributes(const ClassDef &cls);
    bool outputAttribute(const ClassDef &cls, const AttrDef &attr, bool inPredefined);
    void outputCreateEnd(const ClassDef &cls);
    void outputRegisterMulti(const QString &type, const ClassDef &cls);
    void outputRegisterSingle(const QString &type, const ClassDef &cls);
    void outputRegisterConstant(const ClassDef &cls);

    bool outputPrimitive(const AttrDef &attr, const ClassDef &cls);
    bool outputIDRef(const AttrDef &attr, const ClassDef &cls);
    bool outputIDRefArray(const AttrDef &attr, const ClassDef &cls);
    bool outputEnumValue(const QString &prefix, const AttrDef &attr, const ClassDef &cls);
    bool outputEnumValueArray(const QString &prefix, const AttrDef &attr, const ClassDef &cls);

    bool outputModifications(const AttrDef &attr, const ClassDef &cls);
    bool outputMonsterModifications(const AttrDef &attr, const ClassDef &cls);
    bool outputMonsterMoveModifications(const AttrDef &attr, const ClassDef &cls);
    bool outputCosts(const AttrDef &attr, const ClassDef &cls);

    bool outputFunction(const AttrDef &attr, const ClassGenerator::ClassDef &cls, const QString &params = QString::null);

    QString getJSVariableName(const ClassDef &cls);

private:
    bool doOutputIDRef(const AttributeValue &ref);
    bool doOutputModifications(const QString &mod);
    bool doOutputCosts(const QString &costs);
    bool doOutputMonsterModifications(QString v, const ClassDef &cls);
    bool doOutputMonsterMoveModifications(QString v, const ClassDef &cls);

protected:
    QTextStream &m_out;

private:
    QString m_err;
    QString m_warn;

    static QMap<QString, quint32> s_anonymousCounts;
};
}

#endif // CLASSGENERATOR_H
