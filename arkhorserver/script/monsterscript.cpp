#include "monsterscript.h"
#include <QScriptContext>
#include <QScriptEngine>
#include <QStringList>
#include "characterscript.h"
#include "gamescript.h"
#include "propertymodificationscript.h"
#include "gamefieldscript.h"
#include <QDebug>

QReadWriteLock MonsterScript::s_attrFunctionLock;
QReadWriteLock MonsterScript::s_modFunctionLock;

MonsterScript::MonsterScript(QObject *parent)
    : QObject(parent)
    , m_oldDynAttrs(0)
{
    m_fieldBridge = new GameFieldScript(this);
}

Monster *MonsterScript::clone()
{
    MonsterScript *c = new MonsterScript(parent());
    c->MonsterData::operator=(*data());
    c->m_attrFunc = m_attrFunc;
    c->m_onDefeatFunc = m_onDefeatFunc;
    c->m_specialMoveFunc = m_specialMoveFunc;
    c->m_modsFunc = m_modsFunc;
    c->m_onDamageFunc = m_onDamageFunc;
    c->m_onFleeFunc = m_onFleeFunc;
    c->m_onEvadeFunc = m_onEvadeFunc;
    c->m_onHorrorFunc = m_onHorrorFunc;
    return c;
}

GameFieldScript *MonsterScript::fieldScript()
{
    m_fieldBridge->setField(this->m_field);
    return m_fieldBridge;
}

void MonsterScript::placeOnField(int fieldId)
{
    GameField *oldField = this->field();
    GameField *f = gGame->board()->field(static_cast<AH::Common::FieldData::FieldID> (fieldId));
    if (f) {
        f->placeMonster(this);
        if (oldField)
            gGame->changeMonsterMove(this, {oldField->id(), f->id()});
        else
            gGame->changeMonsterAppear(this);
    }
}

MonsterScript *MonsterScript::createMonster(QScriptContext *ctx, QScriptEngine *eng)
{
    Q_UNUSED(eng);
    if (ctx->argumentCount() != 1 || !ctx->argument(0).isObject()) {
        ctx->throwError(QScriptContext::TypeError, "createMonster: Must call with 1 object");
        return nullptr;
    }

    QScopedPointer<MonsterScript> ret(new MonsterScript);

    QScriptValue data = ctx->argument(0);
    ret->m_tid = data.property("tid").toString();
    ret->m_name = data.property("name").toString();
    ret->m_desc = data.property("description").toString();
    ret->m_mythText = data.property("mythText").toString();
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
        ret->m_attributes = GameScript::parseFlags<MonsterAttributes>(data.property("attributes"), MonsterAttributes(0));
    }

    ret->m_specialMoveFunc = data.property("onMove");
    ret->m_onDefeatFunc = data.property("onDefeat");
    ret->m_onDamageFunc = data.property("onDamage");
    ret->m_onEvadeFunc = data.property("onEvade");
    ret->m_onFleeFunc = data.property("onFlee");
    ret->m_onHorrorFunc = data.property("onHorror");
    ret->m_modsFunc = data.property("modifications");

    QString err;
    if (!verify(ret.data(), &err)) {
        ctx->throwError(QScriptContext::TypeError, "createMonster: Invalid Monster data. Errors:\n"+err);
        return nullptr;
    }

    MonsterScript *pRet = ret.take();

    return pRet;
}

AH::Common::MonsterData::MonsterAttributes MonsterScript::attributes() const
{
    if (m_attrFunc.isFunction()) {
        if (!gGameScript->isGameThread()) {
            QReadLocker r(&s_attrFunctionLock);
            return m_oldDynAttrs;
        }

        QScriptValue f = m_attrFunc;
        QScriptValue v = gGameScript->call(GameScript::F_Modification, f);
        MonsterAttributes newAttrs = MonsterAttributes(v.toInt32());

        QWriteLocker w(&s_attrFunctionLock);
        if (m_oldDynAttrs != newAttrs) {
            gGame->invalidateObject(id());
            m_oldDynAttrs = newAttrs;
        }
        return newAttrs;
    }
    return Monster::attributes();
}

void MonsterScript::move(AH::MovementDirection dir)
{
    if (m_movement == Special) {
        gGame->notifier()->acknowledgeMonsterMovement(this);
        gGameScript->call(GameScript::F_Monster, m_specialMoveFunc, getThis());
    } else {
        Monster::move(dir);
    }
}

void MonsterScript::defeat(Character *byCharacter)
{
    if (!gGame->handleDefeatMonster(byCharacter, this)) {
        return;
    }
    if (m_onDefeatFunc.isFunction()) {
        CharacterScript *cs = dynamic_cast<CharacterScript *> (byCharacter);
        QScriptValue res = gGameScript->call(GameScript::F_Monster, m_onDefeatFunc, getThis(), QScriptValueList() << gGameScript->engine()->toScriptValue(cs));
        if (res.isUndefined() || (res.isBool() && res.toBool())) {
            // If no result, or true result, do default action
            Monster::execDefeat(byCharacter);
        } else {
            // Must remove in any case
            if (m_field) {
                m_field->removeMonster(this);
            }
        }
    } else {
        Monster::execDefeat(byCharacter);
    }
}

bool MonsterScript::hasCustomHorrorCheck() const
{
    return m_onHorrorFunc.isFunction();
}

bool MonsterScript::horrorCheck(Character *c)
{
    if (m_onHorrorFunc.isFunction()) {
        CharacterScript *cs = dynamic_cast<CharacterScript *> (c);
        QScriptValue res = gGameScript->call(GameScript::F_Monster, m_onHorrorFunc, getThis(), gGameScript->engine()->toScriptValue(cs));
        if (res.isBool()) {
            return res.toBool();
        }
    }
    return true;
}

bool MonsterScript::damage(Character *c, Monster::DamageType t)
{
    if (m_onDamageFunc.isFunction()) {
        CharacterScript *cs = dynamic_cast<CharacterScript *> (c);
        QScriptValueList args;
        args << t;
        args << gGameScript->engine()->toScriptValue(cs);
        QScriptValue res = gGameScript->call(GameScript::F_Monster, m_onDamageFunc, getThis(), args);
        if (res.isBool()) {
            return res.toBool();
        }
    }
    return true;
}

void MonsterScript::evaded(Character *c)
{
    if (m_onEvadeFunc.isFunction()) {
        CharacterScript *cs = dynamic_cast<CharacterScript *> (c);
        gGameScript->call(GameScript::F_Monster, m_onEvadeFunc, getThis(), gGameScript->engine()->toScriptValue(cs));
    }
}

void MonsterScript::flown(Character *c)
{
    if (m_onFleeFunc.isFunction()) {
        CharacterScript *cs = dynamic_cast<CharacterScript *> (c);
        gGameScript->call(GameScript::F_Monster, m_onFleeFunc, getThis(), gGameScript->engine()->toScriptValue(cs));
    }
}

PropertyModificationList MonsterScript::getModifications() const
{
    if (m_modsFunc.isFunction()) {
        if (!gGameScript->isGameThread()) {
            QReadLocker r(&s_modFunctionLock);
            return Monster::getModifications() + m_oldDynMods;
        }

        QScriptValue f = m_modsFunc;
        QScriptValue v = gGameScript->call(GameScript::F_Modification, f/*, getThis()*/);
        PropertyModificationList lst;
        if (PropertyModificationScript::parsePropertyModificationList(this, v, lst)) {
            QWriteLocker w(&s_modFunctionLock);
            if (m_oldDynMods != lst) {
                gGame->invalidateObject(id());
                m_oldDynMods = lst;
            }
            return Monster::getModifications() + lst;
        } else {
            qWarning() << "Error in properties Function for Monster" << m_tid;
        }
    }
    return Monster::getModifications();
}

QScriptValue MonsterScript::getThis()
{
    return gGameScript->engine()->newQObject(this, QScriptEngine::QtOwnership, QScriptEngine::PreferExistingWrapperObject);
}

bool MonsterScript::verify(MonsterScript *m, QString *msg)
{
    QStringList errs;

    if (m->m_tid.isEmpty()) errs.append("id must be set");
    if (m->m_name.isEmpty()) errs.append("name must be set");
    if (m->m_toughness <= 0) errs.append("toughness must be greater than 0");
    if (m->m_movement == Special && !m->m_specialMoveFunc.isFunction()) errs.append("Special movement function requried");
    if (m->m_onDamageFunc.isValid() && !m->m_onDamageFunc.isFunction()) errs.append("onDamage must be a function");
    if (m->m_onDefeatFunc.isValid() && !m->m_onDefeatFunc.isFunction()) errs.append("onDefeat must be a function");
    if (m->m_onEvadeFunc.isValid() && !m->m_onEvadeFunc.isFunction()) errs.append("onEvade must be a function");
    if (m->m_onFleeFunc.isValid() && !m->m_onFleeFunc.isFunction()) errs.append("onFlee must be a function");
    if (m->m_onHorrorFunc.isValid() && !m->m_onHorrorFunc.isFunction()) errs.append("onHorror must be a function");
    if (m->m_modsFunc.isValid() && !m->m_modsFunc.isFunction()) errs.append("modifications must be a function");

    if (msg) *msg = errs.join("\n");
    if (errs.isEmpty()) {
        return true;
    }
    return false;
}
