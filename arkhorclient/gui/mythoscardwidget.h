#ifndef MYTHOSCARDWIDGET_H
#define MYTHOSCARDWIDGET_H

#include <QWidget>
#include <mythosdata.h>
#include "asyncobjectreceiver.h"

namespace Ui {
class MythosCardWidget;
}

class MythosCardWidget : public QWidget, public AsyncObjectReceiver
{
    Q_OBJECT

public:
    explicit MythosCardWidget(QWidget *parent = 0);
    ~MythosCardWidget();

    virtual void objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc);

public slots:
    void displayMythosCard(const AH::Common::MythosData *mythos);
    void displayNothing();

private:
    void updateMythosCard(const AH::Common::MythosData *mythos);

private:
    QString m_curMythosId;
    Ui::MythosCardWidget *ui;
};

#endif // MYTHOSCARDWIDGET_H
