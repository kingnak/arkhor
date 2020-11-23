#ifndef ACTIONGENERATOR_H
#define ACTIONGENERATOR_H

#include "classgenerator.h"

namespace AHS {

class ActionGenerator : public ClassGenerator
{
public:
    ActionGenerator(QTextStream &out) : ClassGenerator(out) {}

    bool generate(const ClassDef &cls) override;

protected:
    //virtual bool allowNested(const ClassDef &cls) const { Q_UNUSED(cls); return true; }

    void outputCreateActionStart(const ClassDef &cls);
    void outputRegisterAction(const ClassDef &cls);

    QList<AttributeDesc> getAttributes() override;
    bool outputSpecialAttribute(const AttributeDesc &desc, const ClassDef &cls, const AttrDef &attr) override;
};

}

#endif // ACTIONGENERATOR_H
