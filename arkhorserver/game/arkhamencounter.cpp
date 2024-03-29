#include "arkhamencounter.h"
#include "game/gameoption.h"


QList<AH::Common::GameOptionData> ArkhamEncounter::optionData() const
{
    QList<AH::Common::GameOptionData> ret;
    for (auto o : options()) {
#ifdef DEBUG_SCRIPT_BUILD
        if (!o) continue;
#endif
        ret << *o->data();
    }
    return ret;
}
