#ifndef GAMEACTIONSCRIPT_H
#define GAMEACTIONSCRIPT_H

#include <QObject>
#include <QScriptEngine>
#include "game/gameaction.h"

class QScriptContext;

class GameActionScript : public QObject, public GameAction
{
    Q_OBJECT
public:
    explicit GameActionScript(QObject *parent = 0);

    static void castFromValue(const QScriptValue &v, GameActionScript *&o) { o = qobject_cast<GameActionScript *> (v.toQObject()); }
    static QScriptValue castToValue(QScriptEngine *eng, GameActionScript * const &in) { return eng->newQObject(in); }

    Q_PROPERTY(QString id READ id)

    static GameActionScript *createGameAction(QScriptContext *ctx, QScriptEngine *eng);

    virtual bool execute();

signals:

public slots:

private:
    static bool verify(GameActionScript *act, QString *err = NULL);

private:
    QScriptValue m_function;
    QScriptValue m_object;

};

Q_DECLARE_METATYPE(GameActionScript*)

#endif // GAMEACTIONSCRIPT_H
