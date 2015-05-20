#ifndef OTHERWORLDENCOUNTER_H
#define OTHERWORLDENCOUNTER_H

#include <encounterdata.h>
//#include "gameoption.h"
class GameOption;
class GameRegistry;

class OtherWorldEncounter : public AH::Common::EncounterData
{
public:
    OtherWorldEncounter() : m_color(AH::OWC_NoColor) {}
    ~OtherWorldEncounter() {}

    AH::OtherWorldColor color() const { return m_color; }
    virtual QList<GameOption *> options() const = 0;
    virtual bool resolveDependencies(GameRegistry *reg) = 0;

    QList<AH::Common::GameOptionData> optionData() const;

protected:
    QStringList m_optionIds;
    AH::OtherWorldColor m_color;
};

#endif // OTHERWORLDENCOUNTER_H
