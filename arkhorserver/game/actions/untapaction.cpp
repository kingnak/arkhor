#include "untapaction.h"
#include "../game.h"
#include "../gameobject.h"
#include "character.h"
#include "../player.h"

UntapAction::UntapAction()
{
}

AH::GamePhases UntapAction::phases() const
{
    return AH::Upkeep;
}

bool UntapAction::execute()
{
    bool hasUntapped = false;
    QList<GameObject *> &inv = gGame->context().player()->getCharacter()->inventory();
    gGame->notifier()->actionStart(this);
    foreach (GameObject *o, inv) {
        if (o->isExhausted()) {
            o->refresh();
            gGame->invalidateObject(o->id());
            gGame->notifier()->actionUpdate(this, o->name());
            hasUntapped = true;
        }
    }
    gGame->notifier()->actionFinish(this);
    if (hasUntapped)
        gGame->characterDirty(gGame->context().player()->getCharacter());
    return true;
}

QString UntapAction::notificationString(GameAction::NotificationPart part, const QString &desc) const
{
    switch (part) {
    case Finish: if (!desc.isEmpty()) return "{C} untapped {B}";
    }
    return QString::null;
}
