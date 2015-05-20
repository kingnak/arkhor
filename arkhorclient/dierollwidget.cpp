#include "dierollwidget.h"
#include "ui_dierollwidget.h"
#include "flowlayout.h"
#include <QtGui>
#include "objectregistry.h"
#include "utils.h"

using namespace AH::Common;

DieRollWidget::DieRollWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DieRollWidget)
{
    ui->setupUi(this);
    ui->wgtDice->setLayout(new FlowLayout);
    ui->scrlMods->setLayout(new QVBoxLayout);
}

DieRollWidget::~DieRollWidget()
{
    delete ui;
}

void DieRollWidget::displayDieRoll(AH::Common::DieRollTestData data)
{
    int ct = data.rollData().pool().dieCount()+data.rollData().pool().adjustment();

    QList<PropertyModificationData> mods;

    // Setup labels
    ui->lblAdjustment->setText(QString::number(data.rollData().pool().adjustment()));
    ui->lblDieCount->setText(QString::number(ct));
    if (data.rollData().pool().type() == DiePoolData::Property) {
        ui->lblBaseSkill->setText(Utils::stringForProperty(data.rollData().pool().property().property().property()));
        mods = data.rollData().pool().property().modifications();
    } else {
        ui->lblBaseSkill->setText("Fixed");
    }
    ui->lblBaseVal->setText(QString::number(data.rollData().pool().property().property().value()));
    if (data.type() == DieRollTestData::Boolean) {
        ui->lblTargetVal->setText(QString::number(data.targetValue()));
        QStringList s;
        foreach (qint32 i, data.rollData().successRolls()) {
            s << QString::number(i);
        }
        ui->lblSuccessVals->setText(s.join(", "));
    } else {
        ui->lblTargetVal->setText("-");
        ui->lblSuccessVals->setText("-");
    }

    // Display modifiers
    cleanModifiers();
    mods.append(data.clueBurnMods().modifications());
    foreach (PropertyModificationData mod, mods) {
        QLabel *l = new QLabel(QString("%1: %2").arg(mod.modifierId()).arg(mod.modificationAmount()));
        ui->scrlMods->layout()->addWidget(l);
    }

    // Display dice
    QList<quint32> vals = data.rollData().pool().dieValues();
    int totCt = qMax(ct, vals.size());

    cleanDice();

    QLayout *l = ui->wgtDice->layout();
    for (int i = 0; i < totCt; ++i) {
        QLabel *lbl = new QLabel(QString::number(vals.value(i)), this);
        l->addWidget(lbl);
    }

    m_clueBurnFactor = data.diceForClueBurn();

    // Set maximum value
    int clues = ObjectRegistry::instance()->thisCharacter().clues();
    ui->spnClueBurn->setMaximum(clues);
    ui->spnClueBurn->setValue(0);
    if (m_clueBurnFactor == 0) {
        ui->wgtDieAdd->setVisible(false);
    } else {
        ui->wgtDieAdd->setVisible(true);
    }
}

void DieRollWidget::updateClueBurnAmount(int ct)
{
    ui->lblDieAddCount->setText(QString::number(ct*m_clueBurnFactor));
}

void DieRollWidget::on_btnOk_clicked()
{
    AH::Common::DieTestUpdateData upd(QString::null, ui->spnClueBurn->value());
    cleanDice();
    cleanModifiers();
    emit dieUpdateChosen(upd);
}

void DieRollWidget::cleanDice()
{
    QLayout *l = ui->wgtDice->layout();
    if (l) {
        QLayoutItem *child;
        while ((child = l->takeAt(0)) != 0) {
            delete child;
        }
    }
    foreach (QWidget *w, ui->wgtDice->findChildren<QLabel*>()) {
        delete w;
    }
}

void DieRollWidget::cleanModifiers()
{
    QLayout *l = ui->scrlMods->layout();
    if (l) {
        QLayoutItem *child;
        while ((child = l->takeAt(0)) != 0) {
            delete child;
        }
    }
    foreach (QWidget *w, ui->scrlMods->findChildren<QLabel*>()) {
        delete w;
    }
}
