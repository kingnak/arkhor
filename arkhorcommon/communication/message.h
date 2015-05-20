#ifndef MESSAGE_H
#define MESSAGE_H

#include "arkhorcommonglobal.h"
#include <QVariant>
#include <serializer.hpp>

namespace AH{
    namespace Common {
        struct ARKHOR_COMMON_EXPORTS Message
        {
            enum Type {
                NO_MESSAGE,

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

                C_REQUEST_OBJECTS,
                S_DESCRIBE_OBJECTS,

                S_CHOOSE_INVESTIGATOR,
                C_SELECT_INVESTIGATOR,
                S_PLAYER_CHAR_INSTANTIATED,

                S_DIE_ROLL_INFO,
                C_DIE_ROLL_UPDATE,

                S_GAME_START,

                S_CHOOSE_OPTION,
                C_SELECT_OPTION,

                S_CHOOSE_MOVEMENT_PATH,
                C_MOVE_PATH,

                S_CHOOSE_FOCUS,
                C_SELECT_FOCUS,

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
