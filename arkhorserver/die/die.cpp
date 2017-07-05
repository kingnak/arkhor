#include "die/die.h"
#include "randomsource.h"


Die::Die(quint32 min, quint32 max, quint32 multiplier)
    : m_min(min), m_max(max), m_multiplier(multiplier), m_value(UNROLLED_VALUE), m_isRolled(false)
{
}

void Die::roll()
{
    if (!m_isRolled) {
        quint32 v = RandomSource::instance().nextUint(m_min, m_max);
        m_value = v*m_multiplier;
        m_isRolled = true;
    }
}

void Die::unroll()
{
    m_value = UNROLLED_VALUE;
    m_isRolled = false;
}

Die *Die::clone() const
{
    return new Die(m_min, m_max, m_multiplier);
}

////////////////////////////////////

UnequalDie::UnequalDie(const QList<quint32> &values)
    : Die(0, values.size()), m_values(values)
{

}

quint32 UnequalDie::value() const
{
    if (!isRolled()) {
        return Die::value();
    }

    return m_values.value(Die::value());
}

Die *UnequalDie::clone() const
{
    return new UnequalDie(m_values);
}
