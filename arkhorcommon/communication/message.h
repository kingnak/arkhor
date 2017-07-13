#ifndef MESSAGE_H
#define MESSAGE_H

#include "arkhorcommonglobal.h"
#include <QVariant>
#include <serializer.hpp>

namespace AH{
    namespace Common {
        struct ARKHOR_COMMON_EXPORTS Message
        {
            static const quint32 PROTOCOL_VERSION = 0x000100;

            enum Type {
                NO_MESSAGE,

                C_VERSION,
                S_VERSION,

                S_TEXT_MESSAGE,

                MSG_PING,
                MSG_PONG,

                S_PROMPT_ACTIVE,
                C_CONFIRM_ACTIVE,

                S_KILL,
                S_PLAYER_REMOVED,


                C_REGISTER_PLAYER,
                S_REGISTER_PLAYER_SUCCESSFUL,
                S_REGISTER_PLAYER_FAILED,

                C_START_GAME,
                S_GAME_STARTED,

                S_BOARD_CONTENT,
                S_CHARACTER_DATA,
                S_SETTING_DATA,

                C_REQUEST_OBJECTS,
                S_DESCRIBE_OBJECTS,
                S_INVALIDATE_OBJECTS,
                S_INVALIDATE_OBJECTS_BY_TYPE,

                S_CHOOSE_INVESTIGATOR,
                C_SELECT_INVESTIGATOR,
                S_PLAYER_CHAR_INSTANTIATED,

                S_DIE_ROLL_INFO,
                C_DIE_ROLL_UPDATE,

                S_GAME_START,

                S_CHOOSE_OPTION,
                C_SELECT_OPTION,

                S_CHOOSE_SKILL,
                C_SELECT_SKILL,

                //S_CHOOSE_PAYMENT,
                //C_SELECT_PAYMENT,

                S_CHOOSE_MOVEMENT_PATH,
                C_MOVE_PATH,

                S_CHOOSE_FOCUS,
                C_SELECT_FOCUS,

                S_CHOOSE_WEAPONS,
                C_SELECT_WEAPONS,
                C_CANCEL_WEAPONS,

                S_CHOOSE_ENCOUNTER,
                C_SELECT_ENCOUNTER,

                S_ACKNOWLEDGE_MYTHOS,
                S_ABORT_ACKNOWLEDGE,
                C_ACKNOWLEDGED,

                S_OFFER_CHOICE,
                C_SELECT_CHOICE,
                C_CANCEL_CHOICE,

                S_SET_TEMP_DATA,
                S_CLEAR_TEMP_DATA,

                S_WON,
                S_LOST,
                S_DIED,
				S_GAME_ALERT,

                DONT_UNDERSTAND = 0xFFFFFFFE,
                INVALID_MESSAGE = 0xFFFFFFFF
            } type;
            QVariant payload;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, Message);
        };
    }
}

DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, AH::Common::Message::Type);
Q_DECLARE_METATYPE(AH::Common::Message::Type)

#endif // MESSAGE_H
