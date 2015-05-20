#ifndef GAMEOBJECTSCRIPT_H
#define GAMEOBJECTSCRIPT_H

#include <QObject>
#include <QScriptValue>
#include "game/gameobject.h"
#include <QScriptEngine>

class QScriptContext;
class GameRegistry;

class GameObjectScript : public QObject, public GameObject
{
    Q_OBJECT
public:
    explicit GameObjectScript(QObject *parent = 0);

    GameObject *clone();

    Q_PROPERTY(QString id READ id)
    Q_PROPERTY(QString typeId READ typeId)

    static void castFromValue(const QScriptValue &v, GameObjectScript *&o) { o = qobject_cast<GameObjectScript *> (v.toQObject()); }
    static QScriptValue castToValue(QScriptEngine *eng, GameObjectScript * const &in) { return eng->newQObject(in); }

    static GameObjectScript *createGameObject(QScriptContext *ctx, QScriptEngine *eng);

    virtual QList<GameAction *> getActions() const { return m_actMap.values(); }
    virtual QList<GameOption *> getOptions() const { return m_optMap.values(); }
    virtual PropertyModificationList getModifications() const;

    virtual bool resolveDependencies(GameRegistry *reg);

    virtual bool cast(Player *p);

signals:

public slots:

private:
    static bool verify(GameObjectScript *ob, QString *msg = NULL);

private:
    QMap<QString, GameAction *> m_actMap;
    QMap<QString, GameOption *> m_optMap;
    PropertyModificationList m_mods;
    QScriptValue m_castFunc;
};

Q_DECLARE_METATYPE(GameObjectScript*)

#endif // GAMEOBJECTSCRIPT_H
