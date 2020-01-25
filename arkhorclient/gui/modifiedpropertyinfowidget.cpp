#include "modifiedpropertyinfowidget.h"
#include "ui_modifiedpropertyinfowidget.h"
#include "utils.h"

ModifiedPropertyInfoWidget::ModifiedPropertyInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModifiedPropertyInfoWidget)
{
    ui->setupUi(this);
}

ModifiedPropertyInfoWidget::~ModifiedPropertyInfoWidget()
{
    delete ui;
}

void ModifiedPropertyInfoWidget::cleanValue()
{
    ui->scrlMods->cleanModifiers();
    ui->lblBase->clear();
    ui->lblFinal->clear();
    ui->lblProperty->clear();
}

void ModifiedPropertyInfoWidget::displayPropertyValue(AH::Common::ModifiedPropertyValueData value)
{
    ui->lblProperty->setText(Utils::stringForProperty(value.property().property()));
    ui->lblBase->setText(QString::number(value.property().value()));
    ui->lblFinal->setText(QString::number(value.finalVal()));
    ui->scrlMods->setModifiers(value.modifications());
    if (value.modifications().empty()) {
        ui->scrlMods->setVisible(false);
    } else {
        ui->scrlMods->setVisible(true);
    }
}

void ModifiedPropertyInfoWidget::requestObjectInfo(QString id)
{
    emit objectInfoRequested(id);
}
