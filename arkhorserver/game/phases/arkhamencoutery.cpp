#include "arkhamencoutery.h"
#include "../game.h"
#include "character.h"
#include "../player.h"

ArkhamEncoutery::ArkhamEncoutery(Game *game)
    : GamePhase(game)
{
    m_enc = new ArkhamEncounteryOption;
    m_enter = new EnterGateOption;
    m_close = new CloseGateOption;
    m_seal = new SealGateOption;
}

QList<GameOption *> ArkhamEncoutery::getPhaseOptions()
{
    QList<GameOption *> opts;
    bool isSetOut = gGame->context().player()->getCharacter()->isSetOut();
    GameField *field = gGame->context().player()->getCharacter()->field();
    if (!isSetOut && field->type() == AH::Common::FieldData::Location) {
        opts = field->getFieldOptions();
        opts << m_enter;
        opts << m_enc;
        opts << m_close;
        opts << m_seal;
    } else {
        opts << getSkipOption();
    }

    return opts;
}
