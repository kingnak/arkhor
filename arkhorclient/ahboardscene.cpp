#include "ahboardscene.h"
#include "gui/ahfielditem.h"
#include "ahboardfillerhelper.h"
#include <gamestatechangedata.h>
#include <objectdata.h>
#include <gatedata.h>
#include <monsterdata.h>
#include "objectregistry.h"
#include <QtGui>

using namespace AH::Common;

AhBoardScene::AhBoardScene(QObject *parent) :
    QGraphicsScene(parent)
  , m_terrorItem(nullptr)
  , m_terrorLevel(-1)
  , m_scale(1.0)
  , m_inUpdate(false)
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

void AhBoardScene::centerOn(AhFieldItem *f, bool animated, qreal zoom)
{
    emit requestCenterOn(f->id(), animated, zoom);
}

void AhBoardScene::centerOn(QPointF p, bool animated, qreal zoom)
{
    emit requestCenterOn(p, animated, zoom);
}

void AhBoardScene::updateBoardFromData(const QVariantMap &boardMap)
{
    m_pendingUpdates << boardMap;
    if (m_inUpdate) {
        return;
    }

    m_inUpdate = true;
    emit beginAnimation();
    while (!m_pendingUpdates.empty()) {
        auto u = m_pendingUpdates.takeFirst();
        applyUpdate(u);
    }
    emit endAnimation();
    m_inUpdate = false;
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

    animateTerrorLevel(level);
    m_terrorLevel = level;
    if (level < 0 || level >= m_terrorPositions.size()) {
        m_terrorItem->setVisible(false);
        return;
    }
    m_terrorItem->setPos(terrorLevelPosition(level));
    m_terrorItem->setVisible(true);
}

void AhBoardScene::initNeighbourHoodFromBoardData(const QList<AH::Common::GameFieldData> &boardMap)
{
    if (!m_neighbours.isEmpty()) return;
    for (const auto &d : boardMap) {
        for (int nid : d.neighbourIds()) {
            AH::Common::FieldData::FieldID nfId = static_cast<FieldData::FieldID> (nid);
            m_neighbours[d.id()] << nfId;
        }
    }
}

void AhBoardScene::applyUpdate(QVariantMap boardMap)
{
    AH::Common::GameBoardChangeData changes;
    boardMap["_changes"] >> changes;
    animateChanges(changes);

    for (const auto &k : boardMap.keys()) {
        int id = k.toInt();
        AH::Common::FieldData::FieldID fId = static_cast<FieldData::FieldID> (id);
        if (m_fieldMap.contains(fId)) {
            GameFieldData d;
            boardMap[k] >> d;
            m_fieldMap[fId]->updateFromData(d);
        }
    }
}

void AhBoardScene::ensureAnimationObjectsKnown(const GameBoardChangeData &changes)
{
    AH::Common::RequestObjectsData reqs;
    for (const auto &m : changes.monsterAppear) {
        reqs.addRequest({AH::Common::RequestObjectsData::Monster, m.id});
    }
    for (const auto &m : changes.monsterMovements) {
        reqs.addRequest({AH::Common::RequestObjectsData::Monster, m.id});
    }
    for (const auto &g : changes.gateAppear) {
        reqs.addRequest({AH::Common::RequestObjectsData::Gate, g.id});
    }
    for (const auto &c : changes.characterMovements) {
        reqs.addRequest({AH::Common::RequestObjectsData::Character, c.id});
    }

    // Return can be ignored, will ask later
    ObjectRegistry::instance()->getObjectsBlocking(reqs);
}

void AhBoardScene::animateChanges(const GameBoardChangeData &changes)
{
    ensureAnimationObjectsKnown(changes);
    auto reg = ObjectRegistry::instance();

    for (const auto &g : changes.gateAppear) {
        auto f = this->getField(g.location);
        f->animateGateAppear(g.id);
    }

    for (const auto &m : changes.monsterAppear) {
        auto f = this->getField(m.location);
        auto monster = reg->getObject<MonsterData>(m.id);
        f->animateMonsterAppear(monster);
    }

    for (const auto &g : changes.gateDisappear) {
        auto f = this->getField(g.location);
        f->animateGateDisappear();
    }

    for (auto m : changes.monsterMovements) {
        auto f = this->getField(m.path.first());
        auto monster = reg->getObject<MonsterData>(m.id);
        f->animateMonsterMove(monster, m.path);
    }

    for (const auto &m : changes.monsterDisappear) {
        auto f = this->getField(m.location);
        auto monster = reg->getObject<MonsterData>(m.id);
        f->animateMonsterDisappear(monster);
    }

    //animateTerrorLevel(changes.terrorLevel);

    if (!changes.clearOutskirts.isEmpty()) {
        auto f = this->getField(FieldData::FieldID::Sp_Outskirts);
        f->animateMultipleMonsterDisappear(changes.clearOutskirts);
    }

    for (const auto &g : changes.gateOpen) {
        auto f = this->getField(g.location);
        f->animateGateOpen(g.id);
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
}

void AhBoardScene::animateTerrorLevel(int level)
{
    if (m_terrorLevel < 0 || level < 0 || m_terrorLevel == level)
        return;

    QPointF old = terrorLevelPosition(m_terrorLevel);
    QPointF now = terrorLevelPosition(level);

    if (old.isNull() || now.isNull())
        return;

    QSequentialAnimationGroup grp;
    QVariantAnimation *anim = new QVariantAnimation;
    anim->setDuration(500);
    anim->setStartValue(old);
    anim->setEndValue(now);
    connect(anim, &QVariantAnimation::valueChanged, [=](auto v) {m_terrorItem->setPos(v.toPointF()); });
    grp.addPause(250);
    grp.addAnimation(anim);

    QPointF c = (terrorLevelPosition(0) + terrorLevelPosition(m_terrorPositions.size()-1)) / 2;
    centerOn(c, true);

    QEventLoop l;
    connect(&grp, &QAbstractAnimation::finished, &l, &QEventLoop::quit);
    grp.start();
    l.exec();
}

QPointF AhBoardScene::terrorLevelPosition(int lvl)
{
    if (lvl < 0 || lvl >= m_terrorPositions.size()) return {};
    return m_terrorPositions[lvl] - QPointF(m_terrorItem->boundingRect().width(), m_terrorItem->boundingRect().height())/2;
}
