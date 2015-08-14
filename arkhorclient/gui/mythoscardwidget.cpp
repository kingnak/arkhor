#include "mythoscardwidget.h"
#include "ui_mythoscardwidget.h"

MythosCardWidget::MythosCardWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MythosCardWidget)
{
    ui->setupUi(this);
    ui->pageEnvironment->setLongDisplay(true);
    ui->pageRumor->setLongDisplay(true);
}

MythosCardWidget::~MythosCardWidget()
{
    delete ui;
}

void MythosCardWidget::displayMythosCard(const AH::Common::MythosData *mythos)
{
    if (mythos) {
        if (mythos->type() == AH::Common::MythosData::Rumor) {
            ui->pageRumor->displayRumor(mythos);
            ui->stkMythosCards->setCurrentWidget(ui->pageRumor);
        } else if (mythos->type() == AH::Common::MythosData::Environment) {
            ui->pageEnvironment->displayEnvironment(mythos);
            ui->stkMythosCards->setCurrentWidget(ui->pageEnvironment);
        } else {
            displayNothing();
        }
    } else {
        displayNothing();
    }
}

void MythosCardWidget::displayNothing()
{
    ui->pageEnvironment->displayEnvironment(NULL);
    ui->pageRumor->displayRumor(NULL);
}
