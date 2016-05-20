#include "focusaction.h"
#include "../game.h"
#include "character.h"
#include "../player.h"

FocusAction::FocusAction()
{
}

bool FocusAction::execute()
{
    gGame->notifier()->actionStart(this);
    int focusAmount = gGame->context().getCurCharacterProperty(PropertyValue::Prop_Focus).finalVal();
    bool ok = executeOnPlayer(gGame->context().player(), focusAmount);
    if (ok)
        gGame->context().player()->getCharacter()->setFocusAmount(focusAmount);
    gGame->notifier()->actionFinish(this);
    return ok;
}

QString FocusAction::notificationString(GameAction::NotificationPart part, const QString &desc) const
{
    Q_UNUSED(desc);
    if (part == Finish) {
        QString attrs;
        QList<AttributeSlider> a = gGame->context().player()->getCharacter()->getModifyableFocusAttributes();
        int sp = gGame->context().player()->getCharacter()->getAttributeValue(AH::Attr_Speed);
        int sn = gGame->context().player()->getCharacter()->getAttributeValue(AH::Attr_Sneak);
        int fi = gGame->context().player()->getCharacter()->getAttributeValue(AH::Attr_Fight);
        int wi = gGame->context().player()->getCharacter()->getAttributeValue(AH::Attr_Will);
        int lo = gGame->context().player()->getCharacter()->getAttributeValue(AH::Attr_Lore);
        int lu = gGame->context().player()->getCharacter()->getAttributeValue(AH::Attr_Luck);
        attrs = QString("Sp/Sn %1/%2, Fi/Wi %3/%4, Lo/Lu %5/%6").arg(sp).arg(sn).arg(fi).arg(wi).arg(lo).arg(lu);
        return "{C} focused: " + attrs;
    }
    return QString::null;
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
