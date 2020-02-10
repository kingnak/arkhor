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
    void centerOnField(AH::Common::FieldData::FieldID id);

protected:
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    qreal currentScaleFactor() const;
    void scaleView(qreal scaleFactor);
    void scaleTo(qreal scaleFactor);

    QVariantAnimation *zoomAnimation(qreal toFactor, int duration = 500);
    QVariantAnimation *translateAnimation(QPointF center, int duration = 500);

};

#endif // AHGRAPHICSVIEW_H
