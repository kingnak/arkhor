#ifndef CHARACTERSCRIPT_H
#define CHARACTERSCRIPT_H

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>
#include "character.h"

class CharacterScript : public QObject, public Character
{
    Q_OBJECT
public:
    explicit CharacterScript(Investigator *i, QObject *parent = 0);

    static void castFromValue(const QScriptValue &v, CharacterScript *&o) { o = qobject_cast<CharacterScript *> (v.toQObject()); }
    static QScriptValue castToValue(QScriptEngine *eng, CharacterScript * const &in) { return eng->newQObject(in); }

    Q_INVOKABLE void restoreSanity() { Character::restoreSanity(); }
    Q_INVOKABLE void addSanity(int amount) { Character::addSanity(amount); }
    Q_INVOKABLE void damageSanity(int amount) { Character::damageSanity(amount); }

    Q_INVOKABLE void restoreStamina() { Character::restoreStamina(); }
    Q_INVOKABLE void addStamina(int amount) { Character::addStamina(amount); }
    Q_INVOKABLE void damageStamina(int amount) { Character::damageStamina(amount); }

    Q_INVOKABLE bool commitDamage() { return Character::commitDamage(); }

    Q_INVOKABLE void addClue(int amount) { Character::addClue(amount); }
    Q_INVOKABLE void addMoney(int amount) { Character::addMoney(amount); }

    //Q_INVOKABLE bool canPay(AH::Common::Cost cost);
    //Q_INVOKABLE bool pay(AH::Common::Cost cost);

signals:

public slots:

};

Q_DECLARE_METATYPE(CharacterScript*)

#endif // CHARACTERSCRIPT_H
