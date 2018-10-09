#include "gamefieldscript.h"
#include "game/game.h"
#include "characterscript.h"
#include <QDebug>

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

QObjectList GameFieldScript::characters()
{
    Q_ASSERT(m_field);
    QObjectList ret;
    foreach (Character *c, m_field->characters()) {
        CharacterScript *cs = dynamic_cast<CharacterScript *> (c);
        if (cs) {
            ret << cs;
        } else {
            qCritical() << "Non-Script Character detected in GameContextScript::getAllCharacters";
        }
    }

    return ret;
}
