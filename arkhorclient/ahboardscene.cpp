#include "ahboardscene.h"
#include "ahfielditem.h"

AhBoardScene::AhBoardScene(QObject *parent) :
    QGraphicsScene(parent)
{
}

void AhBoardScene::initBoard()
{
    QPixmap bkgImage(":/core/images/gameboard");
    setSceneRect(QRect(QPoint(0,0), bkgImage.size()));
    QGraphicsPixmapItem *boardBkg = new QGraphicsPixmapItem(bkgImage);

    {
        QRectF fRect(263,605,225,85);
        AhFieldItem *f = new AhFieldItem(AH::Common::FieldData::MD_MerchantDist, AhFieldItem::Street, fRect, boardBkg);
        f->setPos(fRect.center());
        f->initSubItems();
        connect(f, SIGNAL(itemInfoRequested(QString)), this, SIGNAL(itemInfoRequested(QString)));
    }

    {
        QRectF fRect(607,710,150,120);
        AhFieldItem *f = new AhFieldItem(AH::Common::FieldData::RT_GeneralStore, AhFieldItem::Location, fRect, boardBkg);
        f->setPos(fRect.center());
        f->initSubItems();
        connect(f, SIGNAL(itemInfoRequested(QString)), this, SIGNAL(itemInfoRequested(QString)));
    }

    addItem(boardBkg);
}
