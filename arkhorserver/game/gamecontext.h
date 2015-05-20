#ifndef GAMECONTEXT_H
#define GAMECONTEXT_H

#include <ahglobal.h>
#include "propertyvalue.h"
//#include "player.h"
#include "propertymodifier.h"
#include "dietesthelper.h"
#include "monsterdata.h"

class Monster;
class Game;
class Player;
class Character;
class Gate;
class AncientOne;

class GameContext
{
public:
    GameContext(Game *g, Player *p, Monster *m,  AH::GamePhase ph)
        : m_game(g), m_player(p), m_monster(m), m_phase(ph), m_dieRoll(NULL) {}

    Player *player() { return m_player; }
    Monster *monster() { return m_monster; }
    Gate *gate() { return m_gate; }
    AH::GamePhase phase() { return m_phase; }
    DieTestHelper::DieTestSpec *dieRoll() { return m_dieRoll; }
    AncientOne *ancientOne();

    void setMonster( Monster *m ) { m_monster = m; }
    void setGate( Gate *g ) { m_gate = g; }
    void setPhase( AH::GamePhase phase ) { m_phase = phase; }
    void setDieRoll( DieTestHelper::DieTestSpec * spec ) { m_dieRoll = spec; }

    ModifiedPropertyValue getCurCharacterProperty(PropertyValue::Property property);
    ModifiedPropertyValue getCurCharacterSkill(AH::Skill skill);
    ModifiedPropertyValue getCurCharacterAttribute(AH::Attribute attr);
    ModifiedPropertyValue getCurCharacterClueBurn(AH::Skill skill);
    ModifiedPropertyValue getCurCharacterDrawObject(AH::GameObjectType type);

    ModifiedPropertyValue getCharacterProperty(const Character *c, PropertyValue::Property property);
    ModifiedPropertyValue getCharacterSkill(const Character *c, AH::Skill skill);
    ModifiedPropertyValue getCharacterAttribute(const Character *c, AH::Attribute attr);
    ModifiedPropertyValue getCharacterClueBurn(const Character *c, AH::Skill skill);
    ModifiedPropertyValue getCharacterDrawObject(const Character *c, AH::GameObjectType type);

    bool checkCurCharacterIgnoreMonsterAttribute(AH::Common::MonsterData::MonsterAttribute attr);
    bool checkCharacterIgnoreMonsterAttribute(const Character *c, AH::Common::MonsterData::MonsterAttribute attr);
    AH::Common::MonsterData::MonsterAttributes getCurCharacterIgnoredMonsterAttributes();
    AH::Common::MonsterData::MonsterAttributes getCharacterIgnoredMonsterAttributes(const Character *c);

    ModifiedPropertyValue getCurMonsterProperty(PropertyValue::Property property);
    ModifiedPropertyValue getMonsterProperty(const Monster *m, PropertyValue::Property property);

    ModifiedPropertyValue getGameProperty(PropertyValue::Property property);

private:
    Game *m_game;
    Player *m_player;
    Monster *m_monster;
    Gate *m_gate;
    AH::GamePhase m_phase;
    DieTestHelper::DieTestSpec *m_dieRoll;

    friend class Game;
};

#endif // GAMECONTEXT_H
