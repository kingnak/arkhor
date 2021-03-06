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
    ~ChoiceWidget() override;

public slots:
    void offerChoice(const AH::Common::ChoiceData &choice);

    void selectFromObjectList(const QStringList &objectIds, int min = 0, int max = 1);
    void selectPayment(const QString &desc, const AH::Common::Cost &cost);
    void selectString(const QString &desc, const QList<AH::Common::ChoiceData::OptionData> &options);

    void selectActiveChoice();

signals:
    void choiceSelected(AH::Common::ChoiceResponseData resp);
    void choiceCanceled();
    void objectInfoRequested(const QString &id);

private slots:
    void on_btnCancel_clicked();

    void displayInfo(const QString &id);

private:
    Ui::ChoiceWidget *ui;
    AH::Common::ChoiceData::ChoiceType m_type;
};

#endif // CHOICEWIDGET_H
