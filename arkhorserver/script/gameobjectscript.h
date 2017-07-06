#ifndef GAMEOBJECTSCRIPT_H
#define GAMEOBJECTSCRIPT_H

#include <QObject>
#include <QScriptValue>
#include "game/gameobject.h"
#include <QScriptEngine>
#include "gameactionscript.h"
#include "gameoptionscript.h"
#include "dynamicpropertyscript.h"

class QScriptContext;
class GameRegistry;
class CharacterScript;

class GameObjectScript : public DynamicScriptableObject, public GameObject
{
    Q_OBJECT
public:
    explicit GameObjectScript(QObject *parent = 0);

    GameObject *clone();

    Q_PROPERTY(QString id READ id)
    Q_PROPERTY(QString typeId READ typeId)
    Q_PROPERTY(CharacterScript* owner READ csOwner)
    Q_PROPERTY(int price READ price)
    Q_PROPERTY(qint32 type READ type)

    Q_INVOKABLE void exhaust() { GameObject::exhaust(); }
    Q_INVOKABLE void returnToDeck() { GameObject::returnToDeck(); }

    Q_INVOKABLE void discard() { returnToDeck(); }
    Q_INVOKABLE void removeFromGame();

    static void castFromValue(const QScriptValue &v, GameObjectScript *&o) { o = qobject_cast<GameObjectScript *> (v.toQObject()); }
    static QScriptValue castToValue(QScriptEngine *eng, GameObjectScript * const &in) { return eng->newQObject(in); }

    static GameObjectScript *createGameObject(QScriptContext *ctx, QScriptEngine *eng);

    virtual QList<GameAction *> getActions() const { return m_actMap.values(); }
    virtual QList<GameOption *> getOptions() const { return m_optMap.values(); }
    virtual PropertyModificationList getModifications() const;

    virtual QList<AH::Common::DynamicPropertyData> dynamicProperties() const;

    virtual bool resolveDependencies(GameRegistry *reg);

    virtual bool cast(Player *p);

    virtual bool onAddToInventory(Character *c);
    virtual bool onRemoveFromInventory(Character *c);

    CharacterScript *csOwner() const;

signals:

public slots:

protected:
    virtual void dynamicPropertyChanged();

private:
    static bool verify(GameObjectScript *ob, QString *msg = NULL);

private:
    QMap<QString, GameAction *> m_actMap;
    QMap<QString, GameOption *> m_optMap;
    PropertyModificationList m_mods;
    QScriptValue m_this;
    QScriptValue m_castFunc;
    QScriptValue m_modsFunc;
    QScriptValue m_onAddFunc;
    QScriptValue m_onRemoveFunc;

private:
    mutable PropertyModificationList m_oldDynMods;
};

//////////////////////////////

class GameObjectScriptProxyAction : public GameAction
{
public:
    GameObjectScriptProxyAction(GameObjectScript *obj, GameActionScript *act)
        : m_obj(obj), m_act(act) {}

    virtual QString id() const { return QString("%1::%2").arg(m_obj->id(), m_act->id()); }
    virtual QString name() const { return m_act->name(); }
    virtual QString description() const { return m_act->description(); }
    virtual QString sourceId() const { return m_obj->id(); }
    virtual AH::GamePhases phases() const { return m_act->phases(); }
    virtual bool execute();
    virtual QString notificationString(NotificationPart part, const QString &desc = QString::null) const;

	GameActionScript *realAction() { return m_act; }

private:
    GameObjectScript *m_obj;
    GameActionScript *m_act;
};

/////////////////////////////////

class GameObjectScriptProxyOption : public GameOption
{
public:
    GameObjectScriptProxyOption(GameObjectScript *obj, GameOptionScript *opt);

    virtual AH::Common::GameOptionData *data();// { return m_opt->data(); }

    virtual QString id() const { return QString("%1::%2").arg(m_obj->id(), m_opt->id()); }
    virtual QString name() const { return m_opt->name(); }
    virtual QString description() const { return m_opt->description(); }
    QString sourceId() const { return m_obj->id(); }
    virtual AH::GamePhases phases() const { return m_opt->phases(); }
    virtual QString actionId() const { return m_opt->actionId(); }

    virtual AH::Common::Cost costs() const { return m_opt->costs(); }
    virtual bool canPay() const { return m_opt->canPay(); }
    virtual bool isAvailable() const;

    virtual AH::ChooseType chooseType() const { return m_opt->chooseType(); }
    virtual AH::ContinueType continueType() const { return m_opt->continueType(); }

    virtual GameAction *action();
    virtual const GameAction *action() const;

	GameOptionScript *realOption() { return m_opt; }

private:
    GameObjectScript *m_obj;
    GameOptionScript *m_opt;
    GameObjectScriptProxyAction *m_proxy;
};

///////////////////////////////////

class CastSpellAction : public GameActionScript
{
public:
    CastSpellAction() {}

    //virtual QString id() const { return "AC_CAST_"+m_spell->id(); }
    //virtual QString name() const { return "Cast "+m_spell->name(); }
    //virtual QString description() const { return m_spell->description(); }

    virtual bool executeOnObject(QScriptValue obj);
    virtual QString notificationString(NotificationPart part, const QString &desc = QString::null) const;

    void setPhases(AH::GamePhases ph) { m_phases = ph; }
    void setName(QString name) { m_name = name; }
};

Q_DECLARE_METATYPE(GameObjectScript*)
Q_DECLARE_METATYPE(QList<GameObjectScript*>)

#endif // GAMEOBJECTSCRIPT_H
