#include "movement.h"
#include "../../character.h"
#include "../gameboard.h"
#include "../player.h"

Movement::Movement(Game *game)
    : GamePhase(game)
{
    m_move = new MoveOption(this);
    m_fight = new FightPhase;
    m_outcome = FightPhase::EndUnknown;
}

Movement::~Movement()
{
    delete m_move;
    delete m_fight;
}

void Movement::characterMoved()
{
    m_outcome = FightPhase::EndUnknown;
    gGame->context().player()->getCharacter()->setExploredGate(NULL);
}

void Movement::enterPhase()
{
    int speed = gGame->context().getCurCharacterSkill(AH::Skill_Speed).finalVal();
    gGame->context().player()->getCharacter()->setMovementAmount(speed);

    m_outcome = FightPhase::EndUnknown;
    m_move->determineMovementType();
}

void Movement::finishPhase()
{
    int clueCt = gGame->context().player()->getCharacter()->field()->takeClues();
    gGame->context().player()->getCharacter()->addClue(clueCt);
}

QList<GameOption *> Movement::getPhaseOptions()
{
    switch (gGame->context().player()->getCharacter()->field()->type()) {
    case AH::Common::FieldData::Location:
    case AH::Common::FieldData::Street:
        if (m_outcome != FightPhase::EndFlown &&  m_fight->isFightSituation()) {
            bool res = m_fight->handleFight();
            m_outcome = m_fight->getOutcome();
            if (!res) {
                return QList<GameOption *>() << getSkipOption();
            }
        }

        return QList<GameOption *>()
                << m_move
                << getSkipOption();
    case AH::Common::FieldData::OtherWorld:
        return QList<GameOption *>()
                << m_move;
    default:
        Q_ASSERT_X(false, "Movement", "Unsupported Field Type");
        return QList<GameOption *>();
    }
}


