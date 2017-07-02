#include "otherworldencountery.h"
#include "../actions/otherworldencounteryaction.h"

OtherWorldEncountery::OtherWorldEncountery(Game *game)
    : GamePhase(game)
{
    m_enc = new OtherWorldEncounteryOption();
}

OtherWorldEncountery::~OtherWorldEncountery()
{
    delete m_enc;
}

QList<GameOption *> OtherWorldEncountery::getPhaseOptions()
{
    if (m_enc->isAvailable())
        return QList<GameOption *>() << m_enc;
    else
        return QList<GameOption *>() << getSkipOption();
}

void OtherWorldEncountery::finishPhase()
{
    gGame->notifier()->clearTempData();
}
