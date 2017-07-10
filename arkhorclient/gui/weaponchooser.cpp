#include "weaponchooser.h"
#include "ui_weaponchooser.h"
#include "objectregistry.h"
#include "resourcepool.h"
#include <QtWidgets>

WeaponChooser::WeaponChooser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WeaponChooser),
    m_totHands(0)
{
    ui->setupUi(this);
    connect(ui->lstWeapons, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(updateHandCount()));
}

WeaponChooser::~WeaponChooser()
{
    delete ui;
}

void WeaponChooser::chooseWeapons(QList<AH::Common::GameObjectData> weapons, AH::Common::ModifiedPropertyValueData hands)
{
    cleanWeapons();

    m_totHands = hands.finalVal();

    foreach (AH::Common::GameObjectData obj, weapons) {

        QListWidgetItem *itm = new QListWidgetItem;
        bool hasData = false;
        if (ObjectRegistry::instance()->hasObject(obj.id())) {
            AH::Common::DescribeObjectsData::ObjectDescription data = ObjectRegistry::instance()->getObject(obj.id());
            if (data.type == AH::Common::RequestObjectsData::Object) {
                hasData = true;
                AH::Common::GameObjectData od;
                data.data >> od;
                QString txt = QString("%2 (%1)").arg(obj.handCount()).arg(od.name());
                itm->setText(txt);
                itm->setIcon(ResourcePool::instance()->loadObjectImage(obj.id(), obj.type()));
            }
        }
        if (!hasData) {
            // Backup if it's not in the object registry (how?)
            QString txt = QString("%2 (%1)").arg(obj.handCount()).arg(obj.id());
            itm->setText(txt);
        }

        itm->setFlags(itm->flags() | Qt::ItemIsUserCheckable);
        if (obj.isEquipped()) {
            itm->setCheckState(Qt::Checked);
        } else {
            itm->setCheckState(Qt::Unchecked);
        }
        itm->setData(IDRole, obj.id());
        itm->setData(HandsRole, obj.handCount());
        //connect(chk, SIGNAL(toggled(bool)), this, SLOT(updateHandCount()));
        ui->lstWeapons->addItem(itm);
    }

    updateHandCount();
}

void WeaponChooser::updateHandCount()
{
    int ct = m_totHands;

    for (int i = 0; i < ui->lstWeapons->count(); ++i) {
        QListWidgetItem *w = ui->lstWeapons->item(i);
        if (w->checkState() == Qt::Checked) {
            ct -= w->data(HandsRole).toInt();
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
    for (int i = 0; i < ui->lstWeapons->count(); ++i) {
        QListWidgetItem *w = ui->lstWeapons->item(i);
        if (w->checkState() == Qt::Checked) {
            lst << w->data(IDRole).toString();
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

void WeaponChooser::on_lstWeapons_currentRowChanged(int row)
{
    if (row < 0 || row >= ui->lstWeapons->count())
        return;
    emit itemInfoRequested(ui->lstWeapons->item(row)->data(IDRole).toString());
}

void WeaponChooser::cleanWeapons()
{
    ui->lstWeapons->clear();
}
