#ifndef GAMEOBJECTWIDGET_H
#define GAMEOBJECTWIDGET_H

#include <QWidget>
#include <gameobjectdata.h>
#include "asyncobjectreceiver.h"

namespace Ui {
class GameObjectWidget;
}

class GameObjectWidget : public QWidget, public AsyncObjectReceiver
{
    Q_OBJECT

public:
    explicit GameObjectWidget(QWidget *parent = 0);
    ~GameObjectWidget();

    virtual void objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc);

public slots:
    void displayGameObject(const AH::Common::GameObjectData *obj);

private:
    void updateGameObject(const AH::Common::GameObjectData *obj);

private:
    QString m_curObjId;
    Ui::GameObjectWidget *ui;
};

#endif // GAMEOBJECTWIDGET_H
