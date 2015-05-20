#include "gamefieldscript.h"
#include "game/game.h"

GameFieldScript::GameFieldScript(QObject *parent) :
    QObject(parent), m_field(NULL)
{
}

void GameFieldScript::lockUntilNextRound()
{
    quint32 lockFlag = gGame->context().phase();
    m_field->lock(lockFlag);
}
