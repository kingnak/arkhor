#include "investigatorgenerator.h"

#include <QTextStream>
#include <QMap>

namespace AHS
{

bool InvestigatorGenerator::generate(const ClassGenerator::ClassDef &cls)
{
    outputClassComment(cls);
    outputCreateInvestigatorStart(cls);
    if (!outputAttributes(cls)) return false;
    outputCreateEnd(cls);
    outputRegisterConstant(cls);
    outputRegisterInvestigator(cls);
    return true;
}

void InvestigatorGenerator::outputCreateInvestigatorStart(const ClassGenerator::ClassDef &cls)
{
    outputCreateStart("Investigator", cls);
}

void InvestigatorGenerator::outputRegisterInvestigator(const ClassGenerator::ClassDef &cls)
{
    outputRegisterSingle("Investigator", cls);
}

QList<ClassGenerator::AttributeDesc> InvestigatorGenerator::getAttributes()
{
    return QList<AttributeDesc>()
            << AttributeDesc("id", AttributeDesc::R_Predefined, AttributeDesc::H_ID, AttributeDesc::V_Primitive)
            << AttributeDesc("name", AttributeDesc::R_Default, AttributeDesc::H_Name, AttributeDesc::V_Primitive)
            << AttributeDesc("occupation", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("homeFieldId", AttributeDesc::R_Required, AttributeDesc::H_Special, AttributeDesc::V_Primitive)
            << AttributeDesc("sanity", AttributeDesc::R_Required, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("stamina", AttributeDesc::R_Required, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("money", AttributeDesc::R_Required, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("focus", AttributeDesc::R_Required, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("clues", AttributeDesc::R_Required, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("fixedPossesionObjectIds", AttributeDesc::R_Required, AttributeDesc::H_IDRef, AttributeDesc::V_Array)
            << AttributeDesc("randomPossessions", AttributeDesc::R_Required, AttributeDesc::H_Special, AttributeDesc::V_Complex)
            << AttributeDesc("attributes", AttributeDesc::R_Required, AttributeDesc::H_Special, AttributeDesc::V_Complex)
            << AttributeDesc("uniqueAbilityId", AttributeDesc::R_Optional, AttributeDesc::H_IDRef, AttributeDesc::V_Primitive)
            << AttributeDesc("onUnconscious", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Function)
            << AttributeDesc("onInsane", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Function)
            << AttributeDesc("onLostInSpaceAndTime", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Function)
            << AttributeDesc("onOpenGate", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Function)
            << AttributeDesc("onAppearMonster", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Function)
               ;
}

bool InvestigatorGenerator::outputSpecialAttribute(const AttributeDesc &desc, const ClassDef &cls, const AttrDef &attr)
{
    if (desc.name == "homeFieldId") {
        return outputEnumValue("Constants.Fields", attr, cls);
    }
    if (desc.name == "randomPossessions") {
        if (attr.type == ArkhorScriptParser::Literal) {
            m_out << attr.content.first;
            return true;
        } else if (attr.type == ArkhorScriptParser::Complex) {
            return outputRandomPossesions(attr.content.first);
        } else {
            return setError("randomPossesions must be Complex or Literal", cls);
        }
    }
    if (desc.name == "attributes") {
        if (attr.type == ArkhorScriptParser::Literal) {
            m_out << attr.content.first;
            return true;
        } else if (attr.type == ArkhorScriptParser::Complex) {
            return outputInvAttributes(attr.content.first, cls);
        } else {
            return setError("attributes must be Complex or Literal", cls);
        }
    }

    if (desc.name == "onAppearMonster") {
        return outputFunction(attr, cls, "m");
    }
    if (desc.name == "onOpenGate") {
        return outputFunction(attr, cls, "g");
    }

    Q_ASSERT_X(false, "Investigator Generator", qPrintable(QString("Special Attribute '%1' not handled").arg(desc.name)));
    return false;
}

bool InvestigatorGenerator::outputRandomPossesions(const QString &v)
{
    QStringList poss = v.split(',', QString::SkipEmptyParts);
    bool first = true;
    for (const auto &itm : poss) {
        if (first)
            m_out << "[ ";
        else
            m_out << ", ";
        first = false;
        QStringList parts = itm.trimmed().split(' ', QString::SkipEmptyParts);
        QString n = parts.value(0);
        parts.pop_front();
        QString val = parts.join(" ");
        m_out << "{ type: Constants.ObjectType." << n << ", amount: " << val << " }";
    }
    if (!first)
        m_out << " ]";
    return true;
}

bool InvestigatorGenerator::outputInvAttributes(QString v, const ClassDef &cls)
{
    //peedSneak {(1,4), (2,3), (3,4), (4,1)},

    using IntPair = QPair<int,int>;
    using IntPairList = QList<IntPair>;
    QMap<QString, IntPairList> attrDefs;

    v = v.trimmed();
    QRegExp rx(R"(\s*(\S+)\s+\{([\(\)\d\s,]+)\}\s*,?\s*)");
    int pos = 0;
    for (int i = 0; i < 3; ++i) {
        pos = rx.indexIn(v, pos);
        if (pos < 0) {
            return setError("Invalid attributes", cls);
        }
        QString name = rx.cap(1);
        QString attrs = rx.cap(2);
        if (attrDefs.contains(name)) {
            return setError(QString("Attributes for '%1' already defined").arg(name));
        }

        pos += rx.matchedLength();

        IntPairList attrVals;
        QRegExp rx2(R"(\s*\(\s*(\d+)\s*,\s*(\d+)\s*\)\s*,?\s*)");
        int pos2 = 0;
        while ((pos2 = rx2.indexIn(attrs, pos2)) >= 0) {
            int v1 = rx2.cap(1).toInt();
            int v2 = rx2.cap(2).toInt();
            pos2 += rx2.matchedLength();
            attrVals << qMakePair(v1, v2);
        }

        attrDefs[name] = attrVals;
    }

    if (!attrDefs.contains("SpeedSneak")) {
        return setError("Attribute definition for SpeedSneak missing", cls);
    }
    if (!attrDefs.contains("FightWill")) {
        return setError("Attribute definition for FightWill missing", cls);
    }
    if (!attrDefs.contains("LoreLuck")) {
        return setError("Attribute definition for LoreLuck missing", cls);
    }

    m_out << '{';
    bool firstL = true;
    for (const auto &a : attrDefs.keys()) {
        if (!firstL) m_out << ',';
        firstL = false;

        m_out << "\n\t\t" << a << ": [";

        bool firstA = true;
        for (auto itm : attrDefs[a]) {
            if (!firstA) m_out << ",";
            firstA = false;
            m_out << "\n\t\t\t[" << itm.first << ',' << itm.second << ']';
        }
        m_out << "\n\t\t]";
    }
    m_out << "\n\t}";

    return true;
}


}
