#ifndef MONSTERGENERATOR_H
#define MONSTERGENERATOR_H

#include "classgenerator.h"

namespace AHS {

class MonsterGenerator : public ClassGenerator
{
public:
    MonsterGenerator(QTextStream &out) : ClassGenerator(out) {}
    virtual bool generate(const ClassDef &cls);

protected:
    void outputCreateMonsterStart(const ClassDef &cls);
    void outputRegisterMonster(const ClassDef &cls);

    virtual QList<AttributeDesc> getAttributes() ;
    virtual bool outputSpecialAttribute(AttributeDesc desc, const ClassDef &cls, const AttrDef &attr);
    //virtual bool outputDefaultAttribute(AttributeDesc desc, const ClassDef &cls);

};

}

#endif // MONSTERGENERATOR_H
