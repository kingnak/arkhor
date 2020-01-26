#ifdef TEST_SCRIPT_BUILD

#ifndef SCRIPTTESTCONFIG_H
#define SCRIPTTESTCONFIG_H

#include <QtGlobal>
#include <ahglobal.h>
#include "../script/gamescript.h"

class ScriptTestConfig
{
    ScriptTestConfig();
    Q_DISABLE_COPY(ScriptTestConfig)
    friend class ScriptTestConfigWidget;
private:
    static bool m_doAsk;
    static bool m_askDrawCommon;
    static bool m_askDrawUnique;
    static bool m_askDrawSpell;
    static bool m_askDrawAlly;
    static bool m_askDrawSkill;
    static bool m_askDrawMonster;
    static bool m_askDrawArkhamEncounter;
    static bool m_askDrawOtherWorldEncounter;
    static bool m_askDrawMythos;
    static bool m_askDrawGate;
    static bool m_askDrawAncientOne;
    static bool m_askRollDie;

    static bool m_debugScript;
    static bool m_debugAction;
    static bool m_debugAvailable;
    static bool m_debugAncientOne;
    static bool m_debugDisplay;
    static bool m_debugModification;
    static bool m_debugInventory;
    static bool m_debugMonster;
    static bool m_debugRumor;
    static bool m_debugPrevent;
    static bool m_debugMythos;

public:
    static bool doAsk() { return m_doAsk; }
    static bool askDraw(AH::GameObjectType t);
    static bool askDrawCommon() { return m_doAsk && m_askDrawCommon; }
    static bool askDrawUnique() { return m_doAsk && m_askDrawUnique; }
    static bool askDrawSpell() { return m_doAsk && m_askDrawSpell; }
    static bool askDrawAlly() { return m_doAsk && m_askDrawAlly; }
    static bool askDrawSkill() { return m_doAsk && m_askDrawSkill; }
    static bool askDrawMonster() { return m_doAsk && m_askDrawMonster; }
    static bool askDrawArkhamEncounter() { return m_doAsk && m_askDrawArkhamEncounter; }
    static bool askDrawOtherWorldEncounter() { return m_doAsk && m_askDrawOtherWorldEncounter; }
    static bool askDrawMythos() { return m_doAsk && m_askDrawMythos; }
    static bool askDrawGate() { return m_doAsk && m_askDrawGate; }
    static bool askDrawAncientOne() { return m_doAsk && m_askDrawAncientOne; }
    static bool askRollDie() { return m_askRollDie; }
    static bool debugScript(GameScript::FunctionType type);

    static QString nameForObjectType(AH::GameObjectType t);
};

#endif // SCRIPTTESTCONFIG_H

#endif
