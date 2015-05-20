#include "diewidget.h"
#include <QPainter>
#include "resourcepool.h"

DieWidget::DieWidget(QWidget *parent) :
    QWidget(parent), m_val(0)
{
    updateImage();
}

QSize DieWidget::minimumSizeHint() const
{
    return QSize(45, 45);
}

QSizePolicy DieWidget::sizePolicy() const
{
    return QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void DieWidget::setDieValue(int val)
{
    m_val = val;
    updateImage();
}

void DieWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.drawPixmap(0, 0, m_cache);
}

void DieWidget::updateImage()
{
    /*
    QSize s = this->size();
    int realSize = qMin(s.width(), s.height());
    s = QSize(realSize-1, realSize-1);
    */
    QSize s = this->sizeHint();
    int realSize = qMin(s.width(), s.height());
    s = QSize(realSize-1, realSize-1);


    QColor c1 = Qt::white;
    QColor c2;
    if (c1.lightness() < 70) {
        c2 = c1.lighter(200);
    } else {
        c2 = c1.darker();
    }

    // Bkg
    m_cache = QPixmap(s);
    m_cache.fill(Qt::transparent);
    QRectF rect(QPointF(), s);
    QPainter p(&m_cache);
    p.setRenderHint(QPainter::Antialiasing);
    QRadialGradient g(rect.center()+QPointF(0.5,0.5), realSize/2.+5);
    g.setColorAt(0, c1);
    g.setColorAt(0.8, c1);
    g.setColorAt(1, c2);
    p.setBrush(QBrush(g));
    p.setPen(Qt::NoPen);
    p.drawRoundRect(rect);

    p.setBrush(Qt::black);
    p.setPen(Qt::black);

    if (m_val > 0 && m_val < 7) {
        qreal d1 = realSize/4.;
        qreal d2 = realSize/2.;
        qreal d3 = realSize/4.*3;
        QPointF points[] = {
            QPointF(d1,d1), QPointF(d3,d1),
            QPointF(d1,d2), QPointF(d3,d2),
            QPointF(d1,d3), QPointF(d3,d3),
            QPointF(d2,d2)
        };

        if (m_val%2) {
            p.drawEllipse(points[6], 4, 4);
        }
        if (m_val > 1) {
            p.drawEllipse(points[0], 4, 4);
            p.drawEllipse(points[5], 4, 4);
        }
        if (m_val > 3) {
            p.drawEllipse(points[1], 4, 4);
            p.drawEllipse(points[4], 4, 4);
        }
        if (m_val > 5) {
            p.drawEllipse(points[2], 4, 4);
            p.drawEllipse(points[3], 4, 4);
        }
    } else {
        QFont f = ResourcePool::instance()->loadMainFont();
        f.setPixelSize(28);
        f.setBold(true);
        p.setFont(f);
        QString s = m_val > 0 ? QString::number(m_val) : "?";
        p.drawText(rect, s, QTextOption(Qt::AlignHCenter | Qt::AlignCenter));
    }

    update();
}
