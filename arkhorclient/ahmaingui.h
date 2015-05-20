#ifndef AHMAINGUI_H
#define AHMAINGUI_H

#include <QWidget>

namespace Ui {
class AhMainGui;
}

class AhMainGui : public QWidget
{
    Q_OBJECT

public:
    explicit AhMainGui(QWidget *parent = 0);
    ~AhMainGui();

private slots:
    void displayItemInfo(const QString &id);

private:
    Ui::AhMainGui *ui;
};

#endif // AHMAINGUI_H
