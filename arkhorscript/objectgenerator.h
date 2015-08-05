#ifndef OBJECTGENERATOR_H
#define OBJECTGENERATOR_H

#include "classgenerator.h"

namespace AHS {

class ObjectGenerator : public ClassGenerator
{
public:
    ObjectGenerator(QTextStream &out);
    bool generate(const ClassDef &cls);

protected:
    void outputCreateObjectStart(const ClassDef &cls);
    void outputRegisterMultiObject(const ClassDef &cls);
    void outputRegisterSingleObject(const ClassDef &cls);

    virtual QList<AttributeDesc> getAttributes();
    virtual bool outputSpecialAttribute(AttributeDesc desc, const ClassDef &cls, const AttrDef &attr);
    virtual bool outputDefaultAttribute(ClassGenerator::AttributeDesc desc, const ClassGenerator::ClassDef &cls);

    virtual QString getObjectTypeName(const ClassDef &cls) const;
};

}

#endif // OBJECTGENERATOR_H
