#ifndef DIE_H
#define DIE_H

#include <QtGlobal>
#include <QList>

class Die
{
public:
    static const quint32 UNROLLED_VALUE = 0xDEADBEEF;

    Die(quint32 min = 1, quint32 max = 6, quint32 multiplier = 1);
    virtual ~Die() = default;

    void roll();
    void unroll();
    void reroll() { unroll(); roll(); }
    virtual quint32 value() const {
        if (!isRolled()) {
            return UNROLLED_VALUE;
        }
        return m_value;
    }

    bool isRolled() const {
        return m_isRolled;
    }

    virtual Die *clone() const;

protected:
    quint32 m_min;
    quint32 m_max;
    quint32 m_multiplier;
    quint32 m_value;

    bool m_isRolled;

#ifdef TEST_SCRIPT_BUILD
    friend class DiePool;
    void setValue(quint32 v) {
        m_value = v;
        m_isRolled = true;
    }

#endif
};

class UnequalDie : public Die
{
public:
    UnequalDie(const QList<quint32> &values);

    quint32 value() const override;
    Die *clone() const override;

private:
    QList<quint32> m_values;
};

#endif // DIE_H
