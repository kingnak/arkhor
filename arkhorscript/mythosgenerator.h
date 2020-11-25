#ifndef MYTHOSGENERATOR_H
#define MYTHOSGENERATOR_H

#include "classgenerator.h"

namespace AHS {

class MythosGenerator : public ClassGenerator
{
public:
    bool generate(const ClassDef &cls) override;

protected:
    void outputCreateMythosStart(const ClassDef &cls);
    void outputRegisterMythos(const ClassDef &cls);

    MythosGenerator(QTextStream &out) : ClassGenerator(out) {}

    QList<AttributeDesc> getAttributes() override;
    bool outputSpecialAttribute(const AttributeDesc &desc, const ClassDef &cls, const AttrDef &attr) override;
    //virtual bool outputDefaultAttribute(AttributeDesc desc, const ClassDef &cls);
};

/////////////////////////////////

class HeadlineGenerator : public MythosGenerator
{
public:
    HeadlineGenerator(QTextStream &out) : MythosGenerator(out) {}

protected:
    bool allowAnonymous(const ClassDef &cls) const override { Q_UNUSED(cls); return true; }

    QList<AttributeDesc> getAttributes() override;
    //virtual bool outputSpecialAttribute(AttributeDesc desc, const ClassDef &cls, const AttrDef &attr);
    bool outputDefaultAttribute(const AttributeDesc &desc, const ClassDef &cls) override;
};

/////////////////////////////////

class EnvironmentGenerator : public MythosGenerator
{
public:
    EnvironmentGenerator(QTextStream &out) : MythosGenerator(out) {}

protected:
    QList<AttributeDesc> getAttributes() override;
    bool outputSpecialAttribute(const AttributeDesc &desc, const ClassDef &cls, const AttrDef &attr) override;
    bool outputDefaultAttribute(const AttributeDesc &desc, const ClassDef &cls) override;
};

/////////////////////////////////

class RumorGenerator : public MythosGenerator
{
public:
    RumorGenerator(QTextStream &out) : MythosGenerator(out) {}

protected:
    QList<AttributeDesc> getAttributes() override;
    bool outputSpecialAttribute(const AttributeDesc &desc, const ClassDef &cls, const AttrDef &attr) override;
    bool outputDefaultAttribute(const AttributeDesc &desc, const ClassDef &cls) override;
};

}

#endif // MYTHOSGENERATOR_H
