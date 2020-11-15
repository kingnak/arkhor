#ifndef DIEROLLACTION_H
#define DIEROLLACTION_H

#include "game/gameoption.h"
#include "game/gameaction.h"
#include "game/gameobject.h"

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
        ReRollOneFailed,
        AutoSucceed
    };

    DieRollOption(ReRollType type, QList<AH::Skill> skills)
        : m_type(type), m_skills(skills), m_used(false), m_discardAfterUse(false), m_source(NULL) {}

    virtual AH::GamePhases phases() const { return AH::DieRoll; }
    virtual bool isAvailable() const;
    virtual bool execute();

    QString name() const;
    QString description() const;
    QString sourceId() const;

    void setCosts(const AH::Common::Cost &costs) { m_costs = costs; }
    void setDiscardAfterUse(bool discard) { m_discardAfterUse = discard; }

    ReRollType type() const { return m_type; }
    QList<AH::Skill> skills() const { return m_skills; }
    bool wasUsed() const { return m_used; }

    void setSource(GameObject *obj) { m_source = obj; }

    void reset() { m_used = false; m_source = NULL; }

    static quint32 nextId() { return ++s_nextId; }

private:
    ReRollType m_type;
    QList<AH::Skill> m_skills;
    bool m_used;
    bool m_discardAfterUse;
    GameObject *m_source;
    static quint32 s_nextId;
};

#endif // DIEROLLACTION_H
