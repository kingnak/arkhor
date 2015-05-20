#ifndef MONSTERSCRIPT_H
#define MONSTERSCRIPT_H

#include <QObject>
#include <QScriptValue>
#include <QScriptable>
#include <QScriptEngine>
#include "monster.h"

class QScriptContext;

class MonsterScript : public QObject, public Monster, public QScriptable
{
    Q_OBJECT
public:
    explicit MonsterScript(QObject *parent = 0);

    Monster *clone();

    static MonsterScript *createMonster(QScriptContext *ctx, QScriptEngine *eng);

    static void castFromValue(const QScriptValue &v, MonsterScript *&o) { o = qobject_cast<MonsterScript *> (v.toQObject()); }
    static QScriptValue castToValue(QScriptEngine *eng, MonsterScript * const &in) { return eng->newQObject(in); }


    MonsterAttributes attributes();
    void move();
    void defeat(Character *byCharacter);

private:
    QScriptValue m_attrFunc;
    QScriptValue m_onDefeatFunc;
    QScriptValue m_specialMoveFunc;
    QScriptValue getThis();

private:
    static bool verify(MonsterScript *m, QString *msg = NULL);
};

Q_DECLARE_METATYPE(MonsterScript*)

#endif // MONSTERSCRIPT_H
