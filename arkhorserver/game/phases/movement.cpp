#include "movement.h"
#include "../../character.h"
#include "../gameboard.h"
#include "../player.h"

Movement::Movement(Game *game)
    : GamePhase(game)
{
    m_move = new MoveOption;
    m_sneak = new EvadeOption;
    m_fight = new FightOption;
}

Movement::~Movement()
{
    delete m_move;
    delete m_sneak;
    delete m_fight;
}

void Movement::enterPhase()
{
    m_sneak->reset();
    int speed = gGame->context().getCurCharacterSkill(AH::Skill_Speed).finalVal();
    gGame->context().player()->getCharacter()->setMovementAmount(speed);

    m_move->determineMovementType();
}

QList<GameOption *> Movement::getPhaseOptions()
{
    switch (gGame->context().player()->getCharacter()->field()->type()) {
    case AH::Common::FieldData::Location:
    case AH::Common::FieldData::Street:
        return QList<GameOption *>()
                << m_move
                << m_sneak
                << m_fight
                << getSkipOption();
    case AH::Common::FieldData::OtherWorld:
        return QList<GameOption *>()
                << m_move;
    default:
        Q_ASSERT_X(false, "Movement", "Unsupported Field Type");
        return QList<GameOption *>();
    }
}


