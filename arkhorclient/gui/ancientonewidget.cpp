#include "ancientonewidget.h"
#include "ui_ancientonewidget.h"
#include "resourcepool.h"
#include "objectregistry.h"
#include "utils.h"

using namespace AH::Common;

AncientOneWidget::AncientOneWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AncientOneWidget),
    m_signalAOChange(false)
{
    ui->setupUi(this);
    ui->lblName->setFont(ResourcePool::instance()->loadMainFont());
    displayAncientOne(NULL);
    connect(ui->lblName, SIGNAL(linkActivated(QString)), this, SIGNAL(ancientOneInfoRequested(QString)));
}

AncientOneWidget::~AncientOneWidget()
{
    delete ui;
}

void AncientOneWidget::objectDescribed(const DescribeObjectsData::ObjectDescription &desc)
{
    if (desc.id == m_curAoId && desc.type == RequestObjectsData::AncientOne) {
        AncientOneData aod;
        desc.data >> aod;
        updateAncientOne(&aod);
    }
}

void AncientOneWidget::displayAncientOne(QString aoId)
{
    if (aoId != m_curAoId) {
        ObjectRegistry::instance()->unsubscribe(this);
        ObjectRegistry::instance()->asyncSubscribeObject(this, aoId);
        m_curAoId = aoId;
        if (!m_curAoId.isEmpty()) {
            m_signalAOChange = true;
        }
    }
}

void AncientOneWidget::displayAncientOne(const AncientOneData *ao)
{
    ObjectRegistry::instance()->unsubscribe(this);
    m_curAoId = QString::null;
    updateAncientOne(ao);
    if (ao) {
        displayAncientOne(ao->id());
    }
}

void AncientOneWidget::updateAncientOne(const AncientOneData *ao)
{
    if (ao) {
        ui->lblName->setText(QString("<a href=\"%1\" style=\"text-decoration:none;color:rgb(0,0,0);\">%2</a>").arg(ao->id(), ao->name()));
        ui->lblCombatAdjustment->setText(Utils::fullNumberString(ao->combatAdjustment()));
        ui->lblPower->setText(QString("<b>%1:</b> %2").arg(ao->powerTitle(), ao->powerText()));
        ui->lblAttack->setText(ao->attackText());
        ui->lblDoomTrack->setText(QString("%1 / %2").arg(ao->doomValue()).arg(ao->doomTrack()));
        ui->lblWorshippers->setText(ao->worshippersText());

        if (m_signalAOChange) {
            emit ancientOneInfoRequested(ao->id());
        }
        m_signalAOChange = false;
    } else {
        ui->lblName->setText("");
        ui->lblCombatAdjustment->setText("");
        ui->lblPower->setText("");
        ui->lblAttack->setText("");
        ui->lblDoomTrack->setText("");
        ui->lblWorshippers->setText("");
    }
}
