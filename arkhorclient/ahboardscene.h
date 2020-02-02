#ifndef AHBOARDSCENE_H
#define AHBOARDSCENE_H

#include <QGraphicsScene>
#include <QVariantMap>
#include <fielddata.h>
#include <gamefielddata.h>

class AhFieldItem;

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
    AhFieldItem *getField(AH::Common::FieldData::FieldID id) { return m_fieldMap.value(id); }
    QList<AhFieldItem *> allFields() { return m_fieldMap.values(); }

public slots:
    void updateBoardFromData(QVariantMap boardMap);
    void setTerrorLevel(int level);
    void initNeighbourHoodFromBoardData(QList<AH::Common::GameFieldData> boardMap);

signals:
    void itemInfoRequested(QString id);
    void fieldClicked(AH::Common::FieldData::FieldID id);
    void fieldInfoRequested(AH::Common::FieldData::FieldID id);

private:
    QMap<AH::Common::FieldData::FieldID, AhFieldItem *> m_fieldMap;
    QMap<AH::Common::FieldData::FieldID, QList<AH::Common::FieldData::FieldID> > m_neighbours;
    QList<QPointF> m_terrorPositions;

    QGraphicsItem *m_terrorItem;

    double m_scale;

    friend class AhBoardfillerHelper;
};

#endif // AHBOARDSCENE_H
