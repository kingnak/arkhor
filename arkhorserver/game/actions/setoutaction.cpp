#include "setoutaction.h"
#include "game/game.h"
#include "game/player.h"
#include "character.h"

SetOutAction::SetOutAction()
{
}

bool SetOutAction::execute()
{
    if (gGame->context().player()->getCharacter()->isSetOut()) {
        gGame->notifier()->actionExecute(this);
        gGame->context().player()->getCharacter()->setSetout(false);
    }
    return true;
}

QString SetOutAction::notificationString(GameAction::NotificationPart part, const QString &desc) const
{
    Q_UNUSED(desc);
    Q_UNUSED(part);
    return "{C} isn't set out anymore";
}
