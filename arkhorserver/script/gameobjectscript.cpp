#include "gameobjectscript.h"
#include <QScriptContext>
#include <qscriptengine.h>
#include "gamescript.h"
#include "game/game.h"
#include "characterscript.h"
#include "propertymodificationscript.h"
#include "game/actions/castspellaction.h"
#include <QDebug>

GameObjectScript::GameObjectScript(QObject *parent) :
    DynamicScriptableObject(parent)
{
}

GameObject *GameObjectScript::clone()
{
    GameObjectScript *c = new GameObjectScript(parent());
    c->GameObjectData::operator=(*data());
    c->m_actMap = m_actMap;
    c->m_optMap = m_optMap;

	// Proxy options/actions must be updated to use new object
	for (auto &a : c->m_actMap) {
		if (auto p = dynamic_cast<GameObjectScriptProxyAction*>(a)) {
			a = new GameObjectScriptProxyAction(c, p->realAction());
		}
	}
	for (auto &o : c->m_optMap) {
		if (auto p = dynamic_cast<GameObjectScriptProxyOption*>(o)) {
			o = new GameObjectScriptProxyOption(c, p->realOption());
		}
	}

    foreach (const PropertyModification &m, m_mods) {
        c->m_mods.append(PropertyModification(c, m.affectedProperty(), m.modificationAmount()));
    }
    c->m_modsFunc = m_modsFunc;
    c->m_onAddFunc = m_onAddFunc;
    c->m_onRemoveFunc = m_onRemoveFunc;
    c->m_castFunc = m_castFunc;
    c->m_this = gGameScript->engine()->newQObject(c);
    clonePropertiesInto(c);

    return c;
}

void GameObjectScript::removeFromGame()
{
    if (this->owner()) {
        this->owner()->removeFromInventory(this);
    }
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

    ret->m_actionIds = GameScript::array2stringlist(data.property("actions"));
    ret->m_optionIds = GameScript::array2stringlist(data.property("options"));

    foreach (QString id, ret->m_actionIds) {
        ret->m_actMap[id] = NULL;
    }
    foreach (QString id, ret->m_optionIds) {
        ret->m_optMap[id] = NULL;
    }

    QScriptValue propMod = data.property("modifications");
    if (propMod.isValid() && !propMod.isUndefined()) {
        if (propMod.isFunction()) {
            ret->m_modsFunc = propMod;
        } else {
            PropertyModificationList lst;
            if (!PropertyModificationScript::parsePropertyModificationList(ret.data(), propMod, lst)) {
                ctx->throwError(QScriptContext::TypeError, "createObject: Invalid Property Modification.");
                return NULL;
            }
            ret->m_mods = lst;
        }
    }

    ret->m_attrs = GameScript::parseFlags<GameObjectData::ObjectAttributes>(data.property("attributes"), GameObjectData::None);

    ret->m_onAddFunc = data.property("onAddToInventory");
    ret->m_onRemoveFunc = data.property("onRemoveFromInventory");

    if (ret->m_type == AH::Obj_Spell) {
        if (data.property("castTarget").isValid())
            ret->m_castTarget = data.property("castTarget").toInt32();
        else
            ret->m_castTarget = 1;
        ret->m_castCost = data.property("castCost").toInt32();
        ret->m_castAdjustment = data.property("castAdjustment").toInt32();
        ret->m_castFunc = data.property("cast");

        QScriptValue castOption;
        AH::GamePhases phases = AH::NoGamePhase;
        if (ret->m_castFunc.isFunction()) {
            // Default cast option
            castOption = eng->newObject();
            phases = GameScript::parseFlags<AH::GamePhases> (data.property("castPhases"), AH::AllPhases);
            castOption.setProperty("isAvailable", data.property("castIsAvailable"));
            castOption.setProperty("chooseType", data.property("castChooseType"));
            castOption.setProperty("continueType", data.property("castContinueType"));
        } else if (ret->m_castFunc.isObject()) {
            // extract Real function
            castOption = ret->m_castFunc;
            ret->m_castFunc = castOption.property("cast");
            castOption.setProperty("cast", QScriptValue());
            phases = GameScript::parseFlags<AH::GamePhases> (castOption.property("phases"), AH::AllPhases);
        }

        if (castOption.isValid()) {
            CastSpellAction *castAct = new CastSpellAction;
            gGame->registerAction(castAct);
            castOption.setProperty("actionId", castAct->id());
            // make supplemental by default
            if (!castOption.property("chooseType").isValid())
                castOption.setProperty("chooseType", AH::ChooseSupplemental);

            GameOptionScript *castOpt = GameOptionScript::createGameOption(castOption, ctx, eng);
            gGame->registerOption(castOpt);
            castAct->setPhases(phases);
            castAct->setName("Cast "+ret->m_name);
            ret->m_optMap[castOpt->id()] = NULL;// = castOpt;
        }
    }

    DynamicPropertyScript::createDynamicProperties(data.property("properties"), ret.data());

    QString err;
    if (!verify(ret.data(), &err)) {
        ctx->throwError(QScriptContext::TypeError, "createObject: Invalid GameObject data. Errors:\n"+err);
        return NULL;
    }

    GameObjectScript *pRet = ret.take();
    QScriptValue object = eng->newQObject(pRet);
    pRet->m_this = object;

    return pRet;
}

PropertyModificationList GameObjectScript::getModifications() const
{
    if (m_modsFunc.isFunction()) {
        QScriptValue f = m_modsFunc;
        QScriptValue v = gGameScript->call(GameScript::F_Modification, f);
        PropertyModificationList lst;
        if (PropertyModificationScript::parsePropertyModificationList(this, v, lst)) {
            if (m_oldDynMods != lst) {
                gGame->invalidateObject(id());
                m_oldDynMods = lst;
            }
            return m_mods + lst;
        } else {
            qWarning() << "Error in properties Function for Object" << m_tid;
        }
    }
    return m_mods;
}

QList<AH::Common::DynamicPropertyData> GameObjectScript::dynamicProperties() const
{
    return this->getDynamicPropertyDataList();
}

bool GameObjectScript::verify(GameObjectScript *ob, QString *msg)
{
    QStringList errs;
    if (ob->typeId().isEmpty()) errs.append("tid must be set");
    if (ob->name().isEmpty()) errs.append("name must be set");
    if (ob->type() == AH::NoObject) errs.append("type must be set");
    if (ob->m_onAddFunc.isValid() && !ob->m_onAddFunc.isFunction()) errs.append("onAddToInventory must be a function");
    if (ob->m_onRemoveFunc.isValid() && !ob->m_onRemoveFunc.isFunction()) errs.append("onRemoveFromInventory must be a function");
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
                GameActionScript *gas = dynamic_cast<GameActionScript *>(a);
                if (gas) {
                    m_actMap[id] = new GameObjectScriptProxyAction(this, gas);
                } else {
                    m_actMap[id] = a;
                }
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
                GameOptionScript *gos = dynamic_cast<GameOptionScript*> (o);
                if (gos) {
                    m_optMap[id] = new GameObjectScriptProxyOption(this, gos);
                } else {
                    m_optMap[id] = o;
                }
            } else {
                qWarning() << "Cannot resolve option" << id << "for object" << this->id();
                ok = false;
            }
        }
    }

    // Options => Actions must be resolved already

    DynamicScriptableObject::resolveDependencies(m_this);

    return ok;
}

bool GameObjectScript::cast(Player *p)
{
    if (!GameObject::cast(p)) {
        return false;
    }

    gGameScript->call(GameScript::F_Action, m_castFunc, m_this);
    return true;
}

bool GameObjectScript::onAddToInventory(Character *c)
{
    if (!GameObject::onAddToInventory(c)) return false;

    if (m_onAddFunc.isFunction()) {
        CharacterScript *cs = dynamic_cast<CharacterScript *> (c);
        QScriptValue res = gGameScript->call(GameScript::F_Inventory, m_onAddFunc, m_this, gGameScript->engine()->toScriptValue(cs));
        if (res.isValid() && !res.isUndefined()) {
            if (res.isBool() && !res.toBool()) {
                // False returned
                return false;
            }
        }
    }
    return true;
}

bool GameObjectScript::onRemoveFromInventory(Character *c)
{
    if (!GameObject::onRemoveFromInventory(c)) return false;

    if (m_onRemoveFunc.isFunction()) {
        CharacterScript *cs = dynamic_cast<CharacterScript *> (c);
        QScriptValue res = gGameScript->call(GameScript::F_Inventory, m_onRemoveFunc, m_this, gGameScript->engine()->toScriptValue(cs));
        if (res.isValid() && !res.isUndefined()) {
            if (res.isBool() && !res.toBool()) {
                // False returned
                return false;
            }
        }
    }
    return true;
}

CharacterScript *GameObjectScript::csOwner() const
{
    Character *o = owner();
    if (o) {
        CharacterScript *cs = dynamic_cast<CharacterScript *> (o);
        if (cs) {
            return cs;
        }
        qWarning() << "Owner is not a CharacterScript";
    }
    return NULL;
}

void GameObjectScript::dynamicPropertyChanged()
{
    gGame->invalidateObject(id());
}

////////////////////////

bool GameObjectScriptProxyAction::execute()
{
    QScriptValue obj = gGameScript->engine()->newQObject(m_obj, QScriptEngine::QtOwnership, QScriptEngine::PreferExistingWrapperObject);
    return m_act->executeOnObject(obj);
}

QString GameObjectScriptProxyAction::notificationString(GameAction::NotificationPart part, const QString &desc) const
{
    return m_act->notificationString(part, desc);
}

////////////////////

GameObjectScriptProxyOption::GameObjectScriptProxyOption(GameObjectScript *obj, GameOptionScript *opt)
:   m_obj(obj),
    m_opt(opt)
{
    m_proxy = dynamic_cast<GameObjectScriptProxyAction*> (opt->action());
    if (!m_proxy) {
        GameActionScript *gas = dynamic_cast<GameActionScript*> (opt->action());
        if (gas) {
            m_proxy = new GameObjectScriptProxyAction(obj, gas);
        }
    }
}

AH::Common::GameOptionData *GameObjectScriptProxyOption::data()
{
    *static_cast<AH::Common::GameOptionData* const> (this) = *m_opt->data();
    m_id = id();
    return this;
}

bool GameObjectScriptProxyOption::isAvailable() const
{
    if (m_obj->isExhausted()) {
        return false;
    }
    QScriptValue obj = gGameScript->engine()->newQObject(m_obj, QScriptEngine::QtOwnership, QScriptEngine::PreferExistingWrapperObject);
    return m_opt->isAvailableWithObject(obj);
}

GameAction *GameObjectScriptProxyOption::action()
{
    return const_cast<GameAction *> (const_cast<const GameObjectScriptProxyOption*> (this)->action());
}

const GameAction *GameObjectScriptProxyOption::action() const
{
    if (m_proxy) {
        return m_proxy;
    }
    return GameOption::action();
}

//////////////////////////////////

bool CastSpellAction::executeOnObject(QScriptValue obj)
{
    GameObjectScript *spell = qscriptvalue_cast<GameObjectScript *> (obj);
    Q_ASSERT(spell && spell->type() == AH::Obj_Spell);
    gGame->notifier()->actionExecute(this, spell->name());
    return spell->cast(gGame->context().player());
}

QString CastSpellAction::notificationString(GameAction::NotificationPart part, const QString &desc) const
{
    Q_UNUSED(desc);
    Q_ASSERT(part == Execute);
    return "{C} casts {D}";
}
