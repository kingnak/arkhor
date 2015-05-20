#ifndef GAMEOPTIONSCRIPT_H
#define GAMEOPTIONSCRIPT_H

#include <QObject>
#include <QScriptEngine>
#include "game/gameoption.h"

class QScriptContext;

class GameOptionScript : public QObject, public GameOption
{
    Q_OBJECT
public:
    explicit GameOptionScript(QObject *parent = 0);

    static void castFromValue(const QScriptValue &v, GameOptionScript *&o) { o = qobject_cast<GameOptionScript *> (v.toQObject()); }
    static QScriptValue castToValue(QScriptEngine *eng, GameOptionScript * const &in) { return eng->newQObject(in); }

    Q_PROPERTY(QString id READ id)

    static GameOptionScript *createGameOption(QScriptContext *ctx, QScriptEngine *eng);
    static GameOptionScript *createGameOption(QScriptValue data, QScriptContext *ctx, QScriptEngine *eng);

    virtual bool isAvailable() const;

signals:

public slots:

private:
    static bool verify(GameOptionScript *op, QString *msg = NULL);

private:
    QScriptValue m_this;
    QScriptValue m_isAvailable;
};

Q_DECLARE_METATYPE(GameOptionScript*)

#endif // GAMEOPTIONSCRIPT_H
