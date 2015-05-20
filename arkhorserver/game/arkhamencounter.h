#ifndef ARKHAMENCOUNTER_H
#define ARKHAMENCOUNTER_H

#include <arkhamencounterdata.h>

class GameOption;
class GameRegistry;

class ArkhamEncounter : public AH::Common::ArkhamEncounterData
{
public:

    virtual QList<GameOption *> options() const = 0;
    virtual bool resolveDependencies(GameRegistry *reg) = 0;

    QList<AH::Common::GameOptionData> optionData() const;

protected:
    QStringList m_optionIds;
};

#endif // ARKHAMENCOUNTER_H
