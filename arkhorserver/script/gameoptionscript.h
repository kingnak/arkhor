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

    bool isAvailableWithObject(QScriptValue obj) const;

signals:

public slots:

private:
    static bool verify(GameOptionScript *op, QString *msg = NULL);

private:
    QScriptValue m_this;
    QScriptValue m_isAvailable;
};

Q_DECLARE_METATYPE(GameOptionScript*)

class GameField;
class GameOptionFieldProxyScript : public GameOption
{
public:
    GameOptionFieldProxyScript(GameField *field, const QString &optionId);

    virtual bool execute();

    virtual bool resolveDependencies(GameRegistry *reg);
    virtual AH::Common::GameOptionData *data();

    virtual QString id() const;
    virtual QString name() const { return m_opt->name(); }
    virtual QString description() const { return m_opt->description(); }
    virtual AH::GamePhases phases() const { return m_opt->phases(); }
    virtual QString actionId() const { return m_opt->actionId(); }

    virtual AH::Common::Cost costs() const { return m_opt->costs(); }
    virtual bool canPay() const { return m_opt->canPay(); }
    virtual bool isAvailable() const;

    virtual AH::ChooseType chooseType() const { return m_opt->chooseType(); }
    virtual AH::ContinueType continueType() const { return m_opt->continueType(); }

    virtual GameAction *action() { return m_opt->action(); }
    virtual const GameAction *action() const { return m_opt->action(); }

    GameOption *realOption() { return m_opt; }

private:
    GameOption *m_opt;
    GameField *m_field;
    QString m_optId;
};

#endif // GAMEOPTIONSCRIPT_H
