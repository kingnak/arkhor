#ifndef AHFIELDITEM_H
#define AHFIELDITEM_H

#include <QGraphicsItem>
#include <fielddata.h>
#include <gamefielddata.h>
#include "asyncobjectreceiver.h"

class ItemStacker;
class StackItem;

class ClueAreaItem;
class ClickAreaItem;
class GateItem;

class AhFieldItem : public QGraphicsObject
{
    Q_OBJECT
public:
    enum FieldItemType {
        Location = QGraphicsItem::UserType + 1,
        Street,
        OtherWorld,
        Sky,
        LostInSpaceAndTime,
        Outskirts
    };

    AhFieldItem(AH::Common::FieldData::FieldID id, FieldItemType type, QRectF rect, QGraphicsItem *parent = 0);

    AH::Common::FieldData::FieldID id() const { return m_id; }

    QRectF boundingRect() const;

    void setFieldRect(QRectF r);
    void setFieldExtent(QSizeF s);
    void setClickable(bool clickable);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 );

    void initSubItems();

    void updateFromData(AH::Common::GameFieldData data);

    static QFont getItemFont(int pxSize = 24, bool bold = true);

signals:
    void itemInfoRequested(QString id);
    void fieldClicked(AH::Common::FieldData::FieldID id);

private slots:
    void characterClicked(const StackItem *itm);
    void monsterClicked(const StackItem *itm);
    void gateClicked(const GateItem *itm);

private:
    void initCharacterItem();
    void initMonsterItem();
    void initSpecialItem();
    void initThisCharacterItem();
    void initClickAreaItem();
    void initClueItem();
    void initGateItem();

    void fieldAreaClicked();

private:
    friend class ClickAreaItem;
    friend class GateItem;

private:
    AH::Common::FieldData::FieldID m_id;
    FieldItemType m_type;
    QRectF m_itemRect;
    QRectF m_fieldRect;
    ItemStacker *m_monsters;
    ItemStacker *m_characters;

    //ItemStacker *m_secondPhaseCharacters;

    ClickAreaItem *m_fieldArea;
    ClueAreaItem *m_clues;
    GateItem *m_gate;
    QGraphicsPixmapItem *m_specialMarker;
    QGraphicsPixmapItem *m_thisCharacter;
};

class ClickAreaItem : public QGraphicsRectItem
{
public:
    ClickAreaItem(QRectF r, AhFieldItem *parent);

    void setActive(bool active);
    bool isActive() const { return m_isActive; }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
    void updateColor();

private:
    bool m_isActive;
    bool m_mouseIn;
    bool m_mouseDown;
    AhFieldItem *m_field;
};

class ClueAreaItem : public QGraphicsRectItem
{
public:
    ClueAreaItem(QRectF rect, QGraphicsItem *parent);
    void setClueCount(quint32 ct);

private:
    void init();

private:
    quint32 m_clueCount;
    QGraphicsPixmapItem *m_icon;
    QGraphicsTextItem *m_text;
};

class GateItem : public QGraphicsRectItem, public AsyncObjectReceiver
{
public:
    GateItem(QRectF rect, AhFieldItem *parent);

    QString gateId() const { return m_gateId; }
    void setGateId(const QString id);
    void unsetGate() { setGateId(QString::null); }

    virtual void objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    void init();

private:
    AhFieldItem *m_parent;
    QString m_gateId;
    QGraphicsPixmapItem *m_pixmap;
};

#endif // AHFIELDITEM_H