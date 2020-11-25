#include "movementchooser.h"
#include "ui_movementchooser.h"
#include "ahboardscene.h"
#include "ahgraphicsview.h"
#include "ahfielditem.h"

MovementChooser::MovementChooser(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MovementChooser)
    , m_scene(nullptr)
    , m_view(nullptr)
    , m_startId(AH::Common::FieldData::FieldID::NO_NO_FIELD)
    , m_curId(AH::Common::FieldData::FieldID::NO_NO_FIELD)
    , m_maxMove(0)
    , m_curMove(0)
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
    m_startId = startId;
    m_maxMove = movementPoints;
    resetMovement();
    connect(m_scene, SIGNAL(fieldClicked(AH::Common::FieldData::FieldID)), this, SLOT(nextItemClicked(AH::Common::FieldData::FieldID)));
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
    disconnect(m_scene, SIGNAL(fieldClicked(AH::Common::FieldData::FieldID)), this, SLOT(nextItemClicked(AH::Common::FieldData::FieldID)));
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
        setFieldCurrent(m_curId, false);
        setNeighboursActive(m_curId, false);
        m_curId = id;
        setFieldCurrent(m_curId, true);
    }
}

void MovementChooser::nextMoveStep(AH::Common::FieldData::FieldID nextId)
{
    setFieldCurrent(m_curId, false);
    setNeighboursActive(m_curId, false);
    m_curId = nextId;
    setNeighboursActive(m_curId, true);
    setFieldCurrent(m_curId, true);
}

void MovementChooser::setNeighboursActive(AH::Common::FieldData::FieldID id, bool active)
{
    QList<AH::Common::FieldData::FieldID> neigh = m_scene->getNeighbours(id);
    for (auto n : neigh) {
        AhFieldItem *f = m_scene->getField(n);
        if (active && !f->isLocked()) {
            f->setClickable(true);
        } else {
            f->setClickable(false);
        }
    }
}

void MovementChooser::setFieldCurrent(AH::Common::FieldData::FieldID id, bool cur)
{
    AhFieldItem *f = m_scene->getField(id);
    if (f) f->setCurrentField(cur);
}

void MovementChooser::disableAllFields()
{
    for (auto f : m_scene->allFields()) {
        f->setClickable(false);
        f->setCurrentField(false);
    }
}
