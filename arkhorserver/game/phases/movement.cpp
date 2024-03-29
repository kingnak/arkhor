#include "movement.h"
#include "../../character.h"
#include "../gameboard.h"
#include "../player.h"
#include "game/mythoscard.h"

Movement::Movement(Game *game)
    : GamePhase(game)
{
    m_undelay = new UndelayOption;
    m_move = new MoveOption(this);
    m_fight = new FightPhase;
    m_place = new PlaceOnFieldOption;
    m_trade = new TradeOption;
    m_outcome = FightPhase::EndUnknown;
}

Movement::~Movement()
{
    delete m_undelay;
    delete m_move;
    delete m_fight;
    delete m_place;
    delete m_trade;
}

void Movement::characterMoved()
{
    m_outcome = FightPhase::EndUnknown;
    gGame->context().player()->getCharacter()->setExploredGate(nullptr);
}

void Movement::enterPhase()
{
    int speed = 0;
    if (!gGame->context().player()->getCharacter()->isDelayed())
        speed = gGame->context().getCurCharacterSkill(AH::Skill_Speed).finalVal();

    gGame->context().player()->getCharacter()->setMovementAmount(speed);

    m_outcome = FightPhase::EndUnknown;
    m_move->determineMovementType();
}

void Movement::finishPhase()
{
    int clueCt = gGame->context().player()->getCharacter()->field()->takeClues();
    gGame->context().player()->getCharacter()->addClue(clueCt);

    // on end movements
    if (gGame->context().environment()) {
        gGame->context().environment()->onEndMovement();
    }
}

QList<GameOption *> Movement::getPhaseOptions()
{
    if (m_undelay->isAvailable()) {
        if (m_fight->isFightSituation()) {
            // Special case: There are monsters
            // First undelay, as character might be delayed by fight outcome
            gGame->context().player()->getCharacter()->setDelayed(false);
            QList<Monster *> monsters = gGame->context().player()->getCharacter()->field()->monsters();
            m_fight->handleFight(monsters);
            // outcome ignored, can never continue. Just skip
            return QList<GameOption*>() << getSkipOption();
        } else {
            return QList<GameOption*>() << m_undelay;
        }
    }

    switch (gGame->context().player()->getCharacter()->field()->type()) {
    case AH::Common::FieldData::Location:
    case AH::Common::FieldData::Street:
        if (m_outcome != FightPhase::EndFlown && m_outcome != FightPhase::EndEvaded &&  m_fight->isFightSituation()) {
            QList<Monster *> monsters = gGame->context().player()->getCharacter()->field()->monsters();
            bool res = m_fight->handleFight(monsters);
            m_outcome = m_fight->getOutcome();
            if (!res) {
                return QList<GameOption *>() << getSkipOption();
            }
        }

        return QList<GameOption *>()
                << getSkipOption()
                << m_move
                << m_trade;
    case AH::Common::FieldData::OtherWorld:
        return QList<GameOption *>()
                << m_move
                << m_trade;

    case AH::Common::FieldData::SpaceAndTime:
    {
        AH::Common::FieldData::FieldID fid = gGame->context().player()->getCharacter()->investigator()->startFieldId();
        GameField *f = gGame->board()->field(fid);
        m_place->setField(f);
        return QList<GameOption *>()
                << m_place;
    }

    default:
        Q_ASSERT_X(false, "Movement", "Unsupported Field Type");
        return QList<GameOption *>();
    }
}


