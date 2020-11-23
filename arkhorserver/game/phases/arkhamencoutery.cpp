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

    if (!isSetOut) {
        opts = field->getFieldOptions();
        if (field->type() == AH::Common::FieldData::Location) {
            opts << m_enter;
            opts << m_enc;
            opts << m_close;
            opts << m_seal;
        } else {
            opts << getSkipOption();
        }
    }

    if (opts.empty()) {
        opts << getSkipOption();
    }

    return opts;
}

void ArkhamEncoutery::enterPhase()
{
    GameField *f = gGame->context().player()->getCharacter()->field();
    Gate *g = f ? f->gate() : nullptr;
    gGame->context().setGate(g);
}

void ArkhamEncoutery::finishPhase()
{
    gGame->context().player()->clearTempData();
}
