#ifndef MYTHOSCARDWIDGET_H
#define MYTHOSCARDWIDGET_H

#include <QWidget>
#include <mythosdata.h>

namespace Ui {
class MythosCardWidget;
}

class MythosCardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MythosCardWidget(QWidget *parent = 0);
    ~MythosCardWidget();

public slots:
    void displayMythosCard(const AH::Common::MythosData *mythos);
    void displayNothing();

private:
    Ui::MythosCardWidget *ui;
};

#endif // MYTHOSCARDWIDGET_H
