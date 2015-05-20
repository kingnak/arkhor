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

signals:
    void itemInfoRequested(QString id);
    void dieUpdateChosen(AH::Common::DieTestUpdateData upd);

private slots:
    void updateClueBurnAmount(int ct);
    void on_btnOk_clicked();
    void modifierLinkClicked();

private:
    void cleanDice();
    void cleanModifiers();

private:
    int m_clueBurnFactor;

private:
    Ui::DieRollWidget *ui;
};

#endif // DIEROLLWIDGET_H
