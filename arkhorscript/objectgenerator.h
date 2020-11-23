#ifndef OBJECTGENERATOR_H
#define OBJECTGENERATOR_H

#include "classgenerator.h"

namespace AHS {

class ObjectGenerator : public ClassGenerator
{
public:
    ObjectGenerator(QTextStream &out);
    bool generate(const ClassDef &cls) override;

protected:
    void outputCreateObjectStart(const ClassDef &cls);
    void outputRegisterMultiObject(const ClassDef &cls);
    void outputRegisterSingleObject(const ClassDef &cls);

    QList<AttributeDesc> getAttributes() override;
    bool outputSpecialAttribute(const AttributeDesc &desc, const ClassDef &cls, const AttrDef &attr) override;
    bool outputDefaultAttribute(const AttributeDesc &desc, const ClassGenerator::ClassDef &cls) override;

    virtual QString getObjectTypeName(const ClassDef &cls) const;
};

}

#endif // OBJECTGENERATOR_H
