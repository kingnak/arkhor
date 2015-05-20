#include "ahfielditem.h"
#include "itemstacker.h"
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
    m_fieldArea(NULL),
    m_clues(NULL),
    m_portal(NULL),
    m_specialMarker(NULL),
    m_thisCharacter(NULL)
{
    m_itemRect = QRectF(-rect.width()/2,-rect.height()/2,rect.width(),rect.height());
}

QRectF AhFieldItem::boundingRect() const
{
    return m_itemRect;
}

void AhFieldItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
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
    initPortalItem();

    /////////// TEST
    QGraphicsEllipseItem *i = new QGraphicsEllipseItem(-5,-5,10,10,this);
    i->setBrush(Qt::red);
    /////////// TEST
}

void AhFieldItem::initCharacterItem()
{
    m_characters = new ItemStacker;
    m_characters->setPicSize(QSize(STACK_ITEM_SIZE,STACK_ITEM_SIZE));
    m_characters->setAutoFillBackground(false);
    m_characters->setAttribute(Qt::WA_TranslucentBackground);
    connect(m_characters, SIGNAL(itemActivated(StackItem)), this, SLOT(characterClicked(StackItem)));
    QGraphicsProxyWidget *prxChar = new QGraphicsProxyWidget(this);
    prxChar->setWidget(m_characters);

    QRectF bound = boundingRect();
    qreal stkSize;
    if (m_type == Location) stkSize = qMin(bound.height()/2., STACK_ITEM_SIZE);
    else stkSize = qMin(bound.height(), STACK_ITEM_SIZE);

    prxChar->resize(stkSize, stkSize);
    prxChar->setPos(bound.topLeft());

    ////////// TEST
    m_characters->addItem(StackItem(QPixmap(":/test/client_resources/test/DarkYoung.png"), "", "Dark Young"));
    m_characters->addItem(StackItem(QPixmap(":/test/client_resources/test/ElderThing.png"), "", "Elder Thing"));
    m_characters->addItem(StackItem(QPixmap(":/test/client_resources/test/Chthonian.png"), "", "Chthonian"));
    ////////// TEST
}

void AhFieldItem::initMonsterItem()
{
    m_monsters = new ItemStacker;
    m_monsters->setPicSize(QSize(STACK_ITEM_SIZE,STACK_ITEM_SIZE));
    m_monsters->setAutoFillBackground(false);
    m_monsters->setAttribute(Qt::WA_TranslucentBackground);
    connect(m_monsters, SIGNAL(itemActivated(StackItem)), this, SLOT(monsterClicked(StackItem)));
    QGraphicsProxyWidget *prxMonst = new QGraphicsProxyWidget(this);
    prxMonst->setWidget(m_monsters);

    QRectF bound = boundingRect();
    qreal stkSize;
    if (m_type == Location) stkSize = qMin(bound.height()/2., STACK_ITEM_SIZE);
    else stkSize = qMin(bound.height(), STACK_ITEM_SIZE);

    prxMonst->resize(stkSize, stkSize);
    prxMonst->setPos(bound.topRight().x()-prxMonst->size().width(), bound.topRight().y());


    ////////// TEST
    m_monsters->addItem(StackItem(QPixmap(":/test/client_resources/test/Byakhee.png"), "", "Byakhee"));
    m_monsters->addItem(StackItem(QPixmap(":/test/client_resources/test/Cultist.png"), "", "Cultist"));
    m_monsters->addItem(StackItem(QPixmap(":/test/client_resources/test/Dhole.png"), "", "Dhole"));
    ////////// TEST
}

void AhFieldItem::initSpecialItem()
{
    m_specialMarker = new QGraphicsPixmapItem(this);
    if (m_type == Street)
        m_specialMarker->setPos(-SPECLIAL_ITEM_SIZE, -SPECLIAL_ITEM_SIZE/2);
    else
        m_specialMarker->setPos(-SPECLIAL_ITEM_SIZE/2, -SPECLIAL_ITEM_SIZE/2);
    m_specialMarker->setPixmap(QPixmap(":/core/marker/activity_1").scaled(QSize(SPECLIAL_ITEM_SIZE, SPECLIAL_ITEM_SIZE)));
}

void AhFieldItem::initThisCharacterItem()
{
    m_thisCharacter = new QGraphicsPixmapItem(this);
    m_thisCharacter->setPos(0, -THIS_CHAR_ITEM_SIZE.height()/2);
}

void AhFieldItem::initClickAreaItem()
{
    m_fieldArea = new ClickAreaItem(m_fieldRect, this);
    m_fieldArea->setZValue(-1);
    m_fieldArea->setActive(true);
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

void AhFieldItem::initPortalItem()
{
    if (m_type != Location)
        return;
    QPixmap port = QPixmap(":/test/client_resources/ah-lurker-monstergate.png").scaled(PORTAL_SIZE, PORTAL_SIZE);
    m_portal = new QGraphicsPixmapItem(port, this);
    m_portal->setZValue(-2);
    m_portal->setPos(-PORTAL_SIZE/2,-PORTAL_SIZE/2);
}

void AhFieldItem::fieldAreaClicked()
{
}

void AhFieldItem::characterClicked(const StackItem &itm)
{
    emit itemInfoRequested(itm.data().toString());
}

void AhFieldItem::monsterClicked(const StackItem &itm)
{
    emit itemInfoRequested(itm.data().toString());
}

////////////////////////////////

ClickAreaItem::ClickAreaItem(QRectF r, AhFieldItem *parent)
    : QGraphicsRectItem(r, parent), m_isActive(false), m_mouseIn(false), m_mouseDown(false)
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

void ClickAreaItem::setActive(bool active)
{
    m_isActive = active;
    updateColor();
}

void ClickAreaItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
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
    m_mouseIn = true;
    updateColor();
}

void ClickAreaItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
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
        m_text->setPos(0,0);
    }
}

void ClueAreaItem::init()
{
    m_icon = new QGraphicsPixmapItem(QPixmap(":/core/marker/clue").scaled(this->boundingRect().size().toSize()), this);
    m_text = new QGraphicsTextItem(this);
    m_text->setFont(QFont("Arial", 16));

    setClueCount(0);
}
