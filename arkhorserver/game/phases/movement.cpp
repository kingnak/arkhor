#include "movement.h"
#include "../../character.h"
#include "../gameboard.h"
#include "../player.h"

Movement::Movement(Game *game)
    : GamePhase(game)
{
    m_move = new MoveOption;
    m_fight = new FightPhase;
}

Movement::~Movement()
{
    delete m_move;
    delete m_fight;
}

void Movement::enterPhase()
{
    int speed = gGame->context().getCurCharacterSkill(AH::Skill_Speed).finalVal();
    gGame->context().player()->getCharacter()->setMovementAmount(speed);

    m_move->determineMovementType();
}

QList<GameOption *> Movement::getPhaseOptions()
{
    switch (gGame->context().player()->getCharacter()->field()->type()) {
    case AH::Common::FieldData::Location:
    case AH::Common::FieldData::Street:
        if (m_fight->isFightSituation()) {
            if (!m_fight->handleFight()) {
                return QList<GameOption *>();
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


