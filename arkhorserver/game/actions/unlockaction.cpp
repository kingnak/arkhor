#include "unlockaction.h"
#include "../game.h"
#include "../gameboard.h"
#include "../gamefield.h"

UnlockAction::UnlockAction() = default;

AH::GamePhases UnlockAction::phases() const
{
    return AH::Upkeep;
}

bool UnlockAction::execute()
{
    for (auto f : gGame->board()->allFields()) {
        if (f->isLocked()) {
            if (f->lockReason().testFlag(GameField::LOCK_ROUND_1)) {
                f->lock(GameField::LOCK_ROUND_2);
                f->unlock(GameField::LOCK_ROUND_1);
            } else if (f->lockReason().testFlag(GameField::LOCK_ROUND_2)) {
                f->unlock(GameField::LOCK_ROUND_2);
                gGame->notifier()->actionExecute(this, f->name());
            }
        }
    }
    return true;
}

QString UnlockAction::notificationString(GameAction::NotificationPart part, const QString &desc) const
{
    Q_UNUSED(part);
    return desc + " was unlocked";
}
