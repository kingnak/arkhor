#ifndef ENVIRONMENTSHORTWIDGET_H
#define ENVIRONMENTSHORTWIDGET_H

#include <QWidget>
#include <mythosdata.h>
#include "asyncobjectreceiver.h"

class QLabel;

class EnvironmentShortWidget : public QWidget, public AsyncObjectReceiver
{
    Q_OBJECT
public:
    explicit EnvironmentShortWidget(QWidget *parent = 0);

    virtual void objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc);

signals:
    void environmentInfoRequested(QString id);

public slots:
    void displayEnvironment(QString id);

private:
    void displayEnvironment(AH::Common::MythosData *env);

private:
    QLabel *m_name;
    QLabel *m_type;
    QLabel *m_mods;
};

#endif // ENVIRONMENTSHORTWIDGET_H
