#include "gamephase.h"
#include "gameaction.h"
#include "gameoption.h"
#include "character.h"
#include "player.h"

GamePhase::GamePhase(Game *game)
    : m_game(game)
{
    m_skip = new SkipOption;
}

GamePhase::~GamePhase()
{
    delete m_skip;
}

void GamePhase::execute()
{
    enterPhase();

    Player *player = gGame->getCurrentPlayer();

    QList<GameAction *> phaseActions = getPhaseActions();
    foreach (GameAction *a, phaseActions) {
        a->execute();
    }

    QList<GameAction *> actions;
    actions << player->getCharacter()->getActions(gGame->context().phase());
    foreach (GameAction *a, actions) {
        a->execute();
    }

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
                mandatory << o;
                opts << o;
            }
        }

        if (mandAvail > 0)
            op = m_game->context().player()->chooseOption(mandatory);
        else
            op = m_game->context().player()->chooseOption(opts);

        if (op && op->isAvailable()) {
            success = op->execute();
        } else {
            success = false;
        }
    } while (!success || op == NULL || (op && op->continueType() == AH::CanContinue));
    if (!success) {
        Q_ASSERT(false);
    }
}
