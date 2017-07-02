#include "otherworldencounteryaction.h"
#include "../game.h"
#include "character.h"
#include "../player.h"
#include "game/otherworldencounter.h"

OtherWorldEncounteryAction::OtherWorldEncounteryAction()
{
}

bool OtherWorldEncounteryAction::execute()
{
    Player *p = gGame->context().player();
    OtherWorldEncounter *enc = gGame->drawOtherWorldEncounter(p->getCharacter()->fieldId());
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

bool OtherWorldEncounteryOption::isAvailable() const
{
    return gGame->context().player()->getCharacter()->field()->type() == AH::Common::FieldData::OtherWorld;
}
