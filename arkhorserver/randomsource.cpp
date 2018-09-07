#include "randomsource.h"
#include <ctime>

RandomSource::RandomSource()
{
    qsrand(time(nullptr));
}

RandomSource &RandomSource::instance()
{
    static RandomSource inst;
    return inst;
}

quint32 RandomSource::nextUint(quint32 min, quint32 max)
{
    quint32 v = qrand();
    quint32 diff = max-min+1;
    v %= diff;
    v += min;
    return v;
}
