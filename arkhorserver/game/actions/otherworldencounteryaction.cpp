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


bool OtherWorldEncounteryOption::isAvailable() const
{
    return gGame->context().player()->getCharacter()->field()->type() == AH::Common::FieldData::OtherWorld;
}
