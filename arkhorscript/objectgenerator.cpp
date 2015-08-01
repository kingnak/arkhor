#include "objectgenerator.h"


#include <QTextStream>

namespace AHS {

ObjectGenerator::ObjectGenerator(QTextStream &out)
    : ClassGenerator(out)
{
}

bool ObjectGenerator::generate(const ClassGenerator::ClassDef &cls)
{
    outputClassComment(cls);
    outputCreateObjectStart(cls);
    if (!outputAttributes(cls)) return false;
    outputCreateObjectEnd(cls);
    outputRegisterObject(cls);
    return true;
}

QList<ClassGenerator::AttributeDesc> ObjectGenerator::getAttributes()
{
    return QList<AttributeDesc>()
            << AttributeDesc("type", AttributeDesc::R_Predefined, AttributeDesc::H_Special, AttributeDesc::V_Primitive)
            << AttributeDesc("tid", AttributeDesc::R_Predefined, AttributeDesc::H_TID, AttributeDesc::V_Primitive)
            << AttributeDesc("name", AttributeDesc::R_Default, AttributeDesc::H_Name, AttributeDesc::V_Primitive)
            << AttributeDesc("description", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("modifications", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Function | AttributeDesc::V_Complex);
}

bool ObjectGenerator::outputSpecialAttribute(ClassGenerator::AttributeDesc desc, const ClassGenerator::ClassDef &cls, const ClassGenerator::AttrDef &attr)
{
    if (desc.name == "type") {
        m_out << "Constants.ObjectType."+cls.elemType;
        return true;
    }

    if (desc.name == "modifications") {
        if (attr.type == AttrDef::Function) {
            m_out << "function() {" << attr.content << "}";
            return true;
        } else if (attr.type == AttrDef::Complex) {
            return outputModifications(attr.content);
        } else {
            return setError("modifications must be a function or complex type", cls);
        }
    }

    m_out << "SPECIAL";
    return true;
}

}
