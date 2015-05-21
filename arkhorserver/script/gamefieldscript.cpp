#include "gamefieldscript.h"
#include "game/game.h"

GameFieldScript::GameFieldScript(QObject *parent) :
    QObject(parent), m_field(NULL)
{
}

void GameFieldScript::lockUntilNextRound()
{
    Q_ASSERT(m_field);
    quint32 lockFlag = gGame->context().phase();
    m_field->lock(lockFlag);
}
