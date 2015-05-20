#ifndef ARKHAMENCOUNTERSCRIPT_H
#define ARKHAMENCOUNTERSCRIPT_H

#include <QObject>
#include <QScriptEngine>
#include "game/arkhamencounter.h"

class QScriptContext;

class ArkhamEncounterScript : public QObject, public ArkhamEncounter
{
    Q_OBJECT
public:
    explicit ArkhamEncounterScript(QObject *parent = 0);

    static void castFromValue(const QScriptValue &v, ArkhamEncounterScript *&o) { o = qobject_cast<ArkhamEncounterScript *> (v.toQObject()); }
    static QScriptValue castToValue(QScriptEngine *eng, ArkhamEncounterScript * const &in) { return eng->newQObject(in); }

    virtual QList<GameOption *> options() const;
    virtual bool resolveDependencies(const Game *game);

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
