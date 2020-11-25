#ifndef OPTIONCHOOSER_H
#define OPTIONCHOOSER_H

#include <QWidget>
#include <QPointer>
#include <gameoptiondata.h>
#include <propertyvaluedata.h>
#include <encounterdata.h>
#include <monsterdata.h>
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
    ~OptionChooser() override;

    QSize sizeHint() const override { return QSize(100,100); }
    QSize minimumSizeHint() const override { return QSize(100,100); }

public slots:
    void setOptions(const QList<AH::Common::GameOptionData> &opts);
    void setSkills(const QList<AH::Common::ModifiedPropertyValueData> &opts);
    void setEncounter(const AH::Common::EncounterData &enc);
    void setMonsters(const QString &desc, const QList<AH::Common::MonsterData> &monsters);

signals:
    void optionChosen(QString id);
    void skillChosen(AH::Common::PropertyValueData::Property skill);
    void encounterChosen(QString id);
    void monsterChosen(QString id);
    void objectDescriptionRequested(QString id);

private slots:
    void cleanupOptions();
    void cleanupMore();
    void describeObject(const QString &id);

    void showOption();
    void on_btnOptionActivate_clicked();

private:
    static QString displayCosts(const AH::Common::Cost &costs);
    void setMoreWidget(QWidget *w);
    void showBaseProperty(const AH::Common::ModifiedPropertyValueData &p);

private:
    enum {
        ChooseOption,
        ChooseSkill,
        ChooseEncounter,
        ChooseMonster
    } m_type;

    Ui::OptionChooser *ui;

    struct OptionDescUpdater : public AsyncObjectReceiver {
        QPointer<QPushButton> btn;
        void objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc) override;
        OptionDescUpdater(QPushButton *b);
    };

    QWidget *m_moreWgt;
};

#endif // OPTIONCHOOSER_H
