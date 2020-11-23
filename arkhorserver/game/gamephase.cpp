#include "gamephase.h"
#include "gameaction.h"
#include "gameoption.h"
#include "character.h"
#include "player.h"

GamePhase::GamePhase(Game *game)
    : m_game(game)
{
}

GamePhase::~GamePhase() = default;

void GamePhase::execute()
{
    enterPhase();
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
    } while (!success || op == nullptr || op->continueType() == AH::CanContinue);
    
    finishPhase();
    gGame->commitUpdates();
}

QList<GameOption *> GamePhase::getOptions()
{
    Player *player = gGame->getCurrentPlayer();

    QList<GameOption *> options;
    options << getPhaseOptions();
    options << player->getCharacter()->getOptions(gGame->context().phase());

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

GameOption *GamePhase::autoChoose(const QList<GameOption *> &options)
{
    auto playAutoSkip = m_game->context().player()->autoSkip();
    if (playAutoSkip == AH::Common::PlayerData::AutoSkipData::NoAutoChoose)
        return nullptr;

    GameOption *autoChoose = nullptr;
    for (auto o : options) {
        // This might update costs/availability:
        o->data();
        if (o->isAvailable()) {
            // Has costs, always show
            if (!o->costs().getAlternatives().empty())
                return nullptr;
            // If second candidate => no auto choose
            if (autoChoose)
                return nullptr;

            // Candidate
            autoChoose = o;
        }
    }

    if (!autoChoose)
        return nullptr;

    if (playAutoSkip == AH::Common::PlayerData::AutoSkipData::AutoChooseAll)
        return autoChoose;

    // Player is now AutoChooseAlways or AutoChoosePossible

    switch (autoChoose->autoChoose()) {
    case GameOption::AutoChoose::Never:
        // Can never automatically choose this option
        return nullptr;
    case GameOption::AutoChoose::Always:
        // Always auto choose this option
        return autoChoose;
    case GameOption::AutoChoose::Possible:
        if (playAutoSkip == AH::Common::PlayerData::AutoSkipData::AutoChoosePossible)
            return autoChoose;
        return nullptr;
    default:
        return nullptr;
    }
}

SkipOption *GamePhase::s_skip = nullptr;
SkipOption *GamePhase::getSkipOption()
{
    if (!s_skip) s_skip = new SkipOption;
    return s_skip;
}
