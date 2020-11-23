#include "gatedatawidget.h"
#include "ui_gatedatawidget.h"
#include "utils.h"
#include "objectregistry.h"

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

void GateDataWidget::objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc)
{
    if (desc.id == m_curGateId) {
        AH::Common::GateData g;
        desc.data >> g;
        updateGate(&g);
    }
}

void GateDataWidget::displayGate(const AH::Common::GateData *g)
{
    ObjectRegistry::instance()->unsubscribe(this);
    m_curGateId = QString::null;
    updateGate(g);
    if (g) {
        m_curGateId = g->id();
        ObjectRegistry::instance()->asyncSubscribeObject(this, g->id(), AH::Common::RequestObjectsData::Gate);
    }
}

void GateDataWidget::updateGate(const AH::Common::GateData *g)
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
        ui->wgtGate->displayGate(nullptr);
        ui->lblSrc->setText("");
        ui->lblCloseAdj->setText("");
        ui->lblDest->setText("");
        ui->lblDims->setText("");

    }
}
