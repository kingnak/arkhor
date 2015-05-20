#include "paymentselectorwidget.h"
#include "flowlayout.h"
#include "../utils.h"
#include <QtGui>

#define PROPERTY_COST_DESCRIPTION "COST_DESCRIPTION"
#define PROPERTY_COST_INDEX "COST_INDEX"

PaymentSelectorWidget::PaymentSelectorWidget(QWidget *parent) :
    QWidget(parent), m_selIdx(-1)
{
    m_display = new QLabel;
    m_optionsWidget = new QWidget;

    m_optionsWidget->setLayout(new FlowLayout);

    QVBoxLayout *l = new QVBoxLayout(this);
    l->addWidget(m_display, 1);
    l->addWidget(m_optionsWidget);
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

void PaymentSelectorWidget::displayPayments(AH::Common::Cost costs)
{
    clearPayments();

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
        QPushButton *btn = new QPushButton(QString("Alternative %1").arg(i+1));
        btn->setProperty(PROPERTY_COST_DESCRIPTION, desc.join("\nAND\n"));
        btn->setProperty(PROPERTY_COST_INDEX, i);
        connect(btn, SIGNAL(clicked()), this, SLOT(alternativeSelected()));
        m_optionsWidget->layout()->addWidget(btn);
        i++;
    }
}

void PaymentSelectorWidget::clearPayments()
{
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

    m_display->setText("");
    m_cost.clear();
    m_selIdx = -1;
}

void PaymentSelectorWidget::alternativeSelected()
{
    m_selIdx = sender()->property(PROPERTY_COST_INDEX).toInt();
    QString desc = sender()->property(PROPERTY_COST_DESCRIPTION).toString();
    m_display->setText(desc);
}