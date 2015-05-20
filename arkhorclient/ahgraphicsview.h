#ifndef AHGRAPHICSVIEW_H
#define AHGRAPHICSVIEW_H

#include <QGraphicsView>

class AhGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit AhGraphicsView(QWidget *parent = 0);

signals:

public slots:
    void zoomIn();
    void zoomOut();

protected:
    void wheelEvent(QWheelEvent *event);

private:
    void scaleView(qreal scaleFactor);
private:
    double m_zoom;

};

#endif // AHGRAPHICSVIEW_H
