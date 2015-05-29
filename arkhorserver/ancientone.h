#ifndef ANCIENTONE_H
#define ANCIENTONE_H

#include <ancientonedata.h>
#include "game/propertymodifier.h"

class Character;

class AncientOne : public AH::Common::AncientOneData
{
public:
    AncientOne();

    void increaseDoomTrack(int amount = 1);
    void decreaseDoomTrack(int amount = 1) { increaseDoomTrack(-amount); }

    bool isDirty() const { return m_dirty; }
    void setDirty(bool dirty = true) { m_dirty = dirty; }

    virtual PropertyModificationList getSlumberModifications() const { return PropertyModificationList(); }
    virtual PropertyModificationList getMonsterModifications(QString typeId) const { Q_UNUSED(typeId) return PropertyModificationList(); }
    /** Called at end of Mythos Phase */
    virtual void mythosPower() {}

    virtual void awake();

    virtual void attack() = 0;
    virtual void newAttackRound();

    void damage(int amount);

    virtual void decreaseAttackAdjustment(int amount) { m_attackAdjustment -= amount; setDirty(); }

protected:
    bool m_awake;
    bool m_dirty;

    int m_damageThreshold;
    int m_damage;
};

#endif // ANCIENTONE_H
