#ifndef AHFIELDITEM_H
#define AHFIELDITEM_H

#include <QGraphicsItem>
#include <fielddata.h>
#include <gamefielddata.h>
#include "asyncobjectreceiver.h"
#include "monsterdata.h"

class QGraphicsProxyWidget;
class ItemStacker;
class StackItem;

class ClueAreaItem;
class ClickAreaItem;
class GateItem;

class QAbstractAnimation;

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

    AhFieldItem(AH::Common::FieldData::FieldID id, FieldItemType type, QRectF rect, double scale, QGraphicsItem *parent = 0);

    AH::Common::FieldData::FieldID id() const { return m_id; }

    QRectF boundingRect() const;

    void setFieldRect(QRectF r);
    void setFieldExtent(QSizeF s);
    void setClickable(bool clickable);
    void setCurrentField(bool cur);

    bool isLocked() const { return m_locked; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 );

    void initSubItems();

    void updateFromData(AH::Common::GameFieldData data);

    static QFont getItemFont(int pxSize = 24, bool bold = true);
    void animateGateAppear(QString id);
    void animateGateDisappear();
    void animateGateOpen(QString id);

    void animateMonsterAppear(AH::Common::MonsterData m);
    void animateMonsterDisappear(AH::Common::MonsterData m);
    void animateMonsterMove(AH::Common::MonsterData m, QList<AH::Common::FieldData::FieldID> path);

signals:
    void itemInfoRequested(QString id);
    void fieldClicked(AH::Common::FieldData::FieldID id);
    void fieldInfoRequested(AH::Common::FieldData::FieldID id);

private slots:
    void characterClicked(const StackItem *itm);
    void monsterClicked(const StackItem *itm);
    void gateClicked(const GateItem *itm);

private:
    void initInfoItem();
    void initCharacterItem();
    void initMonsterItem();
    void initSpecialItem();
    void initThisCharacterItem();
    void initClickAreaItem();
    void initClueItem();
    void initGateItem();

    void fieldAreaClicked();

    void runAnimation(const QVariant &start, const QVariant &end, int duration, std::function<void(const QVariant &)> update);
    void runAnimation(QAbstractAnimation *anim);

    QGraphicsItem *createOverlayMonster(AH::Common::MonsterData m);
    QPointF getMonstersGlobalPos();

private:
    friend class ClickAreaItem;
    friend class GateItem;

private:
    AH::Common::FieldData::FieldID m_id;
    bool m_locked;
    FieldItemType m_type;
    QRectF m_itemRect;
    QRectF m_fieldRect;
    ItemStacker *m_monsters;
    ItemStacker *m_characters;
    ItemStacker *m_secondPhaseCharacters; // For other world

    QGraphicsProxyWidget *m_prxMonst;

    //ItemStacker *m_secondPhaseCharacters;

    ClickAreaItem *m_infoArea;
    ClickAreaItem *m_fieldArea;
    ClueAreaItem *m_clues;
    GateItem *m_gate;
    QGraphicsPixmapItem *m_specialMarker;
    QGraphicsPixmapItem *m_thisCharacter;
    double m_scale;
};

class ClickAreaItem : public QGraphicsRectItem
{
public:
    ClickAreaItem(QRectF r, AhFieldItem *parent);

    void setCurrent(bool cur);
    bool isCurrent() const { return m_isCurrent; }
    void setActive(bool active);
    bool isActive() const { return m_isActive; }
    void setNoHighlight(bool noHighlight);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
    void updateColor();

private:
    bool m_noHighlight = false;
    bool m_isActive;
    bool m_isCurrent;
    bool m_mouseIn;
    bool m_mouseDown;
    AhFieldItem *m_field;
};

class ClueAreaItem : public QGraphicsRectItem
{
public:
    ClueAreaItem(QRectF rect, double scale, QGraphicsItem *parent);
    void setClueCount(quint32 ct);

private:
    void init(double scale);

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
