#include "dierollwidget.h"
#include "ui_dierollwidget.h"
#include "flowlayout.h"
#include <QtGui>

DieRollWidget::DieRollWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DieRollWidget)
{
    ui->setupUi(this);
    setLayout(new FlowLayout);
}

DieRollWidget::~DieRollWidget()
{
    delete ui;
}

void DieRollWidget::displayDieRoll(AH::Common::DieRollTestData data)
{
    int ct = data.rollData().pool().dieCount();
    QList<quint32> vals = data.rollData().pool().dieValues();
    int totCt = qMax(ct, vals.size());

    QLayout *l = layout();
    if (l) {
        QLayoutItem *child;
        while ((child = l->takeAt(0)) != 0) {
            delete child;
        }
    }
    foreach (QWidget *w, findChildren<QWidget*>()) {
        delete w;
    }

    for (int i = 0; i < totCt; ++i) {
        QLabel *lbl = new QLabel(QString::number(vals.value(i)), this);
        l->addWidget(lbl);
    }
}
