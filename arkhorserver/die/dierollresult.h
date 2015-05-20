#ifndef DIEROLLRESULT_H
#define DIEROLLRESULT_H

#include "die/diepool.h"
#include <QList>

class DieRollResultItem
{
public:
    DiePool::DiePoolIndex index() const { return m_idx; }
    quint32 value() const { return m_value; }

    DieRollResultItem() : m_idx(-1), m_value(Die::UNROLLED_VALUE) {}
    DieRollResultItem(const DieRollResultItem &o) : m_idx(o.m_idx), m_value(o.m_value) {}

private:
    DieRollResultItem(DiePool::DiePoolIndex idx, quint32 value) : m_idx(idx), m_value(value) {}

    friend class DieRollResult;

    DiePool::DiePoolIndex m_idx;
    quint32 m_value;
};

class DieRollResult : private QList<DieRollResultItem>
{
public:
    DieRollResult();

    // make usable as QList
    using QList<DieRollResultItem>::begin;
    using QList<DieRollResultItem>::end;
    using QList<DieRollResultItem>::iterator;
    using QList<DieRollResultItem>::const_iterator;

protected:
    void add(DiePool::DiePoolIndex idx, quint32 value);
};

class DieRollResultCreator : public DieRollResult
{
public:
    using DieRollResult::add;
};

#endif // DIEROLLRESULT_H
