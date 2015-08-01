#include "classgenerator.h"

#include <QTextStream>
#include <QSet>

namespace AHS {


ClassGenerator::ClassGenerator(QTextStream &out)
    : m_out(out)
{
}

bool ClassGenerator::setError(QString err)
{
    m_err += err+"\n";
    return false;
}

bool ClassGenerator::setError(QString err, const ClassDef &cls)
{
    m_err += err + QString(" in %2 %3").arg(cls.elemType, cls.elemName);
    return false;
}

bool ClassGenerator::setWarning(QString warn)
{
    m_warn += warn+"\n";
    return false;
}

bool ClassGenerator::setWarning(QString warn, const ClassDef &cls)
{
    m_warn += warn + QString(" in %2 %3").arg(cls.elemType, cls.elemName);
    return false;
}

QString ClassGenerator::idPrefixForClass(QString classType)
{
    if (classType == "CommonItem") {
        return "CI";
    }
    if (classType == "Skill") {
        return "SK";
    }
    if (classType == "UniqueItem") {
        return "UI";
    }
    if (classType == "Spell") {
        return "SP";
    }
    setWarning("Unknown Class Type: " + classType);
    return "??";
}

QString ClassGenerator::generateName(QString clsName)
{
    // TODO
    return clsName;
}

bool ClassGenerator::outputDefaultAttribute(ClassGenerator::AttributeDesc desc, const ClassGenerator::ClassDef &cls)
{
    switch (desc.handleType) {
    case AttributeDesc::H_ID:
    case AttributeDesc::H_Name:
    case AttributeDesc::H_TID:
    case AttributeDesc::H_Special:
        return outputAttribute(cls, AttrDef(desc.name, AttrDef::Literal, cls.elemName), true);
    default:
        ;
    }
    //return setError(QString("No default for attribute '%1'").arg(desc.name), cls);
    Q_ASSERT_X(false, "Default Attribute", qPrintable(QString("No default for attribute '%1'").arg(desc.name)));
    return false;
}

void ClassGenerator::outputClassComment(const ClassDef &cls)
{
    m_out << "\n// Generated " << cls.elemType << " \"" << cls.elemName << "\"\n";
}

void ClassGenerator::outputCreateObjectStart(const ClassDef &cls)
{
    m_out << "var gen_" << cls.elemName << " = game.createObject({\n";
}

bool ClassGenerator::outputAttributes(const ClassDef &cls)
{
    QSet<QString> handled;
    bool first = true;
    foreach (AttrDef a, cls.attrs) {
        if (!first) m_out << ",\n";
        first = false;
        if (!outputAttribute(cls, a, false))
            return false;
        handled << a.name;
    }

    // Handle more: Default if not set, Predefined
    // Check required
    foreach (AttributeDesc a, getAttributes()) {
        switch (a.reqType) {
        case AttributeDesc::R_INVALID:
            Q_ASSERT(false);
        case AttributeDesc::R_Optional:
            break;
        case AttributeDesc::R_Required:
            if (!handled.contains(a.name))
                return setError(QString("Missing required Attribute '%1'").arg(a.name), cls);
            break;
        case AttributeDesc::R_Default:
            if (handled.contains(a.name))
                break;
            // Fallthrough
        case AttributeDesc::R_Predefined:
            if (!first) m_out << ",\n";
            first = false;
            if (!outputDefaultAttribute(a, cls))
                return false;
            break;
        }
    }

    m_out << '\n';
    return true;
}

bool ClassGenerator::outputAttribute(const ClassDef &cls, const AttrDef &attr, bool inPredefined)
{
    // Find definition
    AttributeDesc a;
    foreach (AttributeDesc x, this->getAttributes()) {
        if (x.name == attr.name) {
            a = x;
            break;
        }
    }

    switch (a.reqType) {
    case AttributeDesc::R_INVALID:
        return setError(QString("Not supported Attribute '%1'").arg(attr.name), cls);
    case AttributeDesc::R_Predefined:
        if (!inPredefined)
            return setError(QString("Attribute '%1' must not be defined").arg(attr.name), cls);
    default:
        // OK
        ;
    }

    m_out << "\t" << attr.name << ": ";
    switch (a.handleType) {
    case AttributeDesc::H_Simple:
        Q_ASSERT_X(a.valTypes == AttributeDesc::V_Primitive, "Simple Attribute", "Simple Attributes must be primitive");
        m_out << attr.content;
        break;
    case AttributeDesc::H_TID:
    case AttributeDesc::H_ID:
        m_out << '"' << idPrefixForClass(cls.elemType) << '_' << cls.elemName << '"';
        break;
    case AttributeDesc::H_Name:
        if (inPredefined)
            m_out << '"' << generateName(cls.elemName) << '"';
        else
            m_out << attr.content;
        break;
    case AttributeDesc::H_IDRef:
        if (a.valTypes.testFlag(AttributeDesc::V_Array)) {
            return outputIDRefArray(attr, cls);
        } else {
            return outputIDRef(attr, cls);
        }
        break;
    case AttributeDesc::H_Special:
        if (!outputSpecialAttribute(a, cls, attr))
            return false;
        break;
    default:
        Q_ASSERT("Unsupported Attribute Handle Type");
    }
    //m_out << (last?"\n":",\n");

    return true;
}

void ClassGenerator::outputCreateObjectEnd(const ClassDef &cls)
{
    Q_UNUSED(cls)
    m_out << "});\n";
}

void ClassGenerator::outputRegisterObject(const ClassDef &cls)
{
    m_out << "game.registerMultiObject(" << cls.elemMult << ", gen_" << cls.elemName << ");\n\n";
}

bool ClassGenerator::outputModifications(QString mod)
{
    QStringList mods = mod.split(',', QString::SkipEmptyParts);
    bool first = true;
    foreach (QString m, mods) {
        if (first)
            m_out << "[ ";
        else
            m_out << ", ";
        first = false;
        QStringList parts = m.trimmed().split(' ', QString::SkipEmptyParts);
        QString n = parts.value(0);
        parts.pop_front();
        QString v = parts.join(" ");
        m_out << "{ property: Constants.Mods." << n << ", value: \"" << v << "\" }";
    }
    if (!first)
        m_out << " ]";
    return true;
}

bool ClassGenerator::outputIDRef(const AttrDef &attr, const ClassDef &cls)
{
    if (attr.type == AttrDef::Literal) {
        m_out << attr.content;
        return true;
    } else if (attr.type == AttrDef::IDRef) {
        return doOutputIDRef(attr.content);
    } else {
        return setError(QString("'%1' must be IDRef or Literal").arg(attr.name), cls);
    }
}

bool ClassGenerator::outputIDRefArray(const AttrDef &attr, const ClassDef &cls)
{
    if (attr.type == AttrDef::IDRef || attr.type == AttrDef::Literal) {
        return outputIDRef(attr, cls);
    }
    if (attr.type != AttrDef::Array) {
        return setError(QString("'%1' must be IDRef or Literal or Array").arg(attr.name), cls);
    }
    bool first = true;
    foreach (QString s, attr.array) {
        if (first)
            m_out << "[ ";
        else
            m_out << ", ";
        first = false;
        doOutputIDRef(s);
    }
    if (!first)
        m_out << " ]";
    return true;
}

bool ClassGenerator::doOutputIDRef(QString ref)
{
    QStringList l = ref.split('.');
    m_out << '"' << idPrefixForClass(l.value(0)) << '_' << l.value(1) << '"';
    return true;
}

bool ClassGenerator::outputEnumValue(QString prefix, const AttrDef &attr, const ClassDef &cls)
{
    if (attr.type == AttrDef::EnumValue)
        m_out << prefix << '.' << attr.content;
    else if (attr.type == AttrDef::Literal)
        m_out << attr.content;
    else
        return setError(QString("'%1' must be EnumValue or Literal").arg(attr.name), cls);
    return true;
}

bool ClassGenerator::outputEnumValueArray(QString prefix, const AttrDef &attr, const ClassDef &cls)
{
    if (attr.type == AttrDef::EnumValue || attr.type == AttrDef::Literal) {
        return outputEnumValue(prefix, attr, cls);
    }
    if (!attr.type == AttrDef::Array) {
        return setError(QString("'%1' must be EnumValue, Array or Literal").arg(attr.name), cls);
    }
    bool first = true;
    foreach (QString s, attr.array) {
        if (first)
            m_out << "[ ";
        else
            m_out << ", ";
        first = false;
        m_out << prefix << '.' << s;
    }
    if (!first)
        m_out << " ]";
    return true;
}

bool ClassGenerator::outputFunction(const ClassGenerator::AttrDef &attr, const ClassGenerator::ClassDef &cls, QString params)
{
    switch (attr.type) {
    case AttrDef::Function:
        m_out << "function(" << params << ") {" << attr.content << "}";
        return true;
    case AttrDef::Literal:
        m_out << attr.content;
        return true;
    default:
        return setError(QString("'%1' must be a function or literal type").arg(attr.name), cls);
    }
}

}
