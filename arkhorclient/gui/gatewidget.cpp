#include "gatewidget.h"
#include "utils.h"
#include "resourcepool.h"
#include <QPainter>

using namespace AH::Common;

GateWidget::GateWidget(QWidget *parent) :
    QWidget(parent)
{
}

QPixmap GateWidget::drawGate(const GateData *g, QSize s)
{
    if (g) {
        if (g->isOpen()) {
            QPixmap ret(s);
            ret.fill(Qt::transparent);
            QPainter p(&ret);

            // Main image
            QPixmap base = ResourcePool::instance()->loadOtherWorldGate(g->destination()).scaled(s);
            p.drawPixmap(0, 0, base);

            // Close Adjustment
            QString close = Utils::fullNumberString(g->closeAdjustment());
            QFont f = ResourcePool::instance()->loadMainFont();
            f.setPixelSize(28);
            f.setBold(false);
            QRect closeRect(120, 90, 32, 32);
            p.setClipRect(closeRect);
            QPainterPath txtPath;
            txtPath.addText(115, 115, f, close);
            p.fillPath(txtPath, QColor(0xD2363A));
            QPainterPathStroker txtStroke;
            txtPath = txtStroke.createStroke(txtPath);
            p.drawPath(txtPath);
            p.setClipping(false);

            // Dimension
            QRect dimRect(63, 119, 36, 36);
            QPixmap dim = ResourcePool::instance()->loadDimensionSymbol(static_cast<AH::Dimension> ((int)g->dimensions()));
            p.drawPixmap(dimRect, dim);
            return ret;

        } else {
            return QPixmap(":/core/marker/gate_closed").scaled(s);
        }
    } else {
        QPixmap ret(s);
        ret.fill(Qt::transparent);
        return ret;
    }
}

QSize GateWidget::sizeHint() const
{
    return minimumSizeHint();
}

QSize GateWidget::minimumSizeHint() const
{
    return QSize(160, 160);
}

void GateWidget::displayGate(const AH::Common::GateData *g)
{
    m_cache = drawGate(g);
    update();
}

void GateWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.drawPixmap(0, 0, m_cache);
}
