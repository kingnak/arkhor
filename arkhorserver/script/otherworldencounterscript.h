#ifndef OTHERWORLDENCOUNTERSCRIPT_H
#define OTHERWORLDENCOUNTERSCRIPT_H

#include <QObject>
#include <QScriptEngine>
#include "game/otherworldencounter.h"

class GameRegistry;
class QScriptContext;

class OtherWorldEncounterScript : public QObject, public OtherWorldEncounter
{
    Q_OBJECT
public:
    explicit OtherWorldEncounterScript(QObject *parent = 0);

    static void castFromValue(const QScriptValue &v, OtherWorldEncounterScript *&o) { o = qobject_cast<OtherWorldEncounterScript *> (v.toQObject()); }
    static QScriptValue castToValue(QScriptEngine *eng, OtherWorldEncounterScript * const &in) { return eng->newQObject(in); }

    virtual QList<GameOption *> options() const;
    virtual bool resolveDependencies(GameRegistry *reg);

    static OtherWorldEncounterScript *createEncounter(QScriptContext *ctx, QScriptEngine *eng);

signals:

public slots:

private:
    static bool verify(OtherWorldEncounterScript *act, QString *err = NULL);

private:
    QMap<QString, GameOption *> m_opts;
};

Q_DECLARE_METATYPE(OtherWorldEncounterScript*)

#endif // OTHERWORLDENCOUNTERSCRIPT_H
