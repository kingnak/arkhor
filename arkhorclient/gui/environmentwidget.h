#ifndef ENVIRONMENTWIDGET_H
#define ENVIRONMENTWIDGET_H

#include <QWidget>
#include <mythosdata.h>
#include "asyncobjectreceiver.h"

class QLabel;

class EnvironmentWidget : public QWidget, public AsyncObjectReceiver
{
    Q_OBJECT
public:
    explicit EnvironmentWidget(bool shortDisplay = true, QWidget *parent = 0);

    virtual void objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc);

signals:
    void environmentInfoRequested(QString id);

public slots:
    void displayEnvironment(QString id);
    void setShortDisplay(bool shortDisplay);
    void setLongDisplay(bool longDisplay);

public slots:
    void displayEnvironment(const AH::Common::MythosData *env);

private:
    bool m_shortDisplay;
    QLabel *m_name;
    QLabel *m_type;
    QLabel *m_mods;
    QLabel *m_desc;
};

#endif // ENVIRONMENTWIDGET_H
