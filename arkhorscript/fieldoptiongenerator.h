#ifndef FIELDOPTIONGENERATOR_H
#define FIELDOPTIONGENERATOR_H

#include "quickoptiongenerator.h"

namespace AHS {

class FieldOptionGenerator : public QuickOptionGenerator
{
public:
    FieldOptionGenerator(QTextStream &out) : QuickOptionGenerator(out) {}

    bool generate(const ClassDef &cls) override;

protected:
    QList<AttributeDesc> getAttributes() override;
    bool outputSpecialAttribute(const AttributeDesc &desc, const ClassDef &cls, const AttrDef &attr) override;
    bool outputDefaultAttribute(const AttributeDesc &desc, const ClassDef &cls) override;

private:
    QString m_field;
};

}

#endif // FIELDOPTIONGENERATOR_H
