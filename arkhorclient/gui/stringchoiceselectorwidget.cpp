#include "stringchoiceselectorwidget.h"
#include <QtWidgets>
//#include "flowlayout.h"
#include "../utils.h"
#include "doubleclickbutton.h"

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
    l->addWidget(m_optionsWidget);
    l->addWidget(m_description);
    l->addWidget(m_display);
    l->addStretch(1);

    connect(m_description, SIGNAL(linkActivated(QString)), this, SIGNAL(objectInfoRequested(QString)));
}

QString StringChoiceSelectorWidget::getSelectedChoiceId() const
{
    if (m_selIdx >= 0 && m_selIdx < m_options.size()) {
        return m_options[m_selIdx].id;
    }
    return QString::null;
}

void StringChoiceSelectorWidget::displayChoices(const QString &desc, const QList<AH::Common::ChoiceData::OptionData> &opts)
{
    clearChoices();

    m_description->setText(desc);
    m_options = opts;
    int i = 0;
    for (const auto &o : m_options) {
        DoubleClickButton *btn = new DoubleClickButton(o.name);
        btn->setCheckable(true);
        btn->setProperty(PROPERTY_CHOICE_INDEX, i);
        connect(btn, SIGNAL(clicked()), this, SLOT(choiceSelected()));
        connect(btn, SIGNAL(doubleClicked()), this, SIGNAL(activateChoice()));
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
        while ((child = l->takeAt(0)) != nullptr) {
            delete child;
        }
    }
    for (auto w : m_optionsWidget->findChildren<QPushButton*>()) {
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
    for (auto b : btns) {
        b->setChecked(false);
    }
    qobject_cast<QPushButton*> (sender())->setChecked(true);

    m_selIdx = sender()->property(PROPERTY_CHOICE_INDEX).toInt();
    QString desc = m_options[m_selIdx].description;
    m_display->setText(desc);
    emit choiceOk(true);
}
