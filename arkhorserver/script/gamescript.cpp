#include "gamescript.h"
#include <qscriptengine.h>
#include <QFileInfoList>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "game/gameboard.h"
#include "game/game.h"
#include <propertyvaluedata.h>
#include "investigatorscript.h"
#include "gameactionscript.h"
#include "gameoptionscript.h"
#include "gameobjectscript.h"
#include "characterscript.h"
#include "arkhamencounterscript.h"
#include "monsterscript.h"
#include "gamecontextscript.h"

GameScript::GameScript(Game *game, QObject *parent) :
    QObject(parent), m_game(game)
{
    m_engine = new QScriptEngine(this);
    m_ctx = new GameContextScript(this);
}

bool GameScript::init(const QString &scriptBaseDir)
{
    qScriptRegisterMetaType<GameActionScript*>(m_engine, GameActionScript::castToValue, GameActionScript::castFromValue);
    qScriptRegisterMetaType<GameOptionScript*>(m_engine, GameOptionScript::castToValue, GameOptionScript::castFromValue);
    qScriptRegisterMetaType<GameObjectScript*>(m_engine, GameObjectScript::castToValue, GameObjectScript::castFromValue);
    qScriptRegisterMetaType<ArkhamEncounterScript*>(m_engine, ArkhamEncounterScript::castToValue, ArkhamEncounterScript::castFromValue);
    qScriptRegisterMetaType<CharacterScript*>(m_engine, CharacterScript::castToValue, CharacterScript::castFromValue);
    qScriptRegisterMetaType<MonsterScript*>(m_engine, MonsterScript::castToValue, MonsterScript::castFromValue);

    qScriptRegisterMetaType<GameContextScript*>(m_engine, GameContextScript::castToValue, GameContextScript::castFromValue);

    qRegisterMetaType<GameContextScript*>();
    qRegisterMetaType<CharacterScript*>();

    // Register global objects:
    // Main Game Script
    QScriptValue obj = m_engine->newQObject(this);
    m_engine->globalObject().setProperty("game", obj);

    // Constants:
    QScriptValue c = initConstants();
    m_engine->globalObject().setProperty("Constants", c);

    bool ok = parseScripts(scriptBaseDir);
    if (!ok) return false;
    return m_game->resolveDependencies();
}

GameContextScript *GameScript::getGameContext()
{
    return m_ctx;
}

QScriptValue GameScript::initConstants()
{
    QScriptValue consts = m_engine->newObject();
    initGlobalConstants(consts);
    initFieldConstants(consts);
    return consts;
}

void GameScript::initGlobalConstants(QScriptValue &consts)
{
    // ObjectType
    QScriptValue ot = m_engine->newObject();
    ot.setProperty("CommonItem", AH::Obj_CommonItem, QScriptValue::ReadOnly);
    ot.setProperty("UniqueItem", AH::Obj_UniqueItem, QScriptValue::ReadOnly);
    ot.setProperty("Spell", AH::Obj_Spell, QScriptValue::ReadOnly);
    ot.setProperty("Skill", AH::Obj_Skill, QScriptValue::ReadOnly);
    ot.setProperty("Ally", AH::Obj_Ally, QScriptValue::ReadOnly);
    ot.setProperty("Blessing_Curse", AH::Obj_Blessing_Curse, QScriptValue::ReadOnly);
    ot.setProperty("Special", AH::Obj_Special, QScriptValue::ReadOnly);
    consts.setProperty("ObjectType", ot, QScriptValue::ReadOnly);

    // GamePhase
    QScriptValue gp = m_engine->newObject();
    gp.setProperty("Upkeep", AH::Upkeep, QScriptValue::ReadOnly);
    gp.setProperty("Movement", AH::Movement, QScriptValue::ReadOnly);
    gp.setProperty("ArkhamEncountery", AH::ArkhamEncountery, QScriptValue::ReadOnly);
    gp.setProperty("OtherWorldEncountery", AH::OtherWorldEncountery, QScriptValue::ReadOnly);
    gp.setProperty("Mythos", AH::Mythos, QScriptValue::ReadOnly);
    gp.setProperty("AllPhases", AH::AllPhases, QScriptValue::ReadOnly);
    gp.setProperty("DieRollPhase", AH::DieRoll, QScriptValue::ReadOnly);
    gp.setProperty("FightPhase", AH::FightPhase, QScriptValue::ReadOnly);
    consts.setProperty("GamePhases", gp, QScriptValue::ReadOnly);

    // Payment Items
    QScriptValue pay = m_engine->newObject();
    pay.setProperty("None", AH::Common::CostItem::Pay_None, QScriptValue::ReadOnly);
    pay.setProperty("Money", AH::Common::CostItem::Pay_Money, QScriptValue::ReadOnly);
    pay.setProperty("Clue", AH::Common::CostItem::Pay_Clue, QScriptValue::ReadOnly);
    pay.setProperty("Stamina", AH::Common::CostItem::Pay_Stamina, QScriptValue::ReadOnly);
    pay.setProperty("Sanity", AH::Common::CostItem::Pay_Sanity, QScriptValue::ReadOnly);
    pay.setProperty("Movement", AH::Common::CostItem::Pay_Movement, QScriptValue::ReadOnly);
    pay.setProperty("GateTrophy", AH::Common::CostItem::Pay_GateTrophy, QScriptValue::ReadOnly);
    pay.setProperty("MonsterTrophy", AH::Common::CostItem::Pay_MonsterTrophy, QScriptValue::ReadOnly);
    pay.setProperty("MonsterToughness", AH::Common::CostItem::Pay_MonsterToughness, QScriptValue::ReadOnly);
    consts.setProperty("Costs", pay, QScriptValue::ReadOnly);

    // Property Modifications
    QScriptValue mods = m_engine->newObject();
    mods.setProperty("NoProperty", AH::Common::PropertyValueData::NoProperty, QScriptValue::ReadOnly);
    mods.setProperty("Attr_Speed", AH::Common::PropertyValueData::Attr_Speed, QScriptValue::ReadOnly);
    mods.setProperty("Attr_Sneak", AH::Common::PropertyValueData::Attr_Sneak, QScriptValue::ReadOnly);
    mods.setProperty("Attr_Fight", AH::Common::PropertyValueData::Attr_Fight, QScriptValue::ReadOnly);
    mods.setProperty("Attr_Will", AH::Common::PropertyValueData::Attr_Will, QScriptValue::ReadOnly);
    mods.setProperty("Attr_Lore", AH::Common::PropertyValueData::Attr_Lore, QScriptValue::ReadOnly);
    mods.setProperty("Attr_Luck", AH::Common::PropertyValueData::Attr_Luck, QScriptValue::ReadOnly);
    mods.setProperty("Skill_Speed", AH::Common::PropertyValueData::Skill_Speed, QScriptValue::ReadOnly);
    mods.setProperty("Skill_Sneak", AH::Common::PropertyValueData::Skill_Sneak, QScriptValue::ReadOnly);
    mods.setProperty("Skill_Fight", AH::Common::PropertyValueData::Skill_Fight, QScriptValue::ReadOnly);
    mods.setProperty("Skill_Will", AH::Common::PropertyValueData::Skill_Will, QScriptValue::ReadOnly);
    mods.setProperty("Skill_Lore", AH::Common::PropertyValueData::Skill_Lore, QScriptValue::ReadOnly);
    mods.setProperty("Skill_Luck", AH::Common::PropertyValueData::Skill_Luck, QScriptValue::ReadOnly);
    mods.setProperty("Skill_Evade", AH::Common::PropertyValueData::Skill_Evade, QScriptValue::ReadOnly);
    mods.setProperty("Skill_Combat", AH::Common::PropertyValueData::Skill_Combat, QScriptValue::ReadOnly);
    mods.setProperty("Skill_Horror", AH::Common::PropertyValueData::Skill_Horror, QScriptValue::ReadOnly);
    mods.setProperty("Skill_Spell", AH::Common::PropertyValueData::Skill_Spell, QScriptValue::ReadOnly);
    mods.setProperty("Prop_MaxStamina", AH::Common::PropertyValueData::Prop_MaxStamina, QScriptValue::ReadOnly);
    mods.setProperty("Prop_MaxSanity", AH::Common::PropertyValueData::Prop_MaxSanity, QScriptValue::ReadOnly);
    mods.setProperty("Prop_Focus", AH::Common::PropertyValueData::Prop_Focus, QScriptValue::ReadOnly);
    mods.setProperty("Prop_Movement", AH::Common::PropertyValueData::Prop_Movement, QScriptValue::ReadOnly);
    mods.setProperty("Game_SealClueCost", AH::Common::PropertyValueData::Game_SealClueCost, QScriptValue::ReadOnly);
    mods.setProperty("Damage_General", AH::Common::PropertyValueData::Damage_General, QScriptValue::ReadOnly);
    mods.setProperty("Damage_Magical", AH::Common::PropertyValueData::Damage_Magical, QScriptValue::ReadOnly);
    mods.setProperty("Damage_Physical", AH::Common::PropertyValueData::Damage_Physical, QScriptValue::ReadOnly);
    consts.setProperty("Mods", mods, QScriptValue::ReadOnly);

    // Dimensions
    QScriptValue dims = m_engine->newObject();
    dims.setProperty("Square", AH::Dim_Square, QScriptValue::ReadOnly);
    dims.setProperty("Circle", AH::Dim_Circle, QScriptValue::ReadOnly);
    dims.setProperty("Moon", AH::Dim_Moon, QScriptValue::ReadOnly);
    dims.setProperty("Slash", AH::Dim_Slash, QScriptValue::ReadOnly);
    dims.setProperty("Star", AH::Dim_Star, QScriptValue::ReadOnly);
    dims.setProperty("Diamond", AH::Dim_Diamond, QScriptValue::ReadOnly);
    dims.setProperty("Hexagon", AH::Dim_Hexagon, QScriptValue::ReadOnly);
    dims.setProperty("Plus", AH::Dim_Plus, QScriptValue::ReadOnly);
    dims.setProperty("Triangle", AH::Dim_Triangle, QScriptValue::ReadOnly);
    consts.setProperty("Dimension", dims, QScriptValue::ReadOnly);

    // Monster Movements
    QScriptValue movs = m_engine->newObject();
    movs.setProperty("Normal", AH::Common::MonsterData::Normal, QScriptValue::ReadOnly);
    movs.setProperty("Fast", AH::Common::MonsterData::Fast, QScriptValue::ReadOnly);
    movs.setProperty("Stationary", AH::Common::MonsterData::Stationary, QScriptValue::ReadOnly);
    movs.setProperty("Flying", AH::Common::MonsterData::Flying, QScriptValue::ReadOnly);
    movs.setProperty("Special", AH::Common::MonsterData::Special, QScriptValue::ReadOnly);
    consts.setProperty("Movement", movs, QScriptValue::ReadOnly);

    // Monster Attributes
    QScriptValue monAtt = m_engine->newObject();
    monAtt.setProperty("Ambush", AH::Common::MonsterData::Ambush, QScriptValue::ReadOnly);
    monAtt.setProperty("Endless", AH::Common::MonsterData::Endless, QScriptValue::ReadOnly);
    monAtt.setProperty("PhysicalResistance", AH::Common::MonsterData::PhysicalResistance, QScriptValue::ReadOnly);
    monAtt.setProperty("MagicalResistance", AH::Common::MonsterData::MagicalResistance, QScriptValue::ReadOnly);
    monAtt.setProperty("PhysicalImmunity", AH::Common::MonsterData::PhysicalImmunity, QScriptValue::ReadOnly);
    monAtt.setProperty("MagicalImmunity", AH::Common::MonsterData::MagicalImmunity, QScriptValue::ReadOnly);
    monAtt.setProperty("Nightmarish_1", AH::Common::MonsterData::Nightmarish_1, QScriptValue::ReadOnly);
    monAtt.setProperty("Nightmarish_2", AH::Common::MonsterData::Nightmarish_2, QScriptValue::ReadOnly);
    monAtt.setProperty("Nightmarish_3", AH::Common::MonsterData::Nightmarish_3, QScriptValue::ReadOnly);
    monAtt.setProperty("Overwhelming_1", AH::Common::MonsterData::Overwhelming_1, QScriptValue::ReadOnly);
    monAtt.setProperty("Overwhelming_2", AH::Common::MonsterData::Overwhelming_2, QScriptValue::ReadOnly);
    monAtt.setProperty("Overwhelming_3", AH::Common::MonsterData::Overwhelming_3, QScriptValue::ReadOnly);
    consts.setProperty("Monster", monAtt, QScriptValue::ReadOnly);
}

void GameScript::initFieldConstants(QScriptValue &consts)
{
    QScriptValue fld = m_engine->newObject();
    fld.setProperty("NS_Northside", AH::Common::FieldData::NS_Northside, QScriptValue::ReadOnly);
    fld.setProperty("NS_TrainStation", AH::Common::FieldData::NS_TrainStation, QScriptValue::ReadOnly);
    fld.setProperty("NS_Newspaper", AH::Common::FieldData::NS_Newspaper, QScriptValue::ReadOnly);
    fld.setProperty("NS_CuriosieShoppe", AH::Common::FieldData::NS_CuriosieShoppe, QScriptValue::ReadOnly);

    fld.setProperty("DT_Downtown", AH::Common::FieldData::DT_Downtown, QScriptValue::ReadOnly);
    fld.setProperty("DT_BankOfArhham", AH::Common::FieldData::DT_BankOfArhham, QScriptValue::ReadOnly);
    fld.setProperty("DT_ArkhamAsylum", AH::Common::FieldData::DT_ArkhamAsylum, QScriptValue::ReadOnly);
    fld.setProperty("DT_IndependenceSquare", AH::Common::FieldData::DT_IndependenceSquare, QScriptValue::ReadOnly);

    fld.setProperty("ET_Easttown", AH::Common::FieldData::ET_Easttown, QScriptValue::ReadOnly);
    fld.setProperty("ET_HibbsRoadhouse", AH::Common::FieldData::ET_HibbsRoadhouse, QScriptValue::ReadOnly);
    fld.setProperty("ET_VelmasDiner", AH::Common::FieldData::ET_VelmasDiner, QScriptValue::ReadOnly);
    fld.setProperty("ET_PoliceStation", AH::Common::FieldData::ET_PoliceStation, QScriptValue::ReadOnly);

    fld.setProperty("MD_MerchantDist", AH::Common::FieldData::MD_MerchantDist, QScriptValue::ReadOnly);
    fld.setProperty("MD_UnvisitedIsle", AH::Common::FieldData::MD_UnvisitedIsle, QScriptValue::ReadOnly);
    fld.setProperty("MD_RiverDocks", AH::Common::FieldData::MD_RiverDocks, QScriptValue::ReadOnly);
    fld.setProperty("MD_TheUnnamable", AH::Common::FieldData::MD_TheUnnamable, QScriptValue::ReadOnly);

    fld.setProperty("RT_Rivertown", AH::Common::FieldData::RT_Rivertown, QScriptValue::ReadOnly);
    fld.setProperty("RT_Graveyard", AH::Common::FieldData::RT_Graveyard, QScriptValue::ReadOnly);
    fld.setProperty("RT_BlackCave", AH::Common::FieldData::RT_BlackCave, QScriptValue::ReadOnly);
    fld.setProperty("RT_GeneralStore", AH::Common::FieldData::RT_GeneralStore, QScriptValue::ReadOnly);

    fld.setProperty("MU_MiskatonicU", AH::Common::FieldData::MU_MiskatonicU, QScriptValue::ReadOnly);
    fld.setProperty("MU_ScienceBuilding", AH::Common::FieldData::MU_ScienceBuilding, QScriptValue::ReadOnly);
    fld.setProperty("MU_Administration", AH::Common::FieldData::MU_Administration, QScriptValue::ReadOnly);
    fld.setProperty("MU_Library", AH::Common::FieldData::MU_Library, QScriptValue::ReadOnly);

    fld.setProperty("FH_FrenchHill", AH::Common::FieldData::FH_FrenchHill, QScriptValue::ReadOnly);
    fld.setProperty("FH_TheWitchHouse", AH::Common::FieldData::FH_TheWitchHouse, QScriptValue::ReadOnly);
    fld.setProperty("FH_SilverTwilightLodge", AH::Common::FieldData::FH_SilverTwilightLodge, QScriptValue::ReadOnly);

    fld.setProperty("UT_Uptown", AH::Common::FieldData::UT_Uptown, QScriptValue::ReadOnly);
    fld.setProperty("UT_StMarysHospital", AH::Common::FieldData::UT_StMarysHospital, QScriptValue::ReadOnly);
    fld.setProperty("UT_YeOldeMagickShoppe", AH::Common::FieldData::UT_YeOldeMagickShoppe, QScriptValue::ReadOnly);
    fld.setProperty("UT_Woods", AH::Common::FieldData::UT_Woods, QScriptValue::ReadOnly);

    fld.setProperty("SS_Southside", AH::Common::FieldData::SS_Southside, QScriptValue::ReadOnly);
    fld.setProperty("SS_MasBoardingHouse", AH::Common::FieldData::SS_MasBoardingHouse, QScriptValue::ReadOnly);
    fld.setProperty("SS_SouthChurch", AH::Common::FieldData::SS_SouthChurch, QScriptValue::ReadOnly);
    fld.setProperty("SS_HistoricalSociety", AH::Common::FieldData::SS_HistoricalSociety, QScriptValue::ReadOnly);

    fld.setProperty("OW_AnotherDimension", AH::Common::FieldData::OW_AnotherDimension, QScriptValue::ReadOnly);
    fld.setProperty("OW_Abyss", AH::Common::FieldData::OW_Abyss, QScriptValue::ReadOnly);
    fld.setProperty("OW_CityOfGreatRace", AH::Common::FieldData::OW_CityOfGreatRace, QScriptValue::ReadOnly);
    fld.setProperty("OW_Yuggoth", AH::Common::FieldData::OW_Yuggoth, QScriptValue::ReadOnly);
    fld.setProperty("OW_Celeano", AH::Common::FieldData::OW_Celeano, QScriptValue::ReadOnly);
    fld.setProperty("OW_TheDreamlands", AH::Common::FieldData::OW_TheDreamlands, QScriptValue::ReadOnly);
    fld.setProperty("OW_PlateauOfLeng", AH::Common::FieldData::OW_PlateauOfLeng, QScriptValue::ReadOnly);
    fld.setProperty("OW_R_lyeh", AH::Common::FieldData::OW_R_lyeh, QScriptValue::ReadOnly);

    fld.setProperty("Sp_Sky", AH::Common::FieldData::Sp_Sky, QScriptValue::ReadOnly);
    fld.setProperty("Sp_Outskirts", AH::Common::FieldData::Sp_Outskirts, QScriptValue::ReadOnly);
    fld.setProperty("Sp_SpaceAndTime", AH::Common::FieldData::Sp_SpaceAndTime, QScriptValue::ReadOnly);
    consts.setProperty("Fields", fld, QScriptValue::ReadOnly);
}

void GameScript::registerInvestigator(InvestigatorScript *i)
{
    m_game->registerInvestigator(i);
}

QScriptValue GameScript::createInvestigator()
{
    return InvestigatorScript::createInvestigator(context(), engine());
}

QScriptValue GameScript::registerAction(GameActionScript *a)
{
    m_game->registerAction(a);
    return m_engine->newQObject(a);
}

GameActionScript *GameScript::createAction()
{
    return GameActionScript::createGameAction(context(), engine());
}

QScriptValue GameScript::registerOption(GameOptionScript *o)
{
    m_game->registerOption(o);
    return m_engine->newQObject(o);
}

GameOptionScript *GameScript::createOption()
{
    return GameOptionScript::createGameOption(context(), engine());
}

QScriptValue GameScript::registerObject(GameObjectScript *o)
{
    m_game->registerObject(o);
    return m_engine->newQObject(o);
}

QScriptValue GameScript::registerMultiObject(quint32 count, GameObjectScript *o)
{
    m_game->registerObject(o, count);
    return m_engine->newQObject(o);
}

GameObjectScript *GameScript::createObject()
{
    return GameObjectScript::createGameObject(context(), engine());
}

QScriptValue GameScript::addFieldOption(int fieldId, QString optionId)
{
    AH::Common::FieldData::FieldID fid = static_cast<AH::Common::FieldData::FieldID> (fieldId);
    GameField *f = m_game->board()->field(fid);
    if (!f) {
        return context()->throwError(QScriptContext::ReferenceError, "Unknown field id");
    }
    if (optionId.isEmpty()) {
        return context()->throwError(QScriptContext::ReferenceError, "Option id not set");
    }

    m_game->registerFieldOption(fid, optionId);
    return QScriptValue();
}

ArkhamEncounterScript *GameScript::createArkhamEncounter()
{
    return ArkhamEncounterScript::createEncounter(context(), engine());
}

QScriptValue GameScript::registerArkhamEncounter(ArkhamEncounterScript *e)
{
    m_game->registerArkhamEnconutry(e);
    return m_engine->newQObject(e);
}

MonsterScript *GameScript::createMonster()
{
    return MonsterScript::createMonster(context(), engine());
}

QScriptValue GameScript::registerMonster(quint32 count, MonsterScript *m)
{
    m_game->registerMonster(m, count);
    return m_engine->newQObject(m);
}

QStringList GameScript::array2stringlist(QScriptValue ar)
{
    QStringList ret;
    if (ar.isArray()) {
        int len = ar.property("length").toInt32();
        for (int i = 0; i < len; ++i) {
            ret << ar.property(i).toString();
        }
    }

    return ret;
}

QScriptValueList GameScript::array2list(QScriptValue ar)
{
    QScriptValueList ret;
    if (ar.isArray()) {
        int len = ar.property("length").toInt32();
        for (int i = 0; i < len; ++i) {
            ret << ar.property(i);
        }
    }

    return ret;
}

bool GameScript::parseScripts(QDir base)
{
    QFileInfoList lst = base.entryInfoList(QStringList() << "*.txt" << "*.js", QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files, QDir::Name);
    foreach (QFileInfo fi, lst) {
        if (fi.isDir()) {
            if (!parseScripts(fi.absoluteFilePath()))
                return false;
        } else {
            QFile f(fi.absoluteFilePath());
            if (!f.open(QIODevice::ReadOnly)) {
                qCritical() << "Cannot open file: " << fi.absoluteFilePath();
                return false;
            }
            QTextStream ts(&f);
            QString src = ts.readAll();
            QScriptValue v = m_engine->evaluate(src, fi.absoluteFilePath(), 1);
            if (v.isError()) {
                qCritical() << "Error Parsing file " << fi.absoluteFilePath() << ": " << v.toString();
                return false;
            }
        }
    }

    return true;
}
