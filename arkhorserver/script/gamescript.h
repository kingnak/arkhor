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
class GameField;
class GameFieldScript;

class QScriptEngineDebugger;

class GameScript : public QObject, public QScriptable
{
    Q_OBJECT
public:
    explicit GameScript(Game *game, QObject *parent = 0);

    static GameScript *instance() { return s_instance; }

    bool init(const QString &scriptBaseDir);

    QScriptValue getCharacter(CharacterScript *c);

    QScriptEngine *engine() { return m_engine; }

    Q_PROPERTY(GameContextScript* context READ getGameContext SCRIPTABLE true)
    GameContextScript *getGameContext();

    Q_INVOKABLE GameObjectScript *drawSingleObject(AH::GameObjectType type);
    Q_INVOKABLE GameObjectScript *drawSpecificObject(QString id);

    Q_INVOKABLE bool returnMonstersFromField(AH::Common::FieldData::FieldID fieldId);
    Q_INVOKABLE bool returnMonstersFromFieldType(AH::Common::FieldData::FieldType type);
    Q_INVOKABLE bool returnMonstersFromFields(QList<AH::Common::FieldData::FieldID> fieldIds);
    Q_INVOKABLE bool returnMonsterTypeFromBoard(QString typeId);

    Q_INVOKABLE bool spawnMonster(AH::Common::FieldData::FieldID fieldId);

    Q_INVOKABLE int cardsOnDeck(AH::GameObjectType type);

    Q_INVOKABLE void createGate(AH::Common::FieldData::FieldID fieldId);

    Q_INVOKABLE void lockFieldUntilNextRound(AH::Common::FieldData::FieldID fieldId);

    Q_INVOKABLE void awakeAncientOne();

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

    Q_INVOKABLE QScriptValue getPreventDamageOption();

    Q_INVOKABLE QScriptValue registerSingleObject(GameObjectScript *o);
    Q_INVOKABLE QScriptValue registerObject(GameObjectScript *o);
    Q_INVOKABLE QScriptValue registerMultiObject(qint32 count, GameObjectScript *o);
    Q_INVOKABLE GameObjectScript *createObject();

    Q_INVOKABLE QScriptValue addFieldOption(AH::Common::FieldData::FieldID fieldId, QString optionId);

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
    template<typename T>
    static QList<T> array2TypedList(QScriptValue ar);
    template<typename T>
    static QScriptValue makeArray(const QList<T> lst);

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
    static QScriptValue castEnumToValue(QScriptEngine *eng, T const &in);
    template<typename T>
    static void castEnumFromValue(const QScriptValue &v, T &o);

    template<typename T>
    static QScriptValue castListToValue(QScriptEngine *eng, QList<T> const &in);
    template<typename T>
    static void castListFromValue(const QScriptValue &v, QList<T> &o);

    enum FunctionType {
        F_Action, F_Available, F_AncientOne,
        F_Display, F_Modification, F_Inventory,
        F_Monster, F_Rumor, F_Prevent, F_Mythos,
        F_Character
    };

    // Central method caller
    QScriptValue call(FunctionType t, QScriptValue f, QScriptValue obj, QScriptValue arg);
    QScriptValue call(FunctionType t, QScriptValue f, QScriptValue obj = QScriptValue(), QScriptValueList args = QScriptValueList());

    // Helper for TMP id object
    QScriptValue getTempObject();

    GameFieldScript *getField(GameField *field);

    bool isGameThread() const;

signals:

public slots:

private:
    QScriptValue initConstants();
    void initGlobalConstants(QScriptValue &consts);
    void initFieldConstants(QScriptValue &consts);
    void initGlobalMethods();

    // Quick access
    static QScriptValue quick_CurChar(QScriptContext *, QScriptEngine *);

    bool parseScripts(QString base);
    bool parseScriptsDir(QDir base);
    bool parseScriptsZip(QString file);
    bool parseScriptFile(QIODevice *d, const QString &fn, const QString &outScriptPath = QString::null);
    bool parseScriptAHSFile(QIODevice *d, const QString &fn, const QString &outScriptPath = QString::null);
    bool parseScriptJSFile(QIODevice *d, const QString &fn);

private:
    QScriptEngine *m_engine;

    Game *m_game;
    GameContextScript *m_ctx;

    QMap<AH::Common::FieldData::FieldID, GameFieldScript *> m_fieldCache;

#ifdef DEBUG_SCRIPT_BUILD
    QScriptEngineDebugger *m_debugger;
#endif

    static GameScript *s_instance;
};


template<typename T>
QList<T> GameScript::array2TypedList(QScriptValue ar)
{
    QStringList lst = array2stringlist(ar);
    QList<T> ret;
    for (auto s : lst) {
        ret << static_cast<T> (s.toInt());
    }
    return ret;
}

template<typename T>
QScriptValue GameScript::makeArray(const QList<T> lst)
{
    QScriptValue arr = s_instance->m_engine->newArray(lst.length());
    for (int i = 0; i < lst.length(); ++i) {
        arr.setProperty(i, s_instance->m_engine->toScriptValue(lst[i]));
    }
    return arr;
}

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
QScriptValue GameScript::castEnumToValue(QScriptEngine *eng, T const &in)
{
    Q_UNUSED(eng);
    return QScriptValue(static_cast<qint32>(in));
}

template<typename T>
void GameScript::castEnumFromValue(const QScriptValue &v, T &o)
{
    qint32 vvv = qscriptvalue_cast<qint32> (v);
    o = static_cast<T> (vvv);
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
