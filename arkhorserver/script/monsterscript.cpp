#include "monsterscript.h"
#include <QScriptContext>
#include <QScriptEngine>
#include <QStringList>
#include "characterscript.h"

MonsterScript::MonsterScript(QObject *parent)
    : QObject(parent)
{
}

Monster *MonsterScript::clone()
{
    MonsterScript *c = new MonsterScript(parent());
    c->MonsterData::operator=(*data());
    c->m_attrFunc = m_attrFunc;
    c->m_onDefeatFunc = m_onDefeatFunc;
    c->m_specialMoveFunc = m_specialMoveFunc;
    return c;
}

MonsterScript *MonsterScript::createMonster(QScriptContext *ctx, QScriptEngine *eng)
{
    Q_UNUSED(eng);
    if (ctx->argumentCount() != 1 || !ctx->argument(0).isObject()) {
        ctx->throwError(QScriptContext::TypeError, "createMonster: Must call with 1 object");
        return NULL;
    }

    QScopedPointer<MonsterScript> ret(new MonsterScript);

    QScriptValue data = ctx->argument(0);
    ret->m_tid = data.property("id").toString();
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
    ret->m_combatDamage = data.property("combatDamage").toInt32();
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
        ctx->throwError(QScriptContext::TypeError, "createInvestigator: Invalid Investigator data. Errors:\n"+err);
        return NULL;
    }

    MonsterScript *pRet = ret.take();

    return pRet;
}

AH::Common::MonsterData::MonsterAttributes MonsterScript::attributes()
{
    if (m_attrFunc.isFunction()) {
        QScriptValue v = m_attrFunc.call(getThis());
        return MonsterAttributes(v.toInt32());
    }
    return Monster::attributes();
}

void MonsterScript::move(AH::MovementDirection dir)
{
    if (m_movement == Special) {
        m_specialMoveFunc.call(/*getThis()*/);
    } else {
        Monster::move(dir);
    }
}

void MonsterScript::defeat(Character *byCharacter)
{
    if (m_onDefeatFunc.isFunction()) {
        CharacterScript *cs = dynamic_cast<CharacterScript *> (byCharacter);
        m_onDefeatFunc.call(getThis(), engine()->toScriptValue(cs));
    } else {
        Monster::defeat(byCharacter);
    }
}

QScriptValue MonsterScript::getThis()
{
    return engine()->newQObject(this, QScriptEngine::QtOwnership, QScriptEngine::PreferExistingWrapperObject);
}

bool MonsterScript::verify(MonsterScript *m, QString *msg)
{
    QStringList errs;

    if (m->m_tid.isEmpty()) errs.append("id must be set");
    if (m->m_name.isEmpty()) errs.append("name must be set");
    if (m->m_toughness <= 0) errs.append("toughness must be greater than 0");
    if (m->m_movement == Special && !m->m_specialMoveFunc.isFunction()) errs.append("Special movement function requried");

    if (msg) *msg = errs.join("\n");
    if (errs.isEmpty()) {
        return true;
    }
    return false;
}
