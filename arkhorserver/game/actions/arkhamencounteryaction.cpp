#include "arkhamencounteryaction.h"
#include "game/game.h"
#include "game/player.h"
#include "character.h"
#include <fielddata.h>
#include "game/arkhamencounter.h"
#include "game/drawcardhelper.h"

bool ArkhamEncounteryAction::execute()
{
    Player *p = gGame->context().player();
    DrawCardHelper hlp;
    ModifiedPropertyValue mods = gGame->context().getCurCharacterDrawArkhamEncounter();
    ArkhamEncounter *enc = hlp.drawArkhamEncounter(p, p->getCharacter()->fieldId(), mods.finalVal());
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

QString ArkhamEncounteryAction::notificationString(GameAction::NotificationPart part, const QString &desc) const
{
    Q_UNUSED(desc);
    Q_ASSERT(part == Execute);
    return "{C} has an encounter at {F}";
}


GameOption::AutoChoose ArkhamEncounteryOption::autoChoose() const
{
    if (gGame->context().player()->getCharacter()->field()->type() != AH::Common::FieldData::FieldType::Location) {
        return AutoChoose::Always;
    }
    return AutoChoose::Possible;
}

AH::Common::ModifiedPropertyValueData ArkhamEncounteryOption::baseProperty() const
{
    auto mods = gGame->context().getCurCharacterDrawArkhamEncounter().toModifiedPropertyValueData();
    if (mods.modifications().empty()) {
        // not interesting for client if not modified
        return {};
    }
    return mods;
}
