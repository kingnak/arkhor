#ifndef FIELDOPTIONGENERATOR_H
#define FIELDOPTIONGENERATOR_H

#include "quickoptiongenerator.h"

namespace AHS {

class FieldOptionGenerator : public QuickOptionGenerator
{
public:
    FieldOptionGenerator(QTextStream &out) : QuickOptionGenerator(out) {}

    virtual bool generate(const ClassDef &cls);

protected:
    virtual QList<AttributeDesc> getAttributes();
    virtual bool outputSpecialAttribute(AttributeDesc desc, const ClassDef &cls, const AttrDef &attr);

private:
    QString m_field;
};

}

#endif // FIELDOPTIONGENERATOR_H
