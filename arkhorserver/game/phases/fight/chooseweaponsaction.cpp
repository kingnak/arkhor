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
    gGame->notifier()->actionStart(this);

    QList<GameObject *> lst = gGame->context().player()->getCharacter()->inventory();
    QList<GameObject *> fightObjects;
    for (auto o : lst) {
        if (o->isFightObject()) {
            if (o->isExhausted()) {
                // Unequip for security...
                o->unequip();
            } else {
                fightObjects << o;
            }
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

    gGame->notifier()->actionFinish(this);

    return true;
}

QString ChooseWeaponsAction::notificationString(GameAction::NotificationPart part, const QString &desc) const
{
    Q_UNUSED(desc);
    if (part == Finish) {
        return "{C} equipped: {B}";
    }
    return QString::null;
}

bool ChooseWeaponsAction::doEquip(QList<GameObject *> oldWeapons, QStringList newWeapons, int hands)
{
    int newHands = 0;
    for (auto obj : oldWeapons) {
        if (newWeapons.contains(obj->id())) {
            newHands += obj->handCount();
        }
    }

    if (newHands > hands) {
        return false;
    }

    bool ok = true;
    // Unequip and equip
    for (auto obj : oldWeapons) {
        if (newWeapons.contains(obj->id())) {
            // Should be equipped
            if (obj->isEquipped()) {
                continue;
            }

            bool update = obj->equip(gGame->context().player());
            ok &= update;
            if (update) {
                gGame->notifier()->actionUpdate(this, "+"+obj->name());
            }

        } else {
            // Should not be equipped
            if (!obj->isEquipped()) {
                continue;
            }

            obj->unequip();
            gGame->notifier()->actionUpdate(this, "-"+obj->name());
        }
    }

    return ok;
}
