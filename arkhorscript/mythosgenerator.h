#ifndef MYTHOSGENERATOR_H
#define MYTHOSGENERATOR_H

#include "classgenerator.h"

namespace AHS {

class MythosGenerator : public ClassGenerator
{
public:
    virtual bool generate(const ClassDef &cls);

protected:
    void outputCreateMythosStart(const ClassDef &cls);
    void outputRegisterMythos(const ClassDef &cls);

    MythosGenerator(QTextStream &out) : ClassGenerator(out) {}

    virtual QList<AttributeDesc> getAttributes();
    virtual bool outputSpecialAttribute(AttributeDesc desc, const ClassDef &cls, const AttrDef &attr);
    //virtual bool outputDefaultAttribute(AttributeDesc desc, const ClassDef &cls);
};

/////////////////////////////////

class HeadlineGenerator : public MythosGenerator
{
public:
    HeadlineGenerator(QTextStream &out) : MythosGenerator(out) {}

protected:
    virtual QList<AttributeDesc> getAttributes();
    //virtual bool outputSpecialAttribute(AttributeDesc desc, const ClassDef &cls, const AttrDef &attr);
    virtual bool outputDefaultAttribute(AttributeDesc desc, const ClassDef &cls);
};

/////////////////////////////////

class RumorGenerator : public MythosGenerator
{
public:
    RumorGenerator(QTextStream &out) : MythosGenerator(out) {}

protected:
    virtual QList<AttributeDesc> getAttributes();
    virtual bool outputSpecialAttribute(AttributeDesc desc, const ClassDef &cls, const AttrDef &attr);
    virtual bool outputDefaultAttribute(AttributeDesc desc, const ClassDef &cls);
};

}

#endif // MYTHOSGENERATOR_H
