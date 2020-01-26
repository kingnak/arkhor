#include "choosemonsteraction.h"
#include "game/game.h"
#include "game/player.h"
#include "fightphase.h"
#include "monster.h"

ChooseMonsterAction::ChooseMonsterAction(FightPhase *fight)
    : m_fight(fight)
{

}

bool ChooseMonsterAction::execute()
{
    gGame->notifier()->actionStart(this);

    QList<AH::Common::MonsterData> monsters;
    for (auto m : m_monsters)
        monsters << *m->data();

    QString id = gGame->context().player()->chooseMonster(monsters);
    if (id.isNull()) {
        id = m_monsters.value(0)->id();
    }
    Monster *sel = nullptr;
    for (auto m : m_monsters) {
        if (m->id() == id) {
            sel = m;
            break;
        }
    }

    if (!sel)
        sel = m_monsters.value(0);

    gGame->context().setMonster(sel);
    m_fight->updatePhaseByResult(FightPhase::MonsterChosen);

    return true;
}

QString ChooseMonsterAction::notificationString(GameAction::NotificationPart part, const QString &desc) const
{
    Q_UNUSED(part)
    Q_UNUSED(desc)
    return QString::null;
}
