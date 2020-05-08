#ifndef GAMECONTEXT_H
#define GAMECONTEXT_H

#include <ahglobal.h>
#include "propertyvalue.h"
//#include "player.h"
#include "propertymodifier.h"
#include "dietesthelper.h"
#include "monsterdata.h"
#include <choicedata.h>

class Monster;
class Game;
class Player;
class Character;
class Gate;
class AncientOne;
class MythosCard;
class GameObject;

class GameContext
{
public:
    GameContext(Game *g, Player *p, Monster *m,  AH::GamePhase ph)
        : m_game(g), m_player(p), m_monster(m), m_gate(NULL), m_phase(ph), m_dieRoll(NULL) {}

    Player *player() { return m_player; }
    Monster *monster() { return m_monster; }
    Gate *gate() { return m_gate; }
    AH::GamePhase phase() { return m_phase; }
    DieTestHelper::DieTestSpec *dieRoll() { return m_dieRoll; }
    AncientOne *ancientOne();
    MythosCard *environment();
    MythosCard *rumor();

    void setMonster( Monster *m ) { m_monster = m; }
    void setGate( Gate *g ) { m_gate = g; }
    void setPhase( AH::GamePhase phase ) { m_phase = phase; }
    void setDieRoll( DieTestHelper::DieTestSpec * spec ) { m_dieRoll = spec; }

    //void updateCurMonsterVisibilityType(AH::Common::MonsterData::DisplayType dt);
    //AH::Common::MonsterData::DisplayType getMonsterDisplayType(const Monster *m);

    QString selectChoice(QString desc, const QString &sourceId, QList<AH::Common::ChoiceData::OptionData> options, bool canCancel);
    QString selectChoice(Player *p, QString desc, const QString &sourceId, QList<AH::Common::ChoiceData::OptionData> options, bool canCancel);
    QList<GameObject *> selectObjects(Player *p, const QList<GameObject *> objects, const QString &desc, const QString &sourceId, int min, int max);

    ModifiedPropertyValue getCurCharacterProperty(PropertyValue::Property property);
    ModifiedPropertyValue getCurCharacterSkill(AH::Skill skill);
    ModifiedPropertyValue getCurCharacterAttribute(AH::Attribute attr);
    ModifiedPropertyValue getCurCharacterClueBurn(AH::Skill skill);
    ModifiedPropertyValue getCurCharacterDieRoll(PropertyValue::Property property);
    ModifiedPropertyValue getCurCharacterDrawObject(AH::GameObjectType type);
    ModifiedPropertyValue getCurCharacterDrawArkhamEncounter();
    ModifiedPropertyValue getCurCharacterDrawOtherWorldEncounter();

    ModifiedPropertyValue getCharacterProperty(const Character *c, PropertyValue::Property property);
    ModifiedPropertyValue getCharacterSkill(const Character *c, AH::Skill skill);
    ModifiedPropertyValue getCharacterAttribute(const Character *c, AH::Attribute attr);
    ModifiedPropertyValue getCharacterClueBurn(const Character *c, AH::Skill skill);
    ModifiedPropertyValue getCharacterDieRoll(const Character *c, PropertyValue::Property property);
    ModifiedPropertyValue getCharacterDrawObject(const Character *c, AH::GameObjectType type);
    ModifiedPropertyValue getCharacterDrawArkhamEncounter(const Character *c);
    ModifiedPropertyValue getCharacterDrawOtherWorldEncounter(const Character *c);

    bool checkCurCharacterIgnoreMonsterAttribute(AH::Common::MonsterData::MonsterAttribute attr);
    bool checkCharacterIgnoreMonsterAttribute(const Character *c, AH::Common::MonsterData::MonsterAttribute attr);
    AH::Common::MonsterData::MonsterAttributes getCurCharacterIgnoredMonsterAttributes();
    AH::Common::MonsterData::MonsterAttributes getCharacterIgnoredMonsterAttributes(const Character *c);

    ModifiedPropertyValue getCurMonsterProperty(PropertyValue::Property property);
    ModifiedPropertyValue getMonsterProperty(const Monster *m, PropertyValue::Property property);

    ModifiedPropertyValue getGameProperty(PropertyValue::Property property);

private:
    ModifiedPropertyValue getCharacterDraw(const Character *c, PropertyValue::Property prop);

private:
    Game *m_game;
    Player *m_player;
    Monster *m_monster;
    Gate *m_gate;
    AH::GamePhase m_phase;
    DieTestHelper::DieTestSpec *m_dieRoll;

    typedef QMap<QString, AH::Common::MonsterData::DisplayType> MonsterDisplayUpdates;
    typedef QMap<Player *, MonsterDisplayUpdates> PlayerMonsterDisplayUpdates;
    PlayerMonsterDisplayUpdates m_permanentDisplayUpdates;
    PlayerMonsterDisplayUpdates m_tempDisplayUpdates;

    friend class Game;
};

#endif // GAMECONTEXT_H
