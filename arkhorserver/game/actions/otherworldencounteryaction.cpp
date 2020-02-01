#include "otherworldencounteryaction.h"
#include "../game.h"
#include "character.h"
#include "../player.h"
#include "game/otherworldencounter.h"
#include "game/drawcardhelper.h"

OtherWorldEncounteryAction::OtherWorldEncounteryAction()
{
}

bool OtherWorldEncounteryAction::execute()
{
    Player *p = gGame->context().player();
    return executeOnPlayer(p, p->getCharacter()->fieldId());
}

bool OtherWorldEncounteryAction::executeOnPlayer(Player *p, AH::Common::FieldData::FieldID fieldId)
{
    DrawCardHelper hlp;
    ModifiedPropertyValue mods = gGame->context().getCharacterDrawOtherWorldEncounter(p->getCharacter());
    OtherWorldEncounter *enc = hlp.drawOtherWorldEncounter(p, fieldId, mods.finalVal());
    if (!enc) {
        return false;
    }

    gGame->notifier()->actionExecute(this);

    GameOption *opt = NULL;
    QString id = p->chooseEncounterOption(enc);
    foreach (GameOption *i, enc->options()) {
        if (i->id() == id) {
            opt = i;
            break;
        }
    }
    if (!opt) {
        return false;
    }

    gGame->notifier()->setTempData(enc->description());

    return opt->execute();
}

QString OtherWorldEncounteryAction::notificationString(GameAction::NotificationPart part, const QString &desc) const
{
    Q_UNUSED(desc);
    Q_ASSERT(part == Execute);
    return "{C} has an encounter at {F}";
}

GameOption::AutoChoose OtherWorldEncounteryOption::autoChoose() const
{
    if (gGame->context().player()->getCharacter()->field()->type() != AH::Common::FieldData::FieldType::OtherWorld) {
        return AutoChoose::Always;
    }
    return AutoChoose::Possible;
}

bool OtherWorldEncounteryOption::isAvailable() const
{
    return gGame->context().player()->getCharacter()->field()->type() == AH::Common::FieldData::OtherWorld;
}

AH::Common::ModifiedPropertyValueData OtherWorldEncounteryOption::baseProperty() const
{
    auto mods = gGame->context().getCurCharacterDrawOtherWorldEncounter().toModifiedPropertyValueData();
    if (mods.modifications().empty()) {
        // not interesting for client if not modified
        return {};
    }
    return mods;
}
