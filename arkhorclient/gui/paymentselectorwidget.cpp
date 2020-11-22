#include "paymentselectorwidget.h"
//#include "flowlayout.h"
#include "../utils.h"
#include <QtWidgets>
#include "doubleclickbutton.h"

#define PROPERTY_COST_DESCRIPTION "COST_DESCRIPTION"
#define PROPERTY_COST_INDEX "COST_INDEX"

PaymentSelectorWidget::PaymentSelectorWidget(QWidget *parent) :
    QWidget(parent), m_selIdx(-1)
{
    m_display = new QLabel;
    m_description = new QLabel;
    m_optionsWidget = new QWidget;

    m_optionsWidget->setLayout(new QVBoxLayout);

    QVBoxLayout *l = new QVBoxLayout(this);
    l->addWidget(m_optionsWidget);
    l->addWidget(m_description);
    l->addWidget(m_display);
    l->addStretch(1);

    connect(m_description, SIGNAL(linkActivated(QString)), this, SIGNAL(objectInfoRequested(QString)));
}

AH::Common::CostList PaymentSelectorWidget::getSelectedPayment() const
{
    if (m_selIdx >= 0 && m_selIdx < m_cost.getAlternatives().size()) {
        return m_cost.getAlternatives()[m_selIdx];
    }
    return AH::Common::CostList();
}

int PaymentSelectorWidget::getSelectedPaymentIndex() const
{
    return m_selIdx;
}

void PaymentSelectorWidget::displayPayments(QString desc, AH::Common::Cost costs)
{
    clearPayments();

    m_description->setText(desc);
    m_cost = costs;
    int i = 0;
    foreach (AH::Common::CostList l, m_cost.getAlternatives()) {
        QStringList desc;
        foreach (AH::Common::CostItem i, l) {
            QString s = QString("%1 %2")
                    .arg(i.amount)
                    .arg(Utils::stringForCostItem(i.type));
            desc << s;
        }
        DoubleClickButton *btn = new DoubleClickButton(QString("Alternative &%1").arg(i+1));
        btn->setCheckable(true);
        btn->setProperty(PROPERTY_COST_DESCRIPTION, desc.join("\nAND\n"));
        btn->setProperty(PROPERTY_COST_INDEX, i);
        connect(btn, SIGNAL(clicked()), this, SLOT(alternativeSelected()));
        connect(btn, SIGNAL(doubleClicked()), this, SIGNAL(activateChoice()));
        m_optionsWidget->layout()->addWidget(btn);
        i++;
    }
}

void PaymentSelectorWidget::clearPayments()
{
    emit choiceOk(false);
    QLayout *l = m_optionsWidget->layout();
    if (l) {
        QLayoutItem *child;
        while ((child = l->takeAt(0)) != 0) {
            delete child;
        }
    }
    foreach (QWidget *w, m_optionsWidget->findChildren<QPushButton*>()) {
        delete w;
    }

    m_description->setText("");
    m_display->setText("");
    m_cost.clear();
    m_selIdx = -1;
}

void PaymentSelectorWidget::alternativeSelected()
{
    QList<QPushButton*> btns = m_optionsWidget->findChildren<QPushButton*>();
    foreach (QPushButton *b, btns) {
        b->setChecked(false);
    }
    qobject_cast<QPushButton*> (sender())->setChecked(true);

    m_selIdx = sender()->property(PROPERTY_COST_INDEX).toInt();
    QString desc = sender()->property(PROPERTY_COST_DESCRIPTION).toString();
    m_display->setText(desc);
    emit choiceOk(true);
}
