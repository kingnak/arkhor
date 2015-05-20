#include "gamephase.h"
#include "gameaction.h"
#include "gameoption.h"
#include "character.h"
#include "player.h"

GamePhase::GamePhase(Game *game)
    : m_game(game)//, m_skip(new SkipOption)
{
}

GamePhase::~GamePhase()
{
    //delete m_skip;
}

void GamePhase::execute()
{
    enterPhase();
    gGame->commitUpdates();

    Player *player = gGame->getCurrentPlayer();

    QList<GameAction *> phaseActions = getPhaseActions();
    foreach (GameAction *a, phaseActions) {
        a->execute();
    }
    gGame->commitUpdates();

    QList<GameAction *> actions;
    actions << player->getCharacter()->getActions(gGame->context().phase());
    foreach (GameAction *a, actions) {
        a->execute();
    }
    gGame->commitUpdates();

    // Option Loop
    bool success = false;
    GameOption *op = NULL;
    do {
        QList<GameOption *> options;
        options << player->getCharacter()->getOptions(gGame->context().phase());
        options << getPhaseOptions();

        // Get mandatory and other options
        QList<GameOption *> mandatory;
        QList<GameOption *> opts;
        QList<GameOption *> supp;
        int mandAvail = 0;
        foreach (GameOption *o, options) {
            switch (o->chooseType()) {
            case AH::ChooseMandatory:
                mandatory << o;
                if (o->isAvailable())
                    mandAvail++;
                break;
            case AH::ChooseOptional:
                opts << o;
                break;
            case AH::ChooseSupplemental:
                if (o->isAvailable())
                    supp << o;
                break;
            }
        }

        if (mandAvail > 0)
            op = m_game->context().player()->chooseOption(mandatory + supp);
        else
            op = m_game->context().player()->chooseOption(opts + supp);

        if (op && op->isAvailable()) {
            success = op->execute();
        } else {
            success = false;
        }
        gGame->commitUpdates();
    } while (!success || op == NULL || (op && op->continueType() == AH::CanContinue));
    if (!success) {
        Q_ASSERT(false);
    }

    finishPhase();
    gGame->commitUpdates();
}



SkipOption *GamePhase::s_skip = NULL;
SkipOption *GamePhase::getSkipOption()
{
    if (!s_skip) s_skip = new SkipOption;
    return s_skip;
}
