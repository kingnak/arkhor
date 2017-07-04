#ifndef CHARACTERSCRIPT_H
#define CHARACTERSCRIPT_H

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>
#include "character.h"
#include "gamefieldscript.h"
#include "gameobjectscript.h"

class CharacterScript : public QObject, public Character
{
    Q_OBJECT
public:
    explicit CharacterScript(Investigator *i, QObject *parent = 0);

    static void castFromValue(const QScriptValue &v, CharacterScript *&o) { o = qobject_cast<CharacterScript *> (v.toQObject()); }
    static QScriptValue castToValue(QScriptEngine *eng, CharacterScript * const &in) { return eng->newQObject(in); }

    Q_PROPERTY(QObjectList inventory READ getInventoryScript)

    Q_INVOKABLE void addToInventory(GameObjectScript *o) { Character::addToInventory(o); }
    Q_INVOKABLE void removeFromInventory(GameObjectScript *o) { Character::removeFromInventory(o); }

    Q_INVOKABLE bool hasObject(QString typeId);

    Q_PROPERTY(int sanity READ curSanity)
    Q_INVOKABLE void restoreSanity() { Character::restoreSanity(); }
    Q_INVOKABLE void addSanity(int amount) { Character::addSanity(amount); }
    Q_INVOKABLE void damageSanity(int amount) { Character::damageSanity(amount); }
    //Q_INVOKABLE void preventDamageSanity(int amount) { Character::preventDamageSanity(amount); }

    Q_PROPERTY(int stamina READ curStamina)
    Q_INVOKABLE void restoreStamina() { Character::restoreStamina(); }
    Q_INVOKABLE void addStamina(int amount) { Character::addStamina(amount); }
    Q_INVOKABLE void damageStamina(int amount) { Character::damageStamina(amount); }
    //Q_INVOKABLE void preventDamageStamina(int amount) { Character::preventDamageStamina(amount); }

    Q_INVOKABLE bool commitDamage() { return Character::commitDamage(); }

    Q_INVOKABLE void addClue(int amount) { Character::addClue(amount); }
    Q_INVOKABLE void addMoney(int amount) { Character::addMoney(amount); }
    Q_INVOKABLE void loseMoney(int amount) { Character::loseMoney(amount); }
    Q_INVOKABLE void loseHalfMoney() { Character::loseMoney(money()/2); }

    Q_INVOKABLE int getSkillValue(AH::Skill skill);

    Q_PROPERTY(GameFieldScript* field READ fieldScript)
    GameFieldScript *fieldScript();
    Q_INVOKABLE bool returnToArkham() { return Character::returnToArkham(); }
    Q_INVOKABLE bool placeOnField(AH::Common::FieldData::FieldID fieldId);
    Q_INVOKABLE void addMovementPoint(int amount) { Character::addMovementPoint(amount); }

    Q_INVOKABLE bool canCloseGate();

    Q_INVOKABLE void lostInSpaceAndTime() { Character::lostInSpaceAndTime(); }
    Q_INVOKABLE void devour() { Character::devour(); }

    Q_INVOKABLE void arrest() { Character::arrest(); }
    Q_INVOKABLE void delay() { Character::setDelayed(true); }
    Q_INVOKABLE void setOut() { Character::setSetout(true); }

    Q_INVOKABLE bool canPay(AH::Common::Cost cost) { return Character::canPay(cost); }
    Q_INVOKABLE bool pay(AH::Common::Cost cost) { return Character::pay(cost); }

    Q_INVOKABLE void loseHalfPossessions() { return Character::loseHalfPossessions(); }
    Q_INVOKABLE void losePossessions(int count) { return Character::losePossessions(count); }

	Q_INVOKABLE int genericDieRollSum(QString desc, QString sourceId, int dieCount);
	Q_INVOKABLE bool genericDieRollTest(QString desc, QString sourceId, int dieCount, int target);
	Q_INVOKABLE int genericDieRollCount(QString desc, QString sourceId, int dieCount);
	Q_INVOKABLE bool skillTest(QString desc, QString sourceId, AH::Skill skill, int adjust, int target);
	Q_INVOKABLE int dieRollSkillCount(QString desc, QString sourceId, AH::Skill skill, int adjust);

signals:

public slots:

private:
    QObjectList getInventoryScript() const;

private:
    GameFieldScript *m_fieldBridge;
};

Q_DECLARE_METATYPE(CharacterScript*)
Q_DECLARE_METATYPE(QList<CharacterScript*>)

#endif // CHARACTERSCRIPT_H
