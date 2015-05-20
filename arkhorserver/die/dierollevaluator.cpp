#include "die/dierollevaluator.h"


///////////////////////////////////////////////////////////////////////

bool DieRollBoolEvaluator::boolEvaluate(quint32 val) const
{
    switch (m_evaluationType) {
    case EQUALS:        return val == m_target;
    case LESS:          return val <  m_target;
    case LESS_EQUALS:   return val <= m_target;
    case GREATER:       return val >  m_target;
    case GREATER_EQUALS:return val >= m_target;
    default:            return false;
    }
}
