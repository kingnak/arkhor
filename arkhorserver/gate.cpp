#include "gate.h"
#include "character.h"
#include "game/gamefield.h"

Gate::Gate(AH::Dimension dim, int closeAmount, GameField *dest)
:   m_open(false),
    m_dim(dim),
    m_closeAdjustment(closeAmount),
    m_dest(dest),
    m_field(NULL)
{

}

void Gate::open()
{
    if (!m_open) {
        m_open = true;
        m_dest->addBackGate(this);
    }
}

void Gate::enter(Character *c)
{
    open();
    m_dest->placeCharacter(c);
    c->setOtherWoldPhase(AH::OWP_FirstField);
}

void Gate::comeBack(Character *c)
{
    m_field->placeCharacter(c);
    c->setExploredGate(m_field->gate());
}

void Gate::close(Character *c)
{
    c->setExploredGate(NULL);
    m_field->placeCharacter(c);
    m_field->removeGate(this);
    m_dest->removeGate(this);
    c->addGateMarker(this);
    // TODO: Remove monsters
}

void Gate::seal(Character *c)
{
    m_field->setSealed(true);
    close(c);
}


