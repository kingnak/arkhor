#include "ahfielditem.h"
#include "itemstacker.h"
#include "registryobjectstackitem.h"
#include "resourcepool.h"
#include <QtGui>

static const double STACK_ITEM_SIZE = 75;
static const double SPECLIAL_ITEM_SIZE = 25;
static const double CLUE_ITEM_SIZE = 25;
static const QSizeF THIS_CHAR_ITEM_SIZE = QSize(50,75);
static const double PORTAL_SIZE = 75;

AhFieldItem::AhFieldItem(AH::Common::FieldData::FieldID id, FieldItemType type, QRectF rect, QGraphicsItem *parent)
:   QGraphicsObject(parent),
    m_id(id),
    m_type(type),
    m_monsters(NULL),
    m_characters(NULL),
//    m_secondPhaseCharacters(NULL),
    m_fieldArea(NULL),
    m_clues(NULL),
    m_gate(NULL),
    m_specialMarker(NULL),
    m_thisCharacter(NULL)
{
    m_itemRect = QRectF(-rect.width()/2,-rect.height()/2,rect.width(),rect.height());
}

QRectF AhFieldItem::boundingRect() const
{
    return m_itemRect;
}

void AhFieldItem::setClickable(bool clickable)
{
    if (m_fieldArea) m_fieldArea->setActive(clickable);
}

void AhFieldItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //painter->fillRect(m_itemRect,QColor(0,0,0,128));
}

void AhFieldItem::initSubItems()
{
    if (m_fieldRect.isEmpty()) {
        m_fieldRect = boundingRect();
    }

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
    //if (m_secondPhaseCharacters) m_secondPhaseCharacters->clear();
    if (m_characters) {
        m_characters->clear();        
        foreach (QString id, data.characterIds()) {
            m_characters->addItem(new StackItem(QPixmap(":/test/client_resources/test/jenny_barnes_figure.png"), "", id));
        }
    }

    if (m_monsters) {
        m_monsters->clear();
        foreach (QString id, data.monsterIds()) {
            //m_monsters->addItem(new StackItem(QPixmap(":/test/client_resources/test/Cultist.png"), "", id));
            m_monsters->addItem(new MonsterStackItem(id));
        }
    }

    if (m_clues) m_clues->setClueCount(data.clueAmount());

    if (m_gate) m_gate->setVisible(!data.gateId().isEmpty());

    if (m_specialMarker) {
        m_specialMarker->setVisible(false);
        if (data.isSealed()) {
            m_specialMarker->setPixmap(QPixmap(":/core/marker/elder_sign").scaled(SPECLIAL_ITEM_SIZE, SPECLIAL_ITEM_SIZE));
            m_specialMarker->setVisible(true);
        }
    }
}

void AhFieldItem::initCharacterItem()
{
    if (m_type == Sky) return;
    if (m_type == Outskirts) return;

    m_characters = new ItemStacker;
    m_characters->setPicSize(QSize(STACK_ITEM_SIZE,STACK_ITEM_SIZE));
    m_characters->setFont(getItemFont());
    m_characters->setAutoFillBackground(false);
    m_characters->setAttribute(Qt::WA_TranslucentBackground);
    connect(m_characters, SIGNAL(itemActivated(const StackItem*)), this, SLOT(characterClicked(const StackItem*)));
    QGraphicsProxyWidget *prxChar = new QGraphicsProxyWidget(this);
    prxChar->setWidget(m_characters);

    QRectF bound = boundingRect();
    qreal stkSize;
    if (m_type == Location) stkSize = qMin(bound.height()/2., STACK_ITEM_SIZE);
    else if (m_type == Street) stkSize = qMin(bound.height(), STACK_ITEM_SIZE);
    else if (m_type == OtherWorld) stkSize = qMin(bound.height(), STACK_ITEM_SIZE);

    prxChar->resize(stkSize, stkSize);
    prxChar->setPos(bound.topLeft());

    /*
    if (m_type == OtherWorld) {
        m_secondPhaseCharacters = new ItemStacker;
        m_secondPhaseCharacters->setPicSize(QSize(STACK_ITEM_SIZE,STACK_ITEM_SIZE));
        m_secondPhaseCharacters->setAutoFillBackground(false);
        m_secondPhaseCharacters->setAttribute(Qt::WA_TranslucentBackground);
        connect(m_secondPhaseCharacters, SIGNAL(itemActivated(StackItem)), this, SLOT(characterClicked(StackItem)));
        QGraphicsProxyWidget *prx2nd = new QGraphicsProxyWidget(this);
        prx2nd->setWidget(m_secondPhaseCharacters);
        prx2nd->resize(stkSize, stkSize);
        prxChar->setPos(0,0);
        prx2nd->setPos(bound.topLeft());
    }
    */
}

void AhFieldItem::initMonsterItem()
{
    if (m_type == OtherWorld) return;
    if (m_type == LostInSpaceAndTime) return;

    m_monsters = new ItemStacker;
    m_monsters->setFont(getItemFont());
    //m_monsters->setPicSize(QSize(STACK_ITEM_SIZE,STACK_ITEM_SIZE));
    m_monsters->setPicSize(QSize(200,200));
    m_monsters->setAutoFillBackground(false);
    m_monsters->setAttribute(Qt::WA_TranslucentBackground);
    connect(m_monsters, SIGNAL(itemActivated(const StackItem*)), this, SLOT(monsterClicked(const StackItem*)));
    QGraphicsProxyWidget *prxMonst = new QGraphicsProxyWidget(this);
    prxMonst->setWidget(m_monsters);

    QRectF bound = boundingRect();
    qreal stkSize;
    if (m_type == Location) stkSize = qMin(bound.height()/2., STACK_ITEM_SIZE);
    else stkSize = qMin(bound.height(), STACK_ITEM_SIZE);

    //prxMonst->resize(stkSize, stkSize);

    prxMonst->resize(200,200);
    double fact = stkSize/200.;
    prxMonst->scale(fact, fact);

    prxMonst->setPos(bound.topRight().x()-stkSize, bound.topRight().y());
}

void AhFieldItem::initSpecialItem()
{
    if (m_type == OtherWorld) return;
    if (m_type == Sky) return;
    if (m_type == Outskirts) return;
    if (m_type == LostInSpaceAndTime) return;

    m_specialMarker = new QGraphicsPixmapItem(this);
    if (m_type == Street)
        m_specialMarker->setPos(-SPECLIAL_ITEM_SIZE, -SPECLIAL_ITEM_SIZE/2);
    else
        m_specialMarker->setPos(-SPECLIAL_ITEM_SIZE/2, -SPECLIAL_ITEM_SIZE/2);
    //m_specialMarker->setPixmap(QPixmap(":/core/marker/activity_1").scaled(QSize(SPECLIAL_ITEM_SIZE, SPECLIAL_ITEM_SIZE)));
}

void AhFieldItem::initThisCharacterItem()
{
    if (m_type == Sky) return;
    if (m_type == Outskirts) return;

    m_thisCharacter = new QGraphicsPixmapItem(this);
    if (m_type == Location)
        m_thisCharacter->setPos(-THIS_CHAR_ITEM_SIZE.width()/2, m_itemRect.bottom()-THIS_CHAR_ITEM_SIZE.height());
    else
        m_thisCharacter->setPos(0, -THIS_CHAR_ITEM_SIZE.height()/2);

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
    QRectF r(0,0,CLUE_ITEM_SIZE,CLUE_ITEM_SIZE);
    m_clues = new ClueAreaItem(r, this);
    m_clues->setPos(boundingRect().bottomLeft().x(), boundingRect().bottomLeft().y()-m_clues->boundingRect().width());
    m_clues->setClueCount(3);
}

void AhFieldItem::initGateItem()
{
    if (m_type != Location)
        return;
    QPixmap port = QPixmap(":/test/client_resources/ah-lurker-monstergate.png").scaled(PORTAL_SIZE, PORTAL_SIZE);
    m_gate = new QGraphicsPixmapItem(port, this);
    m_gate->setZValue(-2);
    m_gate->setPos(-PORTAL_SIZE/2,-PORTAL_SIZE/2);
}

void AhFieldItem::fieldAreaClicked()
{
    //QMessageBox::information(NULL, "field", QString::number(this->id()));
    if (m_fieldArea->isActive()) {
        emit fieldClicked(m_id);
    }
}

QFont AhFieldItem::getItemFont(int pxSize, bool bold)
{
    QFont f = ResourcePool::instance()->loadMainFont();
    f.setBold(bold);
    f.setPixelSize(pxSize);
    return f;
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

////////////////////////////////

ClickAreaItem::ClickAreaItem(QRectF r, AhFieldItem *parent)
    : QGraphicsRectItem(r, parent), m_isActive(false), m_mouseIn(false), m_mouseDown(false)
{
    m_field = parent;
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setPen(QPen(Qt::NoPen));
}

void ClickAreaItem::setActive(bool active)
{
    m_isActive = active;
    updateColor();
}

void ClickAreaItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    m_mouseDown = true;
}

void ClickAreaItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_mouseDown) {
        m_mouseDown = false;
        if (boundingRect().contains(event->pos()))
            m_field->fieldAreaClicked();
    }
}

void ClickAreaItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    m_mouseIn = true;
    updateColor();
}

void ClickAreaItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    m_mouseIn = false;
    updateColor();
}

void ClickAreaItem::updateColor()
{
    if (m_isActive) {
        if (m_mouseIn) {
            setBrush(QBrush(QColor(0,0,255,128)));
        } else {
            setBrush(QBrush(QColor(0,0,255,75)));
        }
    } else {
        setBrush(QBrush(QColor(0,0,255,0)));
    }
    update();
}

//////////////////////////////

ClueAreaItem::ClueAreaItem(QRectF rect, QGraphicsItem *parent)
    : QGraphicsRectItem(rect, parent), m_clueCount(0)
{
    init();
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
        m_text->setPos(0, 0);
    }
}

void ClueAreaItem::init()
{
    setPen(QPen(Qt::NoPen));
    m_icon = new QGraphicsPixmapItem(QPixmap(":/core/marker/clue").scaled(this->boundingRect().size().toSize()), this);
    m_text = new QGraphicsTextItem(this);
    m_text->setFont(AhFieldItem::getItemFont(16, true));

    setClueCount(0);
}
