#include "gameoptionscript.h"
#include <QScriptContext>
#include <qscriptengine.h>
#include "gamescript.h"

GameOptionScript::GameOptionScript(QObject *parent) :
    QObject(parent)
{
}

GameOptionScript *GameOptionScript::createGameOption(QScriptContext *ctx, QScriptEngine *eng)
{
    if (ctx->argumentCount() != 1 || !ctx->argument(0).isObject()) {
        ctx->throwError(QScriptContext::TypeError, "createOption: Must call with 1 object");
        return NULL;
    }

    QScopedPointer<GameOptionScript> ret(new GameOptionScript);

    QScriptValue data = ctx->argument(0);

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

    if (!parseCosts(data.property("costs"), ret->m_costs)) {
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

bool GameOptionScript::isAvailable()
{
    if (m_isAvailable.isBool()) {
        return m_isAvailable.toBool();
    }
    if (m_isAvailable.isFunction()) {
        QScriptValue res = m_isAvailable.call(m_this);
        if (res.isBool()) {
            return res.toBool();
        } else {
            qWarning("Scripted GameOption: isAvailable did not return a bool");
        }
    }
    if (!m_isAvailable.isValid() || m_isAvailable.isNull()) {
        return true;
    }
    qWarning("Scripted GameOption: isAvailable is neither function nor bool");
    return false;
}

bool GameOptionScript::verify(GameOptionScript *op, QString *msg)
{
    QStringList errs;
    if (op->id().isEmpty()) errs.append("id must be set");
    //if (op->name().isEmpty()) errs.append("name must be set");
    //if (op->phases() == AH::NoGamePhase) errs.append("phases must be set");
    if (op->actionId().isEmpty()) errs.append("actionId must be set");

    if (msg) *msg = errs.join("\n");
    if (errs.isEmpty()) {
        return true;
    }
    return false;
}

bool GameOptionScript::parseCosts(QScriptValue v, AH::Common::Cost &c)
{
    c.clear();
    if (!v.isValid() || v.isNull()) {
        return true;
    }

    QScriptValueList lst;
    if (v.isArray()) {
        lst = GameScript::array2list(v);
    } else if (v.isObject()) {
        lst = QScriptValueList() << v;
    } else {
        return false;
    }

    if (lst.size() == 0) {
        return true;
        /*
    } else if (lst.size() == 1) {
        AH::Common::CostList cl;
        if (!parseCostList(lst[0], cl)) {
            return false;
        }
        c.addAlternative(cl);
        return true;
        */
    } else {
        foreach (QScriptValue item, lst) {
            AH::Common::CostList cl;
            if (!parseCostList(item, cl)) {
                return false;
            }
            c.addAlternative(cl);
        }
        return true;
    }
}

bool GameOptionScript::parseCostList(QScriptValue v, AH::Common::CostList &cl)
{
    QScriptValueList lst;
    if (v.isArray()) {
        lst = GameScript::array2list(v);
    } else if (v.isObject()) {
        lst = QScriptValueList() << v;
    } else {
        return false;
    }

    if (lst.size() == 0) {
        return false;
        /*
    } else if (lst.size() == 1) {
        AH::Common::CostItem ci;
        if (!parseCostItem(lst[0], ci)) {
            return false;
        }
        cl.append(ci);
        return true;
        */
    } else {
        foreach (QScriptValue item, lst) {
            AH::Common::CostItem ci;
            if (!parseCostItem(item, ci)) {
                return false;
            }
            cl.append(ci);
        }
        return true;
    }
}

bool GameOptionScript::parseCostItem(QScriptValue v, AH::Common::CostItem &ci)
{
    if (!v.isObject()) {
        return false;
    }

    ci.type = static_cast<AH::Common::CostItem::PaymentItem> (v.property("type").toUInt32());
    ci.amount = v.property("amount").toUInt32();

    if (ci.type == AH::Common::CostItem::Pay_None || ci.amount == 0) {
        return false;
    }

    return true;
}
