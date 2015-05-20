#ifndef DIEROLLWIDGET_H
#define DIEROLLWIDGET_H

#include <QWidget>
#include <diedata.h>

namespace Ui {
class DieRollWidget;
}

class DieRollWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DieRollWidget(QWidget *parent = 0);
    ~DieRollWidget();

public slots:
    void displayDieRoll(AH::Common::DieRollTestData data);

private:
    Ui::DieRollWidget *ui;
};

#endif // DIEROLLWIDGET_H
