#include "message.h"

namespace AH{ namespace Common {

void operator <<(QVariant &data, const Message &target)
{
    QVariantMap map;
    map["type"] << target.type;
    map["payload"] << target.payload;
    data << map;
}

void operator >>(const QVariant &data, Message &target)
{
    QVariantMap map;
    data >> map;
    map["type"] >> target.type;
    map["payload"] >> target.payload;
}

QString Message::msg_to_str(Message::Type t)
{
    switch (t) {
    case AH::Common::Message::Type::NO_MESSAGE: return "NO_MESSAGE";
    case AH::Common::Message::Type::C_VERSION: return "C_VERSION";
    case AH::Common::Message::Type::S_VERSION: return "S_VERSION";
    case AH::Common::Message::Type::C_REGISTER_CLIENT: return "C_REGISTER_CLIENT";
    case AH::Common::Message::Type::S_REGISTER_CLIENT: return "S_REGISTER_CLIENT";
    case AH::Common::Message::Type::S_TEXT_MESSAGE: return "S_TEXT_MESSAGE";
    case AH::Common::Message::Type::MSG_PING: return "MSG_PING";
    case AH::Common::Message::Type::MSG_PONG: return "MSG_PONG";
    case AH::Common::Message::Type::S_PROMPT_ACTIVE: return "S_PROMPT_ACTIVE";
    case AH::Common::Message::Type::C_CONFIRM_ACTIVE: return "C_CONFIRM_ACTIVE";
    case AH::Common::Message::Type::S_KILL: return "S_KILL";
    case AH::Common::Message::Type::S_PLAYER_REMOVED: return "S_PLAYER_REMOVED";
    case AH::Common::Message::Type::C_REGISTER_PLAYER: return "C_REGISTER_PLAYER";
    case AH::Common::Message::Type::S_REGISTER_PLAYER_SUCCESSFUL: return "S_REGISTER_PLAYER_SUCCESSFUL";
    case AH::Common::Message::Type::S_REGISTER_PLAYER_FAILED: return "S_REGISTER_PLAYER_FAILED";
    case AH::Common::Message::Type::C_START_GAME: return "C_START_GAME";
    case AH::Common::Message::Type::S_GAME_STARTED: return "S_GAME_STARTED";
    case AH::Common::Message::Type::S_BOARD_CONTENT: return "S_BOARD_CONTENT";
    case AH::Common::Message::Type::S_BOARD_DESCRIPTION: return "S_BOARD_DESCRIPTION";
    case AH::Common::Message::Type::S_CHARACTER_DATA: return "S_CHARACTER_DATA";
    case AH::Common::Message::Type::S_SETTING_DATA: return "S_SETTING_DATA";
    case AH::Common::Message::Type::C_REQUEST_OBJECTS: return "C_REQUEST_OBJECTS";
    case AH::Common::Message::Type::S_DESCRIBE_OBJECTS: return "S_DESCRIBE_OBJECTS";
    case AH::Common::Message::Type::S_INVALIDATE_OBJECTS: return "S_INVALIDATE_OBJECTS";
    case AH::Common::Message::Type::S_INVALIDATE_OBJECTS_BY_TYPE: return "S_INVALIDATE_OBJECTS_BY_TYPE";
    case AH::Common::Message::Type::S_CHOOSE_INVESTIGATOR: return "S_CHOOSE_INVESTIGATOR";
    case AH::Common::Message::Type::C_SELECT_INVESTIGATOR: return "C_SELECT_INVESTIGATOR";
    case AH::Common::Message::Type::S_PLAYER_CHAR_INSTANTIATED: return "S_PLAYER_CHAR_INSTANTIATED";
    case AH::Common::Message::Type::S_DIE_ROLL_INFO: return "S_DIE_ROLL_INFO";
    case AH::Common::Message::Type::C_DIE_ROLL_UPDATE: return "C_DIE_ROLL_UPDATE";
    case AH::Common::Message::Type::S_GAME_START: return "S_GAME_START";
    case AH::Common::Message::Type::S_PHASE_CHANGE: return "S_PHASE_CHANGE";
    case AH::Common::Message::Type::S_PLAYER_CHANGE: return "S_PLAYER_CHANGE";
    case AH::Common::Message::Type::S_CHOOSE_OPTION: return "S_CHOOSE_OPTION";
    case AH::Common::Message::Type::C_SELECT_OPTION: return "C_SELECT_OPTION";
    case AH::Common::Message::Type::S_CHOOSE_SKILL: return "S_CHOOSE_SKILL";
    case AH::Common::Message::Type::C_SELECT_SKILL: return "C_SELECT_SKILL";
    //case AH::Common::Message::Type::S_CHOOSE_PAYMENT: return "S_CHOOSE_PAYMENT";
    //case AH::Common::Message::Type::C_SELECT_PAYMENT: return "C_SELECT_PAYMENT";
    case AH::Common::Message::Type::S_CHOOSE_MOVEMENT_PATH: return "S_CHOOSE_MOVEMENT_PATH";
    case AH::Common::Message::Type::C_MOVE_PATH: return "C_MOVE_PATH";
    case AH::Common::Message::Type::S_CHOOSE_FOCUS: return "S_CHOOSE_FOCUS";
    case AH::Common::Message::Type::C_SELECT_FOCUS: return "C_SELECT_FOCUS";
    case AH::Common::Message::Type::S_CHOOSE_WEAPONS: return "S_CHOOSE_WEAPONS";
    case AH::Common::Message::Type::C_SELECT_WEAPONS: return "C_SELECT_WEAPONS";
    case AH::Common::Message::Type::C_CANCEL_WEAPONS: return "C_CANCEL_WEAPONS";
    case AH::Common::Message::Type::S_CHOOSE_ENCOUNTER: return "S_CHOOSE_ENCOUNTER";
    case AH::Common::Message::Type::C_SELECT_ENCOUNTER: return "C_SELECT_ENCOUNTER";
    case AH::Common::Message::Type::S_CHOOSE_MONSTER: return "S_CHOOSE_MONSTER";
    case AH::Common::Message::Type::C_SELECT_MONSTER: return "C_SELECT_MONSTER";
    case AH::Common::Message::Type::S_ACKNOWLEDGE_MYTHOS: return "S_ACKNOWLEDGE_MYTHOS";
    case AH::Common::Message::Type::S_ACKNOWLEDGE_MONSTER_MOVEMENT: return "S_ACKNOWLEDGE_MONSTER_MOVEMENT";
    case AH::Common::Message::Type::S_ABORT_ACKNOWLEDGE: return "S_ABORT_ACKNOWLEDGE";
    case AH::Common::Message::Type::C_ACKNOWLEDGED: return "C_ACKNOWLEDGED";
    case AH::Common::Message::Type::S_OFFER_CHOICE: return "S_OFFER_CHOICE";
    case AH::Common::Message::Type::C_SELECT_CHOICE: return "C_SELECT_CHOICE";
    case AH::Common::Message::Type::C_CANCEL_CHOICE: return "C_CANCEL_CHOICE";
    case AH::Common::Message::Type::S_SET_TEMP_DATA: return "S_SET_TEMP_DATA";
    case AH::Common::Message::Type::S_CLEAR_TEMP_DATA: return "S_CLEAR_TEMP_DATA";
    case AH::Common::Message::Type::S_WON: return "S_WON";
    case AH::Common::Message::Type::S_LOST: return "S_LOST";
    case AH::Common::Message::Type::S_DIED: return "S_DIED";
    case AH::Common::Message::Type::S_GAME_ALERT: return "S_GAME_ALERT";
    case AH::Common::Message::Type::C_SET_AUTOSKIP: return "C_SET_AUTOSKIP";
    case AH::Common::Message::Type::S_TRADE: return "S_TRADE";
    case AH::Common::Message::Type::C_TRADE: return "C_TRADE";
    case AH::Common::Message::Type::S_CANCEL_TRADE: return "S_CANCEL_TRADE";
    case AH::Common::Message::Type::C_CANCEL_TRADE: return "C_CANCEL_TRADE";
    case AH::Common::Message::Type::DONT_UNDERSTAND: return "DONT_UNDERSTAND";
    case AH::Common::Message::Type::INVALID_MESSAGE: return "INVALID_MESSAGE";
    default: return QString("?? %d").arg(t);
    }
}

}}


DEFINE_ENUM_SERIALIZER(AH::Common::Message::Type)
