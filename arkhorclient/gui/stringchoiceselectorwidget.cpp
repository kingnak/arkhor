#include "stringchoiceselectorwidget.h"
#include <QtWidgets>
//#include "flowlayout.h"
#include "../utils.h"

#define PROPERTY_CHOICE_INDEX "CHOICE_INDEX"

StringChoiceSelectorWidget::StringChoiceSelectorWidget(QWidget *parent) :
    QWidget(parent), m_selIdx(-1)
{
    m_description = new QLabel;
    m_display = new QLabel;
    m_optionsWidget = new QWidget;

    m_display->setWordWrap(true);
    m_optionsWidget->setLayout(new QVBoxLayout);

    QVBoxLayout *l = new QVBoxLayout(this);
    l->addWidget(m_description);
    l->addWidget(m_display, 1);
    l->addWidget(m_optionsWidget);
}

QString StringChoiceSelectorWidget::getSelectedChoiceId() const
{
    if (m_selIdx >= 0 && m_selIdx < m_options.size()) {
        return m_options[m_selIdx].id;
    }
    return QString::null;
}

void StringChoiceSelectorWidget::displayChoices(QString desc, QList<AH::Common::ChoiceData::OptionData> opts)
{
    clearChoices();

    m_description->setText(desc);
    m_options = opts;
    int i = 0;
    foreach (AH::Common::ChoiceData::OptionData o, m_options) {
        QPushButton *btn = new QPushButton(o.name);
        btn->setCheckable(true);
        btn->setProperty(PROPERTY_CHOICE_INDEX, i);
        connect(btn, SIGNAL(clicked()), this, SLOT(choiceSelected()));
        m_optionsWidget->layout()->addWidget(btn);
        i++;
    }
}

void StringChoiceSelectorWidget::clearChoices()
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
    m_options.clear();
    m_selIdx = -1;
}

void StringChoiceSelectorWidget::choiceSelected()
{
    QList<QPushButton*> btns = m_optionsWidget->findChildren<QPushButton*>();
    foreach (QPushButton *b, btns) {
        b->setChecked(false);
    }
    qobject_cast<QPushButton*> (sender())->setChecked(true);

    m_selIdx = sender()->property(PROPERTY_CHOICE_INDEX).toInt();
    QString desc = m_options[m_selIdx].description;
    m_display->setText(desc);
    emit choiceOk(true);
}
