#include "choicewidget.h"
#include "ui_choicewidget.h"

using namespace AH::Common;

ChoiceWidget::ChoiceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChoiceWidget),
    m_type(ChoiceData::None)
{
    ui->setupUi(this);
    connect(ui->wgtObjSelection, SIGNAL(requestObjectInfo(QString)), this, SLOT(displayInfo(QString)));
}

ChoiceWidget::~ChoiceWidget()
{
    delete ui;
}

void ChoiceWidget::offerChoice(ChoiceData choice)
{
    ui->lblTitle->setText(choice.description());
    switch (choice.type()) {
    case ChoiceData::ChooseObjects:
        selectFromObjectList(choice.getObjectIds(), choice.getMin(), choice.getMax());
        break;
    case ChoiceData::ChoosePayment:
        selectPayment(choice.getCosts());
        break;
    case ChoiceData::ChooseString:
        selectString(choice.description(), choice.getStrings());
        break;
    default:
        Q_ASSERT(false);
    }
}

void ChoiceWidget::selectFromObjectList(QStringList objectIds, int min, int max)
{
    m_type = ChoiceData::ChooseObjects;
    ui->wgtObjSelection->setItems(objectIds);
    ui->wgtObjSelection->setMaxItems(max);
    ui->wgtObjSelection->setMinItems(min);
    ui->stkChoices->setCurrentWidget(ui->pageItemSelector);
}

void ChoiceWidget::selectPayment(Cost cost)
{
    m_type = ChoiceData::ChoosePayment;
    ui->pagePaymentSelector->displayPayments(cost);
    ui->stkChoices->setCurrentWidget(ui->pagePaymentSelector);
}

void ChoiceWidget::selectString(QString desc, QList<ChoiceData::OptionData> options)
{
    m_type = ChoiceData::ChooseString;
    ui->pageStrings->displayChoices(desc, options);
    ui->stkChoices->setCurrentWidget(ui->pageStrings);
}

void ChoiceWidget::on_btnOk_clicked()
{
    switch (m_type) {
    case ChoiceData::ChooseObjects:
    {
        QStringList sels = ui->wgtObjSelection->getSelectedItems();
        emit choiceSelected(sels);
        break;
    }

    case ChoiceData::ChoosePayment:
    {
        int sels = ui->pagePaymentSelector->getSelectedPaymentIndex();
        emit choiceSelected(sels);
        break;
    }

    case ChoiceData::ChooseString:
    {
        QString selId = ui->pageStrings->getSelectedChoiceId();
        emit choiceSelected(selId);
        break;
    }
    case ChoiceData::None:
        emit choiceCanceled();
        break;
    }
}

void ChoiceWidget::on_btnCancel_clicked()
{
    ui->wgtObjSelection->clearItems();
    ui->pagePaymentSelector->clearPayments();
    emit choiceCanceled();
}

void ChoiceWidget::displayInfo(const QString &id)
{
    ui->wgtInfo->displayItemInfo(id);
}
