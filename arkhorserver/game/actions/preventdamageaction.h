#ifndef PREVENTDAMAGEACTION_H
#define PREVENTDAMAGEACTION_H

#include <game/gameoption.h>

class GameObject;

class PreventDamageOption : public GameOption
{
public:
    PreventDamageOption() : m_source(NULL) {}

    void setSource(GameObject *obj) { m_source = obj; }

    virtual QString name() const;
    virtual QString description() const;
    virtual AH::GamePhases phases() const { return AH::CommitDamagePhase; }
    virtual bool isAvailable() const;
    virtual bool execute() { return false; }
    virtual bool execute(int damageStamina, int damageSanity) = 0;
    virtual int preventedStamina() const = 0;
    virtual int preventedSanity() const = 0;

    virtual bool canPreventStamina() const = 0;
    virtual bool canPreventSanity() const = 0;

    QString sourceId() const;

    static quint32 nextId() { return ++s_nextId; }

    void updateOverrideDescription(int damageSta, int damageSan);

    void reset();

protected:
    GameObject *m_source;
    QString m_overrideDescription;
    QString m_baseDescription;
    static quint32 s_nextId;
};

#endif // PREVENTDAMAGEACTION_H
