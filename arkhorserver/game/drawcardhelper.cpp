#include "drawcardhelper.h"
#include "game.h"
#include "player.h"
#include "gameobject.h"

DrawCardHelper::DrawCardHelper()
{
}

QList<GameObject *> DrawCardHelper::drawObjects(Player *p, QString desc, AH::GameObjectType type, int ct, int min, int max, bool allowQuickReturn)
{
    QList<GameObject *> allObjs;
    QList<GameObject *> sels;

    // Normalize arguments:
    max = qMin(max, ct);
    min = qMin(min, max);
    if (max <= 0) return sels;
    if (ct <= 0) return sels;

    // Draw objects
    for (int i = 0; i < ct; ++i) {
        GameObject *o = gGame->drawObject(type);
        if (o) {
            allObjs << o;
        }
    }

    sels = doDrawObject(p, desc, allObjs, min, max, allowQuickReturn);
    return sels;
}

QList<GameObject *> DrawCardHelper::drawMixedObjects(Player *p, QString desc, QList<AH::ObjectTypeCount> types, int min, int max, bool allowQuickReturn)
{
    QList<GameObject *> sels;
    if (types.isEmpty()) return sels;

    QList<GameObject *> allObjs;
    foreach (AH::ObjectTypeCount t, types) {
        for (int i = 0; i < t.amount; ++i) {
            allObjs << gGame->drawObject(t.type);
        }
    }

    sels = doDrawObject(p, desc, allObjs, min, max, allowQuickReturn);
    return sels;
}

QList<GameObject *> DrawCardHelper::doDrawObject(Player *p, QString desc, QList<GameObject *> avail, int min, int max, bool allowQuickReturn)
{
    QList<GameObject *> sels;

    // Re-normalize arguments
    int ct = avail.count();
    max = qMin(max, ct);
    min = qMin(min, max);

    if (ct == 0) {
        return sels;
    }

    // Quick returns: Are those ok? More options?
    if (allowQuickReturn && ct == max && min == max) {
        return avail;
    }

    QStringList ids;
    foreach (GameObject *o, avail) {
        ids << o->id();
    }

    // Let user choose!
    AH::Common::ChoiceData choice;
    choice.setSelectObjects(ids, min, max);
    choice.setDescription(desc);
    AH::Common::ChoiceResponseData resp = p->offerChoice(choice);

    QStringList selIds;
    if (resp.isValid()) {
        selIds = resp.toStringList();
    }
    foreach (GameObject *o, avail) {
        if (selIds.contains(o->id())) {
            sels << o;
        } else {
            // Return it to the deck
            gGame->returnObject(o);
        }
    }
    return sels;
}
