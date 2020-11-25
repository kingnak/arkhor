#include "placeonfieldaction.h"
#include "game/game.h"
#include "game/player.h"
#include "character.h"

PlaceOnFieldAction::PlaceOnFieldAction()
    : m_field(nullptr)
{
}

bool PlaceOnFieldAction::execute()
{
    if (!m_field) return false;
    gGame->notifier()->actionExecute(this, m_field->name());
    m_field->placeCharacter(gGame->context().player()->getCharacter());
    return true;
}

QString PlaceOnFieldAction::notificationString(GameAction::NotificationPart part, const QString &desc) const
{
    Q_UNUSED(part);
    Q_UNUSED(desc);
    return "{C} is placed on {F}";
}


bool PlaceOnFieldOption::isAvailable() const
{
    return pofa.m_field != nullptr;
}
