#ifdef TEST_SCRIPT_BUILD
#include "scripttestconfig.h"

bool ScriptTestConfig::m_doAsk = false;
bool ScriptTestConfig::m_askDrawCommon = true;
bool ScriptTestConfig::m_askDrawUnique = true;
bool ScriptTestConfig::m_askDrawSpell = true;
bool ScriptTestConfig::m_askDrawAlly = true;
bool ScriptTestConfig::m_askDrawSkill = true;
bool ScriptTestConfig::m_askDrawMonster = true;
bool ScriptTestConfig::m_askDrawArkhamEncounter = true;
bool ScriptTestConfig::m_askDrawOtherWorldEncounter = true;
bool ScriptTestConfig::m_askDrawMythos = true;
bool ScriptTestConfig::m_askDrawGate = true;
bool ScriptTestConfig::m_askDrawAncientOne = true;
bool ScriptTestConfig::m_askRollDie = false;

bool ScriptTestConfig::m_debugScript = false;
bool ScriptTestConfig::m_debugAction = true;
bool ScriptTestConfig::m_debugAvailable = true;
bool ScriptTestConfig::m_debugAncientOne = true;
bool ScriptTestConfig::m_debugDisplay = true;
bool ScriptTestConfig::m_debugModification = true;
bool ScriptTestConfig::m_debugInventory = true;
bool ScriptTestConfig::m_debugMonster = true;
bool ScriptTestConfig::m_debugRumor = true;
bool ScriptTestConfig::m_debugPrevent = true;
bool ScriptTestConfig::m_debugMythos = true;
bool ScriptTestConfig::m_debugCharacter = true;

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

bool ScriptTestConfig::debugScript(GameScript::FunctionType type)
{
    if (!m_debugScript) return false;
    switch (type) {
    case GameScript::F_Action: return m_debugAction;
    case GameScript::F_Available: return m_debugAvailable;
    case GameScript::F_AncientOne: return m_debugAncientOne;
    case GameScript::F_Display: return m_debugDisplay;
    case GameScript::F_Modification: return m_debugModification;
    case GameScript::F_Inventory: return m_debugInventory;
    case GameScript::F_Monster: return m_debugMonster;
    case GameScript::F_Rumor: return m_debugRumor;
    case GameScript::F_Prevent: return m_debugPrevent;
    case GameScript::F_Mythos: return m_debugMythos;
    case GameScript::F_Character: return m_debugCharacter;
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
