#include "upkeep.h"
#include "../gameaction.h"
#include "../gameoption.h"
#include "../gameobject.h"
#include "../../character.h"
#include "../player.h"

Upkeep::Upkeep(Game *game)
    : GamePhase(game)
{
    m_untap = new UntapAction;
    m_focus = new FocusOption;
}

Upkeep::~Upkeep()
{
    delete m_focus;
}

QList<GameAction *> Upkeep::getPhaseActions()
{
    return QList<GameAction *>()
            << m_untap;
}

QList<GameOption *> Upkeep::getPhaseOptions()
{
    QList<GameOption *> op;
    op << m_focus;
    op << getSkipOption();
    return op;
}

void Upkeep::enterPhase()
{
    gGame->context().player()->getCharacter()->setFocusAmount(gGame->context().player()->getCharacter()->getMaximumFocus());
}

