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
    bool ok = executeOnPlayer(gGame->context().player(), focusAmount);
    if (ok)
        gGame->context().player()->getCharacter()->setFocusAmount(focusAmount);
    return ok;
}

bool FocusAction::executeOnPlayer(Player *p, int &focusAmount)
{
    QList<AttributeSlider> &attrs = p->getCharacter()->getModifyableFocusAttributes();
    QList<int> diffs = p->chooseFocus(attrs, focusAmount);

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

    gGame->characterDirty(p->getCharacter());

    return true;
}

bool FocusOption::isAvailable() const
{
     return gGame->context().getCurCharacterProperty(PropertyValue::Prop_Focus).finalVal() > 0;
}
