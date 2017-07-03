#ifdef TEST_SCRIPT_BUILD
#include "scripttestconfig.h"

bool ScriptTestConfig::doAsk = true;
bool ScriptTestConfig::askDrawCommon = true;
bool ScriptTestConfig::askDrawUnique = true;
bool ScriptTestConfig::askDrawSpell = true;
bool ScriptTestConfig::askDrawAlly = true;
bool ScriptTestConfig::askDrawSkill = true;
bool ScriptTestConfig::askDrawMonster = true;
bool ScriptTestConfig::askDrawArkhamEncounter = false;
bool ScriptTestConfig::askDrawOtherWorldEncounter = false;
bool ScriptTestConfig::askDrawMythos = true;
bool ScriptTestConfig::askDrawGate = false;
bool ScriptTestConfig::askDrawAncientOne = true;
bool ScriptTestConfig::askRollDie = false;
bool ScriptTestConfig::debugScript = false;


bool ScriptTestConfig::askDraw(AH::GameObjectType t)
{
    if (!doAsk) return false;
    switch (t) {
    case AH::Obj_CommonItem: return askDrawCommon;
    case AH::Obj_UniqueItem: return askDrawUnique;
    case AH::Obj_Spell: return askDrawSpell;
    case AH::Obj_Skill: return askDrawSkill;
    case AH::Obj_Ally: return askDrawAlly;
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
