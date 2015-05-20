#include "mythoscard.h"
#include "game.h"

MythosCard::MythosCard()
{
}

void MythosCard::pass()
{
    onPass();
    teardownRumor();
    gGame->board()->field(rumorFieldId())->removeFieldOption(this->rumorFieldOption());
    //gGame->returnMythos(this);
    gGame->setRumor(NULL);
}

void MythosCard::fail()
{
    onFail();
    teardownRumor();
    gGame->board()->field(rumorFieldId())->removeFieldOption(this->rumorFieldOption());
    //gGame->returnMythos(this);
    gGame->setRumor(NULL);
}
