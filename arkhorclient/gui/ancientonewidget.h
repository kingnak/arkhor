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
    ~AncientOneWidget() override;

    void objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc) override;

signals:
    void ancientOneInfoRequested(QString id);

public slots:
    void displayAncientOne(const QString &aoId);
    void displayAncientOne(const AH::Common::AncientOneData *ao);

private:
    void updateAncientOne(const AH::Common::AncientOneData *ao);

private:
    Ui::AncientOneWidget *ui;
    QString m_curAoId;
    bool m_signalAOChange;
};

#endif // ANCIENTONESHORTWIDGET_H
