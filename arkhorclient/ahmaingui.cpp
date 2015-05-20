#include "ahmaingui.h"
#include "ui_ahmaingui.h"
#include "ahboardscene.h"
#include "connectionhandler.h"
#include "flowlayout.h"
#include "objectregistry.h"
#include <QtGui>

using namespace AH::Common;

AhMainGui::AhMainGui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AhMainGui),
    m_conn(NULL),
    m_registry(NULL)
{
    ui->setupUi(this);

    m_scene = new AhBoardScene(this);
    ui->grvBoard->setScene(m_scene);
    m_scene->initBoard();

    ui->wgtMovmentChooser->setBoard(m_scene, ui->grvBoard);

    connect(m_scene, SIGNAL(itemInfoRequested(QString)), this, SLOT(displayItemInfo(QString)));
}

AhMainGui::~AhMainGui()
{
    delete ui;
}

void AhMainGui::initConnection(ConnectionHandler *conn)
{
    m_conn = conn;
    m_registry = new ObjectRegistry(m_conn);

    connect(m_registry, SIGNAL(objectDescribed(AH::Common::DescribeObjectsData::ObjectDescription)), this, SLOT(updateObject(AH::Common::DescribeObjectsData::ObjectDescription)));

    // BOARD
    connect(m_conn, SIGNAL(boardContent(QVariantMap)), m_scene, SLOT(updateBoardFromData(QVariantMap)));

    // OPTION
    connect(m_conn, SIGNAL(chooseOption(QList<AH::Common::GameOptionData>)), this, SLOT(chooseOption(QList<AH::Common::GameOptionData>)));
    connect(ui->wgtOptionChooser, SIGNAL(optionChosen(QString)), this, SLOT(optionChosen(QString)));

    // MOVE
    connect(m_conn, SIGNAL(chooseMovement(AH::Common::FieldData::FieldID,int)), this, SLOT(chooseMove(AH::Common::FieldData::FieldID,int)));
    connect(ui->wgtMovmentChooser, SIGNAL(movementChosen(QList<AH::Common::FieldData::FieldID>)), this, SLOT(movementChosen(QList<AH::Common::FieldData::FieldID>)));
    connect(ui->wgtMovmentChooser, SIGNAL(movementCancelled()), this, SLOT(movementCanceled()));

    // FOCUS
    connect(m_conn, SIGNAL(chooseFocus(QList<AH::Common::AttributeSliderData>,int)), this, SLOT(chooseFocus(QList<AH::Common::AttributeSliderData>,int)));
    connect(ui->wgtFocusChooser, SIGNAL(focusConfirmed(QList<int>)), this, SLOT(focusChosen(QList<int>)));

    // DIE ROLL
    connect(m_conn, SIGNAL(dieRollInfo(AH::Common::DieRollTestData)), this, SLOT(showDieRollInfo(AH::Common::DieRollTestData)));
    connect(ui->wgtDieRoll, SIGNAL(dieUpdateChosen(AH::Common::DieTestUpdateData)), this, SLOT(dieUpdateChosen(AH::Common::DieTestUpdateData)));

}

void AhMainGui::start()
{
    show();
    m_registry->getObject(m_thisCharacterId, AH::Common::RequestObjectsData::Character);
}

void AhMainGui::displayItemInfo(const QString &id)
{
    QMessageBox::information(this, "Info", id);
    DescribeObjectsData::ObjectDescription d = m_registry->getObject(id);
    if (d.first == RequestObjectsData::Unknown) {
        m_pendingDisplayId = id;
    } else {
        m_pendingDisplayId = QString::null;
    }
}

void AhMainGui::displayInventoryData(QListWidgetItem *itm)
{
    if (itm) {
        QString id = itm->data(ObjectIdRole).toString();
        displayItemInfo(id);
    }
}

void AhMainGui::chooseOption(QList<GameOptionData> opts)
{
    ui->tabIntInfInv->setCurrentWidget(ui->tabInteraction);
    ui->stkInteraction->setCurrentWidget(ui->pageOptionChooser);
    ui->wgtOptionChooser->setOptions(opts);
}

void AhMainGui::optionChosen(QString id)
{
    m_conn->selectOption(id);
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
}

void AhMainGui::chooseMove(AH::Common::FieldData::FieldID startId, int movementPoints)
{
    ui->tabIntInfInv->setCurrentWidget(ui->tabInteraction);
    ui->stkInteraction->setCurrentWidget(ui->pageMovementChooser);
    ui->wgtMovmentChooser->chooseMovement(startId, movementPoints);
}

void AhMainGui::movementChosen(QList<FieldData::FieldID> path)
{
    m_conn->selectMovementPath(path);
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
}

void AhMainGui::movementCanceled()
{
    m_conn->selectMovementPath(QList<FieldData::FieldID>());
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
}

void AhMainGui::chooseFocus(QList<AttributeSliderData> sliders, int focusAmount)
{
    ui->tabIntInfInv->setCurrentWidget(ui->tabInteraction);
    ui->stkInteraction->setCurrentWidget(ui->pageFocusChooser);
    ui->wgtFocusChooser->chooseFocus(sliders, focusAmount);
}

void AhMainGui::focusChosen(QList<int> diffs)
{
    m_conn->selectFocus(diffs);
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
}

void AhMainGui::showDieRollInfo(DieRollTestData data)
{
    ui->tabIntInfInv->setCurrentWidget(ui->tabInteraction);
    ui->stkInteraction->setCurrentWidget(ui->pageDieRoll);
    ui->wgtDieRoll->displayDieRoll(data);
}

void AhMainGui::dieUpdateChosen(DieTestUpdateData upd)
{
    m_conn->chooseDieRollUpdate(upd);
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
}

void AhMainGui::updateObject(DescribeObjectsData::ObjectDescription desc)
{
    if (desc.first == RequestObjectsData::Character) {
        CharacterData c;
        desc.second >> c;
        updateCharacter(c);
    }

    if (!m_pendingDisplayId.isEmpty()) {
        // TODO: Check if this is good this way...
        displayItemInfo(m_pendingDisplayId);
    }
}

void AhMainGui::updateCharacter(CharacterData c)
{
    if (c.id() == m_thisCharacterId) {
        m_thisCharacter = c;
        m_registry->getObjectsOfType(c.inventoryIds(), RequestObjectsData::Object);

        ui->lstInventory->clear();
        foreach (QString id, c.inventoryIds()) {
            QListWidgetItem *itm = new QListWidgetItem(id);
            itm->setData(ObjectIdRole, id);
            ui->lstInventory->addItem(itm);
        }
    }
}
