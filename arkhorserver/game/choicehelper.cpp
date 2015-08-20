#include "choicehelper.h"
#include "character.h"
#include "gameobject.h"
#include "game/player.h"
#include <choicedata.h>

bool ChoiceHelper::choosePayment(Character *c, AH::Common::Cost options, AH::Common::CostList &selected)
{
    selected.clear();
    if (options.getAlternatives().size() == 0) {
        return true;
    }
    if (options.getAlternatives().size() == 1) {
        selected = options.getAlternatives()[0];
        return true;
    }

    // Remove unpayable options
    AH::Common::Cost realOptions;
    foreach (AH::Common::CostList l, options.getAlternatives()) {
        if (c->canPay(l)) {
            realOptions.addAlternative(l);
        }
    }

    // Choose:
    AH::Common::ChoiceData choice;
    choice.setSelectPayment(realOptions);
    choice.setCanCancel(true);
    Player *p = gGame->playerForCharacter(c);
    AH::Common::ChoiceResponseData resp = p->offerChoice(choice);
    if (resp.isValid()) {
        int idx = resp.toInt();
        if (idx >= 0 && idx < realOptions.getAlternatives().size()) {
            selected = realOptions.getAlternatives()[idx];
            return true;
        }
    }
    return false;
}

void ChoiceHelper::loseHalfPossesions(Character *c)
{
    // Count losable objects
    int ct = 0;
    foreach (GameObject *o, c->inventory()) {
        if (o->type() == AH::Obj_CommonItem || o->type() == AH::Obj_UniqueItem) {
            if (!o->getAttributes().testFlag(AH::Common::GameObjectData::CannotBeLost)) {
                ct++;
            }
        }
    }

    ct /= 2;
    losePossessions(c, ct);
}

void ChoiceHelper::losePossessions(Character *c, int count)
{
    if (count <= 0) {
        return;
    }

    // Loose objects (let user decide)
    QList<GameObject *> objs;
    QStringList ids;
    foreach (GameObject *o, c->inventory()) {
        if (o->type() == AH::Obj_CommonItem || o->type() == AH::Obj_UniqueItem) {
            if (!o->getAttributes().testFlag(AH::Common::GameObjectData::CannotBeLost)) {
                objs << o;
                ids << o->id();
            }
        }
    }

    AH::Common::ChoiceData choice;
    choice.setSelectObjects(ids, count, count);
    choice.setDescription("Select items to lose");
    Player *p = gGame->playerForCharacter(c);
    AH::Common::ChoiceResponseData resp = p->offerChoice(choice);

    QStringList loseIds = resp.toStringList();
    foreach (GameObject *obj, objs) {
        if (loseIds.contains(obj->id())) {
            c->removeFromInventory(obj);
            gGame->returnObject(obj);
        }
    }
}

