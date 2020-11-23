#ifndef MOVEMENTCHOOSER_H
#define MOVEMENTCHOOSER_H

#include <QWidget>
#include <fielddata.h>

class AhBoardScene;
class AhGraphicsView;

namespace Ui {
class MovementChooser;
}

class MovementChooser : public QWidget
{
    Q_OBJECT

public:
    explicit MovementChooser(QWidget *parent = 0);
    ~MovementChooser() override;

    void setBoard(AhBoardScene *scene, AhGraphicsView *view);

public slots:
    void chooseMovement(AH::Common::FieldData::FieldID startId, int movementPoints);

private slots:
    void finishedMovement();
    void resetMovement();
    void cancelMovement();
    void nextItemClicked(AH::Common::FieldData::FieldID id);

signals:
    void movementChosen(QList<AH::Common::FieldData::FieldID> path);
    void movementCancelled();

private:
    void nextMoveStep(AH::Common::FieldData::FieldID nextId);
    void setNeighboursActive(AH::Common::FieldData::FieldID id, bool active);
    void setFieldCurrent(AH::Common::FieldData::FieldID id, bool cur);
    void disableAllFields();

private:
    Ui::MovementChooser *ui;

    AhBoardScene *m_scene;
    AhGraphicsView *m_view;

    QList<AH::Common::FieldData::FieldID> m_path;
    AH::Common::FieldData::FieldID m_startId;
    AH::Common::FieldData::FieldID m_curId;
    int m_maxMove;
    int m_curMove;
};

#endif // MOVEMENTCHOOSER_H
