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
    outputCreateEnd(cls);
    outputRegisterMultiObject(cls);
    return true;
}

void ObjectGenerator::outputCreateObjectStart(const ClassGenerator::ClassDef &cls)
{
    outputCreateStart("Object", cls);
}

void ObjectGenerator::outputRegisterMultiObject(const ClassGenerator::ClassDef &cls)
{
    outputRegisterMulti("MultiObject", cls);
}

void ObjectGenerator::outputRegisterSingleObject(const ClassGenerator::ClassDef &cls)
{
    outputRegisterSingle("SingleObject", cls);
}

QList<ClassGenerator::AttributeDesc> ObjectGenerator::getAttributes()
{
    return QList<AttributeDesc>()
            << AttributeDesc("type", AttributeDesc::R_Predefined, AttributeDesc::H_Special, AttributeDesc::V_Primitive)
            << AttributeDesc("tid", AttributeDesc::R_Predefined, AttributeDesc::H_TID, AttributeDesc::V_Primitive)
            << AttributeDesc("name", AttributeDesc::R_Default, AttributeDesc::H_Name, AttributeDesc::V_Primitive)
            << AttributeDesc("description", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("modifications", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Function | AttributeDesc::V_Complex)
            << AttributeDesc("onAddToInventory", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Function)
            << AttributeDesc("onRemoveFromInventory", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Function)
            << AttributeDesc("optionIds", AttributeDesc::R_Optional, AttributeDesc::H_IDRef, AttributeDesc::V_Primitive | AttributeDesc::V_Array)
            << AttributeDesc("properties", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
               ;
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

QString ObjectGenerator::getObjectTypeName(const ClassGenerator::ClassDef &cls) const
{
    return cls.elemType;
}

bool ObjectGenerator::outputSpecialAttribute(ClassGenerator::AttributeDesc desc, const ClassGenerator::ClassDef &cls, const ClassGenerator::AttrDef &attr)
{
    if (desc.name == "type") {
        m_out << "Constants.ObjectType."+getObjectTypeName(cls);
        return true;
    }
    if (desc.name == "modifications") {
        return outputModifications(attr, cls);
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

    Q_ASSERT_X(false, "Object Generator", qPrintable(QString("Special Attribute '%1' not handled").arg(desc.name)));
    return true;
}

}
