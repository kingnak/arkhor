#include "ahboardscene.h"
#include "gui/ahfielditem.h"
#include "ahboardfillerhelper.h"
#include <gamestatechangedata.h>
#include <objectdata.h>
#include <gatedata.h>
#include <monsterdata.h>
#include "objectregistry.h"

using namespace AH::Common;

AhBoardScene::AhBoardScene(QObject *parent) :
    QGraphicsScene(parent)
  , m_terrorItem(nullptr)
  , m_scale(1.0)
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
    m_terrorItem->setScale(m_scale);
    addItem(m_terrorItem);
    setTerrorLevel(-1);
}

AhFieldItem *AhBoardScene::getField(FieldData::FieldID id)
{
    if (id & AH::Common::FieldData::OWF_2ndFieldFlag)
        id = static_cast<AH::Common::FieldData::FieldID>(id & ~AH::Common::FieldData::OWF_2ndFieldFlag);
    return m_fieldMap.value(id);
}

void AhBoardScene::centerOn(AhFieldItem *f)
{
    emit requestCenterOn(f->id());
}

void AhBoardScene::centerOn(QPointF p)
{
    emit requestCenterOn(p);
}

void AhBoardScene::updateBoardFromData(QVariantMap boardMap)
{
    AH::Common::GameBoardChangeData changes;
    boardMap["_changes"] >> changes;
    animateChanges(changes);

    for (QString k : boardMap.keys()) {
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
    /*
    if (level < 0) {
        for (auto p : m_terrorPositions) {
            auto i = new QGraphicsPixmapItem(QPixmap(":/core/marker/terror_marker"));
            i->setScale(m_scale);
            i->setPos(p - QPointF(i->boundingRect().width(), i->boundingRect().height())/2);
            addItem(i);
            i->setFlag(QGraphicsItem::ItemIsMovable, true);
        }
        return;
    }
    */

    if (level < 0 || level >= m_terrorPositions.size()) {
        m_terrorItem->setVisible(false);
        return;
    }
    m_terrorItem->setPos(m_terrorPositions[level] - QPointF(m_terrorItem->boundingRect().width(), m_terrorItem->boundingRect().height())/2);
    m_terrorItem->setVisible(true);
}

void AhBoardScene::initNeighbourHoodFromBoardData(QList<AH::Common::GameFieldData> boardMap)
{
    if (!m_neighbours.isEmpty()) return;
    for (auto d : boardMap) {
        for (int nid : d.neighbourIds()) {
            AH::Common::FieldData::FieldID nfId = static_cast<FieldData::FieldID> (nid);
            m_neighbours[d.id()] << nfId;
        }
    }
}

void AhBoardScene::ensureAnimationObjectsKnown(const GameBoardChangeData &changes)
{
    AH::Common::RequestObjectsData reqs;
    for (auto m : changes.monsterAppear) {
        reqs.addRequest({AH::Common::RequestObjectsData::Monster, m.id});
    }
    for (auto m : changes.monsterMovements) {
        reqs.addRequest({AH::Common::RequestObjectsData::Monster, m.id});
    }
    for (auto g : changes.gateAppear) {
        reqs.addRequest({AH::Common::RequestObjectsData::Gate, g.id});
    }
    for (auto c : changes.characterMovements) {
        reqs.addRequest({AH::Common::RequestObjectsData::Character, c.id});
    }

    // Return can be ignored, will ask later
    ObjectRegistry::instance()->getObjectsBlocking(reqs);
}

void AhBoardScene::animateChanges(GameBoardChangeData changes)
{
    emit beginAnimation();
    ensureAnimationObjectsKnown(changes);
    auto reg = ObjectRegistry::instance();

    for (auto g : changes.gateDisappear) {
        auto f = this->getField(g.location);
        f->animateGateDisappear();
    }

    for (auto m : changes.monsterDisappear) {
        auto f = this->getField(m.location);
        auto monster = reg->getObject<MonsterData>(m.id);
        f->animateMonsterDisappear(monster);
    }

    for (auto g : changes.gateAppear) {
        auto f = this->getField(g.location);
        f->animateGateAppear(g.id);
    }

    for (auto m : changes.monsterAppear) {
        auto f = this->getField(m.location);
        auto monster = reg->getObject<MonsterData>(m.id);
        f->animateMonsterAppear(monster);
    }

    for (auto g : changes.gateOpen) {
        auto f = this->getField(g.location);
        f->animateGateOpen(g.id);
    }

    for (auto m : changes.monsterMovements) {
        auto f = this->getField(m.path.first());
        auto monster = reg->getObject<MonsterData>(m.id);
        f->animateMonsterMove(monster, m.path);
    }

    for (auto c : changes.characterMovements) {
        auto f = this->getField(c.path.first());
        auto chr = reg->getObject<CharacterData>(c.id);
        f->animateCharacterMove(chr, c.path);
    }

    for (auto fc : changes.fieldChanges) {
        auto f = this->getField(fc.location);
        f->animateFieldStateChange(fc);
    }

    emit endAnimation();
}
