#include "otherworldencounter.h"
#include "game/gameoption.h"


QList<AH::Common::GameOptionData> OtherWorldEncounter::optionData() const
{
    QList<AH::Common::GameOptionData> ret;
    for (auto o : options()) {
        ret << *o->data();
    }
    return ret;
}
