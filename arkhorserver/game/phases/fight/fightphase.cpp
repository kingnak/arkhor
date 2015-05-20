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
    m_attack = new AttackOption(this);
    m_endOption = new EndOption;
}

FightPhase::~FightPhase()
{
    delete m_evade;
    delete m_fight;
    delete m_horror;
    delete m_chooseWeapons;
    delete m_attack;
    delete m_endOption;
}

bool FightPhase::isFightSituation() const
{
    return gGame->context().player()->getCharacter()->field()->hasMonsters();
}

bool FightPhase::handleFight(QList<Monster *> monsters)
{
    m_monsters = monsters;
    AH::GamePhase returnPhase = gGame->context().phase();
    gGame->context().setPhase(AH::FightPhase);
    m_curPhase = FightEnter;
    m_outcome = EndUnknown;

    execute();

    // Defeated was already called in AttackAction.
    // Call EndCombat on all flown / evaded monsters
    foreach (Monster *m, m_flownMonsters) {
        m->endCombat();
    }

    m_monsters.clear();
    m_flownMonsters.clear();

    gGame->context().setPhase(returnPhase);
    gGame->context().setMonster(NULL);

    if (m_outcome == EndEvaded) {
        // Can continue phase
        return true;
    }
    return false;
}

void FightPhase::updatePhaseByResult(FightPhase::PhaseResult res)
{
    switch (res) {
    case NoResult:
        break;
    case CharacterFlown:
        m_flownMonsters.append(gGame->context().monster());
        m_curPhase = ChooseMonster;
        break;
    case EvadeFailed:
        //m_hadFailedEvade = true;
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
    case WeaponsChosen:
        m_curPhase = ChooseWeapons;
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
        // Remove monster
        m_monsters.removeAll(gGame->context().monster());
    }

    if (!gGame->context().player()->getCharacter()->commitDamage())
    {
        m_outcome = EndFailed;
    }

    gGame->commitUpdates();
}

QList<GameOption *> FightPhase::getPhaseOptions()
{
    if (m_outcome == EndFailed) {
        return QList<GameOption *>() << m_endOption->setName("Failed")->setDesc("You have lost the fight");
    } else if (m_outcome == EndSuccess) {
        return QList<GameOption *>() << m_endOption->setName("Success")->setDesc("You have won the fight");
    }

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
        return fightOrFleeOptions();
    case ChooseWeapons:
        return chooseWeaponsOptions();
    }

    Q_ASSERT_X(false, "FightPhase::getPhaseOptions", "Invalid fight phase detected");
    return QList<GameOption *> ();
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
    m_hasEnteredFight = false;
    //m_hadFailedEvade = false;
    m_flownMonsters.clear();
    return chooseMonsterOptions();
}

QList<GameOption *> FightPhase::chooseMonsterOptions()
{
    QList<Monster *> monsters = m_monsters;
    // remove evaded/flown monsters
    foreach (Monster *m, m_flownMonsters) {
        monsters.removeAll(m);
    }

    if (monsters.isEmpty()) {
        // Case 1: never entered fight => Evaded all monsters
        if (!m_hasEnteredFight) {
            m_outcome = EndEvaded;
            return QList<GameOption*> () << m_endOption->setName("Success")->setDesc("You have evaded all monsters");
        }
        // Case 2: not flown from any monster => Defeated all
        if (m_flownMonsters.isEmpty()) {
            m_outcome = EndSuccess;
            return QList<GameOption*> () << m_endOption->setName("Success")->setDesc("You have defeated all monsters");
        }
        // Case 3: flown from monsters => Defeated / Flown from all
        m_outcome = EndFlown;
        if (m_flownMonsters.size() == m_monsters.size()) {
            // Flown from all
            return QList<GameOption*> () << m_endOption->setName("Success")->setDesc("You have flown from all monsters");
        } else {
            // Flown from some, defeated others
            return QList<GameOption*> () << m_endOption->setName("Success")->setDesc("You have defeated and flown from all monsters");
        }
        /*
        if (m_triedEvade && !m_hadFailedEvade) {
            m_outcome = EndFlown;
            return QList<GameOption*> () << m_endOption->setName("Success")->setDesc("You have flown from all monsters");
        } else {
            m_outcome = EndSuccess;
            return QList<GameOption*> () << m_endOption->setName("Success")->setDesc("You have defeated all monsters");
        }
        */
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
    m_evade->setName("Evade");
    return QList<GameOption *>()
            << m_evade << m_fight;
}

QList<GameOption *> FightPhase::fightOrFleeOptions()
{
    m_hasEnteredFight = true;
    m_curPhase = FightOrFlee;
    m_evade->setName("Flee");
    return QList<GameOption *>()
            << m_evade << m_fight;
}

QList<GameOption *> FightPhase::horrorOptions()
{
    m_hasEnteredFight = true;
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


