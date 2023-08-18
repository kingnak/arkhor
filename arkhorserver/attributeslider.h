#ifndef ATTRIBUTESLIDER_H
#define ATTRIBUTESLIDER_H

#include <ahglobal.h>
#include <attributesliderdata.h>


class AttributeSlider : public AH::Common::AttributeSliderData
{
public:
    AttributeSlider();

    bool addAttributePair(AH::Common::AttributePair pair);

    bool shiftLeft();
    bool shiftRight();
    bool shiftBy(int amount);

    AH::Common::AttributePair currentSetting() const;

    //QList<AttributePair> availableSettings() const { return m_attributeSettings; }

    int distanceBetween(AH::Common::AttributePair p1, AH::Common::AttributePair p2) const;

private:
    //QList<AttributePair> m_attributeSettings;
    //qint16 m_curSetting;
};

#endif // ATTRIBUTESLIDER_H
