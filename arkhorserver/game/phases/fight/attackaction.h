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

    QString id() const override { return "AC_ATTACK"; }

    AH::GamePhases phases() const override { return AH::Movement; }
    bool execute() override;
    QString notificationString(NotificationPart part, const QString &desc = QString::null) const override;

    QString name() const override { return "Attack"; }
    QString description() const override { return ""; }

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

    QString id() const override { return "OP_ATTACK"; }
    QString sourceId() const override;

    bool isAvailable() const override { return true; }
    AH::Common::ModifiedPropertyValueData baseProperty() const override;

private:
    AttackAction aa;
};


#endif // ATTACKACTION_H
