#include "dierollwidget.h"
#include "ui_dierollwidget.h"
#include "flowlayout.h"
#include <QtGui>

DieRollWidget::DieRollWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DieRollWidget)
{
    ui->setupUi(this);
    ui->wgtDice->setLayout(new FlowLayout);
}

DieRollWidget::~DieRollWidget()
{
    delete ui;
}

void DieRollWidget::displayDieRoll(AH::Common::DieRollTestData data)
{
    int ct = data.rollData().pool().dieCount()+data.rollData().pool().adjustment();
    QList<quint32> vals = data.rollData().pool().dieValues();
    int totCt = qMax(ct, vals.size());

    cleanDice();

    QLayout *l = ui->wgtDice->layout();
    for (int i = 0; i < totCt; ++i) {
        QLabel *lbl = new QLabel(QString::number(vals.value(i)), this);
        l->addWidget(lbl);
    }

    m_clueBurnFactor = data.diceForClueBurn();

    // TODO: Set maximum value
    //ui->spnClueBurn->setMaximum(this);
    ui->spnClueBurn->setValue(0);
}

void DieRollWidget::updateClueBurnAmount(int ct)
{
    ui->lblDieAddCount->setText(QString::number(ct*m_clueBurnFactor));
}

void DieRollWidget::on_btnOk_clicked()
{
    AH::Common::DieTestUpdateData upd(QString::null, ui->spnClueBurn->value());
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
    foreach (QWidget *w, ui->wgtDice->findChildren<QWidget*>()) {
        w->deleteLater();
    }
}
