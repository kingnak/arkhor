#ifndef DECK_HPP
#define DECK_HPP

#include <QList>
#include "randomsource.h"

template <typename T>
class Deck
{
public:
    void addCard(T *t)
    { m_deck.append(t); }

    void shuffle();

    T *draw()
    { return m_deck.takeFirst(); }
    T *drawSpecific(T *t);
    T *drawSpecificById(QString id);
    T *peekTop()
    { return m_deck.first(); }
    T *peekBottom()
    { return m_deck.last(); }
    T *drawBottom()
    { return m_deck.takeLast(); }

private:
    QList<T *> m_deck;
};

template<typename T>
T *Deck::drawSpecific(T *t)
{
    m_deck.removeAll(t);
    return t;
}

template<typename T>
T *Deck::drawSpecificById(QString id)
{
    T *ret = NULL;
    foreach (T *t, m_deck) {
        if (t->id() == id) {
            ret = t;
            break;
        }
    }

    return drawSpecific(ret);
}

template<typename T>
void Deck::shuffle()
{
    for (int i = m_deck.size()-1; i >= 0; --i) {
        int n = RandomSource::instance().nextUint(0, i);
        m_deck.swap(n, i);
    }
}

#endif // DECK_HPP
