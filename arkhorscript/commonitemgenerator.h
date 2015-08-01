#ifndef COMMONITEMGENERATOR_H
#define COMMONITEMGENERATOR_H

#include "objectgenerator.h"

namespace AHS {

class CommonItemGenerator : public ObjectGenerator
{
public:
    CommonItemGenerator(QTextStream &out);

protected:
    //virtual QString getType() const;
    virtual QList<AttributeDesc> getAttributes();
    //virtual bool outputSpecialAttribute(AttributeDesc desc, const ClassDef &cls, const AttrDef &attr);
};

}

#endif // COMMONITEMGENERATOR_H
