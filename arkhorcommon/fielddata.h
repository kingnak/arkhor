#ifndef FIELDDATA_H
#define FIELDDATA_H

#include "arkhorcommonglobal.h"
#include "serializer.hpp"

namespace AH {
    namespace Common {
        struct FieldData {
            enum FieldID {
                NO_NO_FIELD,

                NS_Northside = 0x0100,
                NS_TrainStation,
                NS_Newspaper,
                NS_CuriositieShoppe,

                DT_Downtown = 0x0200,
                DT_BankOfArhham,
                DT_ArkhamAsylum,
                DT_IndependenceSquare,

                ET_Easttown = 0x0300,
                ET_HibbsRoadhouse,
                ET_VelmasDiner,
                ET_PoliceStation,

                MD_MerchantDist = 0x0400,
                MD_UnvisitedIsle,
                MD_RiverDocks,
                MD_TheUnnamable,

                RT_Rivertown = 0x0500,
                RT_Graveyard,
                RT_BlackCave,
                RT_GeneralStore,

                MU_MiskatonicU = 0x0600,
                MU_ScienceBuilding,
                MU_Administration,
                MU_Library,

                FH_FrenchHill = 0x0700,
                FH_TheWitchHouse,
                FH_SilverTwilightLodge,

                UT_Uptown = 0x0800,
                UT_StMarysHospital,
                UT_YeOldeMagickShoppe,
                UT_Woods,

                SS_Southside = 0x0900,
                SS_MasBoardingHouse,
                SS_SouthChurch,
                SS_HistoricalSociety,

                OW_AnotherDimension = 0x1000,
                //OW_AnotherDimension_2,
                OW_Abyss = 0x1100,
                //OW_Abyss_2,
                OW_CityOfGreatRace = 0x1200,
                //OW_CityOfGreatRace_2,
                OW_Yuggoth = 0x1300,
                //OW_Yuggoth_2,
                OW_Celeano = 0x1400,
                //OW_Celeano_2,
                OW_TheDreamlands = 0x1500,
                //OW_TheDreamlands_2,
                OW_PlateauOfLeng = 0x1600,
                //OW_PlateauOfLeng_2,
                OW_R_lyeh = 0x1700,
                //OW_R_lyeh_2,

                Sp_Sky = 0x2000,
                Sp_Outskirts,
                Sp_SpaceAndTime
            };

            enum FieldType {
                Street,
                Location,
                Sky,
                OtherWorld,
                SpaceAndTime,
                Outskirts
            };
        };

        DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, FieldData::FieldID);
        DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, FieldData::FieldType);
    }
}

#endif // FIELDDATA_H
