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
#include "otherworldencounterscript.h"
#include "mythoscardscript.h"
#include "characterscript.h"
#include "gamefieldscript.h"
#include "ancientonescript.h"
#include "gatescript.h"
#include "game/actions/dierollaction.h"

#ifdef DEBUG_SCRIPT_BUILD
#include <QScriptEngineDebugger>
#include <QMainWindow>
#include <QAction>

Q_DECLARE_METATYPE(QList<AH::Common::GameOptionData>)

#endif

GameScript *GameScript::s_instance = NULL;

GameScript::GameScript(Game *game, QObject *parent) :
    QObject(parent), m_game(game)
{
    Q_ASSERT(s_instance == NULL);

    s_instance = this;
    m_engine = new QScriptEngine(this);
    m_ctx = new GameContextScript(this);
}

bool GameScript::init(const QString &scriptBaseDir)
{
#ifdef DEBUG_SCRIPT_BUILD
    qRegisterMetaType<QList<AH::Common::GameOptionData> >("QList<AH::Common::GameOptionData>");
    m_debugger = new QScriptEngineDebugger(this);
    m_debugger->attachTo(m_engine);
    m_debugger->standardWindow()->show();
    m_debugger->action(QScriptEngineDebugger::InterruptAction)->trigger();
#endif

    qScriptRegisterMetaType<GameActionScript*>(m_engine, GameActionScript::castToValue, GameActionScript::castFromValue);
    qScriptRegisterMetaType<GameOptionScript*>(m_engine, GameOptionScript::castToValue, GameOptionScript::castFromValue);
    qScriptRegisterMetaType<GameObjectScript*>(m_engine, GameObjectScript::castToValue, GameObjectScript::castFromValue);
    qScriptRegisterMetaType<ArkhamEncounterScript*>(m_engine, ArkhamEncounterScript::castToValue, ArkhamEncounterScript::castFromValue);
    qScriptRegisterMetaType<OtherWorldEncounterScript*>(m_engine, OtherWorldEncounterScript::castToValue, OtherWorldEncounterScript::castFromValue);
    qScriptRegisterMetaType<CharacterScript*>(m_engine, CharacterScript::castToValue, CharacterScript::castFromValue);
    qScriptRegisterMetaType<MonsterScript*>(m_engine, MonsterScript::castToValue, MonsterScript::castFromValue);
    qScriptRegisterMetaType<MythosCardScript*>(m_engine, MythosCardScript::castToValue, MythosCardScript::castFromValue);
    qScriptRegisterMetaType<AncientOneScript*>(m_engine, AncientOneScript::castToValue, AncientOneScript::castFromValue);

    qScriptRegisterMetaType<GameContextScript*>(m_engine, GameContextScript::castToValue, GameContextScript::castFromValue);
    qScriptRegisterMetaType<GameFieldScript*>(m_engine, GameFieldScript::castToValue, GameFieldScript::castFromValue);
    qScriptRegisterMetaType<GateScript*>(m_engine, GateScript::castToValue, GateScript::castFromValue);

    qScriptRegisterMetaType<QList<GameObjectScript*> >(m_engine, GameScript::castListToValue<GameObjectScript*>, GameScript::castListFromValue<GameObjectScript*>);

    qScriptRegisterMetaType<AH::Common::Cost>(m_engine, GameScript::castCostToValue, GameScript::castCostFromValue);

    qScriptRegisterMetaType<AH::ObjectTypeCount>(m_engine, GameScript::castObjTypeCountToValue, GameScript::castObjTypeCountFromValue);
    qScriptRegisterMetaType<QList<AH::ObjectTypeCount> >(m_engine, GameScript::castListToValue<AH::ObjectTypeCount>, GameScript::castListFromValue<AH::ObjectTypeCount>);

    qScriptRegisterMetaType<AH::Common::ChoiceData::OptionData>(m_engine, GameScript::castChoiceOptionToValue, GameScript::castChoiceOptionFromValue);
    qScriptRegisterMetaType<QList<AH::Common::ChoiceData::OptionData> >(m_engine, GameScript::castListToValue<AH::Common::ChoiceData::OptionData>, GameScript::castListFromValue<AH::Common::ChoiceData::OptionData>);

    qRegisterMetaType<GameContextScript*>();
    qRegisterMetaType<CharacterScript*>();
    qRegisterMetaType<QList<CharacterScript*> >();

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

GameObjectScript *GameScript::drawSingleObject(qint32 type)
{
    AH::GameObjectType t = static_cast<AH::GameObjectType> (type);

    GameObject *o = gGame->drawObject(t);
    if (!o) return NULL;
    GameObjectScript *os = dynamic_cast<GameObjectScript *> (o);
    if (!os) {
        gGame->returnObject(o);
        qWarning() << "Drawn object was not a script object";
    }
    return os;
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

    // Skills
    {
        QScriptValue sk = m_engine->newObject();
        sk.setProperty("Speed", AH::Skill_Speed, QScriptValue::ReadOnly);
        sk.setProperty("Sneak", AH::Skill_Sneak, QScriptValue::ReadOnly);
        sk.setProperty("Fight", AH::Skill_Fight, QScriptValue::ReadOnly);
        sk.setProperty("Will", AH::Skill_Will, QScriptValue::ReadOnly);
        sk.setProperty("Lore", AH::Skill_Lore, QScriptValue::ReadOnly);
        sk.setProperty("Luck", AH::Skill_Luck, QScriptValue::ReadOnly);
        sk.setProperty("Evade", AH::Skill_Evade, QScriptValue::ReadOnly);
        sk.setProperty("Combat", AH::Skill_Combat, QScriptValue::ReadOnly);
        sk.setProperty("Horror", AH::Skill_Horror, QScriptValue::ReadOnly);
        sk.setProperty("Spell", AH::Skill_Spell, QScriptValue::ReadOnly);
        consts.setProperty("Skills", sk, QScriptValue::ReadOnly);
    }

    // ObjectType
    {
        QScriptValue ot = m_engine->newObject();
        ot.setProperty("CommonItem", AH::Obj_CommonItem, QScriptValue::ReadOnly);
        ot.setProperty("UniqueItem", AH::Obj_UniqueItem, QScriptValue::ReadOnly);
        ot.setProperty("Spell", AH::Obj_Spell, QScriptValue::ReadOnly);
        ot.setProperty("Skill", AH::Obj_Skill, QScriptValue::ReadOnly);
        ot.setProperty("Ally", AH::Obj_Ally, QScriptValue::ReadOnly);
        ot.setProperty("Blessing_Curse", AH::Obj_Blessing_Curse, QScriptValue::ReadOnly);
        ot.setProperty("Special", AH::Obj_Special, QScriptValue::ReadOnly);
        consts.setProperty("ObjectType", ot, QScriptValue::ReadOnly);
    }

    // ObjectAttribute
    {
        QScriptValue oa = m_engine->newObject();
        oa.setProperty("CannotBeLost", AH::Common::GameObjectData::CannotBeLost, QScriptValue::ReadOnly);
        oa.setProperty("DiscardAfterAttack", AH::Common::GameObjectData::DiscardAfterAttack, QScriptValue::ReadOnly);
        consts.setProperty("ObjectAttribute", oa, QScriptValue::ReadOnly);
    }

    // Continue and Choose Type
    {
        QScriptValue ct = m_engine->newObject();
        ct.setProperty("CannotContinue", AH::CannotContinue, QScriptValue::ReadOnly);
        ct.setProperty("CanContinue", AH::CanContinue, QScriptValue::ReadOnly);
        ct.setProperty("Optional", AH::ChooseOptional, QScriptValue::ReadOnly);
        ct.setProperty("Mandatory", AH::ChooseMandatory, QScriptValue::ReadOnly);
        ct.setProperty("Supplemental", AH::ChooseSupplemental, QScriptValue::ReadOnly);
        consts.setProperty("Option", ct, QScriptValue::ReadOnly);
    }

    // GamePhase
    {
        QScriptValue gp = m_engine->newObject();
        gp.setProperty("Upkeep", AH::Upkeep, QScriptValue::ReadOnly);
        gp.setProperty("Movement", AH::Movement, QScriptValue::ReadOnly);
        gp.setProperty("ArkhamEncountery", AH::ArkhamEncountery, QScriptValue::ReadOnly);
        gp.setProperty("OtherWorldEncountery", AH::OtherWorldEncountery, QScriptValue::ReadOnly);
        gp.setProperty("Mythos", AH::Mythos, QScriptValue::ReadOnly);
        gp.setProperty("AllPhases", AH::AllPhases, QScriptValue::ReadOnly);
        gp.setProperty("DieRollPhase", AH::DieRoll, QScriptValue::ReadOnly);
        gp.setProperty("FightPhase", AH::FightPhase, QScriptValue::ReadOnly);
        //gp.setProperty("Any", static_cast<int> (AH::AllPhases | AH::DieRoll | AH::FightPhase), QScriptValue::ReadOnly);
        gp.setProperty("Any", AH::AllPhases, QScriptValue::ReadOnly);
        consts.setProperty("GamePhases", gp, QScriptValue::ReadOnly);
    }

    // Mythos types
    {
        QScriptValue mt = m_engine->newObject();
        mt.setProperty("Headline", AH::Common::MythosData::Headline, QScriptValue::ReadOnly);
        mt.setProperty("Environment", AH::Common::MythosData::Environment, QScriptValue::ReadOnly);
        mt.setProperty("Rumor", AH::Common::MythosData::Rumor, QScriptValue::ReadOnly);
        consts.setProperty("Mythos", mt, QScriptValue::ReadOnly);
        // TODO: set environment types
    }

    // Payment Items
    {
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
    }

    // Property Modifications
    {
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
        mods.setProperty("DieRoll_All", AH::Common::PropertyValueData::DieRoll_All, QScriptValue::ReadOnly);
        mods.setProperty("DieRoll_Speed", AH::Common::PropertyValueData::DieRoll_Speed, QScriptValue::ReadOnly);
        mods.setProperty("DieRoll_Sneak", AH::Common::PropertyValueData::DieRoll_Sneak, QScriptValue::ReadOnly);
        mods.setProperty("DieRoll_Fight", AH::Common::PropertyValueData::DieRoll_Fight, QScriptValue::ReadOnly);
        mods.setProperty("DieRoll_Will", AH::Common::PropertyValueData::DieRoll_Will, QScriptValue::ReadOnly);
        mods.setProperty("DieRoll_Lore", AH::Common::PropertyValueData::DieRoll_Lore, QScriptValue::ReadOnly);
        mods.setProperty("DieRoll_Luck", AH::Common::PropertyValueData::DieRoll_Luck, QScriptValue::ReadOnly);
        mods.setProperty("DieRoll_Evade", AH::Common::PropertyValueData::DieRoll_Evade, QScriptValue::ReadOnly);
        mods.setProperty("DieRoll_Combat", AH::Common::PropertyValueData::DieRoll_Combat, QScriptValue::ReadOnly);
        mods.setProperty("DieRoll_Horror", AH::Common::PropertyValueData::DieRoll_Horror, QScriptValue::ReadOnly);
        mods.setProperty("DieRoll_Spell", AH::Common::PropertyValueData::DieRoll_Spell, QScriptValue::ReadOnly);
        mods.setProperty("Game_SealClueCost", AH::Common::PropertyValueData::Game_SealClueCost, QScriptValue::ReadOnly);
        mods.setProperty("Damage_General", AH::Common::PropertyValueData::Damage_General, QScriptValue::ReadOnly);
        mods.setProperty("Damage_Magical", AH::Common::PropertyValueData::Damage_Magical, QScriptValue::ReadOnly);
        mods.setProperty("Damage_Physical", AH::Common::PropertyValueData::Damage_Physical, QScriptValue::ReadOnly);
        mods.setProperty("Monster_CombatDamage", AH::Common::PropertyValueData::Monster_CombatDamage, QScriptValue::ReadOnly);
        mods.setProperty("Monster_CombatAdjustment", AH::Common::PropertyValueData::Monster_CombatAdjustment, QScriptValue::ReadOnly);
        mods.setProperty("Monster_HorrorDamage", AH::Common::PropertyValueData::Monster_HorrorDamage, QScriptValue::ReadOnly);
        mods.setProperty("Monster_HorrorAdjustment", AH::Common::PropertyValueData::Monster_HorrorAdjustment, QScriptValue::ReadOnly);
        mods.setProperty("Monster_Awareness", AH::Common::PropertyValueData::Monster_Awareness, QScriptValue::ReadOnly);
        mods.setProperty("Monster_Toughness", AH::Common::PropertyValueData::Monster_Toughness, QScriptValue::ReadOnly);
        mods.setProperty("Monster_Movement", AH::Common::PropertyValueData::Monster_Movement, QScriptValue::ReadOnly);
        mods.setProperty("Ignore_MonsterAttributes", AH::Common::PropertyValueData::Ignore_MonsterAttributes, QScriptValue::ReadOnly);
        consts.setProperty("Mods", mods, QScriptValue::ReadOnly);
    }

    // Die Roll options
    {
        QScriptValue reroll = m_engine->newObject();
        reroll.setProperty("All", DieRollOption::ReRollAll, QScriptValue::ReadOnly);
        reroll.setProperty("AllFailed", DieRollOption::ReRollAllFailed, QScriptValue::ReadOnly);
        reroll.setProperty("OneFailed", DieRollOption::ReRollOneFailed, QScriptValue::ReadOnly);
        consts.setProperty("Reroll", reroll, QScriptValue::ReadOnly);
    }

    // Dimensions
    {
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
    }

    // Other World Colors
    {
        QScriptValue owc = m_engine->newObject();
        owc.setProperty("Red", AH::OWC_Red, QScriptValue::ReadOnly);
        owc.setProperty("Green", AH::OWC_Green, QScriptValue::ReadOnly);
        owc.setProperty("Blue", AH::OWC_Blue, QScriptValue::ReadOnly);
        owc.setProperty("Yellow", AH::OWC_Yellow, QScriptValue::ReadOnly);
        consts.setProperty("OtherWorld", owc, QScriptValue::ReadOnly);
    }

    // Monster Movements
    {
        QScriptValue movs = m_engine->newObject();
        movs.setProperty("Normal", AH::Common::MonsterData::Normal, QScriptValue::ReadOnly);
        movs.setProperty("Fast", AH::Common::MonsterData::Fast, QScriptValue::ReadOnly);
        movs.setProperty("Stationary", AH::Common::MonsterData::Stationary, QScriptValue::ReadOnly);
        movs.setProperty("Flying", AH::Common::MonsterData::Flying, QScriptValue::ReadOnly);
        movs.setProperty("Special", AH::Common::MonsterData::Special, QScriptValue::ReadOnly);
        consts.setProperty("Movement", movs, QScriptValue::ReadOnly);
    }

    // Monster Attributes
    {
        QScriptValue monAtt = m_engine->newObject();
        monAtt.setProperty("Ambush", AH::Common::MonsterData::Ambush, QScriptValue::ReadOnly);
        monAtt.setProperty("Endless", AH::Common::MonsterData::Endless, QScriptValue::ReadOnly);
        monAtt.setProperty("PhysicalResistance", AH::Common::MonsterData::PhysicalResistance, QScriptValue::ReadOnly);
        monAtt.setProperty("MagicalResistance", AH::Common::MonsterData::MagicalResistance, QScriptValue::ReadOnly);
        monAtt.setProperty("PhysicalImmunity", AH::Common::MonsterData::PhysicalImmunity, QScriptValue::ReadOnly);
        monAtt.setProperty("MagicalImmunity", AH::Common::MonsterData::MagicalImmunity, QScriptValue::ReadOnly);
        monAtt.setProperty("Undead", AH::Common::MonsterData::Undead, QScriptValue::ReadOnly);
        monAtt.setProperty("Mask", AH::Common::MonsterData::Mask, QScriptValue::ReadOnly);
        monAtt.setProperty("Nightmarish_1", AH::Common::MonsterData::Nightmarish_1, QScriptValue::ReadOnly);
        monAtt.setProperty("Nightmarish_2", AH::Common::MonsterData::Nightmarish_2, QScriptValue::ReadOnly);
        monAtt.setProperty("Nightmarish_3", AH::Common::MonsterData::Nightmarish_3, QScriptValue::ReadOnly);
        monAtt.setProperty("Overwhelming_1", AH::Common::MonsterData::Overwhelming_1, QScriptValue::ReadOnly);
        monAtt.setProperty("Overwhelming_2", AH::Common::MonsterData::Overwhelming_2, QScriptValue::ReadOnly);
        monAtt.setProperty("Overwhelming_3", AH::Common::MonsterData::Overwhelming_3, QScriptValue::ReadOnly);

        // For Ignore:
        monAtt.setProperty("DamageNightmarish", AH::Common::MonsterData::Nightmarish_ALL, QScriptValue::ReadOnly);
        monAtt.setProperty("DamageOverwhelming", AH::Common::MonsterData::Overwhelming_ALL, QScriptValue::ReadOnly);
        consts.setProperty("Monster", monAtt, QScriptValue::ReadOnly);
    }

    // Monster Damage
    {
        QScriptValue monDmg = m_engine->newObject();
        monDmg.setProperty("Combat", Monster::Combat, QScriptValue::ReadOnly);
        monDmg.setProperty("Horror", Monster::Horror, QScriptValue::ReadOnly);
        monDmg.setProperty("Nightmare", Monster::Nightmare, QScriptValue::ReadOnly);
        monDmg.setProperty("Overwhelm", Monster::Overwhelm, QScriptValue::ReadOnly);
        consts.setProperty("MonsterDamage", monDmg, QScriptValue::ReadOnly);
    }
}

void GameScript::initFieldConstants(QScriptValue &consts)
{
    QScriptValue fld = m_engine->newObject();
    fld.setProperty("NS_Northside", AH::Common::FieldData::NS_Northside, QScriptValue::ReadOnly);
    fld.setProperty("NS_TrainStation", AH::Common::FieldData::NS_TrainStation, QScriptValue::ReadOnly);
    fld.setProperty("NS_Newspaper", AH::Common::FieldData::NS_Newspaper, QScriptValue::ReadOnly);
    fld.setProperty("NS_CuriositieShoppe", AH::Common::FieldData::NS_CuriositieShoppe, QScriptValue::ReadOnly);

    fld.setProperty("DT_Downtown", AH::Common::FieldData::DT_Downtown, QScriptValue::ReadOnly);
    fld.setProperty("DT_BankOfArkham", AH::Common::FieldData::DT_BankOfArhham, QScriptValue::ReadOnly);
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

    QScriptValue fdt = m_engine->newObject();
    fdt.setProperty("Location", AH::Common::FieldData::Location, QScriptValue::ReadOnly);
    fdt.setProperty("Street", AH::Common::FieldData::Street, QScriptValue::ReadOnly);
    fdt.setProperty("Sky", AH::Common::FieldData::Sky, QScriptValue::ReadOnly);
    fdt.setProperty("OtherWorld", AH::Common::FieldData::OtherWorld, QScriptValue::ReadOnly);
    fdt.setProperty("SpaceAndTime", AH::Common::FieldData::SpaceAndTime, QScriptValue::ReadOnly);
    fdt.setProperty("Outskirts", AH::Common::FieldData::Outskirts, QScriptValue::ReadOnly);
    consts.setProperty("FieldType", fdt, QScriptValue::ReadOnly);
}

QScriptValue GameScript::registerInvestigator(InvestigatorScript *i)
{
    if (!m_game->registerInvestigator(i)) {
        return m_engine->currentContext()->throwError("Error registering Investigator");
    }
    return QScriptValue();
}

QScriptValue GameScript::createInvestigator()
{
    return InvestigatorScript::createInvestigator(context(), engine());
}

QScriptValue GameScript::registerAction(GameActionScript *a)
{
    if (!m_game->registerAction(a)) {
        m_engine->currentContext()->throwError("Error registering action");
    }
    return m_engine->newQObject(a);
}

GameActionScript *GameScript::createAction()
{
    return GameActionScript::createGameAction(context(), engine());
}

QScriptValue GameScript::registerOption(GameOptionScript *o)
{
    if (!m_game->registerOption(o)) {
        m_engine->currentContext()->throwError("Error registering option");
    }
    return m_engine->newQObject(o);
}

GameOptionScript *GameScript::createOption()
{
    return GameOptionScript::createGameOption(context(), engine());
}

/*
GameObjectScript *GameScript::drawObject(qint32 type)
{
    AH::GameObjectType t = static_cast<AH::GameObjectType> (type);
    GameObject *o = gGame->drawObject(t);
    if (!o) return NULL;
    GameObjectScript *os = dynamic_cast<GameObjectScript *> (o);
    if (!os) {
        gGame->returnObject(o);
        qWarning() << "Drawn object was not a script object";
    }
    return os;
}
*/

GameObjectScript *GameScript::drawSpecificObject(QString id)
{
    GameObject *o = gGame->drawSpecificObject(id);
    if (!o) return NULL;
    GameObjectScript *os = dynamic_cast<GameObjectScript *> (o);
    if (!os) {
        gGame->returnObject(o);
        qWarning() << "Drawn object was not a script object. Id:" << id;
    }
    return os;
}

int GameScript::cardsOnDeck(qint32 type)
{
    AH::GameObjectType t = static_cast<AH::GameObjectType> (type);
    return gGame->drawableObjectCount(t);
}

void GameScript::createGate(qint32 fld)
{
    AH::Common::FieldData::FieldID fid = static_cast<AH::Common::FieldData::FieldID> (fld);
    GameField *field = gGame->board()->field(fid);
    if (field)
        gGame->createGate(field);
}

QScriptValue GameScript::quickOption()
{
    if (context()->argumentCount() != 1 || !context()->argument(0).isObject()) {
        return context()->throwError(QScriptContext::TypeError, "quickOption: Must call with 1 object");
    }

    QScriptValue data = context()->argument(0);
    QString oid = data.property("id").toString();
    data.setProperty("id", QScriptValue());
    GameActionScript *a = GameActionScript::createGameAction(data, context(), engine());
    if (!a) {
        if (!engine()->hasUncaughtException())
            return context()->throwError(QScriptContext::TypeError, "quickOption: Error creating action");
        else
            return engine()->uncaughtException();
    }

    QScriptValue act = registerAction(a);
    if (act.isError()) {
        return act;
    }

    data.setProperty("activate", QScriptValue());
    data.setProperty("id", oid);
    data.setProperty("actionId", act.property("id"));


    GameOptionScript *o = GameOptionScript::createGameOption(data, context(), engine());
    if (!o) {
        if (!engine()->hasUncaughtException())
            return context()->throwError(QScriptContext::TypeError, "quickOption: Error creating option");
        else
            return engine()->uncaughtException();
    }

    QScriptValue opt = registerOption(o);
    if (opt.isError()) {
        return opt;
    }
    return opt;
}

QScriptValue GameScript::getDieRollOption()
{
    if (context()->argumentCount() != 1 || !context()->argument(0).isObject()) {
        return context()->throwError(QScriptContext::TypeError, "getDieRollOption: Must call with 1 object");
    }

    QScriptValue data = context()->argument(0);
    AH::Skills skills = GameScript::parseFlags<AH::Skills>(data.property("skills"), AH::NoSkill);
    DieRollOption::ReRollType type = static_cast<DieRollOption::ReRollType> (data.property("type").toInt32());
    if (!type || !skills) {
        return context()->throwError("Invalid Die Roll Option");
    }

    QString id = QString("OP_DIE_ROLL_%1_%2")
            .arg(static_cast<int> (type))
            .arg(static_cast<int> (skills));

    if (!gGame->registry()->findOptionById(id)) {
        // Create the option
        DieRollOption *op = new DieRollOption(type, skills);
        op->setId(id);
        gGame->registerOption(op);
    }
    return id;
}

QScriptValue GameScript::registerSingleObject(GameObjectScript *o)
{
    if (!m_game->registerObject(o, 0)) {
        return m_engine->currentContext()->throwError("Error registering Single Object");
    }
    return m_engine->newQObject(o);
}

QScriptValue GameScript::registerObject(GameObjectScript *o)
{
    if (!m_game->registerObject(o)) {
        return m_engine->currentContext()->throwError("Error registering Object");
    }
    return m_engine->newQObject(o);
}

QScriptValue GameScript::registerMultiObject(quint32 count, GameObjectScript *o)
{
    if (!m_game->registerObject(o, count)) {
        return m_engine->currentContext()->throwError("Error registering Multiple Objects");
    }
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
    if (!m_game->registerArkhamEnconutry(e)) {
        return m_engine->currentContext()->throwError("Error registering Arkham Encounter");
    }
    return m_engine->newQObject(e);
}

OtherWorldEncounterScript *GameScript::createOtherWorldEncounter()
{
    return OtherWorldEncounterScript::createEncounter(context(), engine());
}

QScriptValue GameScript::registerOtherWorldEncounter(OtherWorldEncounterScript *e)
{
    if (!m_game->registerOtherWorldEncountery(e)) {
        return m_engine->currentContext()->throwError("Error registering Other World Encounter");
    }
    return m_engine->newQObject(e);
}

MonsterScript *GameScript::createMonster()
{
    return MonsterScript::createMonster(context(), engine());
}

QScriptValue GameScript::registerMonster(quint32 count, MonsterScript *m)
{
    if (!m_game->registerMonster(m, count)) {
        m_engine->currentContext()->throwError("Error registering monster");
    }
    return m_engine->newQObject(m);
}

MythosCardScript *GameScript::createMythosCard()
{
    return MythosCardScript::createMythosCard(context(), engine());
}

QScriptValue GameScript::registerMythosCard(MythosCardScript *m)
{
    if (!m_game->registerMythos(m)) {
        return m_engine->currentContext()->throwError("Error registering Mythos");
    }
    return m_engine->newQObject(m);
}

AncientOneScript *GameScript::createAncientOne()
{
    return AncientOneScript::createAncientOne(context(), engine());
}

QScriptValue GameScript::registerAncientOne(AncientOneScript *a)
{
    if (!m_game->registerAncientOne(a)) {
        return m_engine->currentContext()->throwError("Error registering Ancient One");
    }
    return m_engine->newQObject(a);
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
    } else {
        ret << ar;
    }

    return ret;
}

bool GameScript::parseCosts(QScriptValue v, AH::Common::Cost &c)
{
    c.clear();
    if (!v.isValid() || v.isNull() || v.isUndefined()) {
        return true;
    }

    QScriptValueList lst;
    if (v.isArray()) {
        lst = GameScript::array2list(v);
    } else if (v.isObject()) {
        lst = QScriptValueList() << v;
    } else {
        return false;
    }

    if (lst.size() == 0) {
        return true;
        /*
    } else if (lst.size() == 1) {
        AH::Common::CostList cl;
        if (!parseCostList(lst[0], cl)) {
            return false;
        }
        c.addAlternative(cl);
        return true;
        */
    } else {
        foreach (QScriptValue item, lst) {
            AH::Common::CostList cl;
            if (!parseCostList(item, cl)) {
                return false;
            }
            c.addAlternative(cl);
        }
        return true;
    }
}

bool GameScript::parseCostList(QScriptValue v, AH::Common::CostList &cl)
{
    QScriptValueList lst;
    if (v.isArray()) {
        lst = GameScript::array2list(v);
    } else if (v.isObject()) {
        lst = QScriptValueList() << v;
    } else {
        return false;
    }

    if (lst.size() == 0) {
        return false;
        /*
    } else if (lst.size() == 1) {
        AH::Common::CostItem ci;
        if (!parseCostItem(lst[0], ci)) {
            return false;
        }
        cl.append(ci);
        return true;
        */
    } else {
        foreach (QScriptValue item, lst) {
            AH::Common::CostItem ci;
            if (!parseCostItem(item, ci)) {
                return false;
            }
            cl.append(ci);
        }
        return true;
    }
}

bool GameScript::parseCostItem(QScriptValue v, AH::Common::CostItem &ci)
{
    if (!v.isObject()) {
        return false;
    }

    ci.type = static_cast<AH::Common::CostItem::PaymentItem> (v.property("type").toUInt32());
    ci.amount = v.property("amount").toUInt32();

    if (ci.type == AH::Common::CostItem::Pay_None || ci.amount == 0) {
        return false;
    }

    return true;
}

bool GameScript::parseObjectTypeCount(QScriptValue v, QList<AH::ObjectTypeCount> &o)
{
    o.clear();
    QScriptValueList objTypes = GameScript::array2list(v);
    foreach (QScriptValue v, objTypes) {
        int type = v.property("type").toInt32();
        int amount = v.property("amount").toInt32();
        AH::ObjectTypeCount otc(static_cast<AH::GameObjectType>(type), amount);
        o << otc;
    }
    return true;
}

bool GameScript::parseOptionChoiceData(QScriptValue v, AH::Common::ChoiceData::OptionData &o)
{
    QString id, name, desc;
    if (v.isArray()) {
        int len = v.property("length").toInt32();
        if (len > 0) {
            // Assume id = name if only 1 value.
            // So possible: ["name" (=id)] | ["id", "name"] | ["id", "name, "desc"]
            id = v.property(0).toString();
            name = len > 1 ? v.property(1).toString() : v.property(0).toString();
            desc = v.property(2).toString();
        }
    } else if (v.isObject()) {
        id = v.property("id").toString();
        name = v.property("name").toString();
        desc = v.property("description").toString();
    } else {
        id = name = v.toString();
    }
    o = AH::Common::ChoiceData::OptionData(id, name, desc);
    return true;
}

QScriptValue GameScript::castCostToValue(QScriptEngine *eng, const AH::Common::Cost &in)
{
    //parseCosts()
    Q_UNUSED(eng)
    Q_UNUSED(in)
    Q_ASSERT_X(false, "GameScript::castCostToValue", "Not implemented");
    return QScriptValue();
}

void GameScript::castCostFromValue(const QScriptValue &v, AH::Common::Cost &o)
{
    if (!parseCosts(v, o)) {
        qCritical("Error parsing costs");
    }
}

QScriptValue GameScript::castObjTypeCountToValue(QScriptEngine *eng, const AH::ObjectTypeCount &in)
{
    Q_UNUSED(eng)
    Q_UNUSED(in)
    Q_ASSERT_X(false, "GameScript::castObjTypeCountToValue", "Not implemented");
    return QScriptValue();
}

void GameScript::castObjTypeCountFromValue(const QScriptValue &v, AH::ObjectTypeCount &o)
{
    QList<AH::ObjectTypeCount> l;
    if (!parseObjectTypeCount(v, l)) {
        qCritical("Error parsing Object Type Count");
    } else {
        o = l.value(0);
    }
}

QScriptValue GameScript::castChoiceOptionToValue(QScriptEngine *eng, const AH::Common::ChoiceData::OptionData &in)
{
    Q_UNUSED(eng)
    Q_UNUSED(in)
    Q_ASSERT_X(false, "GameScript::castChoiceOptionToValue", "Not implemented");
    return QScriptValue();
}

void GameScript::castChoiceOptionFromValue(const QScriptValue &v, AH::Common::ChoiceData::OptionData &o)
{
    if (!parseOptionChoiceData(v, o)) {
        qCritical("Error parsing Choice Options");
    }
}

bool GameScript::parseScripts(QDir base)
{
    QFileInfoList lst = base.entryInfoList(QStringList() << "*.txt" << "*.js", QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files, QDir::Name);
    foreach (QFileInfo fi, lst) {
        if (fi.isDir()) {
            if (!parseScripts(fi.absoluteFilePath()))
                return false;
        } else if (fi.fileName().endsWith(".js")){
            QFile f(fi.absoluteFilePath());
            if (!f.open(QIODevice::ReadOnly)) {
                qCritical() << "Cannot open file: " << fi.absoluteFilePath();
                return false;
            }
            QTextStream ts(&f);
            QString src = ts.readAll();

#ifdef DEBUG_SCRIPT_BUILD
            //QString fn = fi.absoluteFilePath();
            //m_debugger->action(QScriptEngineDebugger::InterruptAction)->trigger();
#endif

            if (src.length() > 0) {
                QString fn = fi.absoluteFilePath();
                qDebug(qPrintable(fn));

                src = QString("(function(){%1})();").arg(src);

                QScriptValue v = m_engine->evaluate(src, fn, 1);
                if (v.isError()) {
                    qCritical() << "Error Parsing file " << fi.absoluteFilePath() << ": " << v.toString();
                    return false;
                }
            }
        }
    }

    return true;
}
