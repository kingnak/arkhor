#include "die/diepool.h"
#include "die/dierollresult.h"

#ifdef TEST_SCRIPT_BUILD
#include "scripttest/scripttestdierollwidget.h"
#include "scripttest/scripttestconfig.h"
#endif

DiePool::DiePool()
{
}

DiePool::DiePool(const DiePool &o)
{
    *this = o;
}

DiePool &DiePool::operator = (const DiePool &o) {

    foreach (Die *d, o.m_dice) {
        m_dice << d->clone();
    }
    return *this;
}

DiePool::~DiePool()
{
    qDeleteAll(m_dice);
}

DiePool DiePool::createDiePool(QList<StandardDieSpec> spec)
{
    DiePool p;
    p.addDice(spec);
    return p;
}

DiePool::DiePoolIndex DiePool::addDie(Die *d)
{
    m_dice.append(d);
    d->unroll();
    return static_cast<DiePoolIndex> (m_dice.size()-1);
}

QList<DiePool::DiePoolIndex> DiePool::addDice(QList<StandardDieSpec> spec)
{
    QList<DiePoolIndex> ret;
    foreach (StandardDieSpec s, spec) {
        for (quint8 i = 0; i < s.count; ++i) {
            ret << addDie(DieFactory::instance().createStandardDie(s.type));
        }
    }
    return ret;
}

void DiePool::roll()
{
#ifdef TEST_SCRIPT_BUILD
    if (ScriptTestConfig::askRollDie()) {
        QList<quint32> fixed;
        int toRoll = 0;
        for (Die *d : m_dice) {
            if (d->isRolled()) fixed << d->value();
            else toRoll++;
        }

        if (toRoll == 0) return;
        ScriptTestDieRollWidget wgt;
        QList<quint32> rolled = wgt.roll(fixed, toRoll);
        if (!rolled.isEmpty()) {
            int ct = 0;
            for (Die *d : m_dice) {
                if (!d->isRolled()) d->m_value = rolled.value(ct++, 1);
            }
            return;
        }
    }
#endif

    foreach (Die *d, m_dice) {
        if (!d->isRolled()) {
            d->roll();
        }
    }
}

void DiePool::unroll()
{
    foreach (Die *d, m_dice) {
        d->unroll();
    }
}

void DiePool::reroll(const QSet<DiePool::DiePoolIndex> &dice)
{
#ifdef TEST_SCRIPT_BUILD
    if (ScriptTestConfig::askRollDie()) {
        QList<quint32> fixed;
        int toRoll = 0;
        for (int i = 0; i < m_dice.count(); ++i) {
            if (dice.contains(i)) toRoll++;
            else fixed << m_dice[i]->value();
        }

        if (toRoll == 0) return;
        ScriptTestDieRollWidget wgt;
        QList<quint32> rolled = wgt.roll(fixed, toRoll);
        if (!rolled.isEmpty()) {
            int ct = 0;
            for (int i = 0; i < m_dice.count(); ++i) {
                if (dice.contains(i)) m_dice[i]->m_value = rolled.value(ct++, 1);
            }
            return;
        }
    }
#endif

    foreach (DiePoolIndex idx, dice) {
        m_dice[idx]->roll();
    }
}

DieRollResult DiePool::getResult() const
{
    DieRollResultCreator res;
    for (int i = 0; i < m_dice.size(); ++i) {
        if (m_dice[i]->isRolled()) {
            res.add(i, m_dice[i]->value());
        }
    }

    return res;
}
