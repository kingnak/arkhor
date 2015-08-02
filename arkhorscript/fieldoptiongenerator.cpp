#include "fieldoptiongenerator.h"

#include <QTextStream>

namespace AHS
{

bool FieldOptionGenerator::generate(const ClassGenerator::ClassDef &cls)
{
    if (QuickOptionGenerator::generate(cls)) {
        m_out << "game.addFieldOption(Constants.Fields." << m_field << ", " << getJSVariableName(cls) << ".id);\n\n";
        return true;
    }
    return false;
}

QList<ClassGenerator::AttributeDesc> FieldOptionGenerator::getAttributes()
{
    return QuickOptionGenerator::getAttributes()
            << AttributeDesc("field", AttributeDesc::R_Required, AttributeDesc::H_Special, AttributeDesc::V_Primitive)
               ;
}

bool FieldOptionGenerator::outputSpecialAttribute(AttributeDesc desc, const ClassDef &cls, const AttrDef &attr)
{
    if (desc.name == "field") {
        m_field = attr.content;
        return outputEnumValue("Constants.Fields", attr, cls);
    }
    return QuickOptionGenerator::outputSpecialAttribute(desc, cls, attr);
}


}
