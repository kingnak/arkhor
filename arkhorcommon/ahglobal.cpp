
#include "ahglobal.h"
#include "communication/message.h"
#include "playerdata.h"
#include "investigatordata.h"
#include "characterdata.h"

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


    DEFINE_ENUM_SERIALIZER(AH::Attribute)
    DEFINE_ENUM_SERIALIZER(AH::GamePhase)
    DEFINE_ENUM_SERIALIZER(AH::GamePhases)
    DEFINE_ENUM_SERIALIZER(AH::Skill)
    DEFINE_ENUM_SERIALIZER(AH::GameObjectType)
    DEFINE_ENUM_SERIALIZER(AH::Dimension)
    DEFINE_ENUM_SERIALIZER(AH::OtherWorldPhase)
    DEFINE_ENUM_SERIALIZER(AH::ContinueType)
    DEFINE_ENUM_SERIALIZER(AH::ChooseType)

    void registerCommonMetaTypes()
    {
        qRegisterMetaType<AH::Common::PlayerData>("AH::Common::PlayerData");
        qRegisterMetaType<AH::Common::InvestigatorData>("AH::Common::InvestigatorData");
        qRegisterMetaType<QList<AH::Common::InvestigatorData> >("QList<AH::Common::InvestigatorData>");
        qRegisterMetaType<AH::GameObjectType>("AH::GameObjectType");
        qRegisterMetaType<AH::Common::Message::Type>("AH::Common::Message::Type");
        qRegisterMetaType<AH::Common::Message>("AH::Common::Message");
    }

}
