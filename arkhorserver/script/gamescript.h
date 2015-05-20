#ifndef GAMESCRIPT_H
#define GAMESCRIPT_H

#include <QObject>
#include <QMap>
#include <QScriptValue>
#include <QDir>
#include <QScriptable>
#include <QMetaType>
#include "gamecontextscript.h"

class QScriptEngine;
class QScriptContext;
class Game;
class InvestigatorScript;
class GameActionScript;
class GameOptionScript;
class GameObjectScript;
class ArkhamEncounterScript;
class OtherWorldEncounterScript;
class MonsterScript;
class MythosCardScript;

class QScriptEngineDebugger;

class GameScript : public QObject, public QScriptable
{
    Q_OBJECT
public:
    explicit GameScript(Game *game, QObject *parent = 0);

    static GameScript *instance() { return s_instance; }

    bool init(const QString &scriptBaseDir);

    QScriptEngine *engine() { return m_engine; }

    Q_PROPERTY(GameContextScript* context READ getGameContext SCRIPTABLE true)
    GameContextScript *getGameContext();

    Q_INVOKABLE GameObjectScript *drawObject(qint32 type);
    Q_INVOKABLE GameObjectScript *drawSpecificObject(QString id);

    Q_INVOKABLE void createGate(qint32 fld);


    ////////// SETUP
    Q_INVOKABLE void registerInvestigator(InvestigatorScript *i);
    Q_INVOKABLE QScriptValue createInvestigator();

    Q_INVOKABLE QScriptValue registerAction(GameActionScript *a);
    Q_INVOKABLE GameActionScript *createAction();

    Q_INVOKABLE QScriptValue registerOption(GameOptionScript *o);
    Q_INVOKABLE GameOptionScript *createOption();


    Q_INVOKABLE QScriptValue quickOption();

    Q_INVOKABLE QScriptValue registerSingleObject(GameObjectScript *o);
    Q_INVOKABLE QScriptValue registerObject(GameObjectScript *o);
    Q_INVOKABLE QScriptValue registerMultiObject(quint32 count, GameObjectScript *o);
    Q_INVOKABLE GameObjectScript *createObject();

    Q_INVOKABLE QScriptValue addFieldOption(int fieldId, QString optionId);

    Q_INVOKABLE ArkhamEncounterScript *createArkhamEncounter();
    Q_INVOKABLE QScriptValue registerArkhamEncounter(ArkhamEncounterScript *e);

    Q_INVOKABLE OtherWorldEncounterScript *createOtherWorldEncounter();
    Q_INVOKABLE QScriptValue registerOtherWorldEncounter(OtherWorldEncounterScript *e);

    Q_INVOKABLE MonsterScript *createMonster();
    Q_INVOKABLE QScriptValue registerMonster(quint32 count, MonsterScript *m);

    Q_INVOKABLE MythosCardScript *createMythosCard();
    Q_INVOKABLE QScriptValue registerMythosCard(MythosCardScript *m);

    static QStringList array2stringlist(QScriptValue ar);
    static QScriptValueList array2list(QScriptValue ar);

    template<typename T>
    static T parseFlags(QScriptValue v, T defVal);

signals:

public slots:

private:
    QScriptValue initConstants();
    void initGlobalConstants(QScriptValue &consts);
    void initFieldConstants(QScriptValue &consts);

    bool parseScripts(QDir base);

private:
    QScriptEngine *m_engine;

    Game *m_game;
    GameContextScript *m_ctx;

#ifdef DEBUG_SCRIPT_BUILD
    QScriptEngineDebugger *m_debugger;
#endif

    static GameScript *s_instance;
};



template<typename T>
T GameScript::parseFlags(QScriptValue v, T defVal)
{
    if (v.isArray()) {
        QScriptValueList lst = GameScript::array2list(v);
        T coll = static_cast<T> (0);
        foreach (QScriptValue e, lst) {
            coll |= static_cast<T> (e.toUInt32());
        }
        return coll;
    } else if (v.isValid() && !v.isUndefined()) {
        return static_cast<T> (v.toUInt32());
    } else {
        return defVal;
    }
}


#define gGameScript (GameScript::instance())

Q_DECLARE_METATYPE(GameScript*)

#endif // GAMESCRIPT_H
