#ifndef INVESTIGATOR_H
#define INVESTIGATOR_H

#include <investigatordata.h>
#include "game/game.h"

class Character;

class Investigator : public AH::Common::InvestigatorData
{
public:
    Investigator();

    bool isInstantiated() const { return m_isInstantiated; }
    virtual Character *instantiate();
    //void setInstantiated( bool instantiated ) { m_instantiated = instantiated; }
    void reset() { m_isInstantiated = false; }

    bool resolveDependecies(GameRegistry *reg) const;

protected:
    bool m_isInstantiated;
};

#endif // INVESTIGATOR_H
