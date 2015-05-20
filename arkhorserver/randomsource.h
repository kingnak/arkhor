#ifndef RANDOMSOURCE_H
#define RANDOMSOURCE_H

#include <QtGlobal>

class RandomSource
{
public:
    static RandomSource &instance();

    quint32 nextUint(quint32 min = 0, quint32 max = 0xFFFFFFFF);

protected:
    RandomSource();

private:
    Q_DISABLE_COPY(RandomSource);
};

#endif // RANDOMSOURCE_H
