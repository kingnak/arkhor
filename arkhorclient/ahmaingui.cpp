#include "ahmaingui.h"
#include "ui_ahmaingui.h"
#include "ahboardscene.h"
#include "connectionhandler.h"
#include "flowlayout.h"
#include <QtGui>

static const char *OPTION_DESCRIPTION_PROPERTY = "DESCRIPTION";
static const char *OPTION_ID_PROPERTY = "ID";

using namespace AH::Common;

AhMainGui::AhMainGui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AhMainGui),
    m_conn(NULL)
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

    connect(m_conn, SIGNAL(boardContent(QVariantMap)), m_scene, SLOT(updateBoardFromData(QVariantMap)));
    connect(m_conn, SIGNAL(chooseOption(QList<AH::Common::GameOptionData>)), this, SLOT(setOptions(QList<AH::Common::GameOptionData>)));
    connect(m_conn, SIGNAL(chooseMovement(AH::Common::FieldData::FieldID,int)), this, SLOT(chooseMove(AH::Common::FieldData::FieldID,int)));

    connect(ui->wgtMovmentChooser, SIGNAL(movementChosen(QList<AH::Common::FieldData::FieldID>)), this, SLOT(movementChosen(QList<AH::Common::FieldData::FieldID>)));
    connect(ui->wgtMovmentChooser, SIGNAL(movementCancelled()), this, SLOT(movementCanceled()));
}

void AhMainGui::displayItemInfo(const QString &id)
{
    QMessageBox::information(this, "Info", id);
}

void AhMainGui::setOptions(QList<AH::Common::GameOptionData> opts)
{
    ui->tabIntInfInv->setCurrentWidget(ui->tabInteraction);
    ui->stkInteraction->setCurrentWidget(ui->pageOptions);
    cleanupOptions();

    FlowLayout *l = new FlowLayout(ui->wgtOptionsList);
    foreach (GameOptionData o, opts) {
        QPushButton *b = new QPushButton(o.name());
        b->setProperty(OPTION_DESCRIPTION_PROPERTY, o.description());
        b->setProperty(OPTION_ID_PROPERTY, o.id());
        connect(b, SIGNAL(clicked()), this, SLOT(showOption()));
        l->addWidget(b);
    }

    //ui->wgtOptionsList->setLayout(l);
}

void AhMainGui::cleanupOptions()
{
    QLayout *l = ui->wgtOptionsList->layout();
    if (l) {
        QLayoutItem *child;
        while ((child = l->takeAt(0)) != 0) {
            delete child;
        }
        ui->wgtOptionsList->setLayout(NULL);
        delete l;
    }
    foreach (QWidget *w, ui->wgtOptionsList->findChildren<QWidget*>()) {
        delete w;
    }

    ui->lblOptionDescription->setText("");
    ui->btnOptionActivate->setEnabled(false);
}

void AhMainGui::showOption()
{
    ui->lblOptionDescription->setText(sender()->property(OPTION_DESCRIPTION_PROPERTY).toString());
    ui->btnOptionActivate->setProperty(OPTION_ID_PROPERTY, sender()->property(OPTION_ID_PROPERTY));
    ui->btnOptionActivate->setEnabled(true);
}

void AhMainGui::chooseOption()
{
    QString id = ui->btnOptionActivate->property(OPTION_ID_PROPERTY).toString();
    cleanupOptions();
    m_conn->selectOption(id);
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
}

void AhMainGui::movementCanceled()
{
    m_conn->selectMovementPath(QList<FieldData::FieldID>());
}
