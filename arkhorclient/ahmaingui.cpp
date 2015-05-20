#include "ahmaingui.h"
#include "ui_ahmaingui.h"
#include "ahboardscene.h"
#include <QtGui>

AhMainGui::AhMainGui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AhMainGui)
{
    ui->setupUi(this);

    AhBoardScene *sc = new AhBoardScene(this);
    ui->grvBoard->setScene(sc);
    sc->initBoard();

    connect(sc, SIGNAL(itemInfoRequested(QString)), this, SLOT(displayItemInfo(QString)));
}

AhMainGui::~AhMainGui()
{
    delete ui;
}

void AhMainGui::displayItemInfo(const QString &id)
{
    QMessageBox::information(this, "Info", id);
}
