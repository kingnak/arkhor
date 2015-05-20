#include "arkhamencounter.h"
#include "game/gameoption.h"


QList<AH::Common::GameOptionData> ArkhamEncounter::optionData() const
{
    QList<AH::Common::GameOptionData> ret;
    foreach (GameOption *o, options()) {
        ret << *o->data();
    }
    return ret;
}
