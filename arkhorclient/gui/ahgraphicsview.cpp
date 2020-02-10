#include "ahgraphicsview.h"
#include <QWheelEvent>
#include "ahboardscene.h"
#include "ahfielditem.h"
#include <math.h>
#include <QtGui>
#include <QTimeLine>

AhGraphicsView::AhGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
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

void AhGraphicsView::centerOnFieldAnimated(AH::Common::FieldData::FieldID id)
{
    centerOnField(id, true);
}

void AhGraphicsView::centerOnPointAnimated(QPointF p)
{
    centerOnPoint(p, true);
}

void AhGraphicsView::centerOnFieldStatic(AH::Common::FieldData::FieldID id)
{
    centerOnField(id, false);
}

void AhGraphicsView::centerOnPointStatic(QPointF p)
{
    centerOnPoint(p, false);
}

void AhGraphicsView::centerOnField(AH::Common::FieldData::FieldID id, bool animate)
{
    auto f = qobject_cast<AhBoardScene*>(scene())->getField(id);
    if (f) {
        auto cEnd = f->mapToScene(f->boundingRect().center());
        centerOnPoint(cEnd, animate);
    }
}

void AhGraphicsView::centerOnPoint(QPointF p, bool animate)
{
    if (!animate) {
        centerOn(p);
        scaleTo(1.5);
        return;
    }

    QParallelAnimationGroup *anim = new QParallelAnimationGroup;
    anim->addAnimation(translateAnimation(p));
    anim->addAnimation(zoomAnimation(1.5));
    QEventLoop l;
    connect(anim, &QAbstractAnimation::finished, &l, &QEventLoop::quit);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    l.exec();
}

void AhGraphicsView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier) {
        scaleView(pow((double)2, event->delta() / 240.0));
        event->accept();
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void AhGraphicsView::mousePressEvent(QMouseEvent *event)
{
    emit receivedFocus();
    QGraphicsView::mousePressEvent(event);
}

qreal AhGraphicsView::currentScaleFactor() const
{
    return transform().mapRect(QRectF(0, 0, 1, 1)).width();
}

void AhGraphicsView::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.25 || factor > 4)
        return;

    scale(scaleFactor, scaleFactor);
}

void AhGraphicsView::scaleTo(qreal scaleFactor)
{
    QMatrix m;
    m.scale(scaleFactor, scaleFactor);
    this->setMatrix(m);
}

QVariantAnimation *AhGraphicsView::zoomAnimation(qreal toFactor, int duration)
{
    QVariantAnimation *az = new QVariantAnimation;
    az->setDuration(duration);
    az->setStartValue(currentScaleFactor());
    az->setEndValue(toFactor);
    connect(az, &QVariantAnimation::valueChanged, [=](const QVariant &v) {
        this->scaleTo(v.toDouble());
    });
    return az;
}

QVariantAnimation *AhGraphicsView::translateAnimation(QPointF center, int duration)
{
    auto start = this->mapToScene(viewport()->rect().center());
    QVariantAnimation *ac = new QVariantAnimation;
    ac->setDuration(duration);
    ac->setStartValue(start);
    ac->setEndValue(center);
    connect(ac, &QVariantAnimation::valueChanged, [=](const QVariant &v) {
        this->centerOn(v.toPoint());
    });
    return ac;
}
