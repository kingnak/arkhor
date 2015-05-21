#include "mythoscard.h"
#include "game.h"

MythosCard::MythosCard()
{
}

QList<AH::Common::PropertyModificationData>MythosCard::getModificationData() const
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
    teardownRumor();
    GameField *f = gGame->board()->field(rumorFieldId());
    if (f) {
        f->removeFieldOption(this->rumorFieldOption());
        f->unsetSpecialActionNr();
    }
    //gGame->returnMythos(this);
    gGame->setRumor(NULL);

}
