#include "investigatorscript.h"
#include <QScriptContext>
#include <qscriptengine.h>
#include "gamescript.h"
#include "characterscript.h"
#include "game/gameboard.h"
#include "game/gamefield.h"

InvestigatorScript::InvestigatorScript(QObject *parent) :
    QObject(parent)
{
}

Character *InvestigatorScript::instantiate()
{
    if (m_isInstantiated) return NULL;
    Character *c = new CharacterScript(this);
    m_isInstantiated = true;
    return c;
}

QScriptValue InvestigatorScript::createInvestigator(QScriptContext *ctx, QScriptEngine *eng)
{
    if (ctx->argumentCount() != 1 || !ctx->argument(0).isObject()) {
        return ctx->throwError(QScriptContext::TypeError, "createInvestigator: Must call with 1 object");
    }

    QScopedPointer<InvestigatorScript> ret(new InvestigatorScript);

    QScriptValue data = ctx->argument(0);
    ret->m_id = data.property("id").toString();
    ret->m_name = data.property("name").toString();
    ret->m_occupation = data.property("occupation").toString();
    ret->m_homeFieldId = static_cast<AH::Common::FieldData::FieldID> (data.property("homeFieldId").toInt32());
    ret->m_sanity = data.property("sanity").toInt32();
    ret->m_stamina = data.property("stamina").toInt32();
    ret->m_money = data.property("money").toInt32();
    ret->m_focus = data.property("focus").toInt32();
    ret->m_clues = data.property("clues").toInt32();

    QScriptValue attr = data.property("attributes");
    QScriptValue attrSS = attr.property("SpeedSneak");
    QScriptValue attrFW = attr.property("FightWill");
    QScriptValue attrLL = attr.property("LoreLuck");
    if (!parseAttributeList(attrSS, ret->m_attrSpeedSneak)) {
        return ctx->throwError(QScriptContext::TypeError, "createInvestigator: Invalid Attribute list for Speed / Sneak");
    }
    if (!parseAttributeList(attrFW, ret->m_attrFightWill)) {
        return ctx->throwError(QScriptContext::TypeError, "createInvestigator: Invalid Attribute list for Fight / Will");
    }
    if (!parseAttributeList(attrLL, ret->m_attrLoreLuck)) {
        return ctx->throwError(QScriptContext::TypeError, "createInvestigator: Invalid Attribute list for Lore / Luck");
    }

    ret->m_uniqueAbilityId = data.property("uniqueAbilityId").toString();
    ret->m_fixedPossesionObjectIds = GameScript::array2stringlist( data.property("fixedPossesionObjectIds") );

    QScriptValueList rndPoss = GameScript::array2list( data.property("randomPossessions") );
    foreach (QScriptValue v, rndPoss) {
        int type = v.property("type").toInt32();
        int amount = v.property("amount").toInt32();
        AH::Common::InvestigatorData::ObjectTypeCount otc(static_cast<AH::GameObjectType>(type), amount);
        ret->m_randomPossessions << otc;
    }

    QString err;
    if (!verify(ret.data(), &err)) {
        return ctx->throwError(QScriptContext::TypeError, "createInvestigator: Invalid Investigator data. Errors:\n"+err);
    }

    QScriptValue object = eng->newQObject(ret.take());

    return object;
}

bool InvestigatorScript::parseAttributeList(QScriptValue v, QList<AttributeValuePair> &ret)
{
    if (!v.isArray()) return false;
    QScriptValueList lst = GameScript::array2list(v);
    foreach (QScriptValue e, lst) {
        if (!e.isArray()) return false;
        QScriptValueList pair = GameScript::array2list(e);
        if (pair.size() != 2) return false;
        ret << AttributeValuePair(pair[0].toInt32(), pair[1].toInt32());
    }

    return true;
}

bool InvestigatorScript::verify(InvestigatorScript *inv, QString *msg)
{
    QStringList errs;

    if (inv->m_id.isEmpty()) errs.append("id must be set");
    if (inv->m_name.isEmpty()) errs.append("name must be set");
    AH::Common::FieldData::FieldID fid = static_cast<AH::Common::FieldData::FieldID> (inv->m_homeFieldId);
    if (gGame->board()->field(fid) == NULL) errs.append("home field is invalid");
    if (gGame->board()->field(fid)->type() != AH::Common::FieldData::Location) errs.append("home field must be a Location field");
    if (inv->m_sanity <= 0) errs.append("sanity must be greater than 0");
    if (inv->m_stamina <= 0) errs.append("stamina must be greater than 0");
    if (inv->m_focus <= 0) errs.append("focus must be greater than 0");
    if (inv->attrSpeedSneak().size() < 1) errs.append("Speed/Sneak attributes must not be empty");
    if (inv->attrFightWill().size() < 1) errs.append("Speed/Sneak attributes must not be empty");
    if (inv->attrLoreLuck().size() < 1) errs.append("Speed/Sneak attributes must not be empty");

    if (msg) *msg = errs.join("\n");
    if (errs.isEmpty()) {
        return true;
    }
    return false;
}
