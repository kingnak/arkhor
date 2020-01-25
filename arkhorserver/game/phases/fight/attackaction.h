#ifndef ATTACKACTION_H
#define ATTACKACTION_H

#include <QPair>
#include "game/gameaction.h"
#include "game/gameoption.h"
#include "game/propertymodifier.h"

class Character;
class FightPhase;
class PropertyModificationList;

class AttackAction : public GameAction
{
public:
    AttackAction(FightPhase *fight);

    QString id() const { return "AC_ATTACK"; }

    virtual AH::GamePhases phases() const { return AH::Movement; }
    virtual bool execute();
    virtual QString notificationString(NotificationPart part, const QString &desc = QString::null) const;

    virtual QString name() const { return "Attack"; }
    virtual QString description() const { return ""; }

protected:
    void filterEquipped(PropertyModificationList &lst) const;
    void discardAfterAttack(PropertyModificationList &lst);
    virtual PropertyModificationList getMonsterModifications(Character *c) const;

    //typedef std::pair<ModifiedPropertyValue, PropertyModificationList> AttackModifications;
    struct AttackModifications {
        ModifiedPropertyValue base;
        PropertyModificationList weaponsGeneral;
        PropertyModificationList weaponsPhysical;
        PropertyModificationList weaponsMagical;
        PropertyModificationList monsterPhysical;
        PropertyModificationList monsterMagical;
        ModifiedPropertyValue finalPropery() const;
        ModifiedPropertyValue characterProperty() const;
    };

    AttackModifications getAttackModifications(Character *c, bool asDieRoll = false) const;

    FightPhase *m_fight;
    friend class AttackOption;
};

class AttackOption : public GameOption
{
public:
    AttackOption(FightPhase *fight)
        : GameOption(&aa, AH::CanContinue, AH::ChooseMandatory, AH::Movement),
          aa(fight)
    {}

    QString id() const { return "OP_ATTACK"; }
    QString sourceId() const;

    virtual bool isAvailable() const { return true; }
    virtual AH::Common::ModifiedPropertyValueData baseProperty() const;

private:
    AttackAction aa;
};


#endif // ATTACKACTION_H
