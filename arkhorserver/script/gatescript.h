#ifndef GATESCRIPT_H
#define GATESCRIPT_H

#include <QObject>
#include "gate.h"
#include "characterscript.h"

class GateScript : public QObject
{
    Q_OBJECT
public:
    explicit GateScript(Gate *g, QObject *parent = 0);

    static void castFromValue(const QScriptValue &v, GateScript *&o) { o = qobject_cast<GateScript *> (v.toQObject()); }
    static QScriptValue castToValue(QScriptEngine *eng, GateScript * const &in) { return eng->newQObject(in, QScriptEngine::ScriptOwnership); }

    Q_INVOKABLE bool seal(CharacterScript *byChar);
    Q_INVOKABLE bool close(CharacterScript *byChar);

signals:

public slots:

private:
    Gate *m_gate;
};

Q_DECLARE_METATYPE(GateScript *)

#endif // GATESCRIPT_H
