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
    QList<GameObject *> &inv = gGame->context().player()->getCharacter()->inventory();
    gGame->notifier()->actionStart(this);
    foreach (GameObject *o, inv) {
        if (o->isExhausted()) {
            o->refresh();
            gGame->notifier()->actionUpdate(this, o->name());
        }
    }
    gGame->notifier()->actionFinish(this);
    gGame->characterDirty(gGame->context().player()->getCharacter());
    return true;
}
