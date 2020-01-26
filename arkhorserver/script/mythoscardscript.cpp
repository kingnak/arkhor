#include "mythoscardscript.h"
#include "gamescript.h"
#include "game/game.h"
#include "propertymodificationscript.h"
#include "monstermodifierscript.h"
#include <QDebug>

MythosCardScript::MythosCardScript(QObject *parent) : DynamicScriptableObject(parent)
  , m_envFieldOption(NULL)
  , m_envFieldId(AH::Common::FieldData::NO_NO_FIELD)
  , m_rumorFieldOption(NULL)
  , m_rumorFieldId(AH::Common::FieldData::NO_NO_FIELD)
{
}

QList<AH::Common::DynamicPropertyData> MythosCardScript::dynamicProperties() const
{
    return this->getDynamicPropertyDataList();
}

MythosCardScript *MythosCardScript::createMythosCard(QScriptContext *ctx, QScriptEngine *eng)
{
    if (ctx->argumentCount() != 1 || !ctx->argument(0).isObject()) {
        ctx->throwError(QScriptContext::TypeError, "createMythosCard: Must call with 1 object");
        return NULL;
    }

    QScriptValue data = ctx->argument(0);

    QScopedPointer<MythosCardScript> ret(new MythosCardScript);

    ret->m_id = data.property("id").toString();
    ret->m_name = data.property("name").toString();
    ret->m_desc = data.property("description").toString();
    ret->m_shortDesc = data.property("shortDescription").toString();
    ret->m_type = static_cast<MythosType> (data.property("type").toUInt32());
    ret->m_envType = static_cast<EnvironmentType> (data.property("environmentType").toUInt32());
    ret->m_clueField = static_cast<AH::Common::FieldData::FieldID> (data.property("clueField").toUInt32());
    ret->m_gateField = static_cast<AH::Common::FieldData::FieldID> (data.property("gateField").toUInt32());
    ret->m_moveBlack = GameScript::parseFlags<AH::Dimensions>(data.property("moveBlack"), AH::NoDimension);
    ret->m_moveWhite = GameScript::parseFlags<AH::Dimensions>(data.property("moveWhite"), AH::NoDimension);

    // Type specific:
    // Headline
    if (ret->m_type == Headline) {
        ret->m_headlineFunc = data.property("executeHeadline");
    }

    // Environment
    if (ret->m_type == Environment) {
        ret->m_envFieldOptionId = data.property("environmentFieldOptionId").toString();
        ret->m_envFieldId = static_cast<AH::Common::FieldData::FieldID> (data.property("environmentField").toInt32());
        ret->m_envType = static_cast<AH::Common::MythosData::EnvironmentType> (data.property("environmentType").toInt32());

        QScriptValue endMove = data.property("onEndMovement");
        if (endMove.isFunction()) {
            ret->m_onEndMoveFunc = endMove;
        }

        QScriptValue envMods = data.property("environmentModifications");
        if (envMods.isValid() && !envMods.isUndefined()) {
            PropertyModificationList lst;
            if (!PropertyModificationScript::parsePropertyModificationList(ret.data(), envMods, lst)) {
                ctx->throwError(QScriptContext::TypeError, "createMythosCard: Invalid environment modifications");
                return NULL;
            }
            ret->m_envMods = lst;
        }

        // Monster Modifications
        MonsterModifierScript::parseMonsterModifications(data, *ret, ret.data());
    }

    // Rumor
    if (ret->m_type == Rumor) {
        ret->m_rumorFieldOptionId = data.property("rumorFieldOptionId").toString();
        ret->m_rumorFieldId = static_cast<AH::Common::FieldData::FieldID> (data.property("rumorField").toInt32());
        ret->m_setupRumorFunc = data.property("setupRumor");
        ret->m_teardownRumorFunc = data.property("teardownRumor");
        ret->m_onMythosFunc = data.property("onMythos");
        ret->m_passFunc = data.property("onPass");
        ret->m_failFunc = data.property("onFail");
    }

    DynamicPropertyScript::createDynamicProperties(data.property("properties"), ret.data());

    QString err;
    if (!verify(ret.data(), &err)) {
        ctx->throwError(QScriptContext::TypeError, "createMythosCard: Invalid MythosCard data. Errors:\n"+err);
        return NULL;
    }

    MythosCardScript *pRet = ret.take();
    QScriptValue object = eng->newQObject(pRet);
    pRet->m_object = object;

    return pRet;
}

bool MythosCardScript::resolveDependencies()
{
    if (type() == Rumor) {
        GameOption *opt = gGame->registry()->findOptionById(m_rumorFieldOptionId);
        if (!opt) {
            qCritical() << "Cannot resolve rumor field option:" << m_rumorFieldOptionId;
            return false;
        }
        m_rumorFieldOption = opt;
    }
    if (type() == Environment && !m_envFieldOptionId.isEmpty()) {
        GameOption *opt = gGame->registry()->findOptionById(m_envFieldOptionId);
        if (!opt) {
            qCritical() << "Cannot resolve environment field option:" << m_envFieldOptionId;
            return false;
        }
        m_envFieldOption = opt;
    }

    DynamicScriptableObject::resolveDependencies(m_object);

    return true;
}

void MythosCardScript::resolveDynamicAttributes()
{
    if (m_moveBlack == AH::NoDimension) {
        m_moveBlack = gGame->randomDimension();
    }
    if (m_moveWhite == AH::NoDimension) {
        m_moveWhite = gGame->randomDimension();
    }
    if (m_gateField == AH::Common::FieldData::NO_NO_FIELD) {
        m_gateField = gGame->randomLocation(true);
    }
    if (m_clueField == AH::Common::FieldData::NO_NO_FIELD) {
        m_clueField = gGame->randomLocation(false);
    }
}

void MythosCardScript::executeHeadline()
{
    Q_ASSERT(type() == Headline);
    // Headlines' actions don't work on an "object", but on the current situation, hence the temp object
    gGame->notifier()->setTempData(this->description());
    QScriptValue res = gGameScript->call(GameScript::F_Action, m_headlineFunc, gGameScript->getTempObject());
    gGame->notifier()->clearTempData();
    if (res.isError()) {
        qCritical() << "Mythos executeHeadline Error:" << res.toString();
    }
}

PropertyModificationList MythosCardScript::getModifications() const
{
    return m_envMods;
}

GameOption *MythosCardScript::environmentFieldOption()
{
    return m_envFieldOption;
}

AH::Common::FieldData::FieldID MythosCardScript::environmentFieldId()
{
    return m_envFieldId;
}

void MythosCardScript::onEndMovement()
{
    Q_ASSERT(type() == Environment);
    QScriptValue res = gGameScript->call(GameScript::F_Mythos, m_onEndMoveFunc, m_object);
    if (res.isError()) {
        qCritical() << "Mythos onEndMovement Error:" << res.toString();
    }
}

void MythosCardScript::onMythos()
{
    Q_ASSERT(type() == Rumor);
    QScriptValue res = gGameScript->call(GameScript::F_Rumor, m_onMythosFunc, m_object);
    if (res.isError()) {
        qCritical() << "Mythos onMythos Error:" << res.toString();
    }
}

void MythosCardScript::onPass()
{
    Q_ASSERT(type() == Rumor);
    QScriptValue res = gGameScript->call(GameScript::F_Rumor, m_passFunc, m_object);
    if (res.isError()) {
        qCritical() << "Mythos onPass Error:" << res.toString();
    }
}

void MythosCardScript::onFail()
{
    Q_ASSERT(type() == Rumor);
    QScriptValue res = gGameScript->call(GameScript::F_Rumor, m_failFunc, m_object);
    if (res.isError()) {
        qCritical() << "Mythos onFail Error:" << res.toString();
    }
}

void MythosCardScript::setupRumor()
{
    Q_ASSERT(type() == Rumor);
    QScriptValue res = gGameScript->call(GameScript::F_Rumor, m_setupRumorFunc, m_object);
    if (res.isError()) {
        qCritical() << "Mythos setupRumor Error:" << res.toString();
    }
}

void MythosCardScript::teardownRumor()
{
    Q_ASSERT(type() == Rumor);
    QScriptValue res = gGameScript->call(GameScript::F_Rumor, m_teardownRumorFunc, m_object);
    if (res.isError()) {
        qCritical() << "Mythos teardownRumor Error:" << res.toString();
    }
}

GameOption *MythosCardScript::rumorFieldOption()
{
    return m_rumorFieldOption;
}

AH::Common::FieldData::FieldID MythosCardScript::rumorFieldId()
{
    return m_rumorFieldId;
}

void MythosCardScript::dynamicPropertyChanged()
{
    gGame->invalidateObject(id());
}

bool MythosCardScript::verify(MythosCardScript *myth, QString *err)
{
    QStringList errs;
    if (myth->m_name.isEmpty()) errs << "name must be set";
    if (myth->m_type == None) errs << "type must be set";
    //if (myth->m_clueField == AH::Common::FieldData::NO_NO_FIELD) errs << "clueField must be set";
    //if (myth->m_gateField == AH::Common::FieldData::NO_NO_FIELD) errs << "gateField must be set";
    //if (myth->m_moveBlack == AH::NoDimension) errs << "moveBlack must be set";
    //if (myth->m_moveWhite == AH::NoDimension) errs << "moveWhite must be set";

    if (myth->m_type == Headline) {
        if (!myth->m_headlineFunc.isFunction()) errs << "executeHeadline must be a function";
    }

    if (myth->m_type == Environment) {
        if (myth->m_envType == AH::Common::MythosData::Env_None)
            errs << "environmentType must be set";
        if (!myth->m_envFieldOptionId.isEmpty()) {
            if (myth->m_envFieldId == AH::Common::FieldData::NO_NO_FIELD)
                errs << "environmentField must be set";
        }
        if (myth->m_onEndMoveFunc.isValid() && !myth->m_onEndMoveFunc.isFunction()) errs << "onEndMovement must be a function";
    }

    if (myth->m_type == Rumor) {
        if (myth->m_rumorFieldOptionId.isEmpty()) errs << "rumorFieldOptionId must be set";
        if (!gGame->board()->field(myth->m_rumorFieldId)) errs << "rumorField must be set";
        if (!myth->m_setupRumorFunc.isFunction()) errs << "setupRumor must be a function";
        if (!myth->m_teardownRumorFunc.isFunction()) errs << "teardownRumor must be a function";
        if (!myth->m_onMythosFunc.isFunction()) errs << "onMythos must be a function";
        if (!myth->m_passFunc.isFunction()) errs << "onPass must be a function";
        if (!myth->m_failFunc.isFunction()) errs << "onFail must be a function";
    }

    if (err) *err = errs.join("\n");
    if (errs.isEmpty()) {
        return true;
    }
    return false;
}

/*
AH::Dimensions MythosCardScript::parseDimensions(QScriptValue v)
{
    if (v.isArray()) {
        QScriptValueList lst = GameScript::array2list(v);
        AH::Dimensions dims = AH::NoDimension;
        foreach (QScriptValue v, lst) {
            dims |= static_cast<AH::Dimension> (v.toUInt32());
        }
        return dims;
    } else {
        return static_cast<AH::Dimension> (v.toUInt32());
    }
}
*/
