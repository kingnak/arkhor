#ifndef GAMECONTEXTSCRIPT_H
#define GAMECONTEXTSCRIPT_H

#include <QObject>
#include <QMetaType>
#include <QScriptEngine>
#include <ahglobal.h>

class CharacterScript;

class GameContextScript : public QObject
{
    Q_OBJECT
public:
    explicit GameContextScript(QObject *parent = 0);

    static void castFromValue(const QScriptValue &v, GameContextScript *&o) { o = qobject_cast<GameContextScript *> (v.toQObject()); }
    static QScriptValue castToValue(QScriptEngine *eng, GameContextScript * const &in) { return eng->newQObject(in); }

    Q_PROPERTY(CharacterScript* character READ curCharacter)

    CharacterScript *curCharacter();

    Q_INVOKABLE int genericDieRollSum(int dieCount, QString desc);
    Q_INVOKABLE bool skillTest(QString desc, int skill, int adjust, int target);
    Q_INVOKABLE int dieRollSkillCount(QString desc, int skill, int adjust);

signals:

public slots:

};

Q_DECLARE_METATYPE(GameContextScript*)

#endif // GAMECONTEXTSCRIPT_H
