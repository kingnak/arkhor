#include "weaponchooser.h"
#include "ui_weaponchooser.h"
#include "flowlayout.h"
#include <QtGui>

#define PROPERTY_OBJECT_ID "OBJECT_ID"
#define PROPERTY_HAND_COUNT "HAND_COUNT"

WeaponChooser::WeaponChooser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WeaponChooser),
    m_totHands(0)
{
    ui->setupUi(this);
    ui->wgtWeapons->setLayout(new FlowLayout);
}

WeaponChooser::~WeaponChooser()
{
    delete ui;
}

void WeaponChooser::chooseWeapons(QList<AH::Common::GameObjectData> weapons, AH::Common::ModifiedPropertyValueData hands)
{
    cleanWeapons();

    m_totHands = hands.finalVal();

    QLayout *l = ui->wgtWeapons->layout();
    foreach (AH::Common::GameObjectData obj, weapons) {
        QString txt = QString("%2 (%1)").arg(obj.handCount()).arg(obj.id());
        QCheckBox *chk = new QCheckBox(txt);
        if (obj.isEquipped()) {
            chk->setChecked(true);
        }
        chk->setProperty(PROPERTY_OBJECT_ID, obj.id());
        chk->setProperty(PROPERTY_HAND_COUNT, obj.handCount());
        connect(chk, SIGNAL(toggled(bool)), this, SLOT(updateHandCount()));
        l->addWidget(chk);
    }

    updateHandCount();
}

void WeaponChooser::updateHandCount()
{
    int ct = m_totHands;

    foreach (QCheckBox *b, ui->wgtWeapons->findChildren<QCheckBox*>()) {
        if (b->isChecked()) {
            ct -= b->property(PROPERTY_HAND_COUNT).toInt();
        }
    }

    ui->lblHands->setText(QString::number(ct));
    if (ct < 0) {
        ui->btnOk->setEnabled(false);
    } else {
        ui->btnOk->setEnabled(true);
    }
}

void WeaponChooser::on_btnOk_clicked()
{
    QStringList lst;
    foreach (QCheckBox *b, ui->wgtWeapons->findChildren<QCheckBox*>()) {
        if (b->isChecked()) {
            lst << b->property(PROPERTY_OBJECT_ID).toString();
        }
    }

    cleanWeapons();
    emit weaponsSelected(lst);
}

void WeaponChooser::on_btnCancel_clicked()
{
    cleanWeapons();
    emit weaponsCanceled();
}

void WeaponChooser::cleanWeapons()
{
    QLayout *l = ui->wgtWeapons->layout();
    if (l) {
        QLayoutItem *child;
        while ((child = l->takeAt(0)) != 0) {
            delete child;
        }
    }
    foreach (QWidget *w, ui->wgtWeapons->findChildren<QWidget*>()) {
        delete w;
    }
}
