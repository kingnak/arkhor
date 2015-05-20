#ifndef OPTIONCHOOSER_H
#define OPTIONCHOOSER_H

#include <QWidget>
#include <gameoptiondata.h>
#include <propertyvaluedata.h>

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
    void setSkills(QList<AH::Common::ModifiedPropertyValueData> opts);

signals:
    void optionChosen(QString id);
    void skillChosen(AH::Common::PropertyValueData::Property skill);

private slots:
    void cleanupOptions();

    void showOption();
    void on_btnOptionActivate_clicked();

private:
    enum {
        ChooseOption,
        ChooseSkill
    } m_type;
    Ui::OptionChooser *ui;
};

#endif // OPTIONCHOOSER_H
