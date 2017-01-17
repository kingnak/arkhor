#include "ancientonescript.h"
#include "gamescript.h"
#include "propertymodificationscript.h"
#include "monstermodifierscript.h"

AncientOneScript::AncientOneScript(QObject *parent) :
    QObject(parent)
{
}

AncientOneScript *AncientOneScript::createAncientOne(QScriptContext *ctx, QScriptEngine *eng)
{
    Q_UNUSED(eng);
    if (ctx->argumentCount() != 1 || !ctx->argument(0).isObject()) {
        ctx->throwError(QScriptContext::TypeError, "createAncientOne: Must call with 1 object");
        return NULL;
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

    // Dynamic attack adjustment
    ret->m_attackAdjustment = data.property("attackAdjustment").toInt32();

    ret->m_attackFunc = data.property("attack");

    // slumber modifications
    QScriptValue propMod = data.property("slumberModifications");
    if (propMod.isValid() && !propMod.isUndefined()) {

        PropertyModificationList lst;
        if (!PropertyModificationScript::parsePropertyModificationList(ret.data(), propMod, lst)) {
            ctx->throwError(QScriptContext::TypeError, "createAncientOne: Invalid Slumber Modification.");
            return NULL;
        }
        ret->m_slumberModifications = lst;
    }

    // Monster Modifications
    MonsterModifierScript::parseMonsterModifications(data, *ret, ret.data());

    QString err;
    if (!verify(ret.data(), &err)) {
        ctx->throwError(QScriptContext::TypeError, "createAncientOne: Invalid AncientOne data. Errors:\n"+err);
        return NULL;
    }

    AncientOneScript *pRet = ret.take();
    pRet->m_this = eng->newQObject(pRet);

    return pRet;
}

void AncientOneScript::awake()
{
    AncientOne::awake();
    if (m_awakeFunc.isFunction()) {
        m_awakeFunc.call(m_this);
    }
}

void AncientOneScript::attack()
{
    m_attackFunc.call(m_this);
}

bool AncientOneScript::postAttack()
{
    if (m_postAttackFunc.isFunction()) {
        QScriptValue res = m_postAttackFunc.call(m_this);
        if (res.isValid() && !res.isUndefined()) {
            if (res.isBool() && !res.toBool()) {
                // False returned
                return false;
            }
        }
    }
    return true;
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

    if (msg) *msg = errs.join("\n");
    if (errs.isEmpty()) {
        return true;
    }
    return false;
}
