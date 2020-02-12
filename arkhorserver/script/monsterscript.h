#ifndef MONSTERSCRIPT_H
#define MONSTERSCRIPT_H

#include <QObject>
#include <QScriptValue>
#include <QScriptable>
#include <QScriptEngine>
#include <QReadWriteLock>
#include "monster.h"

class GameFieldScript;
class QScriptContext;

class MonsterScript : public QObject, public Monster
{
    Q_OBJECT
public:
    explicit MonsterScript(QObject *parent = 0);

    Monster *clone();

    Q_PROPERTY(QString id READ id)
    Q_PROPERTY(quint32 attributes READ attributes)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString type READ typeId)
    Q_PROPERTY(quint32 horrorDamage READ horrorDamage)
    Q_PROPERTY(quint32 combatDamage READ combatDamage)
    Q_PROPERTY(GameFieldScript *field READ fieldScript)

    GameFieldScript *fieldScript();

    Q_INVOKABLE void placeOnField(int fieldId);
    Q_INVOKABLE void returnToDeck() { Monster::returnToDeck(true); }

    static MonsterScript *createMonster(QScriptContext *ctx, QScriptEngine *eng);

    static void castFromValue(const QScriptValue &v, MonsterScript *&o) { o = qobject_cast<MonsterScript *> (v.toQObject()); }
    static QScriptValue castToValue(QScriptEngine *eng, MonsterScript * const &in) { return eng->newQObject(in); }


    MonsterAttributes attributes() const;
    void move(AH::MovementDirection dir);
    void defeat(Character *byCharacter);
    virtual bool hasCustomHorrorCheck() const;
    virtual bool horrorCheck(Character *c);
    virtual bool damage(Character *c, DamageType t);
    virtual void evaded(Character *c);
    virtual void flown(Character *c);

    virtual PropertyModificationList getModifications() const;

private:
    QScriptValue m_attrFunc;
    QScriptValue m_onDefeatFunc;
    QScriptValue m_specialMoveFunc;
    QScriptValue m_onDamageFunc;
    QScriptValue m_onEvadeFunc;
    QScriptValue m_onFleeFunc;
    QScriptValue m_modsFunc;
    QScriptValue m_onHorrorFunc;
    QScriptValue getThis();

private:
    static bool verify(MonsterScript *m, QString *msg = NULL);

    GameFieldScript *m_fieldBridge;

private:
    mutable MonsterAttributes m_oldDynAttrs;
    mutable PropertyModificationList m_oldDynMods;
    static QReadWriteLock s_attrFunctionLock;
    static QReadWriteLock s_modFunctionLock;
};

Q_DECLARE_METATYPE(MonsterScript*)

#endif // MONSTERSCRIPT_H
