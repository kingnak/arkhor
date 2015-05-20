#include "gameoption.h"
#include "game.h"
#include "cost.h"
#include "character.h"
#include "player.h"
#include <QString>
#include <QDebug>

using namespace AH::Common;

AH::GamePhases GameOption::phases() const
{
    if (m_action)
        return m_action->phases();
    return m_phases;
}

bool GameOption::execute()
{
    if (isAvailable()) {
        Cost c = costs();
        // PAY!
        if (!c.getAlternatives().isEmpty()) {
            CostList selected = gGame->context().player()->choosePayment(c);
            if (selected.isValid() && gGame->context().player()->getCharacter()->canPay(selected)) {
                if (gGame->context().player()->getCharacter()->pay(selected)) {
                    return m_action->execute();
                }
            }
            return false;
        } else {
            return m_action->execute();
        }
    }
    return false;
}

QString GameOption::name() const
{
    if (m_action)
        return m_action->name();
    return m_name;
}

QString GameOption::description() const
{
    if (m_action)
        return m_action->description();
    return m_description;
}

bool GameOption::resolveDependencies(const Game *game)
{
    if (m_action) return true;
    if (m_actionId.isEmpty()) return true; // ??
    GameAction *a = game->findActionById(m_actionId);
    if (a) {
        m_action = a;
        return true;
    }
    qWarning() << "Cannot resolve action" << m_actionId << "for object" << id();
    return false;
}
