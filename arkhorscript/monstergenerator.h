#ifndef MONSTERGENERATOR_H
#define MONSTERGENERATOR_H

#include "classgenerator.h"

namespace AHS {

class MonsterGenerator : public ClassGenerator
{
public:
    MonsterGenerator(QTextStream &out) : ClassGenerator(out) {}
    bool generate(const ClassDef &cls) override;

protected:
    void outputCreateMonsterStart(const ClassDef &cls);
    void outputRegisterMonster(const ClassDef &cls);

    QList<AttributeDesc> getAttributes() override;
    bool outputSpecialAttribute(const AttributeDesc &desc, const ClassDef &cls, const AttrDef &attr) override;
    //virtual bool outputDefaultAttribute(AttributeDesc desc, const ClassDef &cls);

};

}

#endif // MONSTERGENERATOR_H
