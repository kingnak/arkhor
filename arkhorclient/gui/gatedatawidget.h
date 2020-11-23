#ifndef GATEDATAWIDGET_H
#define GATEDATAWIDGET_H

#include <QWidget>
#include <gatedata.h>
#include "asyncobjectreceiver.h"

namespace Ui {
class GateDataWidget;
}

class GateDataWidget : public QWidget, public AsyncObjectReceiver
{
    Q_OBJECT

public:
    explicit GateDataWidget(QWidget *parent = 0);
    ~GateDataWidget() override;

    void objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc) override;

public slots:
    void displayGate(const AH::Common::GateData *g);

private:
    void updateGate(const AH::Common::GateData *g);

private:
    Ui::GateDataWidget *ui;
    QString m_curGateId;
};

#endif // GATEDATAWIDGET_H
