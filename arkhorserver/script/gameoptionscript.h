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
    static GameOptionScript *createGameOption(const QScriptValue &data, QScriptContext *ctx, QScriptEngine *eng);

    bool isAvailable() const override;
    AH::Common::ModifiedPropertyValueData baseProperty() const override;

    bool isAvailableWithObject(const QScriptValue &obj) const;

signals:

public slots:

private:
    static bool verify(GameOptionScript *op, QString *msg = NULL);

private:
    QScriptValue m_this;
    QScriptValue m_isAvailable;
    AH::Common::PropertyValueData::Property m_basePropertyProp;
};

Q_DECLARE_METATYPE(GameOptionScript*)

class GameField;
class GameOptionFieldProxyScript : public GameOption
{
public:
    GameOptionFieldProxyScript(GameField *field, const QString &optionId);

    bool execute() override;

    bool resolveDependencies(GameRegistry *reg) override;
    AH::Common::GameOptionData *data() override;

    QString id() const override;
    QString name() const override { return m_opt->name(); }
    QString description() const override { return m_opt->description(); }
    AH::GamePhases phases() const override { return m_opt->phases(); }
    QString actionId() const override { return m_opt->actionId(); }
    QString sourceId() const override { return m_opt->sourceId(); }

    AH::Common::Cost costs() const override { return m_opt->costs(); }
    bool canPay() const override { return m_opt->canPay(); }
    bool isAvailable() const override;

    AH::ChooseType chooseType() const override { return m_opt->chooseType(); }
    AH::ContinueType continueType() const override { return m_opt->continueType(); }

    GameAction *action() override { return m_opt->action(); }
    const GameAction *action() const override { return m_opt->action(); }

    GameOption *realOption() { return m_opt; }

private:
    GameOption *m_opt;
    GameField *m_field;
    QString m_optId;
};

#endif // GAMEOPTIONSCRIPT_H
