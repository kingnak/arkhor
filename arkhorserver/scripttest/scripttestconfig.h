#ifdef TEST_SCRIPT_BUILD

#ifndef SCRIPTTESTCONFIG_H
#define SCRIPTTESTCONFIG_H

#include <QtGlobal>
#include <ahglobal.h>

class ScriptTestConfig
{
    ScriptTestConfig();
    Q_DISABLE_COPY(ScriptTestConfig)
public:
    static bool doAsk;
    static bool askDrawCommon;
    static bool askDrawUnique;
    static bool askDrawSpell;
    static bool askDrawAlly;
    static bool askDrawSkill;
    static bool askDrawMonster;
    static bool askDrawArkhamEncounter;
    static bool askDrawOtherWorldEncounter;
    static bool askDrawMythos;
    static bool askDrawGate;
    static bool askDrawAncientOne;
    static bool askRollDie;
    static bool debugScript;

    static bool askDraw(AH::GameObjectType t);

    static QString nameForObjectType(AH::GameObjectType t);
};

#endif // SCRIPTTESTCONFIG_H

#endif
