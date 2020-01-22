#ifndef OPTIONCHOOSER_H
#define OPTIONCHOOSER_H

#include <QWidget>
#include <QPointer>
#include <gameoptiondata.h>
#include <propertyvaluedata.h>
#include <encounterdata.h>
#include "asyncobjectreceiver.h"

namespace Ui {
class OptionChooser;
}

class QPushButton;

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
    void setEncounter(AH::Common::EncounterData enc);

signals:
    void optionChosen(QString id);
    void skillChosen(AH::Common::PropertyValueData::Property skill);
    void encounterChosen(QString id);
    void objectDescriptionRequested(QString id);

private slots:
    void cleanupOptions();
    void cleanupMore();
    void describeObject(QString id);

    void showOption();
    void on_btnOptionActivate_clicked();

private:
    static QString displayCosts(const AH::Common::Cost &costs);
    void setMoreWidget(QWidget *w);

private:
    enum {
        ChooseOption,
        ChooseSkill,
        ChooseEncounter
    } m_type;

    Ui::OptionChooser *ui;

    struct OptionDescUpdater : public AsyncObjectReceiver {
        QPointer<QPushButton> btn;
        void objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc);
        OptionDescUpdater(QPushButton *b);
    };

    QWidget *m_moreWgt;
};

#endif // OPTIONCHOOSER_H
