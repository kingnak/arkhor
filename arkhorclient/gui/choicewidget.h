#ifndef CHOICEWIDGET_H
#define CHOICEWIDGET_H

#include <QWidget>
#include <choicedata.h>

namespace Ui {
class ChoiceWidget;
}

class ChoiceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChoiceWidget(QWidget *parent = 0);
    ~ChoiceWidget();

public slots:
    void offerChoice(AH::Common::ChoiceData choice);

    void selectFromObjectList(QStringList objectIds, int min = 0, int max = 1);
    void selectPayment(AH::Common::Cost cost);
    void selectString(QString desc, QList<AH::Common::ChoiceData::OptionData> options);

signals:
    void choiceSelected(AH::Common::ChoiceResponseData resp);
    void choiceCanceled();

private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();

    void displayInfo(const QString &id);

private:
    Ui::ChoiceWidget *ui;
    AH::Common::ChoiceData::ChoiceType m_type;
};

#endif // CHOICEWIDGET_H
