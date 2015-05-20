#ifndef ARKHAMENCOUNTER_H
#define ARKHAMENCOUNTER_H

#include <arkhamencounterdata.h>

class GameOption;
class Game;

class ArkhamEncounter : public AH::Common::ArkhamEncounterData
{
public:

    virtual QList<GameOption *> options() const = 0;
    virtual bool resolveDependencies(const Game *game) = 0;
};

#endif // ARKHAMENCOUNTER_H
