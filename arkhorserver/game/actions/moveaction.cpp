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
    QList<AH::Common::FieldData::FieldID> path;

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
        path << p.startField()->id();
        for (int i = 1; i < p.size(); ++i) {
            gGame->notifier()->actionUpdate(this, p[i]->name());
            speed--;
            gGame->context().player()->getCharacter()->addMovementPoint(-1);
            if (p[i]->hasMonsters()) {
                // STOP HERE!
                stopField = p[i];
                path << stopField->id();
                break;
            }
            path << p[i]->id();
        }

        stopField->placeCharacter(gGame->context().player()->getCharacter());
        gGame->notifier()->actionFinish(this, stopField->name());
        gGame->changeCharacterMove(gGame->context().player()->getCharacter(), path);
        return true;
    }
    return false;
}

bool MoveAction::moveOtherWorld()
{
    Character *c = gGame->context().player()->getCharacter();
    QList<AH::Common::FieldData::FieldID> path;
    path << c->field()->id();
    switch (c->otherWorldPhase()) {
    case AH::OWP_FirstField:
        c->setOtherWoldPhase(AH::OWP_SecondField);
        gGame->notifier()->actionExecute(this, "second");
        path << static_cast<AH::Common::FieldData::FieldID>(c->field()->id() | AH::Common::FieldData::FieldID::OWF_2ndFieldFlag);
        gGame->changeCharacterMove(c, path);
        return true;
    case AH::OWP_SecondField:
        path[0] = static_cast<AH::Common::FieldData::FieldID>(path[0] | AH::Common::FieldData::OWF_2ndFieldFlag);
        if (c->returnToArkham()) {
            gGame->notifier()->actionExecute(this, c->field()->name());
        } else {
            // no back field
            c->lostInSpaceAndTime();
            gGame->notifier()->actionExecute(this, c->field()->name());
        }
        path << c->field()->id();
        gGame->changeCharacterMove(c, path);
        return true;
    default:
        Q_ASSERT_X(false, "Movement Other World", "Unsupported Other World Phase");
    }

    return false;
}

AH::Common::ModifiedPropertyValueData MoveOption::baseProperty() const
{
    if (!isAvailable())
        return {};
    if (gGame->context().player()->getCharacter()->isDelayed())
        return {};

    switch (gGame->context().player()->getCharacter()->field()->type()) {
    case AH::Common::FieldData::Location:
    case AH::Common::FieldData::Street:
        return gGame->context().getCurCharacterProperty(PropertyValue::Prop_Movement).toModifiedPropertyValueData();
    }
    return {};
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
