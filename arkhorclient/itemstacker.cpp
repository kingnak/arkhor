#include "itemstacker.h"
#include <QtGui>

ItemStacker::ItemStacker(QWidget *parent) :
    QWidget(parent), m_displayOffset(4)
{
    for (int i = DISPLAY_COUNT-1; i >= 0; --i) {
        m_displays[i] = new QPushButton(this);
        m_displays[i]->setVisible(false);
        if (i) m_displays[i]->setEnabled(false);
        else connect(m_displays[i], SIGNAL(clicked()), this, SLOT(itemClicked()));
    }
    m_cur = -1;

    m_next = new QToolButton(this);
    m_prev = new QToolButton(this);
    m_next->setText(">");
    m_prev->setText("<");
    connect(m_next, SIGNAL(clicked()), this, SLOT(rotateNext()));
    connect(m_prev, SIGNAL(clicked()), this, SLOT(rotatePrev()));

    m_next->setVisible(false);
    m_prev->setVisible(false);
}

StackItem ItemStacker::topItem() const
{
    return m_items.value(m_cur);
}

QSize ItemStacker::sizeHint() const
{
    return QSize(qMin(width(), height())+fixedWidth(), qMin(width(), height()));
}

QSize ItemStacker::minimumSizeHint() const
{
    return QSize(50+fixedWidth(), 50);
}

void ItemStacker::setDisplayOffset(int off)
{
    m_displayOffset = off;
}

void ItemStacker::addItem(StackItem itm)
{
    m_items.prepend(itm);
    if (m_cur == -1) {
        m_cur = 0;
    } else if (m_cur > 0) {
        m_cur++;
    }

    updateItemVisibility();

    emit itemAdded(m_items.first());
}

void ItemStacker::removeAt(int idx)
{
    if (idx > m_items.size() || idx < 0) {
        return;
    }

    m_items.removeAt(idx);
    if (m_cur >= idx) {
        m_cur--;
        if (m_cur < 0)
            m_cur = 0;
    }

    updateItemVisibility();

    emit itemRemoved();
}

void ItemStacker::clear()
{
    m_items.clear();
    updateItemVisibility();
    emit itemRemoved();
}

void ItemStacker::setPicSize(QSize s)
{
    resize(s.width()+fixedWidth(), s.height());
}

void ItemStacker::resizeEvent(QResizeEvent *ev)
{
    Q_UNUSED(ev);
    int wn = m_next->sizeHint().width();
    int wp = m_prev->sizeHint().width();

    QSize s = this->size();
    //s.rwidth() -= wn+wp;
    s.rwidth() -= (DISPLAY_COUNT-1)*m_displayOffset;
    s.rheight() -= (DISPLAY_COUNT-1)*m_displayOffset;

    s.rwidth() = qMin(s.width(), s.height());
    s.rheight() = s.width();

    for (int i = 0; i < DISPLAY_COUNT; ++i) {
        QPoint p(i*m_displayOffset, i*m_displayOffset);
        m_displays[i]->setGeometry(QRect(p, s));
        m_displays[i]->setIconSize(s);
    }

    /*
    int hDist = m_displays[0]->rect().right()+(DISPLAY_COUNT-1)*m_displayOffset+BTN_GAP;
    m_next->setGeometry(hDist+wp, 0, m_next->sizeHint().width(), m_next->sizeHint().height());
    m_prev->setGeometry(hDist, 0, m_prev->sizeHint().width(), m_prev->sizeHint().height());
    */
    m_next->setGeometry(this->width()-wn, 0, wn, m_next->sizeHint().height());
    m_prev->setGeometry(this->width()-wp-wn, 0, wp, m_prev->sizeHint().height());
}

void ItemStacker::itemClicked()
{
    emit itemActivated(topItem());
}

void ItemStacker::rotateNext()
{
    if (m_items.size() < 2) {
        return;
    }
    m_cur++;
    if (m_cur >= m_items.size()) {
        m_cur = 0;
    }

    updateItemPixmaps();
}

void ItemStacker::rotatePrev()
{
    if (m_items.size() < 2) {
        return;
    }
    m_cur--;
    if (m_cur < 0) {
        m_cur = m_items.size() - 1;
    }

    updateItemPixmaps();
}

int ItemStacker::fixedWidth() const
{
    //return m_next->sizeHint().width()+m_prev->sizeHint().width() + BTN_GAP;
    return 0;
}

void ItemStacker::updateItemVisibility()
{
    updateItemPixmaps();

    for (int i = 0; i < DISPLAY_COUNT; ++i) {
        m_displays[i]->setVisible(m_items.size() > i);
    }

    m_next->setVisible(m_items.size() > 1);
    m_prev->setVisible(m_items.size() > 1);
}

void ItemStacker::updateItemPixmaps()
{
    int m = qMin(DISPLAY_COUNT+0, m_items.size());
    if (m <= 0) {
        return;
    }

    for (int i = 0; i < m; ++i) {
        int idx = (i+m_cur) % m_items.size();
        m_displays[i]->setIcon(m_items[idx].pixmap());
    }
}

