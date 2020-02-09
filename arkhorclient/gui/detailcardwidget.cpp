#include "detailcardwidget.h"
#include "ancientonecardwidget.h"
#include "investigatorwidget.h"
#include <QtWidgets>

DetailCardWidget::DetailCardWidget(QWidget *parent) : QWidget(parent)
{
    m_stack = new QStackedWidget;
    m_stack->addWidget(new QWidget);

    m_ao = new AncientOneCardWidget;
    m_stack->addWidget(m_ao);

    m_inv = new InvestigatorWidget;
    m_stack->addWidget(m_inv);

    QHBoxLayout *l = new QHBoxLayout(this);
    l->addWidget(m_stack);
}

QSize DetailCardWidget::sizeHint() const
{
    return m_stack->currentWidget()->sizeHint();
}

QSize DetailCardWidget::minimumSizeHint() const
{
    return m_stack->currentWidget()->minimumSizeHint();
}

void DetailCardWidget::clear()
{
    m_stack->setCurrentIndex(0);
}

void DetailCardWidget::displayAncientOne(AH::Common::AncientOneData *ao)
{
    m_ao->displayAncientOne(ao);
    m_stack->setCurrentWidget(m_ao);
}

void DetailCardWidget::displayCharacter(AH::Common::CharacterData *chr)
{
    if (chr)
        m_inv->displayCharacter(*chr);
    m_stack->setCurrentWidget(m_inv);
}
