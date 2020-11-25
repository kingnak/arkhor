#ifndef QUICKOPTIONGENERATOR_H
#define QUICKOPTIONGENERATOR_H

#include "optiongenerator.h"

namespace AHS {

class QuickOptionGenerator : public OptionGenerator
{
public:
    QuickOptionGenerator(QTextStream &out) : OptionGenerator(out) {}
    bool generate(const ClassDef &cls) override;

protected:
    bool allowNested(const ClassDef &cls) const override { Q_UNUSED(cls); return true; }

    QList<AttributeDesc> getAttributes() override;
    bool outputSpecialAttribute(const AttributeDesc &desc, const ClassDef &cls, const AttrDef &attr) override;
};

///////////////////////////////////////////////////////

// Helper
class ArkhamEncounterOptionGenerator : public QuickOptionGenerator
{
public:
    ArkhamEncounterOptionGenerator(QTextStream &out) : QuickOptionGenerator(out) {}
protected:
    QList<AttributeDesc> getAttributes() override;
    bool outputDefaultAttribute(const AttributeDesc &desc, const ClassDef &cls) override;
};

class OtherWorldEncounterOptionGenerator : public QuickOptionGenerator
{
public:
    OtherWorldEncounterOptionGenerator(QTextStream &out) : QuickOptionGenerator(out) {}
protected:
    QList<AttributeDesc> getAttributes() override;
    bool outputDefaultAttribute(const AttributeDesc &desc, const ClassDef &cls) override;
};

}

#endif // QUICKOPTIONGENERATOR_H
