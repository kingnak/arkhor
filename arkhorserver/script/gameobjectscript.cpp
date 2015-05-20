#include "gameobjectscript.h"
#include <QScriptContext>
#include <qscriptengine.h>
#include "gamescript.h"
#include "game/game.h"
#include "propertymodificationscript.h"
#include <QDebug>

GameObjectScript::GameObjectScript(QObject *parent) :
    QObject(parent)
{
}

GameObject *GameObjectScript::clone()
{
    GameObjectScript *c = new GameObjectScript(parent());
    c->GameObjectData::operator=(*data());
    c->m_actMap = m_actMap;
    c->m_optMap = m_optMap;
    //c->m_mods = m_mods;
    foreach (const PropertyModification &m, m_mods) {
        c->m_mods.append(PropertyModification(c, m.affectedProperty(), m.modificationAmount()));
    }

    return c;
}

GameObjectScript *GameObjectScript::createGameObject(QScriptContext *ctx, QScriptEngine *eng)
{
    Q_UNUSED(eng);
    if (ctx->argumentCount() != 1 || !ctx->argument(0).isObject()) {
        ctx->throwError(QScriptContext::TypeError, "createObject: Must call with 1 object");
        return NULL;
    }

    QScopedPointer<GameObjectScript> ret(new GameObjectScript);

    QScriptValue data = ctx->argument(0);

    ret->m_tid = data.property("tid").toString();
    ret->m_name = data.property("name").toString();
    ret->m_description = data.property("description").toString();
    ret->m_type = static_cast<AH::GameObjectType> (data.property("type").toUInt32());

    if (data.property("exhaustable").isBool())
        ret->m_bExhaustable = data.property("exhaustable").toBool();
    else
        ret->m_bExhaustable = false;
    ret->m_bIsExhausted = false;

    ret->m_hands = data.property("hands").toUInt32();
    ret->m_price = data.property("price").toUInt32();

    ret->m_actionIds = GameScript::array2stringlist(data.property("actionIds"));
    ret->m_optionIds = GameScript::array2stringlist(data.property("optionIds"));

    foreach (QString id, ret->m_actionIds) {
        ret->m_actMap[id] = NULL;
    }
    foreach (QString id, ret->m_optionIds) {
        ret->m_optMap[id] = NULL;
    }

    QScriptValue propMod = data.property("modifications");
    if (propMod.isValid()) {
        PropertyModificationList lst;
        if (!PropertyModificationScript::parsePropertyModificationList(ret.data(), propMod, lst)) {
            ctx->throwError(QScriptContext::TypeError, "createObject: Invalid Property Modification.");
            return NULL;
        }
        ret->m_mods = lst;
    }

    ret->m_castCost = data.property("castCost").toInt32();
    ret->m_castAdjustment = data.property("castAdjustment").toInt32();
    ret->m_castFunc = data.property("cast");

    QString err;
    if (!verify(ret.data(), &err)) {
        ctx->throwError(QScriptContext::TypeError, "createObject: Invalid GameObject data. Errors:\n"+err);
        return NULL;
    }

    GameObjectScript *pRet = ret.take();
    //QScriptValue object = eng->newQObject(pRet);
    //pRet->m_object = object;

    return pRet;
}

PropertyModificationList GameObjectScript::getModifications() const
{
    return m_mods;
}

bool GameObjectScript::verify(GameObjectScript *ob, QString *msg)
{
    QStringList errs;
    if (ob->typeId().isEmpty()) errs.append("tid must be set");
    if (ob->name().isEmpty()) errs.append("name must be set");
    if (ob->type() == AH::NoObject) errs.append("type must be set");
    if (ob->type() == AH::Obj_Spell) {
        bool hasMagDmg = false;
        foreach (PropertyModification m, ob->m_mods) {
            if (m.affectedProperty() == AH::Common::PropertyValueData::Damage_Magical)
                hasMagDmg = true;
            if (m.affectedProperty() == AH::Common::PropertyValueData::Damage_Physical)
                errs.append("Spells cannot deal physical damage");
        }

        if ((!ob->m_castFunc.isValid() || !ob->m_castFunc.isFunction()) && !hasMagDmg)
            errs.append("spells must have cast() function or deal magical damage");
    }

    if (msg) *msg = errs.join("\n");
    if (errs.isEmpty()) {
        return true;
    }
    return false;
}

bool GameObjectScript::resolveDependencies(GameRegistry *reg)
{
    bool ok = true;
    foreach (QString id, m_actMap.keys()) {
        if (!m_actMap[id]) {
            GameAction *a = reg->findActionById(id);
            if (a) {
                m_actMap[id] = a;
            } else {
                qWarning() << "Cannot resolve action" << id << "for object" << this->id();
                ok = false;
            }
        }
    }

    foreach (QString id, m_optMap.keys()) {
        if (!m_optMap[id]) {
            GameOption *o = reg->findOptionById(id);
            if (o) {
                m_optMap[id] = o;
            } else {
                qWarning() << "Cannot resolve option" << id << "for object" << this->id();
                ok = false;
            }
        }
    }

    // Options => Actions must be resolved already

    return ok;
}

bool GameObjectScript::cast(Player *p)
{
    if (!GameObject::cast(p)) {
        return false;
    }

    m_castFunc.call();
    return true;
}

