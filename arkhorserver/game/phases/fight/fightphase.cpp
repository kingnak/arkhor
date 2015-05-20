#include "fightphase.h"
#include "evadeaction.h"
#include "fightaction.h"
#include "horroraction.h"
#include "chooseweaponsaction.h"
#include "attackaction.h"
#include "game/player.h"
#include "character.h"
#include "monster.h"

FightPhase::FightPhase()
    : GamePhase(gGame)
{
    m_evade = new EvadeOption(this);
    m_fight = new FightOption(this);
    m_horror = new HorrorOption(this);
    m_chooseWeapons = new ChooseWeaponsOption(this);
}

FightPhase::~FightPhase()
{
    delete m_evade;
    delete m_fight;
    delete m_horror;
    delete m_chooseWeapons;
    delete m_attack;
}

bool FightPhase::isFightSituation() const
{
    return gGame->context().player()->getCharacter()->field()->hasMonsters();
}

bool FightPhase::handleFight()
{
    AH::GamePhase returnPhase = gGame->context().phase();
    gGame->context().setPhase(AH::FightPhase);
    m_curPhase = FightEnter;
    execute();
    gGame->context().setPhase(returnPhase);
    return true;
}

void FightPhase::updatePhaseByResult(FightPhase::PhaseResult res)
{
    switch (res) {
    case CharacterFlown:
        m_flownMonsters.append(gGame->context().monster());
        m_curPhase = ChooseMonster;
        break;
    case EvadeFailed:
        damageStamina();
        m_curPhase = FightOrFlee;
        break;
    case FightChosen:
        if (m_curPhase == FightOrEvade) {
            // Goto Horror
            m_curPhase = Horror;
        } else {
            // Goto Choose Weapons
            m_curPhase = ChooseWeapons;
        }
        break;
    case HorrorFailed:
        damageHorror();
        m_curPhase = FightOrFlee;
        break;
    case HorrorSuccess:
        damageNightmarish();
        m_curPhase = FightOrFlee;
        break;
    case AttackFailed:
        damageStamina();
        m_curPhase = FightOrFlee;
        break;
    case MonsterKilled:
        damageOverwhelming();
        m_curPhase = ChooseMonster;
    }

    // TODO: Check character state here or elsewhere?

}

/*void FightPhase::execute()
{

}*/

QList<GameOption *> FightPhase::getPhaseOptions()
{
    switch (m_curPhase) {
    case FightEnter:
        return fightEnterOptions();
    case ChooseMonster:
        return chooseMonsterOptions();
    case FightOrEvade:
        return fightOrEvadeOptions();
    case Horror:
        return horrorOptions();
    case FightOrFlee:
        return fightOrEvadeOptions();
    case ChooseWeapons:
        return chooseWeaponsOptions();
    }

}

void FightPhase::damageStamina()
{
    gGame->context().player()->getCharacter()->damageStamina(gGame->context().monster()->combatDamage());
}

void FightPhase::damageHorror()
{
    gGame->context().player()->getCharacter()->damageSanity(gGame->context().monster()->horrorDamage());
}

void FightPhase::damageOverwhelming()
{
    AH::Common::MonsterData::MonsterAttributes attrs = gGame->context().monster()->attributes();
    int dmg = 0;
    if (attrs.testFlag(AH::Common::MonsterData::Overwhelming_1)) {
        dmg = 1;
    }
    if (attrs.testFlag(AH::Common::MonsterData::Overwhelming_2)) {
        dmg = 2;
    }
    if (attrs.testFlag(AH::Common::MonsterData::Overwhelming_3)) {
        dmg = 3;
    }
    if (dmg > 0) {
        gGame->context().player()->getCharacter()->damageStamina(dmg);
    }
}

void FightPhase::damageNightmarish()
{
    AH::Common::MonsterData::MonsterAttributes attrs = gGame->context().monster()->attributes();
    int dmg = 0;
    if (attrs.testFlag(AH::Common::MonsterData::Nightmarish_1)) {
        dmg = 1;
    }
    if (attrs.testFlag(AH::Common::MonsterData::Nightmarish_2)) {
        dmg = 2;
    }
    if (attrs.testFlag(AH::Common::MonsterData::Nightmarish_3)) {
        dmg = 3;
    }
    if (dmg > 0) {
        gGame->context().player()->getCharacter()->damageSanity(dmg);
    }
}

QList<GameOption *> FightPhase::fightEnterOptions()
{
    // TODO: Clear all state
    m_outcome = EndUnknown;
    return chooseMonsterOptions();
}

QList<GameOption *> FightPhase::chooseMonsterOptions()
{
    QList<Monster*> monsters = gGame->context().player()->getCharacter()->field()->monsters();
    // TODO: remove evaded/flown monsters
    if (monsters.isEmpty()) {
        m_outcome = EndSuccess;
        return QList<GameOption*> ();
    }

    if (monsters.size() > 1) {
        // TODO: Let user choose monster

    }
    Monster *m = monsters.first();
    gGame->context().setMonster(m);
    return fightOrEvadeOptions();
}

QList<GameOption *> FightPhase::fightOrEvadeOptions()
{
    m_curPhase = FightOrEvade;
    return QList<GameOption *>()
            << m_evade << m_fight;
}

QList<GameOption *> FightPhase::horrorOptions()
{
    m_curPhase = Horror;
    return QList<GameOption *> ()
            << m_horror;
}

QList<GameOption *> FightPhase::chooseWeaponsOptions()
{
    m_curPhase = ChooseWeapons;
    return QList<GameOption *> ()
            << m_chooseWeapons << m_attack;
}


