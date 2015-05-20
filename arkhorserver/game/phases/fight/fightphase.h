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

    enum FightOutcome {
        EndUnknown,
        EndSuccess,
        EndFlown,
        EndEvaded,
        EndFailed
    };

    FightOutcome getOutcome() const { return m_outcome; }

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

    FightOutcome m_outcome;

    void damageStamina();
    void damageHorror();
    void damageOverwhelming();
    void damageNightmarish();

    QList<GameOption *> fightEnterOptions();
    QList<GameOption *> chooseMonsterOptions();
    QList<GameOption *> fightOrEvadeOptions();
    QList<GameOption *> fightOrFleeOptions();
    QList<GameOption *> horrorOptions();
    QList<GameOption *> chooseWeaponsOptions();


private:
    QList<Monster *> m_flownMonsters;
    bool m_hasEnteredFight;
    //bool m_hadFailedEvade;

    EvadeOption *m_evade;
    FightOption *m_fight;
    HorrorOption *m_horror;
    ChooseWeaponsOption *m_chooseWeapons;
    AttackOption *m_attack;

    struct EndOption : public GameOption {
        EndOption() : GameOption(NULL, AH::CannotContinue, AH::ChooseMandatory) {}
        QString id() const { return "OP_END_FIGHT"; }
        bool isAvailable() const { return true; }
        bool execute() { return true; }
        QString name() const { return m_name; }
        QString description() const { return m_desc; }

        EndOption *setName(QString n) { m_name = n; return this; }
        EndOption *setDesc(QString d) { m_desc = d; return this; }

        QString m_name;
        QString m_desc;
    } *m_endOption;

};

#endif // FIGHTPHASE_H
