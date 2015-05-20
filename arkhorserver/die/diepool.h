#ifndef DIEPOOL_H
#define DIEPOOL_H

#include <QList>
#include <QSet>
#include "die/die.h"
#include "die/diefactory.h"

class DieRollResult;

struct StandardDieSpec {
    DieFactory::StandardDie type;
    quint8 count;
    StandardDieSpec(DieFactory::StandardDie type, quint8 count)
        : type(type), count(count) { }
};

class DiePool
{
public:
    typedef quint32 DiePoolIndex;

    DiePool();
    DiePool(const DiePool &o);
    DiePool &operator = (const DiePool &o);
    ~DiePool();

    static DiePool createDiePool(QList<StandardDieSpec> spec);

    /**
     * @brief addDie Adds a unrolled die
     * @param d The die to add
     * @return The index of the added die
     */
    DiePoolIndex addDie(Die *d);

    /**
     * @brief roll Rolls all unrolled dice
     */
    void roll();
    /**
     * @brief unroll Unrolls all dice
     */
    void unroll();
    /**
     * @brief reroll Rerolls the given dice, no matter their state
     * @param dice The indices of dice to reroll
     */
    void reroll(const QSet<DiePoolIndex> &dice);

    /**
     * @brief getResult Gets the current die roll result. Does not include unrolled dice
     * @return The die roll result
     */
    DieRollResult getResult() const;

private:
    QList<Die*> m_dice;
};

#endif // DIEPOOL_H
