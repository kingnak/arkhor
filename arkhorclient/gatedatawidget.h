#ifndef GATEDATAWIDGET_H
#define GATEDATAWIDGET_H

#include <QWidget>
#include <gatedata.h>

namespace Ui {
class GateDataWidget;
}

class GateDataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GateDataWidget(QWidget *parent = 0);
    ~GateDataWidget();

public slots:
    void displayGate(const AH::Common::GateData *g);

private:
    Ui::GateDataWidget *ui;
};

#endif // GATEDATAWIDGET_H
