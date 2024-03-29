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
class MonsterScript;
class AncientOneScript;

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
    Q_PROPERTY(MonsterScript* monster READ curMonster)
    Q_PROPERTY(AncientOneScript *ancientOne READ curAncientOne)

    CharacterScript *curCharacter();
    QObjectList getAllCharacters();
    GateScript *curGate();
    int getTerrorLevel();
    MythosCardScript *activeRumor();
    MonsterScript *curMonster();
    AncientOneScript *curAncientOne();

    Q_INVOKABLE int genericDieRollSum(const QString &desc, const QString &sourceId, int dieCount);
    Q_INVOKABLE bool genericDieRollTest(const QString &desc, const QString &sourceId, int dieCount, int target);
    Q_INVOKABLE int genericDieRollCount(const QString &desc, const QString &sourceId, int dieCount);
    Q_INVOKABLE int genericDieRollCountSuccess(const QString &desc, const QString &sourceId, int dieCount, const QScriptValue &successes);
    Q_INVOKABLE bool skillTest(const QString &desc, const QString &sourceId, AH::Skill skill, int adjust, int target);
    Q_INVOKABLE int dieRollSkillCount(const QString &desc, const QString &sourceId, AH::Skill skill, int adjust);

    Q_INVOKABLE void increaseTerrorLevel(int amount = 1);
    Q_INVOKABLE void decreaseTerrorLevel(int amount = 1) { increaseTerrorLevel(-amount); }

    Q_INVOKABLE QString selectChoice(const QString &desc, const QString &sourceId, const QList<AH::Common::ChoiceData::OptionData> &options);
    Q_INVOKABLE QString selectChoice(const QString &desc, const QString &sourceId, const QList<AH::Common::ChoiceData::OptionData> &options, bool canCancel);

    Q_INVOKABLE bool spontaneousMonsterFight();
    Q_INVOKABLE bool createGateAtCharacterField();

    Q_INVOKABLE QScriptValue allFields(int type);

    Q_INVOKABLE GameObjectScript *drawObject(AH::GameObjectType type);
    Q_INVOKABLE GameObjectScript *drawObject(AH::GameObjectType type, const QString &desc, const QString &sourceId = QString::null);

    Q_INVOKABLE QList<GameObjectScript *> selectObjects(const QList<GameObjectScript *> &objects, const QString &desc, const QString &sourceId, int min, int max);
    Q_INVOKABLE QList<GameObjectScript *> drawMultipleObjects(AH::GameObjectType type, const QString &desc, const QString &sourceId, int count, int min, int max);
    Q_INVOKABLE QList<GameObjectScript *> drawMixedObjects(const QString &desc, const QString &sourceId, const QList<AH::ObjectTypeCount> &types, int min, int max);

signals:

public slots:

};

Q_DECLARE_METATYPE(GameContextScript*)

#endif // GAMECONTEXTSCRIPT_H
