#include "ahboardscene.h"
#include "gui/ahfielditem.h"
#include "ahboardfillerhelper.h"

using namespace AH::Common;

AhBoardScene::AhBoardScene(QObject *parent) :
    QGraphicsScene(parent)
{
}

void AhBoardScene::initBoard()
{
    QPixmap bkgImage(":/core/images/gameboard");
    setSceneRect(QRect(QPoint(0,0), bkgImage.size()));
    QGraphicsPixmapItem *boardBkg = new QGraphicsPixmapItem(bkgImage);

    AhBoardfillerHelper h;
    h.initBoard(this, boardBkg);

    /*
    {
        QRectF fRect(263,605,225,85);
        AhFieldItem *f = new AhFieldItem(AH::Common::FieldData::MD_MerchantDist, AhFieldItem::Street, fRect, boardBkg);
        f->setPos(fRect.center());
        f->initSubItems();
        connect(f, SIGNAL(itemInfoRequested(QString)), this, SIGNAL(itemInfoRequested(QString)));
        m_fieldMap[f->id()] = f;
    }

    {
        QRectF fRect(607,710,150,120);
        AhFieldItem *f = new AhFieldItem(AH::Common::FieldData::RT_GeneralStore, AhFieldItem::Location, fRect, boardBkg);
        f->setPos(fRect.center());
        f->initSubItems();
        connect(f, SIGNAL(itemInfoRequested(QString)), this, SIGNAL(itemInfoRequested(QString)));
        m_fieldMap[f->id()] = f;
    }
    */

    addItem(boardBkg);
}

void AhBoardScene::updateBoardFromData(QVariantMap boardMap)
{
    // Initialize neighbourhood if not done yet
    if (m_neighbours.isEmpty()) {
        initNeighbourHoodFromBoardData(boardMap);
    }

    foreach (QString k, boardMap.keys()) {
        int id = k.toInt();
        AH::Common::FieldData::FieldID fId = static_cast<FieldData::FieldID> (id);
        if (m_fieldMap.contains(fId)) {
            GameFieldData d;
            boardMap[k] >> d;
            m_fieldMap[fId]->updateFromData(d);
        }
    }
}

void AhBoardScene::initNeighbourHoodFromBoardData(QVariantMap boardMap)
{
    foreach (QString k, boardMap.keys()) {
        int id = k.toInt();
        AH::Common::FieldData::FieldID fId = static_cast<FieldData::FieldID> (id);
        GameFieldData d;
        boardMap[k] >> d;
        foreach (int nid, d.neighbourIds()) {
            AH::Common::FieldData::FieldID nfId = static_cast<FieldData::FieldID> (nid);
            m_neighbours[fId] << nfId;
        }
    }
}
