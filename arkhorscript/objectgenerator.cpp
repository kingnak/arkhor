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
            << AttributeDesc("modifications", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Function | AttributeDesc::V_Complex)
            << AttributeDesc("exhaustable", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("onAddToInventory", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Function)
            << AttributeDesc("onRemoveFromInventory", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Function)
            << AttributeDesc("optionIds", AttributeDesc::R_Optional, AttributeDesc::H_IDRef, AttributeDesc::V_Primitive | AttributeDesc::V_Array);
}

bool ObjectGenerator::outputDefaultAttribute(ClassGenerator::AttributeDesc desc, const ClassGenerator::ClassDef &cls)
{
    /*
    // activated by some
    if (desc.name == "hands") {
        outputAttribute(cls, AttrDef("hands", AttrDef::Literal, "1"), false);
        return true;
    }
    */
    //Q_ASSERT_X(false, "Unknown default Attribute", qPrintable(desc.name));
    return ClassGenerator::outputDefaultAttribute(desc, cls);
}

bool ObjectGenerator::outputSpecialAttribute(ClassGenerator::AttributeDesc desc, const ClassGenerator::ClassDef &cls, const ClassGenerator::AttrDef &attr)
{
    if (desc.name == "type") {
        m_out << "Constants.ObjectType."+cls.elemType;
        return true;
    }
    if (desc.name == "modifications") {
        switch (attr.type) {
        case AttrDef::Function:
            m_out << "function() {" << attr.content << "}";
            return true;
        case AttrDef::Complex:
            return outputModifications(attr.content);
        case AttrDef::Literal:
            m_out << attr.content;
            return true;
        default:
            return setError("modifications must be a function or complex type", cls);
        }
    }

    if (desc.name == "onAddToInventory") {
        return outputFunction(attr, cls, "c");
    }

    if (desc.name == "onRemoveFromInventory") {
        return outputFunction(attr, cls, "c");
    }

    // Activated by some
    if (desc.name == "attributes") {
        return outputEnumValueArray("Constants.ObjectAttribute", attr, cls);
    }

    Q_ASSERT_X(false, "Special Attribute", "Special Attribute not handled by subclass");
    return true;
}

}
