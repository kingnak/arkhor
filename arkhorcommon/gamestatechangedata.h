#ifndef GAMESTATECHANGEDATA_H
#define GAMESTATECHANGEDATA_H

#include "arkhorcommonglobal.h"
#include "fielddata.h"

namespace AH {
    namespace Common {

        struct ARKHOR_COMMON_EXPORTS GameBoardChangeData
        {
            typedef QList<FieldData::FieldID> MovementPath;
            struct Movement {
                QString id;
                //FieldData::FieldID start;
                //FieldData::FieldID end;
                MovementPath path;
            };

            struct LocatedChange {
                QString id;
                FieldData::FieldID location;
            };

            QList<Movement> characterMovements;
            QList<Movement> monsterMovements;
            QList<LocatedChange> monsterAppear;
            QList<LocatedChange> monsterDisappear;
            QList<LocatedChange> gateAppear;
            QList<LocatedChange> gateDisappear;
            QList<LocatedChange> gateOpen;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, GameBoardChangeData);
            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, GameBoardChangeData::Movement);
            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, GameBoardChangeData::LocatedChange);
        };
    }
}

#endif // GAMESTATECHANGEDATA_H
