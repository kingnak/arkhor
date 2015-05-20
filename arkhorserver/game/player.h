#ifndef PLAYER_H
#define PLAYER_H

#include <QList>
#include "propertymodifier.h"
#include <ahglobal.h>
#include <playerdata.h>
#include "game/game.h"
#include "gamenotifier.h"
#include "gameinteractor.h"

class Character;

class Player : public AH::Common::PlayerData, public GameNotifier, public GameInteractor
{
public:
    Player();

    void setId(const QString &id) { m_id = id; }

    bool isActive() const { return m_active; }
    void deactivate() { m_active = false; }
    virtual void removedFromGame() {}

    Character *getCharacter();

    void setCharacter(Character *c) { m_character = c; }

private:
    bool m_active;
    Character *m_character;
};

#endif // PLAYER_H
