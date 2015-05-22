#include "ancientoneshortwidget.h"
#include "ui_ancientoneshortwidget.h"
#include "resourcepool.h"
#include "objectregistry.h"
#include "utils.h"

using namespace AH::Common;

AncientOneShortWidget::AncientOneShortWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AncientOneShortWidget)
{
    ui->setupUi(this);
    ui->lblName->setFont(ResourcePool::instance()->loadMainFont());
    displayAncientOne(NULL);
    connect(ui->lblName, SIGNAL(linkActivated(QString)), this, SIGNAL(ancientOneInfoRequested(QString)));
}

AncientOneShortWidget::~AncientOneShortWidget()
{
    delete ui;
}

void AncientOneShortWidget::objectDescribed(const DescribeObjectsData::ObjectDescription &desc)
{
    if (desc.type == RequestObjectsData::AncientOne) {
        AncientOneData aod;
        desc.data >> aod;
        displayAncientOne(&aod);
    }
}

void AncientOneShortWidget::displayAncientOne(QString aoId)
{
    ObjectRegistry::instance()->unsubscribe(this);
    ObjectRegistry::instance()->asyncSubscribeObject(this, aoId);
}

void AncientOneShortWidget::displayAncientOne(AncientOneData *ao)
{
    if (ao) {
        ui->lblName->setText(QString("<a href=\"%1\" style=\"text-decoration:none;color:rgb(0,0,0);\">%2</a>").arg(ao->id(), ao->name()));
        ui->lblCombatAdjustment->setText(Utils::fullNumberString(ao->combatAdjustment()));
        ui->lblPower->setText(QString("<b>%1:</b> %2").arg(ao->powerTitle(), ao->powerText()));
        ui->lblAttack->setText(ao->attackText());
        ui->lblDoomTrack->setText(QString("%1 / %2").arg(ao->doomValue()).arg(ao->doomTrack()));
        ui->lblWorshippers->setText(ao->worshippersText());
    } else {
        ui->lblName->setText("");
        ui->lblCombatAdjustment->setText("");
        ui->lblPower->setText("");
        ui->lblAttack->setText("");
        ui->lblDoomTrack->setText("");
        ui->lblWorshippers->setText("");
    }
}
