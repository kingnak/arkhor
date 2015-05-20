#ifndef OTHERWORLDENCOUNTERY_H
#define OTHERWORLDENCOUNTERY_H

#include "../gamephase.h"

class OtherWorldEncounteryOption;

class OtherWorldEncountery : public GamePhase
{
public:
    OtherWorldEncountery(Game *game);
    ~OtherWorldEncountery();

protected:
    virtual QList<GameOption *> getPhaseOptions();

private:
    OtherWorldEncounteryOption *m_enc;
};

#endif // OTHERWORLDENCOUNTERY_H
