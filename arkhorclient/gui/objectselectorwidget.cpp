#include "objectselectorwidget.h"
#include "ui_objectselectorwidget.h"
#include "objectregistry.h"
#include "resourcepool.h"
#include <gameobjectdata.h>
#include <QInputDialog>

using namespace AH::Common;

ObjectSelectorWidget::ObjectSelectorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObjectSelectorWidget),
    m_isActive(false),
    m_max(0),
    m_min(0)
{
    ui->setupUi(this);
}

ObjectSelectorWidget::~ObjectSelectorWidget()
{
    delete ui;
}

QStringList ObjectSelectorWidget::getSelectedItems() const
{
    QStringList itms;
    for (int i = 0; i < ui->lstSelected->count(); ++i) {
        QString id = ui->lstSelected->item(i)->data(IdRole).toString();
        itms << id;
    }
    return itms;
}

void ObjectSelectorWidget::activate()
{
    if (m_isActive) return;
    connect(ObjectRegistry::instance(), SIGNAL(objectDescribed(AH::Common::DescribeObjectsData::ObjectDescription)), this, SLOT(describeItem(AH::Common::DescribeObjectsData::ObjectDescription)));
    m_isActive = false;
}

void ObjectSelectorWidget::deactivate()
{
    if (!m_isActive) return;
    disconnect(ObjectRegistry::instance(), SIGNAL(objectDescribed(AH::Common::DescribeObjectsData::ObjectDescription)), this, SLOT(describeItem(AH::Common::DescribeObjectsData::ObjectDescription)));
    m_isActive = false;
}

void ObjectSelectorWidget::setItems(const QStringList &objectIds)
{
    clearItems();

    RequestObjectsData reqs;
    for (const auto &id : objectIds) {
        QListWidgetItem *itm = new QListWidgetItem(id);
        itm->setData(IdRole, id);
        m_items.insert(id, itm);
        ui->lstAvailable->addItem(itm);
        if (id.startsWith("$:"))
            updateMoney(itm, 0);
        else
            reqs.addRequest(qMakePair(RequestObjectsData::Unknown, id));
    }

    activate();
    DescribeObjectsData descs = ObjectRegistry::instance()->getObjects(reqs);
    foreach (const DescribeObjectsData::ObjectDescription &d, descs.getDescriptions()) {
        if (d.type != RequestObjectsData::Unknown) {
            describeItem(d);
        }
    }

    setMaxItems(m_max);
}

void ObjectSelectorWidget::setMinItems(int min)
{
    m_min = qMax(0, min);
    ui->lblMin->setText(QString::number(m_min));
    checkItemCount();
}

void ObjectSelectorWidget::setMaxItems(int max)
{
    m_max = qMin(m_items.size(), max);
    ui->lblMax->setText(QString::number(m_max));
    checkItemCount();
}

void ObjectSelectorWidget::showCounts(bool show)
{
    ui->frmCounts->setVisible(show);
}

void ObjectSelectorWidget::describeItem(const DescribeObjectsData::ObjectDescription &desc)
{
    QListWidgetItem *itm = m_items.value(desc.id);
    if (!itm) {
        return;
    }

    switch (desc.type) {
    case RequestObjectsData::Object:
    {
        GameObjectData obj;
        desc.data >> obj;
        QPixmap p = ResourcePool::instance()->loadObjectImage(desc.id, obj.type());
        itm->setIcon(QIcon(p));
        itm->setText(obj.name());
        break;
    }
    default:
        // Cannot select other types.
        break;
    }
}

void ObjectSelectorWidget::clearItems()
{
    deactivate();
    m_items.clear();
    ui->lstAvailable->clear();
    ui->lstSelected->clear();
    emit selectionOk(false);
    emit selectionWrong(true);
}

void ObjectSelectorWidget::selectItem()
{
    int row = ui->lstAvailable->currentRow();
    if (row < 0) return;
    //if (ui->lstSelected->count() < m_max) {
    QListWidgetItem *itm = ui->lstAvailable->takeItem(row);

    if (itm->data(IdRole).toString().startsWith("$:")) {
        selectMoney(row, itm);
        checkItemCount();
        return;
    }
    ui->lstSelected->addItem(itm);
    //}
    checkItemCount();
}

void ObjectSelectorWidget::deselectItem()
{
    int row = ui->lstSelected->currentRow();
    if (row < 0) return;
    QListWidgetItem *itm = ui->lstSelected->takeItem(row);
    if (itm->data(IdRole).toString().startsWith("$:")) {
        updateMoney(ui->lstAvailable, itm->data(IdRole).toString().midRef(2).toInt());
        delete itm;
        checkItemCount();
        return;
    }
    ui->lstAvailable->addItem(itm);
    checkItemCount();
}

void ObjectSelectorWidget::displayItem(QListWidgetItem *itm)
{
    if (!itm) return;
    QString id = itm->data(IdRole).toString();
    emit requestObjectInfo(id);
}

void ObjectSelectorWidget::checkItemCount()
{
    int selCount = ui->lstSelected->count();
    bool ok = (m_min <= selCount && selCount <= m_max);
    emit selectionOk(ok);
    emit selectionWrong(!ok);
}

void ObjectSelectorWidget::selectMoney(int row, QListWidgetItem *itm)
{
    int max = itm->data(IdRole).toString().midRef(2).toInt();
    int sel = (max > 1)
            ? QInputDialog::getInt(this, "Select amount", "How much money you want to spend?", 1, 0, max)
            : max;
    if (sel <= 0) {
        ui->lstAvailable->insertItem(row, itm);
        return;
    }

    if (sel < max) {
        ui->lstAvailable->insertItem(row, itm);
        updateMoney(itm, -sel);
    } else {
        delete itm;
    }
    itm = nullptr;

    updateMoney(ui->lstSelected, sel);
}

void ObjectSelectorWidget::updateMoney(QListWidget *lst, int add)
{
    QListWidgetItem *itm = nullptr;
    for (int i = 0; i < lst->count(); ++i) {
        if (lst->item(i)->data(IdRole).toString().startsWith("$:")) {
            itm = lst->item(i);
            break;
        }
    }

    if (!itm) {
        if (add == 0) return;
        itm = new QListWidgetItem;
        itm->setData(IdRole, "$:0");
        lst->addItem(itm);
    }

    updateMoney(itm, add);
}

void ObjectSelectorWidget::updateMoney(QListWidgetItem *itm, int add)
{
    int amount = itm->data(IdRole).toString().midRef(2).toInt() + add;
    itm->setData(IdRole, QString("$:%1").arg(amount));
    itm->setText(QString("$%1").arg(amount));
    itm->setIcon(QPixmap(":/core/marker/dollar"));
}
