#include "upkeep.h"
#include "../gameaction.h"
#include "../gameoption.h"
#include "../gameobject.h"
#include "../../character.h"
#include "../player.h"

Upkeep::Upkeep(Game *game)
    : GamePhase(game)
{
    m_setout = new SetOutAction;
    m_untap = new UntapAction;
    m_focus = new FocusOption;
    m_unlock = new UnlockAction;
}

Upkeep::~Upkeep()
{
    delete m_setout;
    delete m_untap;
    delete m_focus;
    delete m_unlock;
}

QList<GameAction *> Upkeep::getPhaseActions()
{
    return QList<GameAction *>()
            << m_setout
            << m_untap
            << m_unlock;
}

QList<GameOption *> Upkeep::getPhaseOptions()
{
    QList<GameOption *> op;
    op << getSkipOption();
    op << m_focus;
    return op;
}

void Upkeep::enterPhase()
{
    gGame->context().player()->getCharacter()->setFocusAmount(gGame->context().player()->getCharacter()->getMaximumFocus());
}

