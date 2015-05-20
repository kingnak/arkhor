#ifndef AHBOARDSCENE_H
#define AHBOARDSCENE_H

#include <QGraphicsScene>
#include <QVariantMap>
#include <fielddata.h>

class AhFieldItem;

class AhBoardScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit AhBoardScene(QObject *parent = 0);

    void initBoard();

    QList<AH::Common::FieldData::FieldID> getNeighbours(AH::Common::FieldData::FieldID id) { return m_neighbours.value(id); }
    AhFieldItem *getField(AH::Common::FieldData::FieldID id) { return m_fieldMap.value(id); }
    QList<AhFieldItem *> allFields() { return m_fieldMap.values(); }

public slots:
    void updateBoardFromData(QVariantMap boardMap);

signals:
    void itemInfoRequested(QString id);
    void fieldClicked(AH::Common::FieldData::FieldID id);

public slots:

private:
    void initNeighbourHoodFromBoardData(QVariantMap boardMap);

private:
    QMap<AH::Common::FieldData::FieldID, AhFieldItem *> m_fieldMap;
    QMap<AH::Common::FieldData::FieldID, QList<AH::Common::FieldData::FieldID> > m_neighbours;

    friend class AhBoardfillerHelper;
};

#endif // AHBOARDSCENE_H
