#include "classgenerator.h"

#include <QTextStream>
#include <QSet>

namespace AHS {

QMap<QString, quint32> ClassGenerator::s_anonymousCounts;

ClassGenerator::ClassGenerator(QTextStream &out)
    : m_out(out)
{
}

bool ClassGenerator::fixClass(ClassGenerator::ClassDef &cls)
{
    if (cls.isNested) {
        if (!this->allowNested(cls)) {
            return setError(cls.elemName + " cannot be nested");
        }
    }
    if (cls.isAnonymous) {
        if (this->allowAnonymous(cls)) {
            cls.elemName = QString("%2_anonymous_%1").arg(++s_anonymousCounts[cls.elemType]).arg(cls.elemType);
        } else {
            return setError("Anonymous class not allowed for Type " + cls.elemType);
        }
    }
    if (!cls.hasElemMult && this->allowInfinite(cls)) {
        cls.elemMult = INFINITE_MULT;
    }
    return true;
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

QString ClassGenerator::idPrefixForClass(const QString &classType)
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
    if (classType == "Ally") {
        return "AL";
    }
    if (classType == "Monster") {
        return "MO";
    }
    if (classType == "SpecialObject") {
        return "OBJ";
    }
    if (classType == "Option") {
        return "OP";
    }
    if (classType == "Action") {
        return "AC";
    }
    if (classType == "QuickOption") {
        return "OP";
    }
    if (classType == "AEOption") {
        return "OP";
    }
    if (classType == "OWEOption") {
        return "OP";
    }
    if (classType == "FieldOption") {
        return "OP";
    }
    if (classType == "ArkhamEncounter") {
        return "AE";
    }
    if (classType == "OtherWorldEncounter") {
        return "OE";
    }
    if (classType == "Rumor") {
        return "MY";
    }
    if (classType == "Headline") {
        return "MY";
    }
    if (classType == "Environment") {
        return "MY";
    }
    if (classType == "Investigator") {
        return "IN";
    }
    if (classType == "AncientOne") {
        return "AO";
    }
    if (classType == "BlessingCurse") {
        return "BC";
    }
    if (classType == "SpecialAbility") {
        return "SA";
    }
    //setWarning("Unknown Class Type: " + classType);
    return "??";
}

QString ClassGenerator::constantScopeForClass(const QString &classType)
{
    if (classType == "Rumor" || classType == "Environment" || classType == "Headline") {
        return "Mythos";
    }
    if (classType == "QuickOption") {
        return "Option";
    }
    return classType;
}

QString ClassGenerator::generateName(QString clsName)
{
    QString name;
    for (int i = 0; i < clsName.length(); ++i) {
        if (clsName[i] == '_') {
            name += ' ';
            continue;
        }
        if (clsName[i].isUpper()) {
            name += ' ';
        }
        name += clsName[i];
    }
    return name.trimmed();
}

bool ClassGenerator::outputDefaultAttribute(ClassGenerator::AttributeDesc desc, const ClassGenerator::ClassDef &cls)
{
    switch (desc.handleType) {
    case AttributeDesc::H_ID:
    case AttributeDesc::H_Name:
    case AttributeDesc::H_TID:
    case AttributeDesc::H_Special:
        return outputAttribute(cls, AttrDef(desc.name, ArkhorScriptParser::Literal, cls.elemName), true);
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

void ClassGenerator::outputCreateStart(QString type, const ClassDef &cls)
{
    m_out << "var " << getJSVariableName(cls) << " = game.create" << type << "({\n";
}

QString ClassGenerator::getJSVariableName(const ClassGenerator::ClassDef &cls)
{
    return QString("gen_%1_%2").arg(idPrefixForClass(cls.elemType), cls.elemName);
}

bool ClassGenerator::outputAttributes(const ClassDef &cls)
{
    QSet<QString> handled;
    bool first = true;
    for (auto a : cls.attrs) {
        if (!first) m_out << ",\n";
        first = false;
        if (!outputAttribute(cls, a, false))
            return false;
        handled << a.name;
    }

    // Handle more: Default if not set, Predefined
    // Check required
    for (auto a : getAttributes()) {
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
    QList<AttributeDesc> attList = this->getAttributes();
    for (auto x : attList) {
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
        Q_ASSERT_X(a.valTypes & (AttributeDesc::V_Function | AttributeDesc::V_Primitive), "Simple Attribute", qPrintable(QString("Simple Attributes '%1' must be primitive or function").arg(a.name)));

        switch (attr.type) {
        case ArkhorScriptParser::Function:
            m_out << "function() {" << attr.content.first << "}";
            break;
        case ArkhorScriptParser::Literal:
        case ArkhorScriptParser::Primitive:
            m_out << attr.content.first;
            break;
        case ArkhorScriptParser::IDRef:
            if (!outputIDRef(attr, cls))
                return false;
            break;
        case ArkhorScriptParser::Complex:
        case ArkhorScriptParser::EnumValue:
        case ArkhorScriptParser::ArrayValues:
        case ArkhorScriptParser::None:
        case ArkhorScriptParser::NestedObject:
            Q_ASSERT_X(false, "Simple attribute", qPrintable(QString("Simple Attribute '%1' must be Primitive, Literal, Function, or IDRef").arg(a.name)));
        }

        break;
    case AttributeDesc::H_TID:
    case AttributeDesc::H_ID:
        m_out << '"' << idPrefixForClass(cls.elemType) << '_' << cls.elemName << '"';
        break;
    case AttributeDesc::H_Name:
        if (inPredefined)
            m_out << '"' << generateName(cls.elemName) << '"';
        else
            m_out << attr.content.first;
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

void ClassGenerator::outputCreateEnd(const ClassDef &cls)
{
    Q_UNUSED(cls)
    m_out << "});\n";
}

void ClassGenerator::outputRegisterMulti(QString type, const ClassDef &cls)
{
    m_out << "game.register" << type << "(" << cls.elemMult << ", " << getJSVariableName(cls) << ");\n\n";
}


void ClassGenerator::outputRegisterSingle(QString type, const ClassDef &cls)
{
    m_out << "game.register" << type << "(" << getJSVariableName(cls) << ");\n\n";
}

void ClassGenerator::outputRegisterConstant(const ClassGenerator::ClassDef &cls)
{
    if (cls.isAnonymous) return;
    QString scope = constantScopeForClass(cls.elemType);
    m_out << "game.registerConstant('" << scope << "', '" << cls.elemName << "', '" << idPrefixForClass(cls.elemType) << '_' << cls.elemName << "');\n";
}

bool ClassGenerator::outputPrimitive(const ClassGenerator::AttrDef &attr, const ClassGenerator::ClassDef &cls)
{
    Q_ASSERT(attr.type == AttrDef::Type::Primitive);
    m_out << attr.content.first;
    return true;
}

bool ClassGenerator::outputModifications(const AttrDef &attr, const ClassDef &cls)
{
    switch (attr.type) {
    case ArkhorScriptParser::Function:
        m_out << "function() {" << attr.content.first << "}";
        return true;
    case ArkhorScriptParser::Complex:
        return doOutputModifications(attr.content.first);
    case ArkhorScriptParser::Literal:
        m_out << attr.content.first;
        return true;
    default:
        return setError(QString("'%1' must be a function, literal or complex type").arg(attr.name), cls);
    }
}

bool ClassGenerator::doOutputModifications(QString mod)
{
    QStringList mods = mod.split(',', QString::SkipEmptyParts);
    bool first = true;
    for (auto m : mods) {
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

bool ClassGenerator::outputMonsterModifications(const AttrDef &attr, const ClassDef &cls)
{
    if (attr.type == ArkhorScriptParser::Literal) {
        m_out << attr.content.first;
        return true;
    } else if (attr.type == ArkhorScriptParser::Complex) {
        return doOutputMonsterModifications(attr.content.first, cls);
    } else {
        return setError("monsterModifications must be Complex or Literal", cls);
    }
}

bool ClassGenerator::doOutputMonsterModifications(QString v, const ClassDef &cls)
{
    m_out << '[';
    bool first = true;

    v = v.trimmed();
    QRegExp rx("\\s*(\\*|(?:Attribute \\.)?\\S+)\\s*\\{([^\\}]+)\\}\\s*,?\\s*");
    int pos = 0;
    int lastPos = -1;
    while ((pos = rx.indexIn(v, pos)) >= 0) {
        QString mon = rx.cap(1);
        QString mod = rx.cap(2);
        pos += rx.matchedLength();

        if (!first) m_out << ',';
        if (mon.startsWith("Attribute .")) {
            m_out << "\n\t\t{ attributes: ";
            mon = mon.mid(11);
            outputEnumValue("Constants.Monster", AttrDef("attributes", ArkhorScriptParser::EnumValue, mon), cls);
        } else {
            m_out << "\n\t\t{ id: ";
            if (mon == "*") {
                m_out << "\"*\"";
            } else {
                outputIDRef(AttrDef("monsterId", ArkhorScriptParser::IDRef, "Monster."+mon), cls);
            }
        }
        m_out << ", mod: \n\t\t\t";
        outputModifications(AttrDef("monsterAttributes", ArkhorScriptParser::Complex, mod), cls);
        m_out << "\n\t\t}";
        lastPos = pos;
        first = false;
    }
    if (lastPos < v.length()) {
        return setError("Invalid monster modifications", cls);
    }

    m_out << "\n\t]";

    return true;
}

bool ClassGenerator::outputMonsterMoveModifications(const ClassGenerator::AttrDef &attr, const ClassGenerator::ClassDef &cls)
{
    if (attr.type == ArkhorScriptParser::Literal) {
        m_out << attr.content.first;
        return true;
    } else if (attr.type == ArkhorScriptParser::Complex) {
        return doOutputMonsterMoveModifications(attr.content.first, cls);
    } else {
        return setError("monsterMoveModifications must be Complex or Literal", cls);
    }
}

bool ClassGenerator::doOutputMonsterMoveModifications(QString v, const ClassDef &cls)
{
    m_out << '[';
    bool first = true;

    v = v.trimmed();
    QRegExp rx("\\s*((?:Type \\.)?\\S+)\\s+([^,\\s]+)\\s*,?\\s*");
    int pos = 0;
    int lastPos = -1;
    while ((pos = rx.indexIn(v, pos)) >= 0) {
        QString mon = rx.cap(1);
        QString mod = rx.cap(2);
        pos += rx.matchedLength();

        if (!first) m_out << ',';
        if (mon.startsWith("Type .")) {
            m_out << "\n\t\t{ type: ";
            mon = mon.mid(6);
            outputEnumValue("Constants.Movement", AttrDef("type", ArkhorScriptParser::EnumValue, mon), cls);
        } else {
            m_out << "\n\t\t{ id: ";
            outputIDRef(AttrDef("monsterId", ArkhorScriptParser::IDRef, "Monster."+mon), cls);
        }
        m_out << ", moveAs: \n\t\t\t";
        outputEnumValue("Constants.Movement", AttrDef("moveAs", ArkhorScriptParser::EnumValue, mod), cls);
        m_out << "\n\t\t}";
        lastPos = pos;
        first = false;
    }
    if (lastPos < v.length()) {
        return setError("Invalid monster move modifications", cls);
    }

    m_out << "\n\t]";

    return true;
}

bool ClassGenerator::outputCosts(const ClassGenerator::AttrDef &attr, const ClassGenerator::ClassDef &cls)
{
    switch (attr.type) {
    case ArkhorScriptParser::Complex:
        return doOutputCosts(attr.content.first);
    case ArkhorScriptParser::Literal:
        m_out << attr.content.first;
        return true;
    default:
        return setError(QString("'%1' must be a literal or complex type").arg(attr.name), cls);
    }
}

bool ClassGenerator::doOutputCosts(QString costs)
{
    QStringList lines = costs.split(',');
    m_out << "[ ";
    bool firstL = true;
    for (auto line : lines) {
        if (!firstL) m_out << ',';
        firstL = false;
        m_out << "\n\t\t";

        QStringList parts = line.split('+');
        m_out << "[ ";
        bool firstI = true;
        for (auto itm : parts) {
            if (!firstI) m_out << ',';
            firstI = false;
            m_out << "\n\t\t\t";

            QStringList parts = itm.trimmed().split(' ', QString::SkipEmptyParts);
            QString n = parts.value(0);
            parts.pop_front();
            QString v = parts.join(" ");
            m_out << "{ type: Constants.Costs." << n << ", amount: " << v << " }";
        }
        m_out << "\n\t\t]";
    }
    m_out << "\n\t]";
    return true;
}

bool ClassGenerator::outputIDRef(const AttrDef &attr, const ClassDef &cls)
{
    if (attr.type == ArkhorScriptParser::Literal) {
        m_out << attr.content.first;
        return true;
    } else if (attr.type == ArkhorScriptParser::IDRef) {
        return doOutputIDRef(attr.content);
    } else {
        return setError(QString("'%1' must be IDRef or Literal").arg(attr.name), cls);
    }
}

bool ClassGenerator::outputIDRefArray(const AttrDef &attr, const ClassDef &cls)
{
    if (attr.type == ArkhorScriptParser::Literal) {
        return outputIDRef(attr, cls);
    }
    if (attr.type == ArkhorScriptParser::IDRef) {
        // Must be array
        m_out << '[';
        doOutputIDRef(attr.content);
        m_out << ']';
        return true;
    }
    if (attr.type != ArkhorScriptParser::ArrayValues) {
        return setError(QString("'%1' must be IDRef or Literal or Array").arg(attr.name), cls);
    }
    bool first = true;
    for (auto s : attr.array) {
        if (s.first != ArkhorScriptParser::IDRef) {
            return setError(QString("Array values must be IDRefs for '%1'").arg(attr.name), cls);
        }

        if (first)
            m_out << "[ ";
        else
            m_out << ", ";
        first = false;
        doOutputIDRef(s.second);
    }
    if (!first)
        m_out << " ]";
    return true;
}

bool ClassGenerator::doOutputIDRef(AttributeValue ref)
{
    QStringList l = ref.first.split('.');
    m_out << '"' << idPrefixForClass(l.value(0)) << '_' << l.value(1) << '"';
    return true;
}

bool ClassGenerator::outputEnumValue(QString prefix, const AttrDef &attr, const ClassDef &cls)
{
    if (attr.type == ArkhorScriptParser::EnumValue)
        m_out << prefix << '.' << attr.content.first;
    else if (attr.type == ArkhorScriptParser::Literal)
        m_out << attr.content.first;
    else
        return setError(QString("'%1' must be EnumValue or Literal").arg(attr.name), cls);
    return true;
}

bool ClassGenerator::outputEnumValueArray(QString prefix, const AttrDef &attr, const ClassDef &cls)
{
    if (attr.type == ArkhorScriptParser::EnumValue || attr.type == ArkhorScriptParser::Literal) {
        return outputEnumValue(prefix, attr, cls);
    }
    if (attr.type != ArkhorScriptParser::ArrayValues) {
        return setError(QString("'%1' must be EnumValue, Array or Literal").arg(attr.name), cls);
    }
    bool first = true;
    for (auto s : attr.array) {
        if (s.first != ArkhorScriptParser::EnumValue) {
            return setError(QString("Array element must be EnumValue in '%1'").arg(attr.name), cls);
        }

        if (first)
            m_out << "[ ";
        else
            m_out << ", ";
        first = false;
        m_out << prefix << '.' << s.second.first;
    }
    if (!first)
        m_out << " ]";
    return true;
}

bool ClassGenerator::outputFunction(const ClassGenerator::AttrDef &attr, const ClassGenerator::ClassDef &cls, QString params)
{
    switch (attr.type) {
    case ArkhorScriptParser::Function:
        m_out << "function(" << params << ") {" << attr.content.first << "}";
        return true;
    case ArkhorScriptParser::Literal:
        m_out << attr.content.first;
        return true;
    default:
        return setError(QString("'%1' must be a function or literal type").arg(attr.name), cls);
    }
}

}
