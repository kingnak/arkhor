#ifndef DRAWCARDHELPER_H
#define DRAWCARDHELPER_H

#include <ahglobal.h>

class GameObject;
class Player;

class DrawCardHelper
{
public:
    DrawCardHelper();

    QList<GameObject *> drawObjects(Player *p, QString desc, AH::GameObjectType type, int ct = 1, int min = 1, int max = 1, bool allowQuickReturn = false);
    QList<GameObject *> drawMixedObjects(/*TODO*/);
};

#endif // DRAWCARDHELPER_H
