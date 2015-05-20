#ifndef GAMECONTEXT_H
#define GAMECONTEXT_H

#include <ahglobal.h>
#include "propertyvalue.h"
//#include "player.h"
#include "propertymodifier.h"

class Monster;
class Game;
class Player;
class Character;
class Gate;

class GameContext
{
public:
    GameContext(Game *g, Player *p, Monster *m,  AH::GamePhase ph)
        : m_game(g), m_player(p), m_monster(m), m_phase(ph) {}

    Player *player() { return m_player; }
    Monster *monster() { return m_monster; }
    Gate *gate() { return m_gate; }
    AH::GamePhase phase() { return m_phase; }

    void setMonster( Monster *m ) { m_monster = m; }
    void setGate( Gate *g ) { m_gate = g; }

    ModifiedPropertyValue getCurCharacterProperty(PropertyValue::Property property);
    ModifiedPropertyValue getCurCharacterSkill(AH::Skill skill);
    ModifiedPropertyValue getCurCharacterAttribute(AH::Attribute attr);
    ModifiedPropertyValue getCurCharacterClueBurn(AH::Skill skill);

    ModifiedPropertyValue getCharacterProperty(const Character *c, PropertyValue::Property property);
    ModifiedPropertyValue getCharacterSkill(const Character *c, AH::Skill skill);
    ModifiedPropertyValue getCharacterAttribute(const Character *c, AH::Attribute attr);
    ModifiedPropertyValue getCharacterClueBurn(const Character *c, AH::Skill skill);

    ModifiedPropertyValue getGameProperty(PropertyValue::Property property);

private:
    Game *m_game;
    Player *m_player;
    Monster *m_monster;
    Gate *m_gate;
    AH::GamePhase m_phase;

    friend class Game;
};

#endif // GAMECONTEXT_H
