#ifndef ATTRIBUTESLIDER_H
#define ATTRIBUTESLIDER_H

#include <ahglobal.h>
#include <QList>

struct AttributeValue
{
    AH::Attribute attribute;
    quint8 value;

    AttributeValue(AH::Attribute attribute, quint8 value)
        : attribute(attribute), value(value) {}

    bool operator == (const AttributeValue &o) const {
        return attribute == o.attribute && value == o.value;
    }
};

class AttributePair
{
public:
    AttributePair(AttributeValue first, AttributeValue second)
        : m_first(first), m_second(second) {}

    const AttributeValue first() const { return m_first; }
    const AttributeValue second() const { return m_second; }

    AttributePair() : m_first(AH::NoAttribute, 0), m_second(AH::NoAttribute, 0) {}

    bool operator == (const AttributePair &o) const {
        return m_first == o.m_first && m_second == o.m_second;
    }

private:
    AttributeValue m_first;
    AttributeValue m_second;
};

class AttributeSlider
{
public:
    AttributeSlider();

    bool addAttributePair(AttributePair pair);

    bool shiftLeft();
    bool shiftRight();
    bool shiftBy(int amount);

    const AttributePair currentSetting() const;

    QList<AttributePair> availableSettings() const { return m_attributeSettings; }

    int distanceBetween(AttributePair p1, AttributePair p2) const;

private:
    QList<AttributePair> m_attributeSettings;
    qint8 m_curSetting;
};

#endif // ATTRIBUTESLIDER_H
