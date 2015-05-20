#ifndef GAMEOBJECTWIDGET_H
#define GAMEOBJECTWIDGET_H

#include <QWidget>
#include <gameobjectdata.h>

namespace Ui {
class GameObjectWidget;
}

class GameObjectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameObjectWidget(QWidget *parent = 0);
    ~GameObjectWidget();

public slots:
    void displayGameObject(const AH::Common::GameObjectData *obj);

private:
    Ui::GameObjectWidget *ui;
};

#endif // GAMEOBJECTWIDGET_H
