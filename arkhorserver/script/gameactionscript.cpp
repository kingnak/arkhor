#include "gameactionscript.h"
#include <QScriptContext>
#include <qscriptengine.h>
#include "gamescript.h"

GameActionScript::GameActionScript(QObject *parent) :
    QObject(parent)
{
}

GameActionScript *GameActionScript::createGameAction(QScriptContext *ctx, QScriptEngine *eng)
{
    if (ctx->argumentCount() != 1 || !ctx->argument(0).isObject()) {
        ctx->throwError(QScriptContext::TypeError, "createAction: Must call with 1 object");
        return NULL;
    }

    QScopedPointer<GameActionScript> ret(new GameActionScript);

    QScriptValue data = ctx->argument(0);

    ret->m_id = data.property("id").toString();
    ret->m_name = data.property("name").toString();
    ret->m_description = data.property("description").toString();
    //ret->m_sourceId = data.property("sourceId").toString();

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

    ret->m_function = data.property("activate");

    QString err;
    if (!verify(ret.data(), &err)) {
        ctx->throwError(QScriptContext::TypeError, "createAction: Invalid GameAction data. Errors:\n"+err);
        return NULL;
    }

    GameActionScript *pRet = ret.take();
    QScriptValue object = eng->newQObject(pRet);
    pRet->m_object = object;

    return pRet;
}

bool GameActionScript::execute()
{
    QScriptValue res = m_function.call(m_object);
    if (res.isError()) {
        return false;
    }
    return res.toBool();
}

bool GameActionScript::verify(GameActionScript *act, QString *err)
{
    QStringList errs;
    if (act->m_id.isEmpty()) errs.append("id must be set");
    if (act->m_name.isEmpty()) errs.append("name must be set");
    if (act->m_phases == AH::NoGamePhase) errs.append("phases must be set");
    if (act->m_function.isNull()) errs.append("activate function must be set");
    if (!act->m_function.isFunction()) errs.append("activate must be a function");

    if (err) *err = errs.join("\n");
    if (errs.isEmpty()) {
        return true;
    }
    return false;
}
