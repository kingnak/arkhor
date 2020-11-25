#include "undelayaction.h"
#include "game/game.h"
#include "game/player.h"
#include "character.h"

UndelayAction::UndelayAction() = default;

bool UndelayAction::execute()
{
    if (gGame->context().player()->getCharacter()->isDelayed()) {
        gGame->notifier()->actionExecute(this);
        gGame->context().player()->getCharacter()->setDelayed(false);
    }
    return true;
}

QString UndelayAction::notificationString(GameAction::NotificationPart part, const QString &desc) const
{
    Q_UNUSED(desc);
    Q_UNUSED(part);
    return "{C} isn't delayed anymore";
}


bool UndelayOption::isAvailable() const
{
    return gGame->context().player()->getCharacter()->isDelayed();
}
