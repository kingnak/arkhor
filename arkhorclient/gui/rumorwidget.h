#ifndef RUMORWIDGET_H
#define RUMORWIDGET_H

#include <QWidget>
#include <mythosdata.h>
#include "asyncobjectreceiver.h"

class QLabel;

class RumorWidget : public QWidget, public AsyncObjectReceiver
{
    Q_OBJECT
public:
    explicit RumorWidget(bool shortDisplay = true, QWidget *parent = 0);

    void objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc) override;

signals:
    void rumorInfoRequested(QString id);

public slots:
    void displayRumor(const QString &id);
    void setShortDisplay(bool shortDisplay);
    void setLongDisplay(bool longDisplay);

public slots:
    void displayRumor(const AH::Common::MythosData *rumor);

private:
    QLabel *m_name;
    QLabel *m_desc;
    QLabel *m_dynProps;

    QString m_subscribedId;
    bool m_shortDisplay;
};

#endif // RUMORWIDGET_H
