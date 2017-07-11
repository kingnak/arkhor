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

    addItem(boardBkg);

    m_terrorItem = new QGraphicsPixmapItem(QPixmap(":/core/marker/terror_marker"));
    m_terrorItem->setVisible(false);
    addItem(m_terrorItem);
    setTerrorLevel(-1);
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

void AhBoardScene::setTerrorLevel(int level)
{
    if (level < 0 || level >= m_terrorPositions.size()) {
        m_terrorItem->setVisible(false);
        return;
    }
    m_terrorItem->setPos(m_terrorPositions[level] - QPointF(m_terrorItem->boundingRect().width(), m_terrorItem->boundingRect().height())/2);
    m_terrorItem->setVisible(true);
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
