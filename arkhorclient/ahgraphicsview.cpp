#include "ahgraphicsview.h"
#include <QWheelEvent>
#include <math.h>

AhGraphicsView::AhGraphicsView(QWidget *parent) :
    QGraphicsView(parent), m_zoom(1)
{
}

QSize AhGraphicsView::sizeHint() const
{
    return QAbstractScrollArea::sizeHint();
}

void AhGraphicsView::zoomIn()
{
    scaleView(qreal(1.2));
}

void AhGraphicsView::zoomOut()
{
    scaleView(qreal(1/1.2));
}

void AhGraphicsView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier) {
        scaleView(pow((double)2, -event->delta() / 240.0));
        event->accept();
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void AhGraphicsView::scaleView(qreal scaleFactor)
{

    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.25 || factor > 4)
        return;

    scale(scaleFactor, scaleFactor);
}
