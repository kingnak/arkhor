#include "utils.h"

using namespace AH::Common;

QString Utils::stringForProperty(AH::Common::PropertyValueData::Property p)
{
    switch (p) {
    case PropertyValueData::NoProperty: return "";

    case PropertyValueData::Attr_Speed: return "Speed";
    case PropertyValueData::Attr_Sneak: return "Sneak";
    case PropertyValueData::Attr_Fight: return "Fight";
    case PropertyValueData::Attr_Will: return "Will";
    case PropertyValueData::Attr_Lore: return "Lore";
    case PropertyValueData::Attr_Luck: return "Luck";

    case PropertyValueData::Skill_Speed: return "Speed";
    case PropertyValueData::Skill_Sneak: return "Sneak";
    case PropertyValueData::Skill_Fight: return "Fight";
    case PropertyValueData::Skill_Will: return "Will";
    case PropertyValueData::Skill_Lore: return "Lore";
    case PropertyValueData::Skill_Luck: return "Luck";
    case PropertyValueData::Skill_Evade: return "Evade";
    case PropertyValueData::Skill_Combat: return "Combat";
    case PropertyValueData::Skill_Horror: return "Horror";
    case PropertyValueData::Skill_Spell: return "Spell";

    case PropertyValueData::Prop_MaxStamina: return "Maximum Stamina";
    case PropertyValueData::Prop_MaxSanity: return "Maximum Sanity";
    case PropertyValueData::Prop_Focus: return "Focus";
    case PropertyValueData::Prop_Movement: return "Movement";
    case PropertyValueData::Prop_MinSuccessDieRoll: return "Minimal Success Die Roll";
    case PropertyValueData::Prop_HandCount: return "Hand Count";
    case PropertyValueData::Prop_CastCost: return "Spell Cast Sanity Cost";
    case PropertyValueData::Prop_CastAdjustment: return "Spell Cast Adjustment";

    case PropertyValueData::DieRoll_All: return "All";
    case PropertyValueData::DieRoll_Speed: return "Extra Die per Clue for Speed";
    case PropertyValueData::DieRoll_Sneak: return "Extra Die per Clue for Sneak";
    case PropertyValueData::DieRoll_Fight: return "Extra Die per Clue for Fight";
    case PropertyValueData::DieRoll_Will: return "Extra Die per Clue for Will";
    case PropertyValueData::DieRoll_Lore: return "Extra Die per Clue for Lore";
    case PropertyValueData::DieRoll_Luck: return "Extra Die per Clue for Luck";
    case PropertyValueData::DieRoll_Evade: return "Extra Die per Clue for Evade";
    case PropertyValueData::DieRoll_Combat: return "Extra Die per Clue for Combat";
    case PropertyValueData::DieRoll_Horror: return "Extra Die per Clue for Horror";
    case PropertyValueData::DieRoll_Spell: return "Extra Die per Clue for Spell";

    case PropertyValueData::DrawCard_CommonItem: return "Draw extra Common Item card";
    case PropertyValueData::DrawCard_UniqueItem: return "Draw extra Unique Item card";
    case PropertyValueData::DrawCard_Spell: return "Draw extra Spell card";
    case PropertyValueData::DrawCard_Skill: return "Draw extra Skill card";
    case PropertyValueData::DrawCard_Ally: return "Draw extra Ally card";
    case PropertyValueData::DrawCard_ArkhamEncountery: return "Draw extra Arkham Encountery card";
    case PropertyValueData::DrawCard_OtherWorldEncountery: return "Draw extra Other World Encountery card";

    case PropertyValueData::Game_SealClueCost: return "Seal Clue Cost";
    case PropertyValueData::Game_AllowMaskMonster: return "Mask Monsters";
    case PropertyValueData::Game_MaxGateCount: return "Maximum open Gates";
    case PropertyValueData::Game_MaxBoardMonsterCount: return "Maximum Monsters on Board";
    case PropertyValueData::Game_MaxOutskirtsMonsterCount: return "Maximum Monsters on Outskirts";
    case PropertyValueData::Game_TerrorLevel: return "Terror Level";
    case PropertyValueData::Game_MonsterCountFromGates: return "Monster emerging from Gates";

    case PropertyValueData::Damage_General: return "General Damage";
    case PropertyValueData::Damage_Magical: return "Magical Damage";
    case PropertyValueData::Damage_Physical: return "Physical Damage";

    case PropertyValueData::Monster_CombatDamage: return "Monster Combat Damage";
    case PropertyValueData::Monster_CombatAdjustment: return "Monster Combat Rating";
    case PropertyValueData::Monster_HorrorDamage: return "Monster Horror Damage";
    case PropertyValueData::Monster_HorrorAdjustment: return "Monster Horror Rating";
    case PropertyValueData::Monster_Awareness: return "Monster Awareness";
    case PropertyValueData::Monster_Toughness: return "Monster Toughness";
    case PropertyValueData::Monster_Movement: return "Monster Movement";

    case PropertyValueData::Ignore_MonsterAttributes: return "Ignore Monster Attributes";
    case PropertyValueData::Ignore_PhysicalDamage: return "Ignore Physical Damage";
    case PropertyValueData::Ignore_MagicalDamage: return "Ignore Magical Damage";

        // UNDEFINED VALUES:
    case PropertyValueData::Game_CloseGeneralStoreTerrorLevel:
    case PropertyValueData::Game_CloseCuriosityShoppeTerrorLevel:
    case PropertyValueData::Game_CloseYeOldeMagickShoppeTerrorLevel:
    case PropertyValueData::Game_OverrunArkhamTerrorLevel:
    case PropertyValueData::Game_SealedGatesToWin:
    case PropertyValueData::Game_GateTrophiesToWin:
        break;
    }
    return "";
}

QString Utils::stringForDimension(AH::Dimension dim)
{
    switch (dim)
    {
    case AH::Dim_Square: return "Square";
    case AH::Dim_Circle: return "Circle";
    case AH::Dim_Moon: return "Moon";
    case AH::Dim_Slash: return "Slash";
    case AH::Dim_Star: return "Star";
    case AH::Dim_Diamond: return "Diamond";
    case AH::Dim_Hexagon: return "Hexagon";
    case AH::Dim_Plus: return "Plus";
    case AH::Dim_Triangle: return "Triangle";
    default:
        return "";
    }
}

QStringList Utils::stringsForDimensions(AH::Dimensions dims)
{
    QStringList l;
    for (int i = 1; i <= AH::Dimension_Max_Value_Sentinel; i <<= 1) {
        AH::Dimension d = static_cast<AH::Dimension> (i);
        if (dims.testFlag(d)) {
            l << stringForDimension(d);
        }
    }
    return l;
}

QString Utils::stringForField(FieldData::FieldID fid)
{
    switch (fid) {
    case FieldData::NO_NO_FIELD: return "";
    case FieldData::NS_Northside: return "Northside";
    case FieldData::NS_TrainStation: return "Train Station";
    case FieldData::NS_Newspaper: return "Newspaper";
    case FieldData::NS_CuriositieShoppe: return "Curiositie Shoppe";
    case FieldData::DT_Downtown: return "Downtown";
    case FieldData::DT_BankOfArhham: return "Bank of Arhham";
    case FieldData::DT_ArkhamAsylum: return "Arkham Asylum";
    case FieldData::DT_IndependenceSquare: return "Independence Square";
    case FieldData::ET_Easttown: return "Easttown";
    case FieldData::ET_HibbsRoadhouse: return "Hibb's Roadhouse";
    case FieldData::ET_VelmasDiner: return "Velma's Diner";
    case FieldData::ET_PoliceStation: return "Police Station";
    case FieldData::MD_MerchantDist: return "Merchant District";
    case FieldData::MD_UnvisitedIsle: return "Unvisited Isle";
    case FieldData::MD_RiverDocks: return "River Docks";
    case FieldData::MD_TheUnnamable: return "The Unnamable";
    case FieldData::RT_Rivertown: return "Rivertown";
    case FieldData::RT_Graveyard: return "Graveyard";
    case FieldData::RT_BlackCave: return "Black Cave";
    case FieldData::RT_GeneralStore: return "General Store";
    case FieldData::MU_MiskatonicU: return "Miskatonic University";
    case FieldData::MU_ScienceBuilding: return "Science Building";
    case FieldData::MU_Administration: return "Administration";
    case FieldData::MU_Library: return "Library";
    case FieldData::FH_FrenchHill: return "French Hill";
    case FieldData::FH_TheWitchHouse: return "The Witch House";
    case FieldData::FH_SilverTwilightLodge: return "Silver Twilight Lodge";
    case FieldData::UT_Uptown: return "Uptown";
    case FieldData::UT_StMarysHospital: return "St. Mary's Hospital";
    case FieldData::UT_YeOldeMagickShoppe: return "Ye Olde Magick Shoppe";
    case FieldData::UT_Woods: return "Woods";
    case FieldData::SS_Southside: return "Southside";
    case FieldData::SS_MasBoardingHouse: return "Ma's Boarding House";
    case FieldData::SS_SouthChurch: return "South Church";
    case FieldData::SS_HistoricalSociety: return "Historical Society";
    case FieldData::OW_AnotherDimension: return "Another Dimension";
    case FieldData::OW_Abyss: return "The Abyss";
    case FieldData::OW_CityOfGreatRace: return "The City of the Great Race";
    case FieldData::OW_Yuggoth: return "Yuggoth";
    case FieldData::OW_Celeano: return "Great Hall of Celeano";
    case FieldData::OW_TheDreamlands: return "The Dreamlands";
    case FieldData::OW_PlateauOfLeng: return "Plateau Of Leng";
    case FieldData::OW_R_lyeh: return "R'lyeh";
    case FieldData::Sp_Sky: return "Sky";
    case FieldData::Sp_Outskirts: return "Outskirts";
    case FieldData::Sp_SpaceAndTime: return "Space and Time";
    }
    return "";
}

QString Utils::stringForMovement(MonsterData::MovementType m)
{
    switch (m)
    {
    case AH::Common::MonsterData::Normal: return "Normal";
    case AH::Common::MonsterData::Fast: return "Fast";
    case AH::Common::MonsterData::Stationary: return "Stationary";
    case AH::Common::MonsterData::Flying: return "Flying";
    case AH::Common::MonsterData::Special: return "Special";
    }
    return "";
}

QString Utils::stringForMonsterAttribute(MonsterData::MonsterAttribute a)
{
    switch (a)
    {
    case AH::Common::MonsterData::Ambush: return "Ambush";
    case AH::Common::MonsterData::Endless: return "Endless";
    case AH::Common::MonsterData::PhysicalResistance: return "Physical Resistance";
    case AH::Common::MonsterData::MagicalResistance: return "Magical Resistance";
    case AH::Common::MonsterData::PhysicalImmunity: return "Physical Immunity";
    case AH::Common::MonsterData::MagicalImmunity: return "Magical Immunity";
    case AH::Common::MonsterData::Undead: return "Undead";
    case AH::Common::MonsterData::Mask: return "Mask";
    case AH::Common::MonsterData::Nightmarish_1: return "Nightmarish 1";
    case AH::Common::MonsterData::Nightmarish_2: return "Nightmarish 2";
    case AH::Common::MonsterData::Nightmarish_3: return "Nightmarish 3";
    case AH::Common::MonsterData::Overwhelming_1: return "Overwhelming 1";
    case AH::Common::MonsterData::Overwhelming_2: return "Overwhelming 2";
    case AH::Common::MonsterData::Overwhelming_3: return "Overwhelming 3";

    case AH::Common::MonsterData::MonsterAttribute_Max_Value_Sentinel:
    case AH::Common::MonsterData::Nightmarish_ALL:
    case AH::Common::MonsterData::Overwhelming_ALL:
        Q_ASSERT(false);
    }
    return "";
}

QStringList Utils::stringsForMonsterAttributes(MonsterData::MonsterAttributes attrs)
{
    QStringList l;
    for (int i = 1; i < AH::Common::MonsterData::MonsterAttribute_Max_Value_Sentinel; i <<= 1) {
        AH::Common::MonsterData::MonsterAttribute a = static_cast<AH::Common::MonsterData::MonsterAttribute> (i);
        if (attrs.testFlag(a)) {
            l << stringForMonsterAttribute(a);
        }
    }
    return l;
}

QString Utils::stringForCostItem(CostItem::PaymentItem pi)
{
    switch (pi)
    {
    case AH::Common::CostItem::Pay_None: return "None";
    case AH::Common::CostItem::Pay_Money: return "$";
    case AH::Common::CostItem::Pay_Clue: return "Clue";
    case AH::Common::CostItem::Pay_Stamina: return "Stamina";
    case AH::Common::CostItem::Pay_Sanity: return "Sanity";
    case AH::Common::CostItem::Pay_Movement: return "Movement Point";
    case AH::Common::CostItem::Pay_GateTrophy: return "Gate Trophy";
    case AH::Common::CostItem::Pay_MonsterTrophy: return "Monster Trophy";
    case AH::Common::CostItem::Pay_MonsterToughness: return "Monster Toughness";
    }
    return "";
}

QString Utils::stringForObjectType(AH::GameObjectType t)
{
    switch (t)
    {
    case AH::NoObject: return "";
    case AH::Obj_CommonItem: return "Common Item";
    case AH::Obj_UniqueItem: return "Unique Item";
    case AH::Obj_Spell: return "Spell";
    case AH::Obj_Skill: return "Skill";
    case AH::Obj_Ally: return "Ally";
    case AH::Obj_Blessing_Curse: return "Blessing / Curse";
    case AH::Obj_Special: return "Special";
    }
    return "";
}

QString Utils::stringForObjectAttribute(GameObjectData::ObjectAttribute a)
{
    switch (a)
    {
    case GameObjectData::None: return "";
    case GameObjectData::CannotBeLost: return "Cannot be lost";
    case GameObjectData::DiscardAfterAttack: return "Discard after attack";
    case GameObjectData::DiscardOnEndFight: return "Discard when Ancient One awakes";

    case GameObjectData::ObjectAttribute_Max_Value_Sentinel:
        Q_ASSERT(false);
    }
    return "";
}

QStringList Utils::stringsForObjectAttributes(GameObjectData::ObjectAttributes attrs)
{
    QStringList l;
    for (int i = 1; i < GameObjectData::ObjectAttribute_Max_Value_Sentinel; i <<= 1) {
        GameObjectData::ObjectAttribute a = static_cast<GameObjectData::ObjectAttribute> (i);
        if (attrs.testFlag(a)) {
            l << stringForObjectAttribute(a);
        }
    }
    return l;
}

QString Utils::stringForPhase(AH::GamePhase ph)
{
    QString s;
    switch (ph) {
    case AH::Upkeep: s = "Upkeep"; break;
    case AH::Movement: s = "Movement"; break;
    case AH::ArkhamEncountery: s = "Arkham Encounter"; break;
    case AH::OtherWorldEncountery: s = "Other World Encounter"; break;
    case AH::Mythos: s = "Mythos"; break;
    case AH::EndFightPhase: s = "End Fight"; break;
    default: s = QString("Unknown (%1)").arg(ph); break;
    }

    return s;
}

QString Utils::stringForPropertyModification(PropertyModificationData mod)
{
    QString prop = stringForProperty(mod.affectedProperty());
    QString val;
    switch (mod.type()) {
    case PropertyModificationData::Additive:
        val = fullNumberString(mod.modificationAmount());
        break;
    case PropertyModificationData::Multiplicative:
        val = QString("*%1").arg(mod.modificationAmount());
        break;
    case PropertyModificationData::DividingUp:
        val = QString("^/%1").arg(mod.modificationAmount());
        break;
    case PropertyModificationData::DividingDown:
        val = QString("/%1").arg(mod.modificationAmount());
        break;
    case PropertyModificationData::Setting:
        val = QString("=%1").arg(mod.modificationAmount());
        break;
    case PropertyModificationData::Oring:
        // TODO: Better display for bitfields...
        val = QString("|%1").arg(mod.modificationAmount());
        break;
    }

    return prop + " " + val;
}

QString Utils::stringForMythosType(MythosData::MythosType t, MythosData::EnvironmentType et, QString wrapSubType)
{
    switch (t) {
    case MythosData::Headline: return "Headline";
    case MythosData::Rumor: return "Rumor";
    case MythosData::Environment:
    {
        QString s = stringForMythosEnvironmentType(et);
        if (wrapSubType.isEmpty()) {
            wrapSubType = " (%1)";
        }
        QString e = "Environment" + wrapSubType.arg(s);
        return e;
    }
    default:
        return "";
    }
}

QString Utils::stringForMythosEnvironmentType(MythosData::EnvironmentType et)
{
    switch (et) {
    case MythosData::Env_Weather:
        return "Weather";
    case MythosData::Env_Mystic:
        return "Mystic";
    case MythosData::Env_Urban:
        return "Urban";
    case MythosData::Env_None:
        return "";
    }
    return "";
}
