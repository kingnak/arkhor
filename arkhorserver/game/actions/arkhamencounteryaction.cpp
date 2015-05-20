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

    GameOption *opt = p->chooseEncounterOption(enc);
    if (!opt) {
        return false;
    }

    return opt->execute();
}

