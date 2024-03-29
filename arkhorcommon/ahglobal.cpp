
#include "ahglobal.h"
#include "communication/message.h"
#include "playerdata.h"
#include "investigatordata.h"
#include "characterdata.h"
#include "gameoptiondata.h"
#include "attributesliderdata.h"
#include "diedata.h"
#include "objectdata.h"
#include "gameobjectdata.h"
#include "encounterdata.h"
#include "mythosdata.h"
#include "choicedata.h"
#include "gamesettingdata.h"
#include "monsterdata.h"
#include "tradedata.h"

namespace AH {
    Attribute attributeForSkillTest(Skill skill)
    {
        switch (skill) {
        case NoSkill: return NoAttribute;
        case Skill_Speed: return Attr_Speed;
        case Skill_Sneak: return Attr_Sneak;
        case Skill_Fight: return Attr_Fight;
        case Skill_Will: return Attr_Will;
        case Skill_Lore: return Attr_Lore;
        case Skill_Luck: return Attr_Luck;
        case Skill_Evade: return Attr_Sneak;
        case Skill_Combat: return Attr_Fight;
        case Skill_Horror: return Attr_Will;
        case Skill_Spell: return Attr_Lore;
        default: return NoAttribute;
        }
    }

    Skill baseSkillForSpecialSkill(Skill skill) {
        switch (skill) {
        case Skill_Evade: return Skill_Sneak;
        case Skill_Combat: return Skill_Fight;
        case Skill_Horror: return Skill_Will;
        case Skill_Spell: return Skill_Lore;
        default: return NoSkill;
        }
    }

    DEFINE_ENUM_SERIALIZER(AH::Attribute)
    DEFINE_ENUM_SERIALIZER(AH::GamePhase)
    DEFINE_ENUM_SERIALIZER(AH::GamePhases)
    DEFINE_ENUM_SERIALIZER(AH::Skill)
    DEFINE_ENUM_SERIALIZER(AH::GameObjectType)
    DEFINE_ENUM_SERIALIZER(AH::Dimension)
    DEFINE_ENUM_SERIALIZER(AH::Dimensions)
    DEFINE_ENUM_SERIALIZER(AH::OtherWorldPhase)
    DEFINE_ENUM_SERIALIZER(AH::ContinueType)
    DEFINE_ENUM_SERIALIZER(AH::ChooseType)

    void registerCommonMetaTypes()
    {
        qRegisterMetaType<AH::GamePhase>("AH::GamePhase");
        qRegisterMetaType<AH::Common::PlayerData>("AH::Common::PlayerData");
        qRegisterMetaType<AH::Common::InvestigatorData>("AH::Common::InvestigatorData");
        qRegisterMetaType<QList<AH::Common::InvestigatorData> >("QList<AH::Common::InvestigatorData>");
        qRegisterMetaType<AH::GameObjectType>("AH::GameObjectType");
        qRegisterMetaType<AH::Common::Message::Type>("AH::Common::Message::Type");
        qRegisterMetaType<AH::Common::Message>("AH::Common::Message");
        qRegisterMetaType<AH::Common::GameOptionData>("AH::Common::GameOptionData");
        qRegisterMetaType<QList<AH::Common::GameOptionData> >("QList<AH::Common::GameOptionData>");
        qRegisterMetaType<AH::Common::FieldData::FieldID>("AH::Common::FieldData::FieldID");
        qRegisterMetaType<QList<AH::Common::AttributeSliderData> >("QList<AH::Common::AttributeSliderData>");
        qRegisterMetaType<AH::Common::DieRollTestData>("AH::Common::DieRollTestData");
        qRegisterMetaType<AH::Common::DescribeObjectsData>("AH::Common::DescribeObjectsData");
        qRegisterMetaType<QList<AH::Common::ModifiedPropertyValueData> >("QList<AH::Common::ModifiedPropertyValueData>");
        qRegisterMetaType<AH::Common::CharacterData>("AH::Common::CharacterData");
        qRegisterMetaType<QList<AH::Common::GameObjectData> >("QList<AH::Common::GameObjectData>");
        qRegisterMetaType<AH::Common::ModifiedPropertyValueData>("AH::Common::ModifiedPropertyValueData");
        qRegisterMetaType<AH::Common::EncounterData>("AH::Common::EncounterData");
        qRegisterMetaType<AH::Common::MythosData>("AH::Common::MythosData");
        qRegisterMetaType<AH::Common::ChoiceData>("AH::Common::ChoiceData");
        qRegisterMetaType<AH::Common::GameSettingData>("AH::Common::GameSettingData");
        qRegisterMetaType<AH::Common::RequestObjectsData::ObjectType>("AH::Common::RequestObjectsData::ObjectType");
        qRegisterMetaType<AH::Common::MonsterData>("AH::Common::MonsterData");
        qRegisterMetaType<AH::Common::TradeData>("AH::Common::TradeData");
    }

    void operator <<(QVariant &data, const ObjectTypeCount &c) {
        QVariantMap map;
        map["type"] << c.type;
        map["amount"] << c.amount;
        data << map;
    }

    void operator >>(const QVariant &data, ObjectTypeCount &c) {
        QVariantMap map;
        data >> map;
        map["type"] >> c.type;
        map["amount"] >> c.amount;
    }
}
