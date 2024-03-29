#include "ahboardfillerhelper.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include "gui/ahfielditem.h"
#include "ahboardscene.h"

AhBoardfillerHelper::AhBoardfillerHelper() = default;

void AhBoardfillerHelper::initBoard(AhBoardScene *scene, QGraphicsItem *parent)
{
    double d = initScale(scene);
    initFields(scene, parent, d);
    initTerror(scene, parent, d);
}

double AhBoardfillerHelper::initScale(AhBoardScene *scene)
{
    QFile f(":/core/data/boardscale");
    if (!f.open(QIODevice::ReadOnly)) {
        return 1.0;
    }
    QTextStream ts(&f);
    bool ok;
    // first line is scaling of board (for items)
    double d = ts.readLine().toDouble(&ok);
    if (ok) scene->setBoardScaleFactor(d);

    // second line is scaling of coords in field text files
    d = ts.readLine().toDouble(&ok);
    if (ok) return d;
    return 1.0;
}

void AhBoardfillerHelper::initFields(AhBoardScene *scene, QGraphicsItem *parent, double scale)
{
    QFile file(":/core/data/boardfields");
    file.open(QIODevice::ReadOnly);
    QTextStream ts(&file);

    forever {
        QString l = ts.readLine();
        if (l.isEmpty()) break;
        QStringList parts = l.split(";");
        AH::Common::FieldData::FieldType type = parseType(parts[1]);
        AhFieldItem::FieldItemType fieldType;
        switch (type) {
        case AH::Common::FieldData::Street: fieldType = AhFieldItem::Street; break;
        case AH::Common::FieldData::Location: fieldType = AhFieldItem::Location; break;
        case AH::Common::FieldData::OtherWorld: fieldType = AhFieldItem::OtherWorld; break;
        case AH::Common::FieldData::Sky: fieldType = AhFieldItem::Sky; break;
        case AH::Common::FieldData::SpaceAndTime: fieldType = AhFieldItem::LostInSpaceAndTime; break;
        case AH::Common::FieldData::Outskirts: fieldType = AhFieldItem::Outskirts; break;
        default:
        //Sky
        //SpaceAndTime,
        //Outskirts
            continue;
        }

        AH::Common::FieldData::FieldID id = parseId(parts[0]);
        QRect r = parseRect(parts[2], scale);

        AhFieldItem *f = new AhFieldItem(id, fieldType, r, scene->boardScaleFactor(), parent);
        f->setPos(r.center());
        f->initSubItems();
        QObject::connect(f, SIGNAL(itemInfoRequested(QString)), scene, SIGNAL(itemInfoRequested(QString)));
        QObject::connect(f, SIGNAL(fieldClicked(AH::Common::FieldData::FieldID)), scene, SIGNAL(fieldClicked(AH::Common::FieldData::FieldID)));
        QObject::connect(f, SIGNAL(fieldInfoRequested(AH::Common::FieldData::FieldID)), scene, SIGNAL(fieldInfoRequested(AH::Common::FieldData::FieldID)));
        scene->m_fieldMap[f->id()] = f;
    }
}

void AhBoardfillerHelper::initTerror(AhBoardScene *scene, QGraphicsItem *parent, double scale)
{
    Q_UNUSED(parent);
    QFile f(":/core/data/terrorfields");
    f.open(QIODevice::ReadOnly);
    QTextStream ts(&f);

    forever {
        QString l = ts.readLine();
        if (l.isEmpty()) break;
        QStringList parts = l.split(";");
        if (parts.length() != 2) continue;
        bool okx, oky;
        qreal x = parts[0].toDouble(&okx);
        qreal y = parts[1].toDouble(&oky);
        if (!okx || !oky) continue;
        scene->m_terrorPositions << QPointF(x*scale, y*scale);
    }
}

AH::Common::FieldData::FieldID AhBoardfillerHelper::parseId(const QString &id)
{
    if (id== "NS_Northside") return AH::Common::FieldData::NS_Northside;
    if (id== "NS_TrainStation") return AH::Common::FieldData::NS_TrainStation;
    if (id== "NS_Newspaper") return AH::Common::FieldData::NS_Newspaper;
    if (id== "NS_CuriositieShoppe") return AH::Common::FieldData::NS_CuriositieShoppe;
    if (id== "DT_Downtown") return AH::Common::FieldData::DT_Downtown;
    if (id== "DT_BankOfArhham") return AH::Common::FieldData::DT_BankOfArhham;
    if (id== "DT_ArkhamAsylum") return AH::Common::FieldData::DT_ArkhamAsylum;
    if (id== "DT_IndependenceSquare") return AH::Common::FieldData::DT_IndependenceSquare;
    if (id== "ET_Easttown") return AH::Common::FieldData::ET_Easttown;
    if (id== "ET_HibbsRoadhouse") return AH::Common::FieldData::ET_HibbsRoadhouse;
    if (id== "ET_VelmasDiner") return AH::Common::FieldData::ET_VelmasDiner;
    if (id== "ET_PoliceStation") return AH::Common::FieldData::ET_PoliceStation;
    if (id== "MD_MerchantDist") return AH::Common::FieldData::MD_MerchantDist;
    if (id== "MD_UnvisitedIsle") return AH::Common::FieldData::MD_UnvisitedIsle;
    if (id== "MD_RiverDocks") return AH::Common::FieldData::MD_RiverDocks;
    if (id== "MD_TheUnnamable") return AH::Common::FieldData::MD_TheUnnamable;
    if (id== "RT_Rivertown") return AH::Common::FieldData::RT_Rivertown;
    if (id== "RT_Graveyard") return AH::Common::FieldData::RT_Graveyard;
    if (id== "RT_BlackCave") return AH::Common::FieldData::RT_BlackCave;
    if (id== "RT_GeneralStore") return AH::Common::FieldData::RT_GeneralStore;
    if (id== "MU_MiskatonicU") return AH::Common::FieldData::MU_MiskatonicU;
    if (id== "MU_ScienceBuilding") return AH::Common::FieldData::MU_ScienceBuilding;
    if (id== "MU_Administration") return AH::Common::FieldData::MU_Administration;
    if (id== "MU_Library") return AH::Common::FieldData::MU_Library;
    if (id== "FH_FrenchHill") return AH::Common::FieldData::FH_FrenchHill;
    if (id== "FH_TheWitchHouse") return AH::Common::FieldData::FH_TheWitchHouse;
    if (id== "FH_SilverTwilightLodge") return AH::Common::FieldData::FH_SilverTwilightLodge;
    if (id== "UT_Uptown") return AH::Common::FieldData::UT_Uptown;
    if (id== "UT_StMarysHospital") return AH::Common::FieldData::UT_StMarysHospital;
    if (id== "UT_YeOldeMagickShoppe") return AH::Common::FieldData::UT_YeOldeMagickShoppe;
    if (id== "UT_Woods") return AH::Common::FieldData::UT_Woods;
    if (id== "SS_Southside") return AH::Common::FieldData::SS_Southside;
    if (id== "SS_MasBoardingHouse") return AH::Common::FieldData::SS_MasBoardingHouse;
    if (id== "SS_SouthChurch") return AH::Common::FieldData::SS_SouthChurch;
    if (id== "SS_HistoricalSociety") return AH::Common::FieldData::SS_HistoricalSociety;
    if (id== "OW_AnotherDimension") return AH::Common::FieldData::OW_AnotherDimension;
    if (id== "OW_Abyss") return AH::Common::FieldData::OW_Abyss;
    if (id== "OW_CityOfGreatRace") return AH::Common::FieldData::OW_CityOfGreatRace;
    if (id== "OW_Yuggoth") return AH::Common::FieldData::OW_Yuggoth;
    if (id== "OW_Celeano") return AH::Common::FieldData::OW_Celeano;
    if (id== "OW_TheDreamlands") return AH::Common::FieldData::OW_TheDreamlands;
    if (id== "OW_PlateauOfLeng") return AH::Common::FieldData::OW_PlateauOfLeng;
    if (id== "OW_R_lyeh") return AH::Common::FieldData::OW_R_lyeh;
    if (id== "Sp_Sky") return AH::Common::FieldData::Sp_Sky;
    if (id== "Sp_Outskirts") return AH::Common::FieldData::Sp_Outskirts;
    if (id== "Sp_SpaceAndTime") return AH::Common::FieldData::Sp_SpaceAndTime;
    Q_ASSERT(false);
    return AH::Common::FieldData::NO_NO_FIELD;
}

AH::Common::FieldData::FieldType AhBoardfillerHelper::parseType(const QString &type)
{
    if (type == "Street") return AH::Common::FieldData::Street;
    if (type == "Location") return AH::Common::FieldData::Location;
    if (type == "OtherWorld") return AH::Common::FieldData::OtherWorld;
    if (type == "Sky") return AH::Common::FieldData::Sky;
    if (type == "SpaceAndTime") return AH::Common::FieldData::SpaceAndTime;
    if (type == "Outskirts") return AH::Common::FieldData::Outskirts;
    Q_ASSERT(false);
    return AH::Common::FieldData::Street;
}

QRect AhBoardfillerHelper::parseRect(const QString &r, double scale)
{
    QStringList parts = r.split(",");
    return QRect(scale*parts[0].toInt(), scale*parts[1].toInt(), scale*parts[2].toInt(), scale*parts[3].toInt());
}

