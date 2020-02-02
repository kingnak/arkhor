#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <propertyvaluedata.h>
#include <monsterdata.h>
#include <mythosdata.h>
#include <gameobjectdata.h>
#include <cost.h>

struct Utils {
    static inline QString fullNumberString(int n) {
        return n >= 0 ? QString("+%1").arg(n) : QString::number(n);
    }

    static QString stringForProperty(AH::Common::PropertyValueData::Property p);
    static QString stringForDimension(AH::Dimension dim);
    static QStringList stringsForDimensions(AH::Dimensions dims);
    static QString stringForField(AH::Common::FieldData::FieldID fid);
    static QString stringForFieldType(AH::Common::FieldData::FieldType fType);
    static QString stringForMovement(AH::Common::MonsterData::MovementType m);
    static QString stringForMonsterAttribute(AH::Common::MonsterData::MonsterAttribute a);
    static QStringList stringsForMonsterAttributes(AH::Common::MonsterData::MonsterAttributes attrs);
    static QString stringForCostItem(AH::Common::CostItem::PaymentItem pi);
    static QString stringForObjectType(AH::GameObjectType t);
    static QString stringForObjectAttribute(AH::Common::GameObjectData::ObjectAttribute a);
    static QStringList stringsForObjectAttributes(AH::Common::GameObjectData::ObjectAttributes attrs);
    static QString stringForPhase(AH::GamePhase ph);
    static QString stringForPropertyModification(AH::Common::PropertyModificationData mod);
    static QString stringForMythosType(AH::Common::MythosData::MythosType t, AH::Common::MythosData::EnvironmentType et, QString wrapSubType = QString::null);
    static QString stringForMythosEnvironmentType(AH::Common::MythosData::EnvironmentType et);
};

#endif // UTILS_H
