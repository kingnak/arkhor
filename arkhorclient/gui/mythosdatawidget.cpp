#include "mythosdatawidget.h"
#include "ui_mythosdatawidget.h"
#include "../utils.h"

MythosDataWidget::MythosDataWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MythosDataWidget)
{
    ui->setupUi(this);
    displayMythos(nullptr);
}

MythosDataWidget::~MythosDataWidget()
{
    delete ui;
}

void MythosDataWidget::displayMythos(const AH::Common::MythosData *m)
{
    if (m) {
        ui->lblName->setText(m->name());
        QString type = Utils::stringForMythosType(m->type(), m->environmenType(), "<br/><em>%1</em>");
        ui->lblType->setText(type);
        ui->lblClue->setText(Utils::stringForField(m->clueField()));
        ui->lblGate->setText(Utils::stringForField(m->gateField()));
        QStringList b = Utils::stringsForDimensions(m->blackMovement());
        ui->lblMoveBlack->setText(b.join(", "));
        QStringList w = Utils::stringsForDimensions(m->whiteMovement());
        ui->lblMoveWhite->setText(w.join(", "));
        ui->lblDesc->setText(m->description());
        ui->lblMythosCard->setVisible(true);
    } else {
        ui->lblName->setText("");
        ui->lblType->setText("");
        ui->lblClue->setText("");
        ui->lblGate->setText("");
        ui->lblMoveBlack->setText("");
        ui->lblMoveBlack->setText("");
        ui->lblDesc->setText("");
        ui->lblMythosCard->setVisible(false);
    }
}

void MythosDataWidget::on_btnAcknowledgeMythos_clicked()
{
    emit acknowledgeMythos();
}
