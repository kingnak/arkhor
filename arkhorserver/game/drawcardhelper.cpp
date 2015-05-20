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
    QStringList ids;
    for (int i = 0; i < ct; ++i) {
        GameObject *o = gGame->drawObject(type);
        if (o) {
            allObjs << o;
            ids << o->id();
        }
    }

    // Re-normalize arguments
    ct = allObjs.count();
    max = qMin(max, ct);
    min = qMin(min, max);

    if (ct == 0) {
        return sels;
    }

    // Quick returns: Are those ok? More options?
    if (allowQuickReturn && ct == max && min == max) {
        return allObjs;
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
    foreach (GameObject *o, allObjs) {
        if (selIds.contains(o->id())) {
            sels << o;
        } else {
            // Return it to the deck
            gGame->returnObject(o);
        }
    }

    return sels;
}
