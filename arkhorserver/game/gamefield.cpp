#include "gamefield.h"
#include "../monster.h"
#include "../gate.h"
#include "../character.h"
#include "gameoption.h"

GameField::GameField(AH::Common::FieldData::FieldID id, const QString &name, AH::Common::FieldData::FieldType type)
:   m_gate(NULL),
    m_whiteField(NULL),
    m_blackField(NULL),
    m_lockFlags(0)
{
    m_id = id;
    m_name = name;
    m_type = type;
}

void GameField::putClue(int amount)
{
    m_clues += amount;
    gGame->boardDirty();
    gGame->changeField(AH::Common::GameBoardChangeData::FieldChange::addClues(m_id, amount));
}

int GameField::takeClues()
{
    int ret = m_clues;
    m_clues = 0;
    gGame->boardDirty();
    gGame->changeField(AH::Common::GameBoardChangeData::FieldChange::addClues(m_id, -ret));
    return ret;
}

void GameField::addBackGate(Gate *p)
{
    Q_ASSERT_X(m_type == AH::Common::FieldData::OtherWorld, "Add Back Gate", "Can only add Back Gates to Other Worlds");
    Q_ASSERT_X(!m_backGates.contains(p), "Add Back Gate", "Already have that gate");
    m_backGates.append(p);
    gGame->boardDirty();
}

void GameField::removeGate(Gate *p)
{
    if (m_gate == p) {
        m_gate = NULL;
        m_gateId = "";
    }

    m_backGates.removeAll(p);
    gGame->boardDirty();
}

void GameField::setGate(Gate *p)
{
    Q_ASSERT_X(m_gate == NULL || p == NULL, "Set Gate", "Already have gate");
    Q_ASSERT_X(m_type == AH::Common::FieldData::Location, "Set Gate", "Can only add gate to Interior");
    m_gate = p;
    if (p) {
        p->setSourceField(this);
    }
    gGame->boardDirty();
}

void GameField::setSealed(bool sealed)
{
    if (m_sealed != sealed) {
        m_sealed = sealed;
        gGame->boardDirty();
        if (sealed)
            gGame->changeField(AH::Common::GameBoardChangeData::FieldChange::sealField(m_id));
        else
            gGame->changeField(AH::Common::GameBoardChangeData::FieldChange::unsealField(m_id));
    }
}

void GameField::lockRound()
{
    lock(LockReason::LOCK_ROUND);
}

void GameField::lock(LockReason lockFlag)
{
    bool wasLocked = isLocked();
    if (!m_lockFlags.testFlag(lockFlag)) {
        // Set lock
        m_lockFlags.setFlag(lockFlag);
        gGame->boardDirty();
        if (!wasLocked && isLocked())
            gGame->changeField(AH::Common::GameBoardChangeData::FieldChange::lockField(m_id));
    }
}

void GameField::unlock(LockReason lockFlag)
{
    bool wasLocked = isLocked();
    if (m_lockFlags.testFlag(lockFlag)) {
        // Remove lock
        m_lockFlags.setFlag(lockFlag, false);
        gGame->boardDirty();
        if (wasLocked && !isLocked())
            gGame->changeField(AH::Common::GameBoardChangeData::FieldChange::unlockField(m_id));
    }
}

bool GameField::isLocked() const
{
    return m_lockFlags != 0;
}

void GameField::setSpecialActionNr(int nr)
{
    if (nr != m_specialActionNr) {
        m_specialActionNr = nr;
        gGame->boardDirty();
        gGame->changeField(AH::Common::GameBoardChangeData::FieldChange::setEvent(m_id, nr));
    }
}

void GameField::unsetSpecialActionNr()
{
    if (m_specialActionNr) {
        gGame->returnSpecialActionNumber(m_specialActionNr);
        m_specialActionNr = 0;
        gGame->boardDirty();
        gGame->changeField(AH::Common::GameBoardChangeData::FieldChange::setEvent(m_id, 0));
    }
}

bool GameField::isConnectedTo(GameField *other) const
{
    return m_neighbours.contains(other);
}

void GameField::placeCharacter(Character *c)
{
    if (c) {
        if (c->field()) {
            c->field()->removeCharacter(c);
        }

        m_characters.append(c);
        c->setField(this);

        gGame->boardDirty();
    }
}

void GameField::placeMonster(Monster *m)
{
    if (m) {
        if (m->field()) {
            m->field()->removeMonster(m);
        }

        m_monsters.append(m);
        m->setField(this);

        gGame->boardDirty();
    }
}

void GameField::removeCharacter(Character *c)
{
    if (c) {
        if (c->field() == this) {
            m_characters.removeAll(c);
            c->setField(NULL);

            gGame->boardDirty();
        }
    }
}

void GameField::removeMonster(Monster *m)
{
    if (m) {
        if (m->field() == this) {
            m_monsters.removeAll(m);
            m->setField(NULL);

            gGame->boardDirty();
        }
    }
}

QList<AH::Common::GameFieldData::FieldOptionDescription> GameField::fieldOptions() const
{
    QList<FieldOptionDescription> ret;
    for (auto o : m_options) {
        ret << FieldOptionDescription{o->id(), o->name(), o->sourceId()};
    }
    return ret;
}


// Overloads for GameFieldData:
QStringList GameField::characterIds() const
{
    QStringList ret;
    for (Character *c : m_characters) {
        ret << c->id();
    }
    return ret;
}

QStringList GameField::secondPhaseCharacterIds() const
{
    QStringList ret;
    if (type() == AH::Common::FieldData::OtherWorld) {
        for (Character *c : m_characters) {
            if (c->otherWorldPhase() == AH::OtherWorldPhase::OWP_SecondField) {
                ret << c->id();
            }
        }
    }
    return ret;
}

QStringList GameField::monsterIds() const
{
    QStringList ret;
    for (auto m : m_monsters) {
        ret << m->id();
    }
    return ret;
}

QString GameField::gateId() const
{
    if (m_gate) {
        return m_gate->id();
    }
    return QString::null;
}

QList<int> GameField::neighbourIds() const
{
    QList<int> ret;
    for (auto f : m_neighbours) {
        ret << f->id();
    }
    return ret;
}
