#ifndef DRAWCARDHELPER_H
#define DRAWCARDHELPER_H

#include <ahglobal.h>

class GameObject;
class Player;

class DrawCardHelper
{
public:
    DrawCardHelper();

    QList<GameObject *> drawObjects(Player *p, QString desc, AH::GameObjectType type, int ct = 1, int min = 1, int max = 1, bool allowQuickReturn = true);
    QList<GameObject *> drawMixedObjects(Player *p, QString desc, QList<AH::ObjectTypeCount> types, int min = 1, int max = 1, bool allowQuickReturn = true);

private:
    QList<GameObject *> doDrawObject(Player *p, QString desc, QList<GameObject *> avail, int min, int max, bool allowQuickReturn);
};

#endif // DRAWCARDHELPER_H
