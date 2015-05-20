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

    Q_INVOKABLE void restoreSanity() { Character::addSanity(1000000); }
    Q_INVOKABLE void addSanity(int amount) { Character::addSanity(amount); }

signals:

public slots:

};

Q_DECLARE_METATYPE(CharacterScript*)

#endif // CHARACTERSCRIPT_H
