#ifndef FIGHTPHASE_H
#define FIGHTPHASE_H

#include <QList>
#include "game/gameoption.h"
#include "game/gamephase.h"

class EvadeOption;
class FightOption;
class HorrorOption;
class ChooseWeaponsOption;
class AttackOption;

class FightPhase : public GamePhase
{
public:
    FightPhase();
    ~FightPhase();

    bool isFightSituation() const;
    bool handleFight();

    //void execute();

    enum PhaseResult {
        NoResult,
        CharacterFlown,
        EvadeFailed,
        FightChosen,
        HorrorFailed,
        HorrorSuccess,
        WeaponsChosen,
        MonsterKilled,
        AttackFailed,
    };

    void updatePhaseByResult(PhaseResult res);

protected:
    //virtual void enterPhase();
    //virtual QList<GameAction *> getPhaseActions();
    virtual QList<GameOption *> getPhaseOptions();

private:

    enum {
        FightEnter,
        ChooseMonster,
        FightOrEvade,
        Horror,
        FightOrFlee,
        ChooseWeapons,
    } m_curPhase;

    enum {
        EndUnknown,
        EndSuccess,
        EndFailed
    } m_outcome;

    void damageStamina();
    void damageHorror();
    void damageOverwhelming();
    void damageNightmarish();

    QList<GameOption *> fightEnterOptions();
    QList<GameOption *> chooseMonsterOptions();
    QList<GameOption *> fightOrEvadeOptions();
    QList<GameOption *> horrorOptions();
    QList<GameOption *> chooseWeaponsOptions();


private:
    QList<Monster *> m_flownMonsters;

    EvadeOption *m_evade;
    FightOption *m_fight;
    HorrorOption *m_horror;
    ChooseWeaponsOption *m_chooseWeapons;
    AttackOption *m_attack;

};

#endif // FIGHTPHASE_H
