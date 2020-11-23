#include "tradewidget.h"
#include "ui_tradewidget.h"
#include "objectlistitem.h"
#include "objectregistry.h"

TradeWidget::TradeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TradeWidget)
{
    ui->setupUi(this);
    ui->wgtCounterOffer->showCounts(false);
    ui->wgtCounterOffer->setMinItems(0);
    ui->wgtInitialOffer->showCounts(false);
    ui->wgtInitialOffer->setMinItems(0);
}

TradeWidget::~TradeWidget()
{
    delete ui;
}

void TradeWidget::showTrade(const AH::Common::TradeData &td)
{
    clear();
    if (td.phase == AH::Common::TradeData::NoOffer)
        return;

    m_trade = td;
    if (td.phase == AH::Common::TradeData::InitialOffer) {
        ui->stackedWidget->setCurrentWidget(ui->pageInitial);
        ui->wgtInitialOffer->setItems(td.originOffer);
        ui->wgtInitialOffer->setMinItems(1);
        ui->wgtInitialOffer->setMaxItems(1000);
        ui->lblTitel->setText(QString("Trade with %1").arg(getName(td.destChar)));
    } else if (td.phase == AH::Common::TradeData::CounterOffer) {
        ui->stackedWidget->setCurrentWidget(ui->pageCounter);
        ui->wgtCounterOffer->setItems(td.destOffer);
        ui->wgtCounterOffer->setMinItems(1);
        ui->wgtCounterOffer->setMaxItems(1000);
        fillList(ui->lstOfferInitial, td.originOffer);
        ui->lblTitel->setText(QString("Trade with %1").arg(getName(td.originChar)));
    } else if (td.phase == AH::Common::TradeData::AcceptOffer) {
        ui->stackedWidget->setCurrentWidget(ui->pageFinal);
        fillList(ui->lstFinalOffer, td.originOffer);
        fillList(ui->lstFinalCounter, td.destOffer);
        ui->lblTitel->setText(QString("Trade with %1").arg(getName(td.destChar)));
        ui->btnAcceptFinal->setEnabled(true);
    }
}

void TradeWidget::clear()
{
    m_trade = {};
    ui->stackedWidget->setCurrentWidget(ui->pageInitial);
    ui->wgtCounterOffer->clearItems();
    ui->wgtInitialOffer->clearItems();
    ui->lstFinalOffer->clear();
    ui->lstFinalCounter->clear();
    ui->lstOfferInitial->clear();
    ui->btnAcceptFinal->setEnabled(false);
    ui->btnCounterOffer->setEnabled(false);
    ui->btnInitialOffer->setEnabled(false);
}

void TradeWidget::doCancel()
{
    emit cancelTrade();
    clear();
}

void TradeWidget::doOffer()
{
    AH::Common::TradeData ret;
    ret.phase = m_trade.phase;
    ret.originChar = m_trade.originChar;
    ret.destChar = m_trade.destChar;

    if (ret.phase == AH::Common::TradeData::InitialOffer) {
        ret.originOffer = ui->wgtInitialOffer->getSelectedItems();
        emit selectedTrade(ret);
    } else if (ret.phase == AH::Common::TradeData::CounterOffer) {
        ret.originOffer = m_trade.originOffer;
        ret.destOffer = ui->wgtCounterOffer->getSelectedItems();
        emit selectedTrade(ret);
    } else if (ret.phase == AH::Common::TradeData::AcceptOffer) {
        ret.originOffer = m_trade.originOffer;
        ret.destOffer = m_trade.destOffer;
        emit selectedTrade(ret);
    } else {
        emit cancelTrade();
    }

    clear();
}

void TradeWidget::doAccept()
{
    doOffer();
}

void TradeWidget::displayItem(QListWidgetItem *itm)
{
    if (itm)
        displayItem(itm->data(ObjectListItem::ObjectIdRole).toString());
}

void TradeWidget::displayItem(const QString &id)
{
    if (!id.startsWith("$:"))
        emit itemInfoRequested(id);
}

void TradeWidget::fillList(QListWidget *lst, const QStringList &items)
{
    QStringList reqs;
    for (const QString &s : items) {
        if (!items.startsWith("$:"))
            reqs << s;
    }
    ObjectRegistry::instance()->getObjectsOfType(reqs, AH::Common::RequestObjectsData::Unknown);
    lst->clear();
    for (const QString &id : items) {
        QListWidgetItem *itm = new ObjectListItem(id);
        lst->addItem(itm);
    }
}

QString TradeWidget::getName(QString characterId)
{
    auto obj = ObjectRegistry::instance()->getObject(characterId, AH::Common::RequestObjectsData::Character);
    if (obj.type != AH::Common::RequestObjectsData::Unknown) {
        AH::Common::CharacterData c;
        obj.data >> c;
        return c.investigatorData().name();
    }
    return characterId;
}
