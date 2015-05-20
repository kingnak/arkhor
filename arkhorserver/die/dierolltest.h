#ifndef DIEROLLTEST_H
#define DIEROLLTEST_H

#include "die/diepool.h"
#include "die/dierollcountevaluator.h"
#include <QString>


class DieRollTest
{
public:
    virtual ~DieRollTest();
    static DieRollTest *parse(const QString &spec);

    bool execute();

protected:
    DieRollBoolEvaluator *m_eval;

    DieRollTest();
    Q_DISABLE_COPY(DieRollTest)
};

#endif // DIEROLLTEST_H
