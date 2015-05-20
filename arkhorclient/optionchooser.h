#ifndef OPTIONCHOOSER_H
#define OPTIONCHOOSER_H

#include <QWidget>
#include <gameoptiondata.h>

namespace Ui {
class OptionChooser;
}

class OptionChooser : public QWidget
{
    Q_OBJECT

public:
    explicit OptionChooser(QWidget *parent = 0);
    ~OptionChooser();

public slots:
    void setOptions(QList<AH::Common::GameOptionData> opts);

signals:
    void optionChosen(QString id);

private slots:
    void cleanupOptions();

    void showOption();
    void on_btnOptionActivate_clicked();

private:
    Ui::OptionChooser *ui;
};

#endif // OPTIONCHOOSER_H
