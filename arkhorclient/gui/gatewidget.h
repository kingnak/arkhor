#ifndef GATEWIDGET_H
#define GATEWIDGET_H

#include <QWidget>
#include <gatedata.h>

class GateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GateWidget(QWidget *parent = 0);

    static QPixmap drawGate(const AH::Common::GateData *g, QSize s = QSize(160, 160));

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:

public slots:
    void displayGate(const AH::Common::GateData *g);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap m_cache;
};

#endif // GATEWIDGET_H
