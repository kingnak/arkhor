#include "moveaction.h"
#include "../game.h"
#include "../gameboard.h"
#include "gate.h"
#include "character.h"
#include "../player.h"
#include "game/phases/movement.h"

MoveAction::MoveAction(Movement *m)
    : m_movement(m)
{
}

bool MoveAction::execute()
{
    if (gGame->context().player()->getCharacter()->isDelayed()) {
        gGame->context().player()->getCharacter()->setDelayed(false);
        return true;
    }
    switch (gGame->context().player()->getCharacter()->field()->type()) {
    case AH::Common::FieldData::Location:
    case AH::Common::FieldData::Street:
        return moveArkham();
    case AH::Common::FieldData::OtherWorld:
        return moveOtherWorld();
    default:
        Q_ASSERT_X(false, "Movement", "Unsupported Field Type");
    }
    return false;
}

QString MoveAction::notificationString(GameAction::NotificationPart part, const QString &desc) const
{
    switch (gGame->context().player()->getCharacter()->field()->type()) {
    case AH::Common::FieldData::Location:
    case AH::Common::FieldData::Street:
        switch (part) {
        case Finish: return "{C} moved to {F} via {B}";
        }
        break;
    case AH::Common::FieldData::OtherWorld:
        if (part == Execute) {
            if (desc == "second") {
                return "{C} moves to second {F} field";
            } else {
                return "{C} returns to {D}";
            }
        }
        break;
    default:
        Q_ASSERT_X(false, "Movement", "Unsupported Field Type");
    }
    return QString::null;
}

bool MoveAction::moveArkham()
{
    int speed = gGame->context().getCurCharacterProperty(PropertyValue::Prop_Movement).finalVal();
    MovementPath p = gGame->context().player()->chooseMovement(gGame->context().player()->getCharacter()->field(), speed);

    if (p.isEmpty()) {
        return false;
    }

    if (p.startField() == gGame->context().player()->getCharacter()->field()
            && gGame->board()->validatePath(p)
            && p.length() <= speed)
    {
        m_movement->characterMoved();
        gGame->notifier()->actionStart(this, p.first()->name());
        GameField *stopField = p.endField();
        for (int i = 1; i < p.size(); ++i) {
            gGame->notifier()->actionUpdate(this, p[i]->name());
            speed--;
            if (p[i]->hasMonsters()) {
                // STOP HERE!
                stopField = p[i];
                break;
            }
        }

        stopField->placeCharacter(gGame->context().player()->getCharacter());
        gGame->context().player()->getCharacter()->setMovementAmount(speed);
        gGame->notifier()->actionFinish(this, stopField->name());
        return true;
    }
    return false;
}

bool MoveAction::moveOtherWorld()
{
    Character *c = gGame->context().player()->getCharacter();
    switch (c->otherWorldPhase()) {
    case AH::OWP_FirstField:
        c->setOtherWoldPhase(AH::OWP_SecondField);
        gGame->notifier()->actionExecute(this, "second");
        return true;
    case AH::OWP_SecondField:
        if (!c->field()->backGates().isEmpty()) {
            // TODO: Let choose gate
            Gate *p = c->field()->backGates().at(0);
            p->comeBack(c);
            gGame->notifier()->actionExecute(this, p->sourceField()->name());
        }
        return true;
    default:
        Q_ASSERT_X(false, "Movement Other World", "Unsupported Other World Phase");
    }

    return false;
}

AH::Common::ModifiedPropertyValueData MoveOption::baseProperty() const
{
    return gGame->context().getCurCharacterSkill(AH::Skill_Speed).toModifiedPropertyValueData();
}

void MoveOption::determineMovementType()
{
    if (gGame->context().player()->getCharacter()->isDelayed()) {
        m_chooseType = AH::ChooseMandatory;
        m_continueType = AH::CannotContinue;
        return;
    }
    switch (gGame->context().player()->getCharacter()->field()->type()) {
    case AH::Common::FieldData::Location:
    case AH::Common::FieldData::Street:
        m_chooseType = AH::ChooseOptional;
        m_continueType = AH::CanContinue;
        break;
    case AH::Common::FieldData::OtherWorld:
        m_chooseType = AH::ChooseMandatory;
        m_continueType = AH::CannotContinue;
        break;
    default:
        m_chooseType = AH::ChooseOptional;
        m_continueType = AH::CanContinue;
    }
}

bool MoveOption::isAvailable() const
{
    if (gGame->context().player()->getCharacter()->isDelayed()) {
        return true;
    }
    switch (gGame->context().player()->getCharacter()->field()->type()) {
    case AH::Common::FieldData::Location:
    case AH::Common::FieldData::Street:
        return gGame->context().getCurCharacterProperty(PropertyValue::Prop_Movement).finalVal() > 0;
    case AH::Common::FieldData::OtherWorld:
        return true;
    default:
        return false;
    }

}
