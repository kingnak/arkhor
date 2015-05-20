#include "movementchooser.h"
#include "ui_movementchooser.h"
#include "ahboardscene.h"
#include "ahgraphicsview.h"
#include "ahfielditem.h"

MovementChooser::MovementChooser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MovementChooser)
{
    ui->setupUi(this);
}

MovementChooser::~MovementChooser()
{
    delete ui;
}

void MovementChooser::setBoard(AhBoardScene *scene, AhGraphicsView *view)
{
    m_scene = scene;
    m_view = view;
}

void MovementChooser::chooseMovement(AH::Common::FieldData::FieldID startId, int movementPoints)
{
    m_curId = AH::Common::FieldData::NO_NO_FIELD;
    setEnabled(true);
    connect(m_scene, SIGNAL(fieldClicked(AH::Common::FieldData::FieldID)), this, SLOT(nextItemClicked(AH::Common::FieldData::FieldID)));
    m_startId = startId;
    m_maxMove = movementPoints;
    resetMovement();
}

void MovementChooser::finishedMovement()
{
    disableAllFields();
    disconnect(m_scene, SIGNAL(fieldClicked(AH::Common::FieldData::FieldID)), this, SLOT(nextItemClicked(AH::Common::FieldData::FieldID)));
    emit movementChosen(m_path);
    setEnabled(false);
}

void MovementChooser::resetMovement()
{
    disableAllFields();
    nextMoveStep(m_startId);
    m_curMove = m_maxMove;
    ui->lblRemMovementPoints->setText(QString::number(m_curMove));
    m_path.clear();
    m_path << m_startId;
}

void MovementChooser::cancelMovement()
{
    disableAllFields();
    setEnabled(false);
    connect(m_scene, SIGNAL(fieldClicked(AH::Common::FieldData::FieldID)), this, SLOT(nextItemClicked(AH::Common::FieldData::FieldID)));
    emit movementCancelled();
}

void MovementChooser::nextItemClicked(AH::Common::FieldData::FieldID id)
{
    m_curMove--;
    m_path << id;
    ui->lblRemMovementPoints->setText(QString::number(m_curMove));
    if (m_curMove > 0) {
        nextMoveStep(id);
    } else {
        setNeighboursActive(m_curId, false);
        m_curId = id;
    }
}

void MovementChooser::nextMoveStep(AH::Common::FieldData::FieldID nextId)
{
    setNeighboursActive(m_curId, false);
    m_curId = nextId;
    setNeighboursActive(m_curId, true);
}

void MovementChooser::setNeighboursActive(AH::Common::FieldData::FieldID id, bool active)
{
    QList<AH::Common::FieldData::FieldID> neigh = m_scene->getNeighbours(id);
    foreach (AH::Common::FieldData::FieldID n, neigh) {
        AhFieldItem *f = m_scene->getField(n);
        f->setClickable(active);
    }
}

void MovementChooser::disableAllFields()
{
    foreach (AhFieldItem *f, m_scene->allFields()) {
        f->setClickable(false);
    }
}
