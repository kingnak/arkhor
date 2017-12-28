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
    gGame->board()->unlockFields(gGame->context().phase());
    gGame->commitUpdates();

    Player *player = gGame->getCurrentPlayer();

    QList<GameAction *> phaseActions = getPhaseActions();
    for (GameAction *a : phaseActions) {
        a->execute();
    }
    //gGame->commitUpdates();

    QList<GameAction *> actions;
    actions << player->getCharacter()->getActions(gGame->context().phase());
    for (GameAction *a : actions) {
        a->execute();
    }
    gGame->commitUpdates();

    // Option Loop
    bool success = false;
    GameOption *op = nullptr;
    do {
        op = nullptr;
        auto opts = getOptions();
        op = autoChoose(opts);

        if (!op)
            op = m_game->context().player()->chooseOption(opts);

        if (op && op->isAvailable()) {
            success = op->execute();
        } else {
            success = false;
        }
        gGame->commitUpdates();
    } while (!success || op == nullptr || (op && op->continueType() == AH::CanContinue));
    if (!success) {
        Q_ASSERT(false);
    }

    finishPhase();
    gGame->commitUpdates();
}

QList<GameOption *> GamePhase::getOptions()
{
    Player *player = gGame->getCurrentPlayer();

    QList<GameOption *> options;
    options << player->getCharacter()->getOptions(gGame->context().phase());
    options << getPhaseOptions();

    // Get mandatory and other options
    QList<GameOption *> mandatory;
    QList<GameOption *> opts;
    QList<GameOption *> supp;
    int mandAvail = 0;
    for (GameOption *o : options) {
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

    QList<GameOption *> effectiveOpts = (mandAvail > 0 ? mandatory : opts) + supp;
    return effectiveOpts;
}

GameOption *GamePhase::autoChoose(const QList<GameOption *> options)
{
    if (m_game->context().player()->autoSkip() == AH::Common::PlayerData::NoAutoSkip)
        return nullptr;

    GameOption *autoChoose = nullptr;
    for (auto o : options) {
        // This might update costs/availability:
        o->data();
        if (o->isAvailable() && o->canPay()) {
            // If second candidate => no auto choose
            if (autoChoose)
                return nullptr;

            // Candidate
            autoChoose = o;
        }
    }

    // Automatic Skip
    if (autoChoose->id() == s_skip->id())
        return autoChoose;

    // Other single option => check for auto choose
    if (!this->canAutoChoosePhase())
        return nullptr;
    if (m_game->context().player()->autoSkip() != AH::Common::PlayerData::AutoChoose)
        return nullptr;

    return autoChoose;
}

SkipOption *GamePhase::s_skip = nullptr;
SkipOption *GamePhase::getSkipOption()
{
    if (!s_skip) s_skip = new SkipOption;
    return s_skip;
}
