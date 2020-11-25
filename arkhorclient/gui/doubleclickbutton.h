#ifndef DOUBLECLICKBUTTON_H
#define DOUBLECLICKBUTTON_H

#include <QPushButton>
#include <QMouseEvent>

class DoubleClickButton : public QPushButton
{
    Q_OBJECT
public:
    DoubleClickButton(const QString &title, QWidget *parent = nullptr)
        : QPushButton(title, parent) {}
signals:
    void doubleClicked();
protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            emit doubleClicked();
            event->accept();
        }
    }
};


#endif // DOUBLECLICKBUTTON_H
