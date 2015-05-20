#ifndef DIEROLLACTION_H
#define DIEROLLACTION_H

#include "game/gameoption.h"
#include "game/gameaction.h"
/*
class DieRollAction : public GameAction
{
public:
    enum ReRollType {
        ReRollAll,
        ReRollAllFailed,
        ReRollOneFailed
    };

    DieRollAction(ReRollType type, AH::Skills skills)
        : m_type(type), m_skills(skills) {}

    bool execute();

    ReRollType type() const { return m_type; }
    AH::Skills skills() const { return m_skills; }

private:
    ReRollType m_type;
    AH::Skills m_skills;
};

class DieRollOption : public GameOption
{
public:
    DieRollOption(DieRollAction::ReRollType type, AH::Skills skills)
        : dra(type, skills) {}

    DieRollAction::ReRollType type() const { return dra.type(); }
    AH::Skills skills() const { return dra.skills(); }

private:
    DieRollAction dra;
};
*/

class DieRollOption : public GameOption
{
public:
    enum ReRollType {
        ReRollAll = 1,
        ReRollAllFailed,
        ReRollOneFailed
    };

    DieRollOption(ReRollType type, AH::Skills skills)
        : m_type(type), m_skills(skills), m_used(false) {}

    virtual AH::GamePhases phases() const { return AH::DieRoll; }
    virtual bool isAvailable() const;
    virtual bool execute();

    QString name() const;
    QString description() const;

    ReRollType type() const { return m_type; }
    AH::Skills skills() const { return m_skills; }
    bool wasUsed() const { return m_used; }

    void reset() { m_used = false; }

private:
    ReRollType m_type;
    AH::Skills m_skills;
    bool m_used;
};

#endif // DIEROLLACTION_H
