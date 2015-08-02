#ifndef ACTIONGENERATOR_H
#define ACTIONGENERATOR_H

#include "classgenerator.h"

namespace AHS {

class ActionGenerator : public ClassGenerator
{
public:
    ActionGenerator(QTextStream &out) : ClassGenerator(out) {}

    virtual bool generate(const ClassDef &cls);

protected:
    void outputCreateActionStart(const ClassDef &cls);
    void outputRegisterAction(const ClassDef &cls);

    virtual QList<AttributeDesc> getAttributes();
    virtual bool outputSpecialAttribute(AttributeDesc desc, const ClassDef &cls, const AttrDef &attr);
};

}

#endif // ACTIONGENERATOR_H
