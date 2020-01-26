#include "mythoscard.h"
#include "game.h"

MythosCard::MythosCard()
{
}

QList<AH::Common::PropertyModificationData> MythosCard::getModificationData() const
{
    QList<AH::Common::PropertyModificationData> ret;
    foreach (PropertyModification m, getModifications()) {
        ret << *(m.data());
    }
    return ret;
}

void MythosCard::pass()
{
    onPass();
    cleanup();
}

void MythosCard::fail()
{
    onFail();
    cleanup();
}

void MythosCard::cleanup()
{
    if (type() == Rumor) {
        teardownRumor();
        GameField *f = gGame->board()->field(rumorFieldId());
        if (f) {
            f->removeFieldOption(this->rumorFieldOption());
            f->unsetSpecialActionNr();
        }
        //gGame->returnMythos(this);
        gGame->setRumor(NULL);
    } else if (type() == Environment) {
        GameField *f = gGame->board()->field(environmentFieldId());
        if (f) {
            f->removeFieldOption(this->environmentFieldOption());
            f->unsetSpecialActionNr();
        }
        gGame->setEnvironment(NULL);
    }
}
