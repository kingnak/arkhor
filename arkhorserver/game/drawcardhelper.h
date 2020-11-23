#ifndef DRAWCARDHELPER_H
#define DRAWCARDHELPER_H

#include <ahglobal.h>
#include <fielddata.h>

class GameObject;
class Player;
class ArkhamEncounter;
class OtherWorldEncounter;

class DrawCardHelper
{
public:
    DrawCardHelper();

    QList<GameObject *> drawObjects(Player *p, const QString &desc, AH::GameObjectType type, int ct = 1, int min = 1, int max = 1, bool allowQuickReturn = true, const QString &sourceId = QString::null);
    QList<GameObject *> drawMixedObjects(Player *p, const QString &desc, const QList<AH::ObjectTypeCount> &types, int min = 1, int max = 1, bool allowQuickReturn = true, const QString &sourceId = QString::null);

    ArkhamEncounter *drawArkhamEncounter(Player *p, AH::Common::FieldData::FieldID fieldId, int ct = 1);
    OtherWorldEncounter *drawOtherWorldEncounter(Player *p, AH::Common::FieldData::FieldID fieldId, int ct = 1);


private:
    QList<GameObject *> doDrawObject(Player *p, const QString &desc, QList<GameObject *> avail, int min, int max, bool allowQuickReturn, const QString &sourceId = QString::null);
};

#endif // DRAWCARDHELPER_H
