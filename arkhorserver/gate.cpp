#include "gate.h"
#include "character.h"
#include "game/gamefield.h"

Gate::Gate(AH::Dimensions dim, int closeAdjustment, GameField *dest)
:   m_destField(dest),
    m_sourceField(nullptr)
{
    m_dims = dim;
    m_adjustment = closeAdjustment;
}

AH::Common::FieldData::FieldID Gate::source() const
{
    if (m_sourceField) return m_sourceField->id();
    return AH::Common::FieldData::NO_NO_FIELD;
}

AH::Common::FieldData::FieldID Gate::destination() const
{
    if (m_destField) return m_destField->id();
    return AH::Common::FieldData::NO_NO_FIELD;
}

void Gate::open()
{
    if (!m_open) {
        m_open = true;
        m_destField->addBackGate(this);
        gGame->invalidateObject(m_id);
        gGame->changeGateOpen(this);
    }
}

void Gate::enter(Character *c)
{
    open();
    m_destField->placeCharacter(c);
    c->setOtherWoldPhase(AH::OWP_FirstField);
    // TODO: Different change notification?
    gGame->changeCharacterMove(c, QList<AH::Common::FieldData::FieldID>() << m_sourceField->id() << m_destField->id());
}

void Gate::drawThrough(Character *c)
{
    c->setDelayed(true);
    enter(c);
}

void Gate::comeBack(Character *c)
{
    m_sourceField->placeCharacter(c);
    c->setExploredGate(m_sourceField->gate());
}

void Gate::close(Character *c)
{
    c->setExploredGate(nullptr);
    m_sourceField->placeCharacter(c);
    m_sourceField->removeGate(this);
    m_destField->removeGate(this);
    c->addGateTrophy(this);
    // Remove monsters
    gGame->closeGateCleanup(this);
}

void Gate::seal(Character *c)
{
    m_sourceField->setSealed(true);
    close(c);
}


