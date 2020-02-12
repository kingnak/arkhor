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
                MovementPath path;
            };

            struct LocatedChange {
                QString id;
                FieldData::FieldID location;
            };

            struct FieldChange {
                FieldData::FieldID location;
                int clue = 0;
                bool lock = false;
                bool unlock = false;
                bool seal = false;
                bool unseal = false;
                int eventNr = false;
                FieldChange(FieldData::FieldID id = FieldData::NO_NO_FIELD) : location(id) {}
                static FieldChange addClues(FieldData::FieldID id, int nr) { FieldChange f(id); f.clue = nr; return f; }
                static FieldChange lockField(FieldData::FieldID id) { FieldChange f(id); f.lock = true; return f; }
                static FieldChange unlockField(FieldData::FieldID id) { FieldChange f(id); f.unlock = true; return f; }
                static FieldChange sealField(FieldData::FieldID id) { FieldChange f(id); f.seal = true; return f; }
                static FieldChange unsealField(FieldData::FieldID id) { FieldChange f(id); f.unseal = true; return f; }
                static FieldChange setEvent(FieldData::FieldID id, int nr) { FieldChange f(id); f.eventNr = nr; return f; }
            };

            QList<Movement> characterMovements;
            QList<Movement> monsterMovements;
            QList<LocatedChange> monsterAppear;
            QList<LocatedChange> monsterDisappear;
            QList<LocatedChange> gateAppear;
            QList<LocatedChange> gateDisappear;
            QList<LocatedChange> gateOpen;
            QList<FieldChange> fieldChanges;
            QStringList clearOutskirts;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, GameBoardChangeData);
            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, GameBoardChangeData::Movement);
            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, GameBoardChangeData::LocatedChange);
            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, GameBoardChangeData::FieldChange);
        };
    }
}

#endif // GAMESTATECHANGEDATA_H
