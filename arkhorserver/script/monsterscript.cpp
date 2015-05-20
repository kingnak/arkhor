#include "monsterscript.h"
#include <QScriptContext>
#include <QScriptEngine>
#include <QStringList>
#include "characterscript.h"

MonsterScript::MonsterScript(QObject *parent)
    : QObject(parent)
{
}

QScriptValue MonsterScript::createMonster(QScriptContext *ctx, QScriptEngine *eng)
{
    if (ctx->argumentCount() != 1 || !ctx->argument(0).isObject()) {
        return ctx->throwError(QScriptContext::TypeError, "createMonster: Must call with 1 object");
    }

    QScopedPointer<MonsterScript> ret(new MonsterScript);

    QScriptValue data = ctx->argument(0);
    ret->m_id = data.property("id").toString();
    ret->m_name = data.property("name").toString();
    ret->m_desc = data.property("description").toString();
    int dim = data.property("dimension").toInt32();
    ret->m_dimension = static_cast<AH::Dimension> (dim);
    int mov = data.property("movementType").toInt32();
    ret->m_movement = static_cast<MovementType> (mov);
    ret->m_awareness = data.property("awareness").toInt32();
    ret->m_horrorAdjustment = data.property("horrorAdjustment").toInt32();
    ret->m_horrorDamage = data.property("horrorDamage").toInt32();
    ret->m_combatAdjustment = data.property("combatAdjustment").toInt32();
    ret->m_combatDamage = data.property("combatValue").toInt32();
    ret->m_toughness = data.property("toughness").toInt32();

    QScriptValue att = data.property("attributes");
    if (att.isFunction()) {
        ret->m_attrFunc = att;
    } else {
        ret->m_attributes = MonsterAttributes(att.toUInt32());
    }

    ret->m_specialMoveFunc = data.property("onMove");
    ret->m_onDefeatFunc = data.property("onDefeat");

    QString err;
    if (!verify(ret.data(), &err)) {
        return ctx->throwError(QScriptContext::TypeError, "createInvestigator: Invalid Investigator data. Errors:\n"+err);
    }

    MonsterScript *pRet = ret.take();
    QScriptValue object = eng->newQObject(pRet);
    pRet->m_this = object;

    return object;
}

AH::Common::MonsterData::MonsterAttributes MonsterScript::attributes()
{
    if (m_attrFunc.isFunction()) {
        QScriptValue v = m_attrFunc.call(m_this);
        return MonsterAttributes(v.toInt32());
    }
    return Monster::attributes();
}

void MonsterScript::move()
{
    if (m_movement == Special) {
        m_specialMoveFunc.call(m_this);
    } else {
        Monster::move();
    }
}

void MonsterScript::defeat(Character *byCharacter)
{
    if (m_onDefeatFunc.isFunction()) {
        CharacterScript *cs = dynamic_cast<CharacterScript *> (byCharacter);
        m_onDefeatFunc.call(m_this, engine()->toScriptValue(cs));
    } else {
        Monster::defeat(byCharacter);
    }
}

bool MonsterScript::verify(MonsterScript *m, QString *msg)
{
    QStringList errs;

    if (m->m_id.isEmpty()) errs.append("id must be set");
    if (m->m_name.isEmpty()) errs.append("name must be set");
    if (m->m_toughness <= 0) errs.append("toughness must be greater than 0");
    if (m->m_movement == Special && !m->m_specialMoveFunc.isFunction()) errs.append("Special movement function requried");

    if (msg) *msg = errs.join("\n");
    if (errs.isEmpty()) {
        return true;
    }
    return false;
}
