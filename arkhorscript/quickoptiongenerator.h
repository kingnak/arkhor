#ifndef QUICKOPTIONGENERATOR_H
#define QUICKOPTIONGENERATOR_H

#include "optiongenerator.h"

namespace AHS {

class QuickOptionGenerator : public OptionGenerator
{
public:
    QuickOptionGenerator(QTextStream &out) : OptionGenerator(out) {}
    virtual bool generate(const ClassDef &cls);

protected:
    virtual bool allowNested(const ClassDef &cls) const { Q_UNUSED(cls); return true; }

    virtual QList<AttributeDesc> getAttributes();
    virtual bool outputSpecialAttribute(AttributeDesc desc, const ClassDef &cls, const AttrDef &attr);
};

///////////////////////////////////////////////////////

// Helper
class ArkhamEncounterOptionGenerator : public QuickOptionGenerator
{
public:
    ArkhamEncounterOptionGenerator(QTextStream &out) : QuickOptionGenerator(out) {}
protected:
    virtual QList<AttributeDesc> getAttributes();
    virtual bool outputDefaultAttribute(AttributeDesc desc, const ClassDef &cls);
};

class OtherWorldEncounterOptionGenerator : public QuickOptionGenerator
{
public:
    OtherWorldEncounterOptionGenerator(QTextStream &out) : QuickOptionGenerator(out) {}
protected:
    virtual QList<AttributeDesc> getAttributes();
    virtual bool outputDefaultAttribute(AttributeDesc desc, const ClassDef &cls);
};

}

#endif // QUICKOPTIONGENERATOR_H
