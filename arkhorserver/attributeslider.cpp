#include "attributeslider.h"

using namespace AH::Common;

AttributeSlider::AttributeSlider()
{
    m_curSettingPos = -1;
}

bool AttributeSlider::addAttributePair(AttributePair pair)
{
    // Must have 2 valid, different attributes
    if (pair.first().attribute == AH::NoAttribute || pair.second().attribute == AH::NoAttribute) {
        return false;
    }
    if (pair.first().attribute == pair.second().attribute) {
        return false;
    }

    if (m_attributeSettings.isEmpty()) {
        m_attributeSettings.append(pair);
        m_curSettingPos = 0;
        return true;
    } else {
        // Must have same settings
        if (m_attributeSettings.value(0).first().attribute == pair.first().attribute
         && m_attributeSettings.value(0).second().attribute == pair.second().attribute )
        {
            m_attributeSettings.append(pair);
            return true;
        }
        return false;
    }
}

bool AttributeSlider::shiftLeft()
{
    if (m_curSettingPos > 0) {
        m_curSettingPos--;
        return true;
    }
    return false;
}

bool AttributeSlider::shiftRight()
{
    if (m_curSettingPos < m_attributeSettings.size()-1) {
        m_curSettingPos++;
        return true;
    }
    return false;
}

bool AttributeSlider::shiftBy(int amount)
{
    while (amount < 0) {
        if (!shiftLeft()) return false;
        amount ++;
    }

    while (amount > 0) {
        if (!shiftRight()) return false;
        amount --;
    }

    return true;
}

const AttributePair AttributeSlider::currentSetting() const
{
    return m_attributeSettings.value(m_curSettingPos);
}

int AttributeSlider::distanceBetween(AttributePair p1, AttributePair p2) const
{
    int idx1 = m_attributeSettings.indexOf(p1);
    int idx2 = m_attributeSettings.indexOf(p2);
    if (idx1 < 0 || idx2 < 0) {
        return INT_MAX;
    }
    return idx2 - idx1;
}
