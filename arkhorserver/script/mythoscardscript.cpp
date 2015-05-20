#include "mythoscardscript.h"
#include "gamescript.h"
#include "game/game.h"

MythosCardScript::MythosCardScript(QObject *parent) :
    QObject(parent)
{
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
    ret->m_type = static_cast<MythosType> (data.property("type").toUInt32());
    ret->m_envType = static_cast<EnvironmentType> (data.property("environmentType").toUInt32());
    ret->m_clueField = static_cast<AH::Common::FieldData::FieldID> (data.property("clueField").toUInt32());
    ret->m_gateField = static_cast<AH::Common::FieldData::FieldID> (data.property("gateField").toUInt32());
    ret->m_moveBlack = parseDimensions(data.property("moveBlack"));
    ret->m_moveWhite = parseDimensions(data.property("moveWhite"));

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

bool MythosCardScript::verify(MythosCardScript *myth, QString *err)
{
    QStringList errs;
    if (myth->m_name.isEmpty()) errs << "name must be set";
    if (myth->m_type == None) errs << "type must be set";
    //if (myth->m_clueField == AH::Common::FieldData::NO_NO_FIELD) errs << "clueField must be set";
    //if (myth->m_gateField == AH::Common::FieldData::NO_NO_FIELD) errs << "gateField must be set";
    //if (myth->m_moveBlack == AH::NoDimension) errs << "moveBlack must be set";
    //if (myth->m_moveWhite == AH::NoDimension) errs << "moveWhite must be set";

    if (err) *err = errs.join("\n");
    if (errs.isEmpty()) {
        return true;
    }
    return false;
}

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
