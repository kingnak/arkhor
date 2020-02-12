#ifndef AHBOARDSCENE_H
#define AHBOARDSCENE_H

#include <QGraphicsScene>
#include <QVariantMap>
#include <fielddata.h>
#include <gamefielddata.h>
#include <gamestatechangedata.h>
#include "gui/ahfielditem.h"

class AhBoardScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit AhBoardScene(QObject *parent = 0);

    double boardScaleFactor() {
        return m_scale;
    }

    void setBoardScaleFactor(double d) {
        m_scale = d;
    }

    void initBoard();

    QList<AH::Common::FieldData::FieldID> getNeighbours(AH::Common::FieldData::FieldID id) { return m_neighbours.value(id); }
    AhFieldItem *getField(AH::Common::FieldData::FieldID id);
    QList<AhFieldItem *> allFields() { return m_fieldMap.values(); }

    void centerOn(AhFieldItem *f, bool animated = true, qreal zoom = 1.5);
    void centerOn(QPointF p, bool animated = false, qreal zoom = 1.5);

public slots:
    void updateBoardFromData(QVariantMap boardMap);
    void setTerrorLevel(int level);
    void initNeighbourHoodFromBoardData(QList<AH::Common::GameFieldData> boardMap);

signals:
    void itemInfoRequested(QString id);
    void fieldClicked(AH::Common::FieldData::FieldID id);
    void fieldInfoRequested(AH::Common::FieldData::FieldID id);
    void requestCenterOn(AH::Common::FieldData::FieldID id, bool animated, qreal zoom);
    void requestCenterOn(const QPointF &p, bool animated, qreal zoom);

    void beginAnimation();
    void endAnimation();

private:
    void applyUpdate(QVariantMap boardMap);
    void ensureAnimationObjectsKnown(const AH::Common::GameBoardChangeData &changes);
    void animateChanges(AH::Common::GameBoardChangeData changes);
    void animateTerrorLevel(int level);
    QPointF terrorLevelPosition(int lvl);

private:
    QMap<AH::Common::FieldData::FieldID, AhFieldItem *> m_fieldMap;
    QMap<AH::Common::FieldData::FieldID, QList<AH::Common::FieldData::FieldID> > m_neighbours;
    QList<QPointF> m_terrorPositions;

    QGraphicsItem *m_terrorItem;
    int m_terrorLevel;

    double m_scale;

    QList<QVariantMap> m_pendingUpdates;
    bool m_inUpdate;

    friend class AhBoardfillerHelper;
};

#endif // AHBOARDSCENE_H
