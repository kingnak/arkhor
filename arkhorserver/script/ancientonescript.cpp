#include "ancientonescript.h"
#include "gamescript.h"
#include "propertymodificationscript.h"
#include "monstermodifierscript.h"
#include "gamescript.h"
#include "characterscript.h"
#include "monsterscript.h"

AncientOneScript::AncientOneScript(QObject *parent) :
    QObject(parent)
{
}

AncientOneScript *AncientOneScript::createAncientOne(QScriptContext *ctx, QScriptEngine *eng)
{
    Q_UNUSED(eng);
    if (ctx->argumentCount() != 1 || !ctx->argument(0).isObject()) {
        ctx->throwError(QScriptContext::TypeError, "createAncientOne: Must call with 1 object");
        return nullptr;
    }

    QScopedPointer<AncientOneScript> ret(new AncientOneScript);

    QScriptValue data = ctx->argument(0);
    ret->m_id = data.property("id").toString();
    ret->m_name = data.property("name").toString();
    ret->m_combatAdjustment = data.property("combatAdjustment").toInt32();
    ret->m_worshippersText = data.property("worshippersText").toString();
    ret->m_powerTitle = data.property("powerTitle").toString();
    ret->m_powerText = data.property("powerText").toString();
    ret->m_battleStartText = data.property("battleStartText").toString();
    ret->m_attackText = data.property("attackText").toString();
    ret->m_defenses = static_cast<AH::Common::MonsterData::MonsterAttributes> (data.property("defenses").toUInt32());
    ret->m_doomTrack = data.property("doomTrack").toInt32();
    ret->m_awakeFunc = data.property("onAwake");
    ret->m_postAttackFunc = data.property("postAttack");
    ret->m_onUnconsciousFunc = data.property("onUnconscious");
    ret->m_onInsaneFunc = data.property("onInsane");
    ret->m_onLostFunc = data.property("onLostInSpaceAndTime");
    ret->m_onDefeatMonsterFunc = data.property("onDefeatMonster");

    // Dynamic attack adjustment
    ret->m_attackAdjustment = data.property("attackAdjustment").toInt32();

    ret->m_attackFunc = data.property("attack");

    ret->m_onEndMythosFunc = data.property("onEndMythos");

    // slumber modifications
    QScriptValue propMod = data.property("slumberModifications");
    if (propMod.isValid() && !propMod.isUndefined()) {

        PropertyModificationList lst;
        if (!PropertyModificationScript::parsePropertyModificationList(ret.data(), propMod, lst)) {
            ctx->throwError(QScriptContext::TypeError, "createAncientOne: Invalid Slumber Modification.");
            return nullptr;
        }
        ret->m_slumberModifications = lst;
    }

    // Monster Modifications
    MonsterModifierScript::parseMonsterModifications(data, *ret, ret.data());

    ret->m_ignEnvTypes = parseIgnoreEnvironmentTypes(data.property("ignoreEnvironmentTypes"));

    QString err;
    if (!verify(ret.data(), &err)) {
        ctx->throwError(QScriptContext::TypeError, "createAncientOne: Invalid AncientOne data. Errors:\n"+err);
        return nullptr;
    }

    AncientOneScript *pRet = ret.take();
    pRet->m_this = eng->newQObject(pRet);

    return pRet;
}

void AncientOneScript::awake()
{
    AncientOne::awake();
    if (m_awakeFunc.isFunction()) {
        gGameScript->call(GameScript::F_AncientOne, m_awakeFunc, m_this);
    }
}

bool AncientOneScript::onUnconscious(Character *c)
{
    if (m_onUnconsciousFunc.isFunction()) {
        auto res = gGameScript->call(GameScript::F_AncientOne, m_onUnconsciousFunc, m_this, gGameScript->engine()->toScriptValue(dynamic_cast<CharacterScript*>(c)));
        if (res.isBool()) {
            return res.toBool();
        }
    }

    return true;
}

bool AncientOneScript::onInsane(Character *c)
{
    if (m_onInsaneFunc.isFunction()) {
        auto res = gGameScript->call(GameScript::F_AncientOne, m_onInsaneFunc, m_this, gGameScript->engine()->toScriptValue(dynamic_cast<CharacterScript*>(c)));
        if (res.isBool()) {
            return res.toBool();
        }
    }

    return true;
}

bool AncientOneScript::onLostInSpaceAndTime(Character *c)
{
    if (m_onLostFunc.isFunction()) {
        auto res = gGameScript->call(GameScript::F_AncientOne, m_onLostFunc, m_this, gGameScript->engine()->toScriptValue(dynamic_cast<CharacterScript*>(c)));
        if (res.isBool()) {
            return res.toBool();
        }
    }

    return true;
}

bool AncientOneScript::onDefeatMonster(Character *c, Monster *m)
{
    if (m_onDefeatMonsterFunc.isFunction()) {
        QScriptValueList args;
        args << gGameScript->engine()->toScriptValue(dynamic_cast<CharacterScript*>(c));
        args << gGameScript->engine()->toScriptValue(dynamic_cast<MonsterScript*>(m));
        auto res = gGameScript->call(GameScript::F_AncientOne, m_onDefeatMonsterFunc, m_this, args);
        if (res.isBool()) {
            return res.toBool();
        }
    }

    return true;
}

void AncientOneScript::onEndMythos()
{
    if (m_onEndMythosFunc.isFunction()) {
        gGameScript->call(GameScript::F_AncientOne, m_onEndMythosFunc, m_this);
    }
}

void AncientOneScript::attack()
{
    gGameScript->call(GameScript::F_AncientOne, m_attackFunc, m_this);
}

bool AncientOneScript::postAttack()
{
    if (m_postAttackFunc.isFunction()) {
        QScriptValue res = gGameScript->call(GameScript::F_AncientOne, m_postAttackFunc, m_this);
        if (res.isValid() && !res.isUndefined()) {
            if (res.isBool() && !res.toBool()) {
                // False returned
                return false;
            }
        }
    }
    return true;
}

QList<AH::Common::MythosData::EnvironmentType> AncientOneScript::parseIgnoreEnvironmentTypes(const QScriptValue &v)
{
    if (!v.isValid()) {
        return {};
    }

    if (v.isNumber()) {
        return QList<AH::Common::MythosData::EnvironmentType>()
                << static_cast<AH::Common::MythosData::EnvironmentType> (v.toInt32());
    }

    if (v.isArray()) {
        return GameScript::array2TypedList<AH::Common::MythosData::EnvironmentType> (v);
    }
    return {};
}

bool AncientOneScript::verify(AncientOneScript *m, QString *msg)
{
    QStringList errs;

    if (m->m_id.isEmpty()) errs.append("id must be set");
    if (m->m_name.isEmpty()) errs.append("name must be set");
    if (m->m_doomTrack <= 0) errs.append("doomTrack must be greater than 0");
    if (m->m_awakeFunc.isValid() && !m->m_awakeFunc.isFunction()) errs.append("onAwake must be a function");
    if (!m->m_attackFunc.isValid() || !m->m_attackFunc.isFunction()) errs.append("attack must be a function");
    if (m->m_postAttackFunc.isValid() && !m->m_postAttackFunc.isFunction()) errs.append("postAttack must be a function");
    if (m->m_onEndMythosFunc.isValid() && !m->m_onEndMythosFunc.isFunction()) errs.append("onEndMythos must be a function");
    if (m->m_onUnconsciousFunc.isValid() && !m->m_onUnconsciousFunc.isFunction()) errs.append("onUnconscious must be a function");
    if (m->m_onInsaneFunc.isValid() && !m->m_onInsaneFunc.isFunction()) errs.append("onInsane must be a function");
    if (m->m_onLostFunc.isValid() && !m->m_onLostFunc.isFunction()) errs.append("onLostInSpaceAndTime must be a function");
    if (m->m_onDefeatMonsterFunc.isValid() && !m->m_onDefeatMonsterFunc.isFunction()) errs.append("onDefeatMonster must be a function");

    if (msg) *msg = errs.join("\n");
    if (errs.isEmpty()) {
        return true;
    }
    return false;
}
