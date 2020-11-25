#ifndef SPECIALOBJECTGENERATOR_H
#define SPECIALOBJECTGENERATOR_H

#include "objectgenerator.h"

namespace AHS {

class SpecialObjectGenerator : public ObjectGenerator
{
public:
    SpecialObjectGenerator(QTextStream &out) : ObjectGenerator(out) {}

    bool allowInfinite(const ClassDef &cls) const override { Q_UNUSED(cls); return true; }

protected:
    QList<AttributeDesc> getAttributes() override;
    QString getObjectTypeName(const ClassDef &cls) const override;

};

}

#endif // SPECIALOBJECTGENERATOR_H
