#ifdef TEST_SCRIPT_BUILD
#include "scripttestconfig.h"

bool ScriptTestConfig::m_doAsk = true;
bool ScriptTestConfig::m_askDrawCommon = true;
bool ScriptTestConfig::m_askDrawUnique = true;
bool ScriptTestConfig::m_askDrawSpell = true;
bool ScriptTestConfig::m_askDrawAlly = true;
bool ScriptTestConfig::m_askDrawSkill = true;
bool ScriptTestConfig::m_askDrawMonster = true;
bool ScriptTestConfig::m_askDrawArkhamEncounter = true;
bool ScriptTestConfig::m_askDrawOtherWorldEncounter = true;
bool ScriptTestConfig::m_askDrawMythos = true;
bool ScriptTestConfig::m_askDrawGate = false;
bool ScriptTestConfig::m_askDrawAncientOne = true;
bool ScriptTestConfig::m_askRollDie = false;
bool ScriptTestConfig::m_debugScript = false;

bool ScriptTestConfig::askDraw(AH::GameObjectType t)
{
    switch (t) {
    case AH::Obj_CommonItem: return askDrawCommon();
    case AH::Obj_UniqueItem: return askDrawUnique();
    case AH::Obj_Spell: return askDrawSpell();
    case AH::Obj_Skill: return askDrawSkill();
    case AH::Obj_Ally: return askDrawAlly();
    default: return false;
    }
}

QString ScriptTestConfig::nameForObjectType(AH::GameObjectType t)
{
    switch (t) {
    case AH::NoObject: return "NoObject";
    case AH::Obj_CommonItem: return "CommonItem";
    case AH::Obj_UniqueItem: return "UniqueItem";
    case AH::Obj_Spell: return "Spell";
    case AH::Obj_Skill: return "Skill";
    case AH::Obj_Ally: return "Ally";
    case AH::Obj_Blessing_Curse: return "Blessing_Curse";
    case AH::Obj_Special: return "Special";
    default: return "?";
    }
}

#endif
