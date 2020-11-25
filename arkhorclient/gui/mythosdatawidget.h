#ifndef MYTHOSDATAWIDGET_H
#define MYTHOSDATAWIDGET_H

#include <QWidget>
#include <mythosdata.h>

namespace Ui {
class MythosDataWidget;
}

class MythosDataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MythosDataWidget(QWidget *parent = 0);
    ~MythosDataWidget() override;

signals:
    void acknowledgeMythos();

public slots:
    void displayMythos(const AH::Common::MythosData *m);

private slots:
    void on_btnAcknowledgeMythos_clicked();

private:
    Ui::MythosDataWidget *ui;
};

/*
class MythosWidget : public QWidget
{

};
*/

#endif // MYTHOSDATAWIDGET_H
