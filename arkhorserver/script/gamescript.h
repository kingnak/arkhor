#ifndef GAMESCRIPT_H
#define GAMESCRIPT_H

#include <QObject>
#include <QMap>
#include <QScriptValue>
#include <QDir>
#include <QScriptable>
#include <QMetaType>
#include "gamecontextscript.h"
#include <cost.h>
#include <fielddata.h>

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
class AncientOneScript;

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

    Q_INVOKABLE GameObjectScript *drawSingleObject(qint32 type);
    Q_INVOKABLE GameObjectScript *drawSpecificObject(QString id);

    /** fieldId must be a AH::Common::FieldData::FieldID */
    Q_INVOKABLE bool returnMonstersFromField(quint32 fieldId);
    /** type must be a AH::Common::FieldData::FieldType */
    Q_INVOKABLE bool returnMonstersFromFieldType(quint32 type);
    /** fieldIds must be a list of AH::Common::FieldData::FieldID */
    Q_INVOKABLE bool returnMonstersFromFields(QList<quint32> fieldIds);
    Q_INVOKABLE bool returnMonsterTypeFromBoard(QString typeId);

    /** fId must be a AH::Common::FieldData::FieldID */
    Q_INVOKABLE bool spawnMonster(quint32 fId);

    Q_INVOKABLE int cardsOnDeck(qint32 type);

    Q_INVOKABLE void createGate(qint32 fld);


    ////////// SETUP
    Q_INVOKABLE bool registerConstant(QString scope, QString name, QString value);

    Q_INVOKABLE QScriptValue registerInvestigator(InvestigatorScript *i);
    Q_INVOKABLE QScriptValue createInvestigator();

    Q_INVOKABLE QScriptValue registerAction(GameActionScript *a);
    Q_INVOKABLE GameActionScript *createAction();

    Q_INVOKABLE QScriptValue registerOption(GameOptionScript *o);
    Q_INVOKABLE GameOptionScript *createOption();

    Q_INVOKABLE QScriptValue quickOption();
    Q_INVOKABLE QScriptValue getDieRollOption();

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

    Q_INVOKABLE AncientOneScript *createAncientOne();
    Q_INVOKABLE QScriptValue registerAncientOne(AncientOneScript *a);

    ///////// HELPER
    static QStringList array2stringlist(QScriptValue ar);
    static QScriptValueList array2list(QScriptValue ar);

    static bool parseCosts(QScriptValue v, AH::Common::Cost &c);
    static bool parseCostList(QScriptValue v, AH::Common::CostList &cl);
    static bool parseCostItem(QScriptValue v, AH::Common::CostItem &ci);

    static bool parseObjectTypeCount(QScriptValue v, QList<AH::ObjectTypeCount> &o);

    static bool parseOptionChoiceData(QScriptValue v, AH::Common::ChoiceData::OptionData &o);

    template<typename T>
    static T parseFlags(QScriptValue v, T defVal);

    //////// CASTER
    static QScriptValue castCostToValue(QScriptEngine *eng, AH::Common::Cost const &in);
    static void castCostFromValue(const QScriptValue &v, AH::Common::Cost &o);

    static QScriptValue castObjTypeCountToValue(QScriptEngine *eng, AH::ObjectTypeCount const &in);
    static void castObjTypeCountFromValue(const QScriptValue &v, AH::ObjectTypeCount &o);

    static QScriptValue castChoiceOptionToValue(QScriptEngine *eng, AH::Common::ChoiceData::OptionData const &in);
    static void castChoiceOptionFromValue(const QScriptValue &v, AH::Common::ChoiceData::OptionData &o);

    template<typename T>
    static QScriptValue castListToValue(QScriptEngine *eng, QList<T> const &in);
    template<typename T>
    static void castListFromValue(const QScriptValue &v, QList<T> &o);

signals:

public slots:

private:
    QScriptValue initConstants();
    void initGlobalConstants(QScriptValue &consts);
    void initFieldConstants(QScriptValue &consts);
    void initGlobalMethods();

    // Quick access
    static QScriptValue quick_CurChar(QScriptContext *, QScriptEngine *);

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


template<typename T>
QScriptValue GameScript::castListToValue(QScriptEngine *eng, QList<T> const &in)
{
    QScriptValue arr = eng->newArray(in.length());
    for (int i = 0; i < in.length(); ++i) {
        arr.setProperty(i, eng->toScriptValue(in[i]));
    }
    return arr;
}

template<typename T>
void GameScript::castListFromValue(const QScriptValue &v, QList<T> &o)
{
    o.clear();
    if (v.isArray()) {
        int len = v.property("length").toInt32();
        for (int i = 0; i < len; ++i) {
            QScriptValue elem = v.property(i);
            o << qscriptvalue_cast<T> (elem);
        }
    } else {
        o << qscriptvalue_cast<T> (v);
    }
}


#define gGameScript (GameScript::instance())

Q_DECLARE_METATYPE(GameScript*)
Q_DECLARE_METATYPE(AH::Common::Cost)
Q_DECLARE_METATYPE(AH::ObjectTypeCount)
Q_DECLARE_METATYPE(QList<AH::ObjectTypeCount>)
Q_DECLARE_METATYPE(AH::Common::ChoiceData::OptionData)
Q_DECLARE_METATYPE(QList<AH::Common::ChoiceData::OptionData>)

#endif // GAMESCRIPT_H
