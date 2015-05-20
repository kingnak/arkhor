#include "die/dierolltest.h"
#include "die/dierollcountevaluator.h"
#include <QRegExp>
#include <QStringList>

DieRollTest::DieRollTest()
    : m_eval(NULL)
{

}

DieRollTest::~DieRollTest()
{
    delete m_eval;
}

DieRollTest *DieRollTest::parse(const QString &spec)
{
    DieRollTest *ret = new DieRollTest;
    // Format "(1)d(6){(4,5,6)}[(1)]?(>)?"

    QRegExp rx("(\\d+)d(\\d+)\\{(\\d+(?:,\\d+)*)\\}(?:\\[(\\d+)\\])?(>|<|>=|<=|=)?");
    if (rx.exactMatch(spec)) {
        quint8 count = rx.cap(1).toUInt();
        quint32 type = rx.cap(2).toUInt();
        QString successList = rx.cap(3);
        quint32 successCount = 1;
        if (rx.cap(4).length() > 0) {
            successCount = rx.cap(4).toUInt();
        }
        DieRollBoolEvaluator::EvaluationType compType = DieRollBoolEvaluator::GREATER_EQUALS;
        if (rx.cap(5).length() > 0) {
            QString compTypeStr = rx.cap(5);
            if (compTypeStr == ">") compType = DieRollBoolEvaluator::GREATER;
            else if (compTypeStr == ">=") compType = DieRollBoolEvaluator::GREATER_EQUALS;
            else if (compTypeStr == "<") compType = DieRollBoolEvaluator::LESS;
            else if (compTypeStr == "<=") compType = DieRollBoolEvaluator::LESS_EQUALS;
            else if (compTypeStr == "=") compType = DieRollBoolEvaluator::EQUALS;
        }

        QSet<quint32> successes;
        foreach (QString s, successList.split(',')) {
            successes << s.toUInt();
        }

        DieFactory::StandardDie stdDie = DieFactory::D6;
        switch (type) {
        case 2: stdDie = DieFactory::D2; break;
        case 10: stdDie = DieFactory::D10; break;
        case 20: stdDie = DieFactory::D20; break;
        case 6:
        default:
            break;
        }

        StandardDieSpec dieSpec(stdDie, count);
        DiePool pool = DiePool::createDiePool(QList<StandardDieSpec>() << dieSpec);
        DieRollCountBoolEvaluator *eval = new DieRollCountBoolEvaluator(pool, successes, successCount, compType);

        ret->m_eval = eval;
    }
    return ret;
}

bool DieRollTest::execute()
{
    if (m_eval) {
        m_eval->rerollAll();
        m_eval->evaluate();
        return m_eval->getBoolResult();
    } else {
        return false;
    }
}
