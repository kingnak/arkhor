#include "entergateaction.h"
#include "../game.h"
#include "../../gate.h"
#include "character.h"
#include "../player.h"

EnterGateAction::EnterGateAction()
{
}

bool EnterGateAction::execute()
{
    gGame->notifier()->actionExecute(this);
    gGame->context().player()->getCharacter()->field()->gate()->enter(gGame->context().player()->getCharacter());
    return true;
}

QString EnterGateAction::notificationString(GameAction::NotificationPart part, const QString &desc) const
{
    Q_UNUSED(desc);
    Q_ASSERT(part == Execute);
    return "{C} entered gate at {F}";
}


bool EnterGateOption::isAvailable() const
{
    Character *c = gGame->context().player()->getCharacter();
    Gate *g = c->field()->gate();
    return c->getExploredGate() == NULL && g != NULL;
}
