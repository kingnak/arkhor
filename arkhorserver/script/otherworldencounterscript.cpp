#include "otherworldencounterscript.h"
#include <QScriptContext>
#include "gamescript.h"
#include "game/game.h"
#include "game/gameregistry.h"
#include <QDebug>

OtherWorldEncounterScript::OtherWorldEncounterScript(QObject *parent) :
    QObject(parent), OtherWorldEncounter()
{
}

QList<GameOption *> OtherWorldEncounterScript::options() const
{
    return m_opts.values();
}

bool OtherWorldEncounterScript::resolveDependencies(GameRegistry *reg)
{
    bool ok = true;
    for (auto id : m_opts.keys()) {
        if (m_opts[id] == NULL) {
            GameOption *op = reg->findOptionById(id);
            if (op) {
                m_opts[id] = op;
            } else {
                qWarning() << "Cannot resolve option" << id << "for Other World Encounter on field" << this->fieldId();
                ok = false;
            }
        }
    }
    return ok;
}

OtherWorldEncounterScript *OtherWorldEncounterScript::createEncounter(QScriptContext *ctx, QScriptEngine *eng)
{
    Q_UNUSED(eng);
    if (ctx->argumentCount() != 1 || !ctx->argument(0).isObject()) {
        ctx->throwError(QScriptContext::TypeError, "createEncounter: Must call with 1 object");
        return NULL;
    }

    QScopedPointer<OtherWorldEncounterScript> ret(new OtherWorldEncounterScript);

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

    ret->m_color = static_cast<AH::OtherWorldColor> (data.property("color").toUInt32());

    QString err;
    if (!verify(ret.data(), &err)) {
        ctx->throwError(QScriptContext::TypeError, "createEncounter: Invalid Encounter data. Errors:\n"+err);
        return NULL;
    }

    OtherWorldEncounterScript *pRet = ret.take();
    return pRet;
}


bool OtherWorldEncounterScript::verify(OtherWorldEncounterScript *act, QString *err)
{
    QStringList errs;
    if (act->m_description.isEmpty()) errs.append("description must be set");
    if (act->m_optionIds.isEmpty()) errs.append("options must be set");
    //if (act->m_fieldId == AH::Common::FieldData::NO_NO_FIELD) errs.append("field must be set");
    if (act->m_fieldId != AH::Common::FieldData::NO_NO_FIELD)
        if (gGame->board()->field(act->m_fieldId)->type() != AH::Common::FieldData::OtherWorld) errs.append("field must be an other world field");
    if (act->m_color == AH::OWC_NoColor) errs.append("color most be set");

    if (err) *err = errs.join("\n");
    if (errs.isEmpty()) {
        return true;
    }
    return false;
}
