#include "ahfielditem.h"
#include "itemstacker.h"
#include "registryobjectstackitem.h"
#include "resourcepool.h"
#include "objectregistry.h"
#include "gatewidget.h"
#include <QtGui>
#include <QtWidgets>
#include "ahboardscene.h"
#include "monsterwidget.h"
#include "characterwidget.h"

static const double STACK_ITEM_SIZE = 75;
static const double SPECLIAL_ITEM_SIZE = 25;
static const double CLUE_ITEM_SIZE = 25;
static const double THIS_CHAR_ITEM_W = 50;
static const double THIS_CHAR_ITEM_H = 75;
static const double PORTAL_SIZE = 75;

AhFieldItem::AhFieldItem(AH::Common::FieldData::FieldID id, FieldItemType type, QRectF rect, double scale, QGraphicsItem *parent)
:   QGraphicsObject(parent),
    m_id(id),
    m_locked(false),
    m_type(type),
    m_monsters(NULL),
    m_characters(NULL),
    m_secondPhaseCharacters(NULL),
//    m_secondPhaseCharacters(NULL),
    m_infoArea(NULL),
    m_fieldArea(NULL),
    m_clues(NULL),
    m_gate(NULL),
    m_specialMarker(NULL),
    m_thisCharacter(NULL),
    m_prxMonst(NULL),
    m_prxChar(NULL),
    m_prxChar2nd(NULL),
    m_scale(scale)
{
    m_itemRect = QRectF(-rect.width()/2,-rect.height()/2,rect.width(),rect.height());
    //this->setFlag(QGraphicsItem::ItemIsMovable, true);
}

QRectF AhFieldItem::boundingRect() const
{
    return m_itemRect;
}

void AhFieldItem::setClickable(bool clickable)
{
    if (m_fieldArea) m_fieldArea->setActive(clickable);
}

void AhFieldItem::setCurrentField(bool cur)
{
    if (m_fieldArea) m_fieldArea->setCurrent(cur);
}

void AhFieldItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
    /*
    auto r = this->mapToScene(this->boundingRect()).boundingRect();
    painter->fillRect(m_itemRect,QColor(0,0,0,128));
    painter->fillRect(0, 0, 100, 24, Qt::white);
    painter->drawText(0, 20, QString("%1,%2").arg(r.left()/m_scale).arg(r.top()/m_scale));
    */
}

void AhFieldItem::initSubItems()
{
    if (m_fieldRect.isEmpty()) {
        m_fieldRect = boundingRect();
    }

    initInfoItem();
    initCharacterItem();
    initMonsterItem();
    initSpecialItem();
    initThisCharacterItem();
    initClickAreaItem();
    initClueItem();
    initGateItem();

    /////////// TEST
    //QGraphicsEllipseItem *i = new QGraphicsEllipseItem(-5,-5,10,10,this);
    //i->setBrush(Qt::red);
    /////////// TEST
}

void AhFieldItem::updateFromData(AH::Common::GameFieldData data)
{
    if (m_secondPhaseCharacters) m_secondPhaseCharacters->clear();
    if (m_characters) {
        m_characters->clear();        
        for (QString id : data.characterIds()) {
            if (m_secondPhaseCharacters && data.secondPhaseCharacterIds().contains(id)) {
                m_secondPhaseCharacters->addItem(new CharacterStackItem(id, m_scale));
            } else {
                m_characters->addItem(new CharacterStackItem(id, m_scale));
            }
        }
    }

    if (m_monsters) {
        m_monsters->clear();
        foreach (QString id, data.monsterIds()) {
            m_monsters->addItem(new MonsterStackItem(id, m_scale));
        }
    }

    if (m_clues) m_clues->setClueCount(data.clueAmount());

    if (m_gate) m_gate->setGateId(data.gateId());

    m_locked = data.isLocked();
    if (m_specialMarker) {
        m_specialMarker->setVisible(false);
        if (data.isSealed()) {
            m_specialMarker->setPixmap(QPixmap(":/core/marker/elder_sign").scaled(SPECLIAL_ITEM_SIZE*m_scale, SPECLIAL_ITEM_SIZE*m_scale));
            m_specialMarker->setVisible(true);
        }
        if (m_locked) {
            m_specialMarker->setPixmap(QPixmap(":/core/marker/location_locked"));
            m_specialMarker->setVisible(true);
        }
        if (data.hasSpecialAction()) {
            // TODO: set marker according to number...
            m_specialMarker->setPixmap(QPixmap(":/core/marker/activity_1").scaled(SPECLIAL_ITEM_SIZE*m_scale, SPECLIAL_ITEM_SIZE*m_scale));
            m_specialMarker->setVisible(true);
        }
    }
}

void AhFieldItem::initCharacterItem()
{
    if (m_type == Sky) return;
    if (m_type == Outskirts) return;

    m_characters = new ItemStacker;
    m_characters->setPicSize(QSize(STACK_ITEM_SIZE*m_scale,STACK_ITEM_SIZE*m_scale));
    m_characters->setFont(getItemFont());
    m_characters->setAutoFillBackground(false);
    m_characters->setAttribute(Qt::WA_TranslucentBackground);
    connect(m_characters, SIGNAL(itemActivated(const StackItem*)), this, SLOT(characterClicked(const StackItem*)));
    m_prxChar = new QGraphicsProxyWidget(this);
    m_prxChar->setWidget(m_characters);

    QRectF bound = boundingRect();
    qreal stkSize;
    if (m_type == Location) stkSize = qMin(bound.height()/2., STACK_ITEM_SIZE*m_scale);
    else if (m_type == Street) stkSize = qMin(bound.height(), STACK_ITEM_SIZE*m_scale);
    else if (m_type == OtherWorld) stkSize = qMin(bound.height(), STACK_ITEM_SIZE*m_scale);
    else stkSize = qMin(bound.height(), STACK_ITEM_SIZE*m_scale);

    m_prxChar->resize(stkSize, stkSize);
    m_prxChar->setPos(bound.topLeft());

    if (m_type == OtherWorld) {
        m_secondPhaseCharacters = new ItemStacker;
        m_secondPhaseCharacters->setPicSize(QSize(STACK_ITEM_SIZE*m_scale,STACK_ITEM_SIZE*m_scale));
        m_secondPhaseCharacters->setAutoFillBackground(false);
        m_secondPhaseCharacters->setAttribute(Qt::WA_TranslucentBackground);
        connect(m_secondPhaseCharacters, SIGNAL(itemActivated(const StackItem*)), this, SLOT(characterClicked(const StackItem*)));
        m_prxChar2nd = new QGraphicsProxyWidget(this);
        m_prxChar2nd->setWidget(m_secondPhaseCharacters);
        m_prxChar2nd->resize(stkSize, stkSize);
        m_prxChar->setPos(0,0);
        m_prxChar2nd->setPos(bound.topLeft());
    }

}

void AhFieldItem::initMonsterItem()
{
    if (m_type == OtherWorld) return;
    if (m_type == LostInSpaceAndTime) return;

    m_monsters = new ItemStacker;
    m_monsters->setFont(getItemFont());
    //m_monsters->setPicSize(QSize(STACK_ITEM_SIZE*m_scale,STACK_ITEM_SIZE*m_scale));
    m_monsters->setPicSize(QSize(200,200));
    m_monsters->setAutoFillBackground(false);
    m_monsters->setAttribute(Qt::WA_TranslucentBackground);
    connect(m_monsters, SIGNAL(itemActivated(const StackItem*)), this, SLOT(monsterClicked(const StackItem*)));
    m_prxMonst = new QGraphicsProxyWidget(this);
    m_prxMonst->setWidget(m_monsters);

    QRectF bound = boundingRect();
    qreal stkSize;
    if (m_type == Location) stkSize = qMin(bound.height()/2., STACK_ITEM_SIZE*m_scale);
    else stkSize = qMin(bound.height(), STACK_ITEM_SIZE*m_scale);

    //prxMonst->resize(stkSize, stkSize);

    m_prxMonst->resize(200,200);
    double fact = stkSize/200.;
    //prxMonst->scale(fact, fact);
    m_prxMonst->setScale(fact);

    m_prxMonst->setPos(bound.topRight().x()-stkSize, bound.topRight().y());
}

void AhFieldItem::initSpecialItem()
{
    if (m_type == OtherWorld) return;
    if (m_type == Sky) return;
    if (m_type == Outskirts) return;
    if (m_type == LostInSpaceAndTime) return;

    m_specialMarker = new QGraphicsPixmapItem(this);
    m_specialMarker->setFlag(QGraphicsItem::ItemClipsToShape, true);
    if (m_type == Street)
        m_specialMarker->setPos(-SPECLIAL_ITEM_SIZE*m_scale, -SPECLIAL_ITEM_SIZE*m_scale/2);
    else
        m_specialMarker->setPos(-SPECLIAL_ITEM_SIZE*m_scale/2, -SPECLIAL_ITEM_SIZE*m_scale/2);
    //m_specialMarker->setPixmap(QPixmap(":/core/marker/activity_1").scaled(QSize(SPECLIAL_ITEM_SIZE*m_scale, SPECLIAL_ITEM_SIZE*m_scale)));
}

void AhFieldItem::initThisCharacterItem()
{
    if (m_type == Sky) return;
    if (m_type == Outskirts) return;

    m_thisCharacter = new QGraphicsPixmapItem(this);
    if (m_type == Location)
        m_thisCharacter->setPos(-THIS_CHAR_ITEM_W*m_scale/2, m_itemRect.bottom()-THIS_CHAR_ITEM_H*m_scale);
    else
        m_thisCharacter->setPos(0, -THIS_CHAR_ITEM_H*m_scale/2);

    // TODO

    //QPixmap p = QPixmap(":/test/client_resources/test/jenny_barnes_figure.png").scaled(THIS_CHAR_ITEM_SIZE.toSize());
    //m_thisCharacter->setPixmap(p);
}

void AhFieldItem::initClickAreaItem()
{
    if (m_type == OtherWorld) return;
    if (m_type == Sky) return;
    if (m_type == Outskirts) return;
    if (m_type == LostInSpaceAndTime) return;

    m_fieldArea = new ClickAreaItem(m_fieldRect, this);
    m_fieldArea->setZValue(-1);
    m_fieldArea->setActive(false);
}

void AhFieldItem::initClueItem()
{
    if (m_type != Location)
        return;
    QRectF r(0,0,CLUE_ITEM_SIZE*m_scale,CLUE_ITEM_SIZE*m_scale);
    m_clues = new ClueAreaItem(r, m_scale, this);
    m_clues->setPos(boundingRect().bottomLeft().x(), boundingRect().bottomLeft().y()-m_clues->boundingRect().width());
    m_clues->setClueCount(3);
}

void AhFieldItem::initGateItem()
{
    if (m_type != Location)
        return;
    //m_gate = new GateItem(QRectF(0,0,PORTAL_SIZE*m_scale,PORTAL_SIZE*m_scale), this);
    m_gate = new GateItem(QRectF(-80,-80,160,160), this);
    m_gate->setScale(PORTAL_SIZE*m_scale/160.);
    m_gate->setZValue(-2);
    //m_gate->setPos(-PORTAL_SIZE*m_scale/2,-PORTAL_SIZE*m_scale/2);
}

void AhFieldItem::fieldAreaClicked()
{
    if (m_fieldArea && m_fieldArea->isActive()) {
        emit fieldClicked(m_id);
    } else if (m_infoArea) {
        emit fieldInfoRequested(m_id);
    }
}

void AhFieldItem::runAnimation(const QVariant &start, const QVariant &end, int duration, std::function<void (const QVariant &)> update)
{
    QVariantAnimation anim;
    anim.setDuration(duration);
    anim.setStartValue(start);
    anim.setEndValue(end);
    connect(&anim, &QVariantAnimation::valueChanged, update);
    runAnimation(&anim);
}

void AhFieldItem::runAnimation(QAbstractAnimation *anim)
{
    QEventLoop loop;
    connect(anim, &QVariantAnimation::finished, &loop, &QEventLoop::quit);
    anim->start();
    loop.exec();
}

QFont AhFieldItem::getItemFont(int pxSize, bool bold)
{
    QFont f = ResourcePool::instance()->loadMainFont();
    f.setBold(bold);
    f.setPixelSize(pxSize);
    return f;
}

void AhFieldItem::animateGateAppear(QString id)
{
    if (!m_gate) return;
    if (!m_gate->gateId().isEmpty()) return;
    qobject_cast<AhBoardScene*>(scene())->centerOn(this);
    m_gate->setOpacity(0);
    m_gate->setGateId(id);
    runAnimation(0., 1., 500, [=](auto v) {
        m_gate->setOpacity(v.toDouble());
        m_gate->setRotation(180-v.toDouble()*180);
        m_gate->setScale(v.toDouble());
        m_gate->update();
    });
}

void AhFieldItem::animateGateDisappear()
{
    if (!m_gate) return;
    if (m_gate->gateId().isEmpty()) return;
    qobject_cast<AhBoardScene*>(scene())->centerOn(this);
    runAnimation(1., 0., 500, [=](auto v) {
        m_gate->setOpacity(v.toDouble());
        m_gate->setScale(1.-v.toDouble());
        m_gate->update();
    });
    m_gate->unsetGate();
    m_gate->setOpacity(1);
}

void AhFieldItem::animateGateOpen(QString id)
{
    if (!m_gate) return;
    if (m_gate->gateId().isEmpty()) return;

    // Make a dummy overlay
    QGraphicsPixmapItem itm(this);
    itm.setPos(m_gate->pos());
    itm.moveBy(-m_gate->boundingRect().width()/2, -m_gate->boundingRect().height()/2);
    itm.setScale(m_gate->scale());
    AH::Common::GateData g;
    QPixmap src = GateWidget::drawGate(&g).scaled(m_gate->boundingRect().size().toSize());
    itm.setPixmap(src);

    qobject_cast<AhBoardScene*>(scene())->centerOn(this);
    runAnimation(0., 1., 500, [&](auto v) {
        QRadialGradient gr(QPointF(src.width()/2., src.height()/2.), src.width()/2.);
        gr.setColorAt(1, Qt::transparent);
        gr.setColorAt(v.toDouble()+.25, Qt::transparent);
        gr.setColorAt(v.toDouble(), qRgba(0,0,0,128));
        gr.setColorAt(v.toDouble()-2.5, Qt::black);
        gr.setColorAt(0, Qt::black);

        QPixmap res(src.size());
        res.fill(Qt::transparent);
        QPainter p(&res);
        p.drawPixmap(0, 0, src);
        p.setCompositionMode(QPainter::CompositionMode_DestinationOut);
        p.fillRect(res.rect(), gr);
        itm.setPixmap(res);
    });
}

void AhFieldItem::animateMonsterAppear(AH::Common::MonsterData m)
{
    // Dummy item
    auto itm = createOverlayMonster(m);
    itm->setOpacity(0);
    scene()->addItem(itm);
    qobject_cast<AhBoardScene*>(scene())->centerOn(this);
    runAnimation(0., 1., 500, [&](auto v) { itm->setOpacity(v.toDouble()); });
    m_monsters->addItem(new MonsterStackItem(m.id(), m_scale));
    delete itm;
}

void AhFieldItem::animateMonsterDisappear(AH::Common::MonsterData m)
{
    // TODO: needed? or different way in defeat?
}

void AhFieldItem::animateMonsterMove(AH::Common::MonsterData m, QList<AH::Common::FieldData::FieldID> path)
{
    auto scn = qobject_cast<AhBoardScene*>(scene());
    m_monsters->removeById(m.id());
    scn->getField(path.last())->m_monsters->removeById(m.id());

    auto itm = createOverlayMonster(m);
    scn->addItem(itm);
    scn->centerOn(this);

    QSequentialAnimationGroup grp;
    // 1st element is start
    for (int i = 1; i < path.size(); ++i) {
        QVariantAnimation *anim = new QVariantAnimation;
        anim->setDuration(500);

        QPointF s = scn->getField(path[i-1])->getMonstersGlobalPos();
        QPointF e = scn->getField(path[i])->getMonstersGlobalPos();

        anim->setStartValue(s);
        anim->setEndValue(e);
        connect(anim, &QVariantAnimation::valueChanged, [&](auto v) {
           itm->setPos(v.toPointF());
           scn->centerOn(v.toPointF());
        });

        grp.addAnimation(anim);
        grp.addPause(500);
    }

    runAnimation(&grp);
    scn->getField(path.last())->m_monsters->addItem(new MonsterStackItem(m.id(), m_scale));
    delete itm;
}

void AhFieldItem::animateCharacterMove(AH::Common::CharacterData c, QList<AH::Common::FieldData::FieldID> path)
{
    auto scn = qobject_cast<AhBoardScene*>(scene());
    auto endField = scn->getField(path.last());

    m_characters->removeById(c.id());
    if (m_secondPhaseCharacters) m_secondPhaseCharacters->removeById(c.id());
    endField->m_characters->removeById(c.id());
    if (endField->m_secondPhaseCharacters) endField->m_secondPhaseCharacters->removeById(c.id());

    auto itm = createOverlayCharacter(c, path.first() & AH::Common::FieldData::OWF_2ndFieldFlag);
    scn->addItem(itm);
    scn->centerOn(this);

    QSequentialAnimationGroup grp;
    // 1st element is start
    for (int i = 1; i < path.size(); ++i) {
        QVariantAnimation *anim = new QVariantAnimation;
        anim->setDuration(500);

        QPointF s = scn->getField(path[i-1])->getCharacterGlobalPos(path[i-1] & AH::Common::FieldData::OWF_2ndFieldFlag);
        QPointF e = scn->getField(path[i])->getCharacterGlobalPos(path[i]  & AH::Common::FieldData::OWF_2ndFieldFlag);

        anim->setStartValue(s);
        anim->setEndValue(e);
        connect(anim, &QVariantAnimation::valueChanged, [&](auto v) {
           itm->setPos(v.toPointF());
           scn->centerOn(v.toPointF());
        });

        grp.addAnimation(anim);
        grp.addPause(500);
    }

    runAnimation(&grp);
    //scn->getField(path.last())->m_monsters->addItem(new MonsterStackItem(m.id(), m_scale));

    if ((path.last() & AH::Common::FieldData::OWF_2ndFieldFlag) && endField->m_secondPhaseCharacters)
        endField->m_secondPhaseCharacters->addItem(new CharacterStackItem(c.id(), m_scale));
    else
        endField->m_characters->addItem(new CharacterStackItem(c.id(), m_scale));
    delete itm;
}

QGraphicsItem *AhFieldItem::createOverlayMonster(AH::Common::MonsterData m)
{
    QGraphicsPixmapItem *itm = new QGraphicsPixmapItem;

    auto s = m_prxMonst->mapToScene(m_prxMonst->boundingRect()).boundingRect().adjusted(0,0,-5,-5).size().toSize();
    auto p = getMonstersGlobalPos();

    itm->setPos(p);
    itm->setPixmap(MonsterFrontWidget::drawMonster(&m, m_scale).scaled(s));
    return itm;
}

QPointF AhFieldItem::getMonstersGlobalPos()
{
    return m_prxMonst->mapToScene(m_prxMonst->boundingRect().topLeft());
}

QGraphicsItem *AhFieldItem::createOverlayCharacter(AH::Common::CharacterData c, bool secondField)
{
    QGraphicsPixmapItem *itm = new QGraphicsPixmapItem;

    auto s = m_prxChar->mapToScene(m_prxChar->boundingRect()).boundingRect().adjusted(0,0,-5,-5).size().toSize();
    auto p = getCharacterGlobalPos(secondField);

    itm->setPos(p);
    itm->setPixmap(CharacterWidget::drawCharacterWithStats(&c, m_scale).scaled(s));
    return itm;
}

QPointF AhFieldItem::getCharacterGlobalPos(bool secondField)
{
    if (secondField && m_prxChar2nd)
        return m_prxChar2nd->mapToScene(m_prxChar2nd->boundingRect().topLeft());
    return m_prxChar->mapToScene(m_prxChar->boundingRect().topLeft());
}

void AhFieldItem::characterClicked(const StackItem *itm)
{
    if (itm)
        emit itemInfoRequested(itm->data().toString());
}

void AhFieldItem::monsterClicked(const StackItem *itm)
{
    if (itm)
        emit itemInfoRequested(itm->data().toString());
}

void AhFieldItem::gateClicked(const GateItem *itm)
{
    if (itm)
        emit itemInfoRequested(itm->gateId());
}

void AhFieldItem::initInfoItem()
{
    m_infoArea = new ClickAreaItem(m_fieldRect, this);
    m_infoArea->setNoHighlight(true);
    m_infoArea->setActive(true);
    m_infoArea->setZValue(-3);
}

////////////////////////////////

ClickAreaItem::ClickAreaItem(QRectF r, AhFieldItem *parent)
    : QGraphicsRectItem(r, parent), m_isActive(false), m_isCurrent(false), m_mouseIn(false), m_mouseDown(false)
{
    m_field = parent;
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setPen(QPen(Qt::NoPen));
    setFlag(QGraphicsItem::ItemClipsToShape, true);
}

void ClickAreaItem::setCurrent(bool cur)
{
    m_isCurrent = cur;
    updateColor();
}

void ClickAreaItem::setActive(bool active)
{
    m_isActive = active;
    updateColor();
}

void ClickAreaItem::setNoHighlight(bool noHighlight)
{
    m_noHighlight = noHighlight;
    updateColor();
}

void ClickAreaItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (!m_isActive) {
        event->ignore();
        return;
    }
    m_mouseDown = true;
}

void ClickAreaItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (!m_isActive) {
        event->ignore();
        return;
    }
    if (m_mouseDown) {
        m_mouseDown = false;
        if (boundingRect().contains(event->pos()))
            m_field->fieldAreaClicked();
    }
}

void ClickAreaItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (!m_isActive) {
        event->ignore();
        return;
    }

    m_mouseIn = true;
    updateColor();
}

void ClickAreaItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (!m_isActive) {
        event->ignore();
        return;
    }

    m_mouseIn = false;
    updateColor();
}

void ClickAreaItem::updateColor()
{
    if (m_noHighlight) {
        setBrush(QBrush(QColor(0,0,255,0)));
    } else if (m_isActive) {
        if (m_mouseIn) {
            setBrush(QBrush(QColor(0,0,255,128)));
        } else {
            setBrush(QBrush(QColor(0,0,255,75)));
        }
    } else if (m_isCurrent) {
        setBrush(QBrush(QColor(255,0,0,128)));
    } else {
        setBrush(QBrush(QColor(0,0,255,0)));
    }
    update();
}

//////////////////////////////

ClueAreaItem::ClueAreaItem(QRectF rect, double scale, QGraphicsItem *parent)
    : QGraphicsRectItem(rect, parent), m_clueCount(0)
{
    init(scale);
}

void ClueAreaItem::setClueCount(quint32 ct)
{
    m_clueCount = ct;
    if (ct == 0) {
        m_icon->setVisible(false);
        m_text->setVisible(false);
    } else if (ct == 1) {
        m_icon->setVisible(true);
        m_text->setVisible(false);
    } else {
        m_icon->setVisible(true);
        m_text->setPlainText(QString::number(ct));
        m_text->setVisible(true);
        m_text->adjustSize();
        m_text->setPos((m_icon->boundingRect().width()-m_text->boundingRect().width())/2, (m_icon->boundingRect().height()-m_text->boundingRect().height())/2);
    }
}

void ClueAreaItem::init(double scale)
{
    setPen(QPen(Qt::NoPen));
    m_icon = new QGraphicsPixmapItem(QPixmap(":/core/marker/clue").scaled(this->boundingRect().size().toSize()), this);
    m_text = new QGraphicsTextItem(this);
    m_text->setFont(AhFieldItem::getItemFont(16*scale, true));

    setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
    setClueCount(0);
}

//////////////////////////////

GateItem::GateItem(QRectF rect, AhFieldItem *parent)
    : QGraphicsRectItem(rect, parent), m_parent(parent)
{
    init();
}

void GateItem::setGateId(const QString id)
{
    if (id == m_gateId) return;

    if (!m_gateId.isEmpty()) {
        ObjectRegistry::instance()->unsubscribe(this, id);
        //this->setFlag(GraphicsItemFlag::Item)
    }
    m_gateId = id;
    if (m_gateId.isEmpty()) {
        m_pixmap->setPixmap(QPixmap());
    } else {
        ObjectRegistry::instance()->asyncSubscribeObject(this, m_gateId, AH::Common::RequestObjectsData::Gate);
    }
}

void GateItem::objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc)
{
    AH::Common::GateData g;
    desc.data >> g;
    m_pixmap->setPixmap(GateWidget::drawGate(&g).scaled(boundingRect().size().toSize()));
}

void GateItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    if (!m_gateId.isEmpty()) {
        m_parent->gateClicked(this);
    } else {
        QGraphicsItem::mousePressEvent(event);
    }
}

void GateItem::init()
{
    setPen(Qt::NoPen);
    m_pixmap = new QGraphicsPixmapItem(this);
    m_pixmap->moveBy(-boundingRect().width()/2, -boundingRect().height()/2);
    setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
}
