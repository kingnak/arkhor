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

    int size() const { return m_deck.size(); }
    void shuffle();

    T *draw()
    { return m_deck.isEmpty() ? NULL : m_deck.takeFirst(); }
    T *drawSpecificById(QString id);
    T *drawSpecificByTypeId(QString tid);
    T *peekTop()
    { return m_deck.isEmpty() ? NULL : m_deck.first(); }
    T *peekBottom()
    { return m_deck.isEmpty() ? NULL : m_deck.last(); }
    T *drawBottom()
    { return m_deck.isEmpty() ? NULL : m_deck.takeLast(); }
    void returnToDeck(T *t)
    { if (t) m_deck.append(t); }

#ifdef TEST_SCRIPT_BUILD
    QList<T*> all() { return m_deck; }
#endif

private:
    QList<T *> m_deck;
};

template<typename T>
T *Deck<T>::drawSpecificById(QString id)
{
    T *ret = NULL;
    foreach (T *t, m_deck) {
        if (t->id() == id) {
            ret = t;
            break;
        }
    }

    if (ret)
        m_deck.removeAll(ret);
    return ret;
}

template<typename T>
T *Deck<T>::drawSpecificByTypeId(QString tid)
{
    T *ret = NULL;
    foreach (T *t, m_deck) {
        if (t->typeId() == tid) {
            ret = t;
            break;
        }
    }

    if (ret)
        m_deck.removeAll(ret);
    return ret;
}

template<typename T>
void shuffle_list(QList<T> &l)
{
    for (int i = l.size()-1; i >= 0; --i) {
        int n = RandomSource::instance().nextUint(0, i);
        l.swap(n, i);
    }
}

template<typename T>
void Deck<T>::shuffle()
{
    shuffle_list(m_deck);
}

#endif // DECK_HPP
