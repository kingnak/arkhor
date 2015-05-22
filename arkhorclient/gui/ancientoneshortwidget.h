#ifndef ANCIENTONESHORTWIDGET_H
#define ANCIENTONESHORTWIDGET_H

#include <QWidget>
#include "asyncobjectreceiver.h"
#include <ancientonedata.h>

namespace Ui {
class AncientOneShortWidget;
}

class AncientOneShortWidget : public QWidget, public AsyncObjectReceiver
{
    Q_OBJECT

public:
    explicit AncientOneShortWidget(QWidget *parent = 0);
    ~AncientOneShortWidget();

    virtual void objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc);

signals:
    void ancientOneInfoRequested(QString id);

public slots:
    void displayAncientOne(QString aoId);

private:
    void displayAncientOne(AH::Common::AncientOneData *ao);

private:
    Ui::AncientOneShortWidget *ui;
};

#endif // ANCIENTONESHORTWIDGET_H
