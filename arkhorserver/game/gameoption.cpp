#include "gameoption.h"
#include "game.h"
#include "cost.h"
#include "character.h"
#include "player.h"
#include <QString>
#include <QDebug>

using namespace AH::Common;

GameOptionData *GameOption::data()
{
    //m_id = id();
    m_name = name();
    m_description = description();
    //m_isAvailable = isAvailable();
    //m_costs = costs();
    m_canPay = gGame->context().player()->getCharacter()->canPay(costs());
    return GameOptionData::data();
}

AH::GamePhases GameOption::phases() const
{
    if (action())
        return action()->phases();
    return m_phases;
}

bool GameOption::execute()
{
    if (isAvailable()) {
        Cost c = costs();
        // PAY!
        if (!c.getAlternatives().isEmpty()) {
            /*
            CostList selected;
            if (c.getAlternatives().size() > 1) {
                selected = gGame->context().player()->choosePayment(c);
            } else {
                selected = c.getAlternatives().first();
            }
            if (selected.isValid() && gGame->context().player()->getCharacter()->canPay(selected)) {
                if (gGame->context().player()->getCharacter()->pay(selected)) {
                    return action()->execute();
                }
            }
            */
            if (gGame->context().player()->getCharacter()->pay(c)) {
                return action()->execute();
            }
            return false;
        } else {
            return action()->execute();
        }
    }
    return false;
}

QString GameOption::name() const
{
    if (action())
        return action()->name();
    return m_name;
}

QString GameOption::description() const
{
    if (action())
        return action()->description();
    return m_description;
}

bool GameOption::resolveDependencies(GameRegistry *reg)
{
    if (m_action) return true;
    if (m_actionId.isEmpty()) return true; // ??
    GameAction *a = reg->findActionById(m_actionId);
    if (a) {
        m_action = a;
        return true;
    }
    qWarning() << "Cannot resolve action" << m_actionId << "for object" << id();
    return false;
}
