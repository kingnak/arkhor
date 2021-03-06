#ifndef ANCIENTONE_H
#define ANCIENTONE_H

#include <ancientonedata.h>
#include "game/propertymodifier.h"
#include "game/monstermodifier.h"
#include "mythosdata.h"

class Character;

class AncientOne : public AH::Common::AncientOneData, public PropertyModifier, protected MonsterModifier
{
public:
    AncientOne();

    void increaseDoomTrack(int amount = 1);
    void decreaseDoomTrack(int amount = 1) { increaseDoomTrack(-amount); }

    bool isDirty() const { return m_dirty; }
    void setDirty(bool dirty = true) { m_dirty = dirty; }

    QString modifierId() const override { return m_id; }
    PropertyModificationList getModifications() const override;

    virtual PropertyModificationList getCombatModifications() const;
    virtual PropertyModificationList getSlumberModifications() const { return PropertyModificationList(); }
    /*
    virtual PropertyModificationList getMonsterModifications(QString typeId) const { Q_UNUSED(typeId) return PropertyModificationList(); }
    virtual PropertyModificationList getMonsterModifications(AH::Common::MonsterData::MonsterAttributes attrs, bool matchAll = false) const
    { Q_UNUSED(attrs) Q_UNUSED(matchAll) return PropertyModificationList(); }
    */

    virtual void onEndMythos() {}

    using MonsterModifier::getMonsterModifications;
    using MonsterModifier::getMonsterMovementModifications;

    /** Called at end of Mythos Phase */
    virtual void mythosPower() {}

    virtual void awake();

    virtual void attack() = 0;
    virtual bool postAttack() { return true; }
    virtual void newAttackRound();

    virtual bool onUnconscious(Character *c) { Q_UNUSED(c) return true; }
    virtual bool onInsane(Character *c) { Q_UNUSED(c) return true; }
    virtual bool onLostInSpaceAndTime(Character *c) { Q_UNUSED(c) return true; }
    virtual bool onDefeatMonster(Character *c, Monster *m) { Q_UNUSED(c) Q_UNUSED(m) return true; }

    bool isDefeated() const;

    void damage(int amount);

    virtual void decreaseAttackAdjustment(int amount) { m_attackAdjustment -= amount; setDirty(); }
    virtual QList<AH::Common::MythosData::EnvironmentType> ignoreEnvironmentTypes() const { return {}; }

protected:
    const PropertyModifier *getMonsterMovementModifier() const override { return this; }

protected:
    bool m_awake;
    bool m_dirty;

    int m_damageThreshold;
    int m_damage;
};

#endif // ANCIENTONE_H
