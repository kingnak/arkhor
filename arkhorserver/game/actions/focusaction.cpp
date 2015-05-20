#include "focusaction.h"
#include "../game.h"
#include "character.h"
#include "../player.h"

FocusAction::FocusAction()
{
}

bool FocusAction::execute()
{
    int focusAmount = gGame->context().getCurCharacterProperty(PropertyValue::Prop_Focus).finalVal();
    QList<AttributeSlider> &attrs = gGame->context().player()->getCharacter()->getModifyableFocusAttributes();
    QList<int> diffs = gGame->context().player()->chooseFocus(attrs, focusAmount);

    int sum = 0;
    foreach (int i, diffs) {
        sum += qAbs(i);
    }

    if (sum > focusAmount) {
        return false;
    }

    for (int i = 0; i < 3 && i < diffs.size(); i++) {
        attrs[i].shiftBy(diffs[i]);
    }

    focusAmount -= sum;

    gGame->context().player()->getCharacter()->setFocusAmount(focusAmount);

    return true;
}

bool FocusOption::isAvailable()
{
     return gGame->context().getCurCharacterProperty(PropertyValue::Prop_Focus).finalVal() > 0;
}
