#include "die/diepool.h"
#include "die/dierollresult.h"

#ifdef TEST_SCRIPT_BUILD
#include "scripttest/scripttestdierollwidget.h"
#include "scripttest/scripttestconfig.h"
#endif

DiePool::DiePool() = default;

DiePool::DiePool(const DiePool &o)
{
    *this = o;
}

DiePool &DiePool::operator = (const DiePool &o) {

    for (auto d : o.m_dice) {
        m_dice << d->clone();
    }
    return *this;
}

DiePool::~DiePool()
{
    qDeleteAll(m_dice);
}

DiePool DiePool::createDiePool(const QList<StandardDieSpec> &spec)
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

QList<DiePool::DiePoolIndex> DiePool::addDice(const QList<StandardDieSpec> &spec)
{
    QList<DiePoolIndex> ret;
    for (auto s : spec) {
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
        QString s;
        for (auto d : m_dice) {
            if (d->isRolled()) s += ('0'+d->value());
            else s += '0';
        }

        if (s.indexOf('0') >= 0) {
            s = ScriptTestDieRollWidget().roll(s);
            if (s.length() < m_dice.length()) s += QString(m_dice.count() - s.length(), '0');
        }

        for (int i = 0; i < m_dice.count(); ++i) {
            int v = qMax(s.at(i).isDigit() ? int(s.at(i).cell()-'0') : 0, 0);
            if (v < 1 || v > 6)
                m_dice[i]->roll();
            else
                m_dice[i]->setValue(static_cast<quint32> (v));
        }
        return;
    }
#endif

    for (auto d : m_dice) {
        d->roll();
    }
}


void DiePool::unroll()
{
    for (auto d : m_dice) {
        d->unroll();
    }
}

void DiePool::reroll(const QSet<DiePool::DiePoolIndex> &dice)
{
    for (DiePoolIndex idx : dice) {
        m_dice[idx]->unroll();
    }
    roll();
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
