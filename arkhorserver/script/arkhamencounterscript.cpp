#include "arkhamencounterscript.h"
#include <QScriptContext>
#include "gamescript.h"
#include "game/game.h"
#include "game/gameregistry.h"
#include <QDebug>

ArkhamEncounterScript::ArkhamEncounterScript(QObject *parent) :
    QObject(parent)
{
}

QList<GameOption *> ArkhamEncounterScript::options() const
{
    return m_opts.values();
}

bool ArkhamEncounterScript::resolveDependencies(GameRegistry *reg)
{
    bool ok = true;
    for (auto id : m_opts.keys()) {
        if (m_opts[id] == NULL) {
            GameOption *op = reg->findOptionById(id);
            if (op) {
                m_opts[id] = op;
            } else {
                qWarning() << "Cannot resolve option" << id << "for Arkham Encounter on field" << this->fieldId();
                ok = false;
            }
        }
    }
    return ok;
}

ArkhamEncounterScript *ArkhamEncounterScript::createEncounter(QScriptContext *ctx, QScriptEngine *eng)
{
    Q_UNUSED(eng);
    if (ctx->argumentCount() != 1 || !ctx->argument(0).isObject()) {
        ctx->throwError(QScriptContext::TypeError, "createEncounter: Must call with 1 object");
        return NULL;
    }

    QScopedPointer<ArkhamEncounterScript> ret(new ArkhamEncounterScript);

    QScriptValue data = ctx->argument(0);
#ifdef TEST_SCRIPT_BUILD
    ret->m_id = data.property("id").toString();
#endif
    ret->m_fieldId = static_cast<AH::Common::FieldData::FieldID> (data.property("field").toUInt32());
    ret->m_description = data.property("description").toString();
    ret->m_optionIds = GameScript::array2stringlist(data.property("options"));
    for (auto id : ret->m_optionIds) {
        ret->m_opts[id] = NULL;
    }

    QString err;
    if (!verify(ret.data(), &err)) {
        ctx->throwError(QScriptContext::TypeError, "createEncounter: Invalid Encounter data. Errors:\n"+err);
        return NULL;
    }

    ArkhamEncounterScript *pRet = ret.take();
    return pRet;
}


bool ArkhamEncounterScript::verify(ArkhamEncounterScript *act, QString *err)
{
    QStringList errs;
    if (act->m_description.isEmpty()) errs.append("description must be set");
    if (act->m_optionIds.isEmpty()) errs.append("options must be set");
    if (act->m_fieldId == AH::Common::FieldData::NO_NO_FIELD) errs.append("field must be set");

    if (err) *err = errs.join("\n");
    if (errs.isEmpty()) {
        return true;
    }
    return false;
}
