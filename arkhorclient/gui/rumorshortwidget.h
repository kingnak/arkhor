#ifndef RUMORSHORTWIDGET_H
#define RUMORSHORTWIDGET_H

#include <QWidget>
#include <mythosdata.h>
#include "asyncobjectreceiver.h"

class QLabel;

class RumorShortWidget : public QWidget, public AsyncObjectReceiver
{
    Q_OBJECT
public:
    explicit RumorShortWidget(QWidget *parent = 0);

    virtual void objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc);

signals:
    void rumorInfoRequested(QString id);

public slots:
    void displayRumor(QString id);

private:
    void displayRumor(AH::Common::MythosData *rumor);

private:
    QLabel *m_name;
    QLabel *m_shortDesc;
    QLabel *m_dynProps;

    QString m_subscribedId;
};

#endif // RUMORSHORTWIDGET_H
