#include "arkhamencounteryaction.h"
#include "game/game.h"
#include "game/player.h"
#include "character.h"
#include <fielddata.h>
#include "game/arkhamencounter.h"

bool ArkhamEncounteryAction::execute()
{
    Player *p = gGame->context().player();
    ArkhamEncounter *enc = gGame->drawArkhamEncounter(p->getCharacter()->fieldId());
    if (!enc) {
        return false;
    }

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

    return opt->execute();
}

