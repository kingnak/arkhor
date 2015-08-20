#ifndef AHGRAPHICSVIEW_H
#define AHGRAPHICSVIEW_H

#include <QGraphicsView>

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

protected:
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    void scaleView(qreal scaleFactor);
private:
    double m_zoom;

};

#endif // AHGRAPHICSVIEW_H
