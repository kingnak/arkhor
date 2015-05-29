#include "chooseweaponsaction.h"
#include "game/gameobject.h"
#include "game/game.h"
#include "game/player.h"
#include "character.h"
#include "fightphase.h"

ChooseWeaponsAction::ChooseWeaponsAction(FightPhase *fight)
    : m_fight(fight)
{
}


bool ChooseWeaponsAction::execute()
{
    QList<GameObject *> lst = gGame->context().player()->getCharacter()->inventory();
    QList<GameObject *> fightObjects;
    foreach (GameObject *o, lst) {
        if (o->isFightObject()) {
            fightObjects << o;
        }
    }

    ModifiedPropertyValue hands = gGame->context().getCurCharacterProperty(AH::Common::PropertyValueData::Prop_HandCount);
    QStringList sel;
    if (gGame->context().player()->chooseWeapons(fightObjects, hands, sel))
    {
        if (doEquip(fightObjects, sel, hands.finalVal())) {

        }
    }

    if (m_fight)
        m_fight->updatePhaseByResult(FightPhase::WeaponsChosen);
    return true;
}

bool ChooseWeaponsAction::doEquip(QList<GameObject *> oldWeapons, QStringList newWeapons, int hands)
{
    int newHands = 0;
    foreach (GameObject *obj, oldWeapons) {
        if (newWeapons.contains(obj->id())) {
            newHands += obj->handCount();
        }
    }

    if (newHands > hands) {
        return false;
    }

    bool ok = true;
    // Unequip and equip
    foreach (GameObject *obj, oldWeapons) {
        if (newWeapons.contains(obj->id())) {
            // Should be equipped
            if (obj->isEquipped()) {
                continue;
            }

            ok &= obj->equip(gGame->context().player());

        } else {
            // Should not be equipped
            if (!obj->isEquipped()) {
                continue;
            }

            obj->unequip();
        }
    }

    return ok;
}
