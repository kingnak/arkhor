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

    virtual bool isAvailable();

signals:

public slots:

private:
    static bool verify(GameOptionScript *op, QString *msg = NULL);

    static bool parseCosts(QScriptValue v, AH::Common::Cost &c);
    static bool parseCostList(QScriptValue v, AH::Common::CostList &cl);
    static bool parseCostItem(QScriptValue v, AH::Common::CostItem &ci);

private:
    QScriptValue m_object;
    QScriptValue m_isAvailable;
};

Q_DECLARE_METATYPE(GameOptionScript*)

#endif // GAMEOPTIONSCRIPT_H
