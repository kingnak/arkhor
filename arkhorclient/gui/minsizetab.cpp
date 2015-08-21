#include "minsizetab.h"
#include <QtGui>

MinSizeStack::MinSizeStack(QWidget *p) : QWidget(p), m_cur(0)
{
}

void MinSizeStack::setOrientation(Qt::Orientation orient)
{
    if (orient == Qt::Horizontal) new QHBoxLayout(this);
    else new QVBoxLayout(this);
}

void MinSizeStack::addWidget(QWidget *w)
{
    m_wgts << w;
    layout()->addWidget(w);
    showCurrentWidget();
}

int MinSizeStack::count() const
{
    return m_wgts.size();
}

const QWidget *MinSizeStack::currentWidget() const {
    return m_wgts[m_cur];
}

QSize MinSizeStack::sizeHint() const
{
    return currentWidget()->minimumSize();
}

int MinSizeStack::indexOf(QWidget *w) const
{
    return m_wgts.indexOf(w);
}

void MinSizeStack::setCurrentIndex(int idx)
{
    m_cur = idx;
    showCurrentWidget();
}

void MinSizeStack::setCurrentWidget(QWidget *w)
{
    setCurrentIndex(m_wgts.indexOf(w));
}

void MinSizeStack::showCurrentWidget()
{
    foreach (QWidget *w, m_wgts)
        w->hide();
    m_wgts.at(m_cur)->show();
    updateGeometry();
}

////////////////////////

MinSizeTab::MinSizeTab(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *l = new QVBoxLayout(this);
    l->setContentsMargins(0,0,0,0);
    m_bar = new QTabBar;
    m_stack = new MinSizeStack;
    l->addWidget(m_bar);
    l->addWidget(m_stack, 1);

    connect(m_bar, SIGNAL(currentChanged(int)), this, SLOT(setCurrentIndex(int)));
}

void MinSizeTab::buildFromTab(QTabWidget *tab)
{
    while (tab->count() > 0) {
        m_bar->addTab(tab->tabIcon(0), tab->tabText(0));
        QWidget *w = tab->widget(0);
        tab->removeTab(0);
        m_stack->addWidget(w);
    }
    tab->setVisible(false);
}

void MinSizeTab::setOrientation(Qt::Orientation orient)
{
    m_stack->setOrientation(orient);
}

void MinSizeTab::setCurrentWidget(QWidget *w)
{
    setCurrentIndex(m_stack->indexOf(w));
}

void MinSizeTab::setCurrentIndex(int idx)
{
    if (idx >= 0 && idx < m_stack->count())
        m_stack->setCurrentIndex(idx);
}

