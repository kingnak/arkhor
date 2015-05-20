#include "gatedatawidget.h"
#include "ui_gatedatawidget.h"
#include "utils.h"

GateDataWidget::GateDataWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GateDataWidget)
{
    ui->setupUi(this);
}

GateDataWidget::~GateDataWidget()
{
    delete ui;
}

void GateDataWidget::displayGate(const AH::Common::GateData *g)
{
    if (g) {
        ui->wgtGate->displayGate(g);
        ui->lblSrc->setText(Utils::stringForField(g->source()));
        if (g->isOpen()) {
            ui->lblCloseAdj->setText(Utils::fullNumberString(g->closeAdjustment()));
            ui->lblDest->setText(Utils::stringForField(g->destination()));
            ui->lblDims->setText(Utils::stringForDimension(static_cast<AH::Dimension> ((int) g->dimensions())));
        } else {
            ui->lblCloseAdj->setText("?");
            ui->lblDest->setText("?");
            ui->lblDims->setText("?");
        }
    } else {
        ui->wgtGate->displayGate(NULL);
        ui->lblSrc->setText("");
        ui->lblCloseAdj->setText("");
        ui->lblDest->setText("");
        ui->lblDims->setText("");

    }
}
