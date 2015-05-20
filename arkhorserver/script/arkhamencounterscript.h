#ifndef ARKHAMENCOUNTERSCRIPT_H
#define ARKHAMENCOUNTERSCRIPT_H

#include <QObject>
#include <QScriptEngine>
#include "game/arkhamencounter.h"

class GameRegistry;
class QScriptContext;

class ArkhamEncounterScript : public QObject, public ArkhamEncounter
{
    Q_OBJECT
public:
    explicit ArkhamEncounterScript(QObject *parent = 0);

#ifdef DEBUG_SCRIPT_BUILD
    Q_PROPERTY(quint32 fieldId READ fieldId)
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(QList<AH::Common::GameOptionData> optionData READ optionData)
    Q_PROPERTY(QStringList optionIds READ optionIds)

    QStringList optionIds() const { return m_optionIds; }
#endif

    static void castFromValue(const QScriptValue &v, ArkhamEncounterScript *&o) { o = qobject_cast<ArkhamEncounterScript *> (v.toQObject()); }
    static QScriptValue castToValue(QScriptEngine *eng, ArkhamEncounterScript * const &in) { return eng->newQObject(in); }

    virtual QList<GameOption *> options() const;
    virtual bool resolveDependencies(GameRegistry *reg);

    static ArkhamEncounterScript *createEncounter(QScriptContext *ctx, QScriptEngine *eng);

signals:

public slots:

private:
    static bool verify(ArkhamEncounterScript *act, QString *err = NULL);

private:
    QMap<QString, GameOption *> m_opts;
};

Q_DECLARE_METATYPE(ArkhamEncounterScript*)

#endif // ARKHAMENCOUNTERSCRIPT_H
