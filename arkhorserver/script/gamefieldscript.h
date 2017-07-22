#ifndef GAMEFIELDSCRIPT_H
#define GAMEFIELDSCRIPT_H

#include <QObject>
#include "game/gamefield.h"
#include <QScriptValue>
#include <QScriptEngine>

class GameFieldScript : public QObject
{
    Q_OBJECT
public:
    explicit GameFieldScript(QObject *parent = 0);

    static void castFromValue(const QScriptValue &v, GameFieldScript *&o) { o = qobject_cast<GameFieldScript *> (v.toQObject()); }
    static QScriptValue castToValue(QScriptEngine *eng, GameFieldScript * const &in) { return eng->newQObject(in); }

    void setField(GameField *f) { m_field = f; }

    Q_INVOKABLE void lockUntilNextRound();
    Q_INVOKABLE bool hasMonsters() const { return m_field ? m_field->hasMonsters() : false; }

	Q_PROPERTY(QObjectList characters READ characters)
	QObjectList characters();

    Q_PROPERTY(quint32 type READ type)
    quint32 type() { return m_field ? m_field->type() : 0; }

private:
    GameField *m_field;
};

Q_DECLARE_METATYPE(GameFieldScript*)

#endif // GAMEFIELDSCRIPT_H
