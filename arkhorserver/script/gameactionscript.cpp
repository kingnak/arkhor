#include "gameactionscript.h"
#include <QScriptContext>
#include <qscriptengine.h>
#include "gamescript.h"
#include "game/game.h"
#include "game/player.h"
#include "character.h"
#include "script/gameobjectscript.h"
#include <QDebug>

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
    QScriptValue data = ctx->argument(0);

    return createGameAction(data, ctx, eng);
}

GameActionScript *GameActionScript::createGameAction(QScriptValue data, QScriptContext *ctx, QScriptEngine *eng)
{
    Q_UNUSED(eng)
    QScopedPointer<GameActionScript> ret(new GameActionScript);

    ret->m_id = data.property("id").toString();
    ret->m_name = data.property("name").toString();
    ret->m_description = data.property("description").toString();
    //ret->m_sourceId = data.property("sourceId").toString();

    ret->m_phases = GameScript::parseFlags<AH::GamePhases>(data.property("phases"), AH::NoGamePhase);
    ret->m_function = data.property("activate");

    QString err;
    if (!verify(ret.data(), &err)) {
        ctx->throwError(QScriptContext::TypeError, "createAction: Invalid GameAction data. Errors:\n"+err);
        return NULL;
    }

    GameActionScript *pRet = ret.take();
    //QScriptValue object = eng->newQObject(pRet);
    //pRet->m_object = object;

    return pRet;
}

bool GameActionScript::execute()
{
    //QScriptValue res = m_function.call(m_object);
    // Might call a die roll, using temporary object data...
    // Use an object with id "%TMP%"
    QScriptValue obj = m_function.engine()->newObject();
    obj.setProperty("id", "%TMP%");
    return executeOnObject(obj);
}

bool GameActionScript::executeOnObject(QScriptValue obj)
{
    GameObjectScript *gobj = qscriptvalue_cast<GameObjectScript *> (obj);
    if (gobj)
        gGame->notifier()->actionStart(this, gobj->name());
    else
        gGame->notifier()->actionStart(this);

    QScriptValue res = m_function.call(obj);
    if (res.isError()) {
        QString s = res.toString();
        qWarning() << "Script Action Error: " << s;
        gGame->notifier()->actionFinish(this, "Error");
        return false;
    }
    gGame->context().player()->getCharacter()->commitDamage();

    gGame->notifier()->actionFinish(this);
    if (res.isBool())
        return res.toBool();
    return true;
}

QString GameActionScript::notificationString(GameAction::NotificationPart part, const QString &desc) const
{
    if (part == Start) {
        if (desc.isEmpty()) {
            return "{C} activates ?";
        } else {
            return "{C} uses {D}";
        }
    }
    if (part == Finish) {
        if (desc == "Error") {
            return "ERROR";
        }
    }
    return QString::null;
}

bool GameActionScript::verify(GameActionScript *act, QString *err)
{
    QStringList errs;
    //if (act->m_id.isEmpty()) errs.append("id must be set");
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
