#ifndef ANCIENTONEWIDGET_H
#define ANCIENTONEWIDGET_H

#include <QWidget>
#include "asyncobjectreceiver.h"
#include <ancientonedata.h>

namespace Ui {
class AncientOneWidget;
}

class AncientOneWidget : public QWidget, public AsyncObjectReceiver
{
    Q_OBJECT

public:
    explicit AncientOneWidget(QWidget *parent = 0);
    ~AncientOneWidget();

    virtual void objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc);

signals:
    void ancientOneInfoRequested(QString id);

public slots:
    void displayAncientOne(QString aoId);
    void displayAncientOne(const AH::Common::AncientOneData *ao);

private:
    Ui::AncientOneWidget *ui;
};

#endif // ANCIENTONESHORTWIDGET_H
