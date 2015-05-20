#ifndef OPTIONCHOOSER_H
#define OPTIONCHOOSER_H

#include <QWidget>
#include <gameoptiondata.h>
#include <propertyvaluedata.h>
#include <arkhamencounterdata.h>

namespace Ui {
class OptionChooser;
}

class OptionChooser : public QWidget
{
    Q_OBJECT

public:
    explicit OptionChooser(QWidget *parent = 0);
    ~OptionChooser();

    QSize sizeHint() const { return QSize(100,100); }
    QSize minimumSizeHint() const { return QSize(100,100); }

public slots:
    void setOptions(QList<AH::Common::GameOptionData> opts);
    void setSkills(QList<AH::Common::ModifiedPropertyValueData> opts);
    void setEncounter(AH::Common::ArkhamEncounterData enc);

signals:
    void optionChosen(QString id);
    void skillChosen(AH::Common::PropertyValueData::Property skill);
    void encounterChosen(QString id);

private slots:
    void cleanupOptions();

    void showOption();
    void on_btnOptionActivate_clicked();

private:
    QString displayCosts(const AH::Common::Cost &costs);

private:
    enum {
        ChooseOption,
        ChooseSkill,
        ChooseEncounter
    } m_type;

    Ui::OptionChooser *ui;
};

#endif // OPTIONCHOOSER_H
