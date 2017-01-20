#include "preventdamageaction.h"
#include <game/gameobject.h>

quint32 PreventDamageOption::s_nextId = 0;

QString PreventDamageOption::name() const
{
    QStringList parts;
    if (canPreventStamina()) parts << "Stamina";
    if (canPreventSanity()) parts << "Sanity";

    if (parts.isEmpty()) return "";
    return QString("Prevent %1 damage").arg(parts.join(" and "));
}

QString PreventDamageOption::description() const
{
    if (m_overrideDescription.isEmpty()) return m_baseDescription;
    return m_baseDescription + "\n\n" + m_overrideDescription;
}

bool PreventDamageOption::isAvailable() const
{
    if (m_source) {
        if (m_source->hasPreventedDamage() || m_source->isExhausted()) {
            return false;
        }
        return true;
    }
    // TODO: Option without object?
    return false;
}

QString PreventDamageOption::sourceId() const
{
    if (m_source) {
        return m_source->id();
    }
    return GameOption::sourceId();
}

void PreventDamageOption::updateOverrideDescription(int damageSta, int damageSan)
{
    QStringList parts;
    if (damageSta > 0) parts << QString("%1 Stamina").arg(damageSta);
    if (damageSan > 0) parts << QString("%1 Sanity").arg(damageSan);
    if (parts.isEmpty()) m_overrideDescription = "";
    else m_overrideDescription = QString("(Currently %1 damage)").arg(parts.join(" and "));
}

void PreventDamageOption::reset()
{
    m_source = NULL;
    m_overrideDescription = "";
}
