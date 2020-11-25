#include "gameoptionscript.h"
#include <QScriptContext>
#include <qscriptengine.h>
#include <game/game.h>
#include "gamescript.h"

GameOptionScript::GameOptionScript(QObject *parent)
    : QObject(parent)
    , GameOption(nullptr, AH::CannotContinue, AH::ChooseOptional)
    , m_basePropertyProp(AH::Common::PropertyValueData::NoProperty)
{
}

GameOptionScript *GameOptionScript::createGameOption(QScriptContext *ctx, QScriptEngine *eng)
{
    if (ctx->argumentCount() != 1 || !ctx->argument(0).isObject()) {
        ctx->throwError(QScriptContext::TypeError, "createOption: Must call with 1 object");
        return nullptr;
    }
    QScriptValue data = ctx->argument(0);

    return createGameOption(data, ctx, eng);
}

GameOptionScript *GameOptionScript::createGameOption(const QScriptValue &data, QScriptContext *ctx, QScriptEngine *eng)
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
        for (auto v : lst) {
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
        return nullptr;
    }

    if (data.property("baseProperty").isValid() && !data.property("baseProperty").isUndefined())
        ret->m_basePropertyProp = static_cast<AH::Common::PropertyValueData::Property> (data.property("baseProperty").toUInt32());

    QString err;
    if (!verify(ret.data(), &err)) {
        ctx->throwError(QScriptContext::TypeError, "createOption: Invalid GameOption data. Errors:\n"+err);
        return nullptr;
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

AH::Common::ModifiedPropertyValueData GameOptionScript::baseProperty() const
{
    if (m_basePropertyProp != AH::Common::PropertyValueData::NoProperty) {
        return gGame->context().getCurCharacterProperty(m_basePropertyProp).toModifiedPropertyValueData();
    }
    return {};
}

bool GameOptionScript::isAvailableWithObject(const QScriptValue &obj) const
{
    if (m_isAvailable.isBool()) {
        return m_isAvailable.toBool();
    }
    if (m_isAvailable.isFunction()) {
        QScriptValue av = m_isAvailable;
        QScriptValue res = gGameScript->call(GameScript::F_Available, av, obj);
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

////////////////////////////////////////

#include "game/gamefield.h"
#include "game/gameregistry.h"
#include "game/game.h"
#include <QDebug>

GameOptionFieldProxyScript::GameOptionFieldProxyScript(GameField *field, const QString &optionId)
    : m_opt(nullptr)
    , m_field(field)
    , m_optId(optionId)
{

}

bool GameOptionFieldProxyScript::execute()
{
    gGame->notifier()->setTempData(m_opt->description());
    m_opt->setSourceId(GameRegistry::TempObjectId);
    bool res = m_opt->execute();
    m_opt->setSourceId("");
    gGame->notifier()->clearTempData();
    return res;
}

bool GameOptionFieldProxyScript::resolveDependencies(GameRegistry *reg)
{
    if (m_opt) return true;
    if (m_optId.isEmpty()) return true; // ??
    GameOption *o = reg->findOptionById(m_optId);
    if (o) {
        m_opt = o;
        return true;
    }
    qWarning() << "Cannot resolve option" << m_optId << "for field option" << id();
    return false;
}

AH::Common::GameOptionData *GameOptionFieldProxyScript::data()
{
    *static_cast<AH::Common::GameOptionData* const> (this) = *m_opt->data();
    m_id = id();
    return this;
}

QString GameOptionFieldProxyScript::id() const
{
    return QString("FIELD_%1::%2").arg(m_field->id()).arg(m_optId);
}

bool GameOptionFieldProxyScript::isAvailable() const
{
    if (m_field->isLocked()) {
        return false;
    }
    return m_opt->isAvailable();
}
