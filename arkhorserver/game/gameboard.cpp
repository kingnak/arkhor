#include "gameboard.h"

GameBoard::GameBoard()
    : m_dirty(false)
{
}

void GameBoard::init()
{
    addField(new GameField(AH::Common::FieldData::NS_Northside, "Northside", AH::Common::FieldData::Street));
    addField(new GameField(AH::Common::FieldData::NS_TrainStation, "Train Station", AH::Common::FieldData::Location));
    addField(new GameField(AH::Common::FieldData::NS_Newspaper, "Newspaper", AH::Common::FieldData::Location));
    addField(new GameField(AH::Common::FieldData::NS_CuriosieShoppe, "Curiositie Shoppe", AH::Common::FieldData::Location));
    connectfield(AH::Common::FieldData::NS_TrainStation, AH::Common::FieldData::NS_Northside, true, true);
    connectfield(AH::Common::FieldData::NS_Newspaper, AH::Common::FieldData::NS_Northside, true, true);
    connectfield(AH::Common::FieldData::NS_CuriosieShoppe, AH::Common::FieldData::NS_Northside, true, true);

    addField(new GameField(AH::Common::FieldData::DT_Downtown, "Downtown", AH::Common::FieldData::Street));
    addField(new GameField(AH::Common::FieldData::DT_ArkhamAsylum, "Arkham Asylum", AH::Common::FieldData::Location));
    addField(new GameField(AH::Common::FieldData::DT_BankOfArhham, "Bank of Arhham", AH::Common::FieldData::Location));
    addField(new GameField(AH::Common::FieldData::DT_IndependenceSquare, "Independence Square", AH::Common::FieldData::Location));
    connectfield(AH::Common::FieldData::DT_ArkhamAsylum, AH::Common::FieldData::DT_Downtown, true, true);
    connectfield(AH::Common::FieldData::DT_BankOfArhham, AH::Common::FieldData::DT_Downtown, true, true);
    connectfield(AH::Common::FieldData::DT_IndependenceSquare, AH::Common::FieldData::DT_Downtown, true, true);

    addField(new GameField(AH::Common::FieldData::ET_Easttown, "Easttown", AH::Common::FieldData::Street));
    addField(new GameField(AH::Common::FieldData::ET_HibbsRoadhouse, "Hibb's Roadhouse", AH::Common::FieldData::Location));
    addField(new GameField(AH::Common::FieldData::ET_PoliceStation, "Police Station", AH::Common::FieldData::Location));
    addField(new GameField(AH::Common::FieldData::ET_VelmasDiner, "Velma's Diner", AH::Common::FieldData::Location));
    connectfield(AH::Common::FieldData::ET_HibbsRoadhouse, AH::Common::FieldData::ET_Easttown, true, true);
    connectfield(AH::Common::FieldData::ET_PoliceStation, AH::Common::FieldData::ET_Easttown, true, true);
    connectfield(AH::Common::FieldData::ET_VelmasDiner, AH::Common::FieldData::ET_Easttown, true, true);

    addField(new GameField(AH::Common::FieldData::MD_MerchantDist, "Merchant District", AH::Common::FieldData::Street));
    addField(new GameField(AH::Common::FieldData::MD_RiverDocks, "River Docks", AH::Common::FieldData::Location));
    addField(new GameField(AH::Common::FieldData::MD_TheUnnamable, "The Unnamable", AH::Common::FieldData::Location));
    addField(new GameField(AH::Common::FieldData::MD_UnvisitedIsle, "Unvisited Isle", AH::Common::FieldData::Location));
    connectfield(AH::Common::FieldData::MD_RiverDocks, AH::Common::FieldData::MD_MerchantDist, true, true);
    connectfield(AH::Common::FieldData::MD_TheUnnamable, AH::Common::FieldData::MD_MerchantDist, true, true);
    connectfield(AH::Common::FieldData::MD_UnvisitedIsle, AH::Common::FieldData::MD_MerchantDist, true, true);

    addField(new GameField(AH::Common::FieldData::RT_Rivertown, "Rivertown", AH::Common::FieldData::Street));
    addField(new GameField(AH::Common::FieldData::RT_BlackCave, "Black Cave", AH::Common::FieldData::Location));
    addField(new GameField(AH::Common::FieldData::RT_GeneralStore, "General Store", AH::Common::FieldData::Location));
    addField(new GameField(AH::Common::FieldData::RT_Graveyard, "Grave Yard", AH::Common::FieldData::Location));
    connectfield(AH::Common::FieldData::RT_BlackCave, AH::Common::FieldData::RT_Rivertown, true, true);
    connectfield(AH::Common::FieldData::RT_GeneralStore, AH::Common::FieldData::RT_Rivertown, true, true);
    connectfield(AH::Common::FieldData::RT_Graveyard, AH::Common::FieldData::RT_Rivertown, true, true);

    addField(new GameField(AH::Common::FieldData::MU_MiskatonicU, "Miskatonic University", AH::Common::FieldData::Street));
    addField(new GameField(AH::Common::FieldData::MU_Administration, "Administration", AH::Common::FieldData::Location));
    addField(new GameField(AH::Common::FieldData::MU_Library, "Library", AH::Common::FieldData::Location));
    addField(new GameField(AH::Common::FieldData::MU_ScienceBuilding, "Science Building", AH::Common::FieldData::Location));
    connectfield(AH::Common::FieldData::MU_Administration, AH::Common::FieldData::MU_MiskatonicU, true, true);
    connectfield(AH::Common::FieldData::MU_Library, AH::Common::FieldData::MU_MiskatonicU, true, true);
    connectfield(AH::Common::FieldData::MU_ScienceBuilding, AH::Common::FieldData::MU_MiskatonicU, true, true);

    addField(new GameField(AH::Common::FieldData::FH_FrenchHill, "French Hill", AH::Common::FieldData::Street));
    addField(new GameField(AH::Common::FieldData::FH_SilverTwilightLodge, "Silver Twilight Lodge", AH::Common::FieldData::Location));
    addField(new GameField(AH::Common::FieldData::FH_TheWitchHouse, "The Witch House", AH::Common::FieldData::Location));
    connectfield(AH::Common::FieldData::FH_SilverTwilightLodge, AH::Common::FieldData::FH_FrenchHill, true, true);
    connectfield(AH::Common::FieldData::FH_TheWitchHouse, AH::Common::FieldData::FH_FrenchHill, true, true);

    addField(new GameField(AH::Common::FieldData::UT_Uptown, "Uptown", AH::Common::FieldData::Street));
    addField(new GameField(AH::Common::FieldData::UT_StMarysHospital, "St. Mary's Hospital", AH::Common::FieldData::Location));
    addField(new GameField(AH::Common::FieldData::UT_Woods, "Woods", AH::Common::FieldData::Location));
    addField(new GameField(AH::Common::FieldData::UT_YeOldeMagickShoppe, "Ye Olde Magick Shoppe", AH::Common::FieldData::Location));
    connectfield(AH::Common::FieldData::UT_StMarysHospital, AH::Common::FieldData::UT_Uptown, true, true);
    connectfield(AH::Common::FieldData::UT_Woods, AH::Common::FieldData::UT_Uptown, true, true);
    connectfield(AH::Common::FieldData::UT_YeOldeMagickShoppe, AH::Common::FieldData::UT_Uptown, true, true);

    addField(new GameField(AH::Common::FieldData::SS_Southside, "Southside", AH::Common::FieldData::Street));
    addField(new GameField(AH::Common::FieldData::SS_HistoricalSociety, "Historical Society", AH::Common::FieldData::Location));
    addField(new GameField(AH::Common::FieldData::SS_MasBoardingHouse, "Ma's Boarding House", AH::Common::FieldData::Location));
    addField(new GameField(AH::Common::FieldData::SS_SouthChurch, "South Church", AH::Common::FieldData::Location));
    connectfield(AH::Common::FieldData::SS_HistoricalSociety, AH::Common::FieldData::SS_Southside, true, true);
    connectfield(AH::Common::FieldData::SS_MasBoardingHouse, AH::Common::FieldData::SS_Southside, true, true);
    connectfield(AH::Common::FieldData::SS_SouthChurch, AH::Common::FieldData::SS_Southside, true, true);

    // Street connectors:
    connectfield(AH::Common::FieldData::NS_Northside, AH::Common::FieldData::DT_Downtown, true, false);
    connectfield(AH::Common::FieldData::NS_Northside, AH::Common::FieldData::MD_MerchantDist, false, true);

    connectfield(AH::Common::FieldData::DT_Downtown, AH::Common::FieldData::ET_Easttown, true, false);
    connectfield(AH::Common::FieldData::DT_Downtown, AH::Common::FieldData::NS_Northside, false, true);
    connectfield(AH::Common::FieldData::DT_Downtown, AH::Common::FieldData::MD_MerchantDist, false, false);

    connectfield(AH::Common::FieldData::ET_Easttown, AH::Common::FieldData::RT_Rivertown, true, false);
    connectfield(AH::Common::FieldData::ET_Easttown, AH::Common::FieldData::DT_Downtown, false, true);

    connectfield(AH::Common::FieldData::MD_MerchantDist, AH::Common::FieldData::NS_Northside, true, false);
    connectfield(AH::Common::FieldData::MD_MerchantDist, AH::Common::FieldData::MU_MiskatonicU, false, true);
    connectfield(AH::Common::FieldData::MD_MerchantDist, AH::Common::FieldData::RT_Rivertown, false, false);

    connectfield(AH::Common::FieldData::RT_Rivertown, AH::Common::FieldData::FH_FrenchHill, true, false);
    connectfield(AH::Common::FieldData::RT_Rivertown, AH::Common::FieldData::ET_Easttown, false, true);

    connectfield(AH::Common::FieldData::MU_MiskatonicU, AH::Common::FieldData::MD_MerchantDist, true, false);
    connectfield(AH::Common::FieldData::MU_MiskatonicU, AH::Common::FieldData::UT_Uptown, false, true);
    connectfield(AH::Common::FieldData::MU_MiskatonicU, AH::Common::FieldData::FH_FrenchHill, false, false);

    connectfield(AH::Common::FieldData::FH_FrenchHill, AH::Common::FieldData::SS_Southside, true, false);
    connectfield(AH::Common::FieldData::FH_FrenchHill, AH::Common::FieldData::RT_Rivertown, false, true);

    connectfield(AH::Common::FieldData::UT_Uptown, AH::Common::FieldData::MU_MiskatonicU, true, false);
    connectfield(AH::Common::FieldData::UT_Uptown, AH::Common::FieldData::SS_Southside, false, true);

    connectfield(AH::Common::FieldData::SS_Southside, AH::Common::FieldData::UT_Uptown, true, false);
    connectfield(AH::Common::FieldData::SS_Southside, AH::Common::FieldData::FH_FrenchHill, false, true);

    // Other worlds:
    addField(new GameField(AH::Common::FieldData::OW_AnotherDimension, "Another Dimension first", AH::Common::FieldData::OtherWorld));
    //addField(new GameField(AH::Common::FieldData::OW_AnotherDimension_2, "Another Dimension second", AH::Common::FieldData::OtherWorld));
    //connectfield(AH::Common::FieldData::OW_AnotherDimension_1, AH::Common::FieldData::OW_AnotherDimension_2, false, false);

    addField(new GameField(AH::Common::FieldData::OW_Abyss, "Abyss first", AH::Common::FieldData::OtherWorld));
    //addField(new GameField(AH::Common::FieldData::OW_Abyss_2, "Abyss second", AH::Common::FieldData::OtherWorld));
    //connectfield(AH::Common::FieldData::OW_Abyss_1, AH::Common::FieldData::OW_Abyss_2, false, false);

    addField(new GameField(AH::Common::FieldData::OW_CityOfGreatRace, "City of Great Race first", AH::Common::FieldData::OtherWorld));
    //addField(new GameField(AH::Common::FieldData::OW_CityOfGreatRace_2, "City of Great Race second", AH::Common::FieldData::OtherWorld));
    //connectfield(AH::Common::FieldData::OW_CityOfGreatRace_1, AH::Common::FieldData::OW_CityOfGreatRace_2, false, false);

    addField(new GameField(AH::Common::FieldData::OW_Yuggoth, "Yuggoth first", AH::Common::FieldData::OtherWorld));
    //addField(new GameField(AH::Common::FieldData::OW_Yuggoth_2, "Yuggoth second", AH::Common::FieldData::OtherWorld));
    //connectfield(AH::Common::FieldData::OW_Yuggoth_1, AH::Common::FieldData::OW_Yuggoth_2, false, false);

    addField(new GameField(AH::Common::FieldData::OW_Celeano, "Celeano first", AH::Common::FieldData::OtherWorld));
    //addField(new GameField(AH::Common::FieldData::OW_Celeano_2, "Celeano second", AH::Common::FieldData::OtherWorld));
    //connectfield(AH::Common::FieldData::OW_Celeano_1, AH::Common::FieldData::OW_Celeano_2, false, false);

    addField(new GameField(AH::Common::FieldData::OW_TheDreamlands, "The Dreamlands first", AH::Common::FieldData::OtherWorld));
    //addField(new GameField(AH::Common::FieldData::OW_TheDreamlands_2, "The Dreamlands second", AH::Common::FieldData::OtherWorld));
    //connectfield(AH::Common::FieldData::OW_TheDreamlands_1, AH::Common::FieldData::OW_TheDreamlands_2, false, false);

    addField(new GameField(AH::Common::FieldData::OW_PlateauOfLeng, "Plateau of Leng first", AH::Common::FieldData::OtherWorld));
    //addField(new GameField(AH::Common::FieldData::OW_PlateauOfLeng_2, "Plateau of Leng second", AH::Common::FieldData::OtherWorld));
    //connectfield(AH::Common::FieldData::OW_PlateauOfLeng_1, AH::Common::FieldData::OW_PlateauOfLeng_2, false, false);

    addField(new GameField(AH::Common::FieldData::OW_R_lyeh, "R'lyeh first", AH::Common::FieldData::OtherWorld));
    //addField(new GameField(AH::Common::FieldData::OW_R_lyeh_2, "R'lyeh second", AH::Common::FieldData::OtherWorld));
    //connectfield(AH::Common::FieldData::OW_R_lyeh_1, AH::Common::FieldData::OW_R_lyeh_2, false, false);

    // Specials
    addField(new GameField(AH::Common::FieldData::Sp_Outskirts, "Outskirts", AH::Common::FieldData::Outskirts));
    addField(new GameField(AH::Common::FieldData::Sp_Sky, "Sky", AH::Common::FieldData::Sky));
    addField(new GameField(AH::Common::FieldData::Sp_SpaceAndTime, "Lost in Space & Time", AH::Common::FieldData::SpaceAndTime));
}

bool GameBoard::validatePath(MovementPath p)
{
    for (int i = 1; i < p.size(); ++i) {
        if (!p.at(i-1)->isConnectedTo(p.at(i))) {
            return false;
        }
    }

    return true;
}

QList<GameField *> GameBoard::fields(AH::Common::FieldData::FieldType type)
{
    QList<GameField *> ret;
    foreach (GameField *f, m_fields) {
        if (f->type() == type) {
            ret << f;
        }
    }
    return ret;
}

QList<GameField *> GameBoard::allFields()
{
    return m_fields.values();
}

void GameBoard::connectfield(AH::Common::FieldData::FieldID src, AH::Common::FieldData::FieldID dst, bool w, bool b)
{
    m_fields[src]->m_neighbours.insert(m_fields[dst]);
    m_fields[dst]->m_neighbours.insert(m_fields[src]);
    if (w)
        m_fields[src]->m_whiteField = m_fields[dst];

    if (b)
        m_fields[src]->m_blackField = m_fields[dst];
}

void GameBoard::addField(GameField *f)
{
    m_fields[f->id()] = f;
}
