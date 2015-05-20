#ifndef GAMECONTEXTSCRIPT_H
#define GAMECONTEXTSCRIPT_H

#include <QObject>
#include <QMetaType>
#include <QScriptEngine>
#include <ahglobal.h>
#include <choicedata.h>

class CharacterScript;
class GateScript;
class GameObjectScript;
class MythosCardScript;

class GameContextScript : public QObject
{
    Q_OBJECT
public:
    explicit GameContextScript(QObject *parent = 0);

    static void castFromValue(const QScriptValue &v, GameContextScript *&o) { o = qobject_cast<GameContextScript *> (v.toQObject()); }
    static QScriptValue castToValue(QScriptEngine *eng, GameContextScript * const &in) { return eng->newQObject(in); }

    Q_PROPERTY(CharacterScript* character READ curCharacter)
    Q_PROPERTY(QObjectList allCharacters READ getAllCharacters)
    Q_PROPERTY(GateScript* gate READ curGate)
    Q_PROPERTY(int terrorLevel READ getTerrorLevel)
    Q_PROPERTY(MythosCardScript* rumor READ activeRumor)

    CharacterScript *curCharacter();
    QObjectList getAllCharacters();
    GateScript *curGate();
    int getTerrorLevel();
    MythosCardScript *activeRumor();

    Q_INVOKABLE int genericDieRollSum(QString desc, int dieCount);
    Q_INVOKABLE bool genericDieRollTest(QString desc, int dieCount, int target);
    Q_INVOKABLE int genericDieRollCount(QString desc, int dieCount);
    Q_INVOKABLE bool skillTest(QString desc, int skill, int adjust, int target);
    Q_INVOKABLE int dieRollSkillCount(QString desc, int skill, int adjust);

    Q_INVOKABLE QString selectChoice(QString desc, QList<AH::Common::ChoiceData::OptionData> options);

    Q_INVOKABLE bool spontaneousMonsterFight();

    Q_INVOKABLE GameObjectScript *drawObject(qint32 type);
    Q_INVOKABLE GameObjectScript *drawObject(qint32 type, QString desc);

    Q_INVOKABLE QList<GameObjectScript *> drawMultipleObjects(qint32 type, QString desc, int count, int min, int max);
    Q_INVOKABLE QList<GameObjectScript *> drawMixedObjects(QString desc, QList<AH::ObjectTypeCount> types, int min, int max);

signals:

public slots:

};

Q_DECLARE_METATYPE(GameContextScript*)

#endif // GAMECONTEXTSCRIPT_H
