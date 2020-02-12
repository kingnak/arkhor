#ifndef AHGRAPHICSVIEW_H
#define AHGRAPHICSVIEW_H

#include <QGraphicsView>
#include <fielddata.h>

class QVariantAnimation;

class AhGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit AhGraphicsView(QWidget *parent = 0);

public:
    QSize sizeHint() const;

signals:
    void receivedFocus();

public slots:
    void zoomIn();
    void zoomOut();
    void centerOnFieldAnimated(AH::Common::FieldData::FieldID id, qreal zoom = 1.5);
    void centerOnPointAnimated(QPointF p, qreal zoom = 1.5);
    void centerOnFieldStatic(AH::Common::FieldData::FieldID id, qreal zoom = 1.5);
    void centerOnPointStatic(QPointF p, qreal zoom = 1.5);
    void centerOnField(AH::Common::FieldData::FieldID id, bool animate, qreal zoom = 1.5);
    void centerOnPoint(QPointF p, bool animate, qreal zoom = 1.5);

    void storeViewport();
    void restoreViewport();

protected:
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    qreal currentScaleFactor() const;
    void scaleView(qreal scaleFactor);
    void scaleTo(qreal scaleFactor);

    QVariantAnimation *zoomAnimation(qreal toFactor, int duration = 500);
    QVariantAnimation *translateAnimation(QPointF center, int duration = 500);

private:
    QPointF m_vpCenter;
    qreal m_vpZoom;
};

#endif // AHGRAPHICSVIEW_H
