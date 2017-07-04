#ifndef ARKHAMENCOUNTER_H
#define ARKHAMENCOUNTER_H

#include <encounterdata.h>

class GameOption;
class GameRegistry;

class ArkhamEncounter : public AH::Common::EncounterData
{
public:

    virtual QList<GameOption *> options() const = 0;
    virtual bool resolveDependencies(GameRegistry *reg) = 0;

    QList<AH::Common::GameOptionData> optionData() const;

protected:
    QStringList m_optionIds;

#ifdef TEST_SCRIPT_BUILD
    QString m_id;
public:
    QString id() const { return m_id; }
#endif
};

#endif // ARKHAMENCOUNTER_H
