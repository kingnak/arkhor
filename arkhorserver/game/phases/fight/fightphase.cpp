#include "fightphase.h"
#include "evadeaction.h"
#include "fightaction.h"
#include "horroraction.h"
#include "choosemonsteraction.h"
#include "chooseweaponsaction.h"
#include "attackaction.h"
#include "game/player.h"
#include "character.h"
#include "monster.h"
#include "game/gameobject.h"

FightPhase::FightPhase()
    : GamePhase(gGame)
{
    m_chooseMonster = new ChooseMonsterOption(this);
    m_evade = new EvadeOption(this);
    m_fight = new FightOption(this);
    m_horror = new HorrorOption(this);
    m_chooseWeapons = new ChooseWeaponsOption(this);
    m_attack = new AttackOption(this);
    m_endOption = new EndOption;
}

FightPhase::~FightPhase()
{
    delete m_chooseMonster;
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

    // Trick: dynamic monsters will be invalidated when requesting attrs/mods
    for (auto m : m_monsters) {
        m->attributes();
        m->getModifications();
    }

    AH::GamePhase returnPhase = gGame->context().phase();
    gGame->context().setPhase(AH::FightPhase);
    m_curPhase = FightEnter;
    m_outcome = EndUnknown;

    execute();

    // Defeated was already called in AttackAction.
    // Call EndCombat on all flown / evaded monsters
    for (auto m : m_flownMonsters) {
        m->endCombat();
    }

    m_monsters.clear();
    m_flownMonsters.clear();

    gGame->context().setPhase(returnPhase);
    gGame->context().setMonster(NULL);

    // Unequip all spells
    unequipSpells();

    if (m_outcome == EndEvaded) {
        // Can continue phase
        return true;
    }
    return false;
}

void FightPhase::updatePhaseByResult(FightPhase::PhaseResult res)
{
    bool canContinueFight = true;
    switch (res) {
    case NoResult:
        break;
    case CharacterFlown:
        m_flownMonsters.append(gGame->context().monster());
        m_curPhase = ChooseMonster;
        break;
    case MonsterChosen:
        if (m_hasEnteredFight)
            m_curPhase = FightOrFlee;
        else
            m_curPhase = FightOrEvade;
        break;
    case EvadeFailed:
        //m_hadFailedEvade = true;
        canContinueFight = damageStamina();
        m_curPhase = FightOrFlee;
        break;
    case FightChosen:
        //gGame->context().updateCurMonsterVisibilityType(AH::Common::MonsterData::FullBack);
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
    case HorrorPassed:
        m_curPhase = FightOrFlee;
        break;
    case HorrorFailed:
        canContinueFight = damageHorror();
        m_curPhase = FightOrFlee;
        break;
    case HorrorSuccess:
        canContinueFight = damageNightmarish();
        m_curPhase = FightOrFlee;
        break;
    case AttackFailed:
        canContinueFight = damageStamina();
        m_curPhase = FightOrFlee;
        break;
    case MonsterKilled:
        canContinueFight = damageOverwhelming();
        m_curPhase = ChooseMonster;
        // Remove monster
        m_monsters.removeAll(gGame->context().monster());
        // Unequip all spells
        unequipSpells();
        break;
    }

    if (!gGame->context().player()->getCharacter()->commitDamage())
    {
        m_outcome = EndFailed;
    }
    if (!canContinueFight) {
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

bool FightPhase::damageStamina()
{
    gGame->context().player()->getCharacter()->damageStamina(gGame->context().monster()->combatDamage());
    return gGame->context().monster()->damage(gGame->context().player()->getCharacter(), Monster::Combat);
}

bool FightPhase::damageHorror()
{
    gGame->context().player()->getCharacter()->damageSanity(gGame->context().monster()->horrorDamage());
    return gGame->context().monster()->damage(gGame->context().player()->getCharacter(), Monster::Horror);
}

bool FightPhase::damageOverwhelming()
{
    AH::Common::MonsterData::MonsterAttributes attrs = gGame->context().monster()->attributes();
    if (gGame->context().checkCurCharacterIgnoreMonsterAttribute(AH::Common::MonsterData::Overwhelming_ALL)) {
        return true;
    }

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
    return gGame->context().monster()->damage(gGame->context().player()->getCharacter(), Monster::Overwhelm);
}

bool FightPhase::damageNightmarish()
{
    AH::Common::MonsterData::MonsterAttributes attrs = gGame->context().monster()->attributes();
    if (gGame->context().checkCurCharacterIgnoreMonsterAttribute(AH::Common::MonsterData::Nightmarish_ALL)) {
        return true;
    }

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
    return gGame->context().monster()->damage(gGame->context().player()->getCharacter(), Monster::Nightmare);
}

void FightPhase::unequipSpells()
{
    QList<GameObject *> lst = gGame->context().player()->getCharacter()->inventory();
    for (auto obj : lst) {
        if (obj->isFightObject()) {
            if (obj->type() == AH::Obj_Spell) {
                if (obj->isEquipped()) {
                    obj->unequip();
                }
            }
        }
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
    for (Monster *m : m_flownMonsters) {
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
        // Let user choose monster
        m_chooseMonster->setMonsters(monsters);
        return QList<GameOption *> ()
                << m_chooseMonster;
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
    return attackOptions();
}

QList<GameOption *> FightPhase::horrorOptions()
{
    m_hasEnteredFight = true;
    m_curPhase = Horror;

    if (!m_horror->isAvailable()) {
        // Skip a horror test that does nothing, just succeed it
        return fightOrFleeOptions();
    }

    return QList<GameOption *> ()
            << m_horror;
}

QList<GameOption *> FightPhase::chooseWeaponsOptions()
{
    return attackOptions();
}

QList<GameOption *> FightPhase::attackOptions()
{
    m_hasEnteredFight = true;
    m_evade->setName("Flee");
    m_curPhase = ChooseWeapons;
    return QList<GameOption *> ()
            << m_chooseWeapons << m_attack << m_evade;
}


