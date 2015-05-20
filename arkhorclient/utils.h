#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <propertyvaluedata.h>
#include <monsterdata.h>
#include <cost.h>

struct Utils {
    static inline QString fullNumberString(int n) {
        return n >= 0 ? QString("+%1").arg(n) : QString::number(n);
    }

    static QString stringForProperty(AH::Common::PropertyValueData::Property p);
    static QString stringForDimension(AH::Dimension dim);
    static QString stringForMovement(AH::Common::MonsterData::MovementType m);
    static QString stringForMonsterAttribute(AH::Common::MonsterData::MonsterAttribute a);
    static QStringList stringsForMonsterAttributes(AH::Common::MonsterData::MonsterAttributes attrs);
    static QString stringForCostItem(AH::Common::CostItem::PaymentItem pi);
};

#endif // UTILS_H
