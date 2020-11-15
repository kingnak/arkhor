#include "die/diefactory.h"
#include "die/die.h"

DieFactory::DieFactory()
{
}

DieFactory &DieFactory::instance()
{
    static DieFactory inst;
    return inst;
}

Die *DieFactory::createStandardDie(DieFactory::StandardDie type)
{
    switch (type) {
    case D2: return createDN(2);
    case D6: return createDN(6);
    case D10: return createDN(10);
    case D20: return createDN(20);
    default: return NULL;
    }
}

Die *DieFactory::createDN(quint32 N)
{
    return new Die(1, N);
}

Die *DieFactory::createFixedDie(quint32 value)
{
    Die *d = new Die(value, value);
    d->roll();
    return d;
}
