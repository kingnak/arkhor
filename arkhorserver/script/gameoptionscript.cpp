#include "gameoptionscript.h"
#include <QScriptContext>
#include <qscriptengine.h>
#include "gamescript.h"

GameOptionScript::GameOptionScript(QObject *parent) :
    QObject(parent), GameOption(NULL, AH::CannotContinue, AH::ChooseOptional)
{
}

GameOptionScript *GameOptionScript::createGameOption(QScriptContext *ctx, QScriptEngine *eng)
{
    if (ctx->argumentCount() != 1 || !ctx->argument(0).isObject()) {
        ctx->throwError(QScriptContext::TypeError, "createOption: Must call with 1 object");
        return NULL;
    }
    QScriptValue data = ctx->argument(0);

    return createGameOption(data, ctx, eng);
}

GameOptionScript *GameOptionScript::createGameOption(QScriptValue data, QScriptContext *ctx, QScriptEngine *eng)
{
    QScopedPointer<GameOptionScript> ret(new GameOptionScript);


    ret->m_id = data.property("id").toString();
    //ret->m_name = data.property("name").toString();
    //ret->m_description = data.property("description").toString();
    //ret->m_sourceId = data.property("sourceId").toString();

    /*
    ret->m_phases = AH::NoGamePhase;
    if (data.property("phases").isArray()) {
        QScriptValueList lst = GameScript::array2list(data.property("phases"));
        AH::GamePhases ph = AH::NoGamePhase;
        foreach (QScriptValue v, lst) {
            ph |= static_cast<AH::GamePhase> (v.toUInt32());
        }
        ret->m_phases = ph;
    } else {
        ret->m_phases = static_cast<AH::GamePhases> (data.property("phases").toUInt32());
    }
    */

    ret->m_actionId = data.property("actionId").toString();
    ret->m_isAvailable = data.property("isAvailable");
    if (data.property("continueType").isValid() && !data.property("continueType").isUndefined())
        ret->m_continueType = static_cast<AH::ContinueType> (data.property("continueType").toUInt32());
    if (data.property("chooseType").isValid() && !data.property("chooseType").isUndefined())
        ret->m_chooseType = static_cast<AH::ChooseType> (data.property("chooseType").toUInt32());

    if (!GameScript::parseCosts(data.property("costs"), ret->m_costs)) {
        ctx->throwError(QScriptContext::TypeError, "createOption: Invalid Costs specification");
        return NULL;
    }

    QString err;
    if (!verify(ret.data(), &err)) {
        ctx->throwError(QScriptContext::TypeError, "createOption: Invalid GameOption data. Errors:\n"+err);
        return NULL;
    }

    GameOptionScript *pRet = ret.take();
    QScriptValue object = eng->newQObject(pRet);
    pRet->m_this = object;

    return pRet;
}

bool GameOptionScript::isAvailable() const
{
    return isAvailableWithObject(m_this);
}

bool GameOptionScript::isAvailableWithObject(QScriptValue obj) const
{
    if (m_isAvailable.isBool()) {
        return m_isAvailable.toBool();
    }
    if (m_isAvailable.isFunction()) {
        QScriptValue av = m_isAvailable;
        QScriptValue res = av.call(obj);
        if (res.isBool()) {
            return res.toBool();
        } else {
            qWarning("Scripted GameOption: isAvailable did not return a bool");
        }
    }
    if (!m_isAvailable.isValid() || m_isAvailable.isNull() || m_isAvailable.isUndefined()) {
        return GameOption::isAvailable();
    }
    qWarning("Scripted GameOption: isAvailable is neither function nor bool");
    return false;
}

bool GameOptionScript::verify(GameOptionScript *op, QString *msg)
{
    QStringList errs;
    //if (op->id().isEmpty()) errs.append("id must be set");
    //if (op->name().isEmpty()) errs.append("name must be set");
    //if (op->phases() == AH::NoGamePhase) errs.append("phases must be set");
    if (op->actionId().isEmpty()) errs.append("actionId must be set");

    if (msg) *msg = errs.join("\n");
    if (errs.isEmpty()) {
        return true;
    }
    return false;
}

