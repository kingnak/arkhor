#ifndef DIEFACTORY_H
#define DIEFACTORY_H

#include <QtGlobal>

class Die;

class DieFactory
{
public:
    static DieFactory &instance();

    enum StandardDie {
        D6, D10, D20, D2
    };

    Die *createStandardDie(StandardDie type);
    Die *createDN(quint32 N);
    Die *createFixedDie(quint32 value);

private:
    DieFactory();
    Q_DISABLE_COPY(DieFactory)
};

#endif // DIEFACTORY_H
