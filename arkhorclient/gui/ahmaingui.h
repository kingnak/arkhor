#ifndef AHMAINGUI_H
#define AHMAINGUI_H

#include <QWidget>
#include <gameoptiondata.h>
#include <fielddata.h>
#include <attributesliderdata.h>
#include <diedata.h>
#include <characterdata.h>
#include <objectdata.h>
#include <gameobjectdata.h>
#include <monsterdata.h>
#include <gatedata.h>
#include <encounterdata.h>
#include <mythosdata.h>
#include <choicedata.h>
#include <gamesettingdata.h>
#include <QListWidgetItem>
#include <playerdata.h>
#include "asyncobjectreceiver.h"

class ObjectRegistry;
class ConnectionHandler;
class AhBoardScene;
class QListWidgetItem;

namespace Ui {
class AhMainGui;
}

class AhMainGui : public QWidget
{
    Q_OBJECT

public:
    explicit AhMainGui(QWidget *parent = 0);
    ~AhMainGui();

    void initConnection(ConnectionHandler *conn);
    void setThisPlayerId(QString id);
    void setThisCharacterId(QString id);

    enum {
        ObjectIdRole = Qt::UserRole
    };

public slots:
    void start();
    void refitGui();
    void dismissInfoPane();

private slots:
    void doDismissInfoPane();

    void characterInstantiated(QString playerId, QString characterId);

    void displayItemInfo(const QString &id);

    void displayInventoryData(QListWidgetItem *itm);

    void gameSettingUpdate(AH::Common::GameSettingData data);

    void textMessage(const QString &msg);

    void chooseOption(QList<AH::Common::GameOptionData> opts);
    void optionChosen(QString id);

    void chooseMove(AH::Common::FieldData::FieldID startId, int movementPoints);
    void movementChosen(QList<AH::Common::FieldData::FieldID> path);
    void movementCanceled();

    void chooseFocus(QList<AH::Common::AttributeSliderData> sliders, int focusAmount);
    void focusChosen(QList<int> diffs);

    void chooseSkill(QList<AH::Common::ModifiedPropertyValueData> options);
    void skillChoosen(AH::Common::PropertyValueData::Property skill);

    void chooseMonster(QString desc, QList<AH::Common::MonsterData> monsters);
    void monsterSelected(QString id);

    void showDieRollInfo(AH::Common::DieRollTestData data);
    void dieUpdateChosen(AH::Common::DieTestUpdateData upd);

    //void updateObject(AH::Common::DescribeObjectsData::ObjectDescription desc);
    void updateCharacter(AH::Common::CharacterData c);

    void chooseWeapons(QList<AH::Common::GameObjectData> weapons, AH::Common::ModifiedPropertyValueData hands);
    void weaponsCanceled();
    void weaponsSelected(QStringList weaponIds);

    void chooseEncounter(AH::Common::EncounterData encounter);
    void encounterSelected(QString id);

    void displayMythos(AH::Common::MythosData mythos);
    void acknowledgeMythos();
    void finishMythos();

    void displayMonsterMovement(AH::Common::MonsterData monster);
    void acknowledgeMonsterMovement();
    void finishMonsterMovement();

    void offerChoice(AH::Common::ChoiceData choice);
    void choiceSelected(AH::Common::ChoiceResponseData resp);
    void choiceCanceled();

    void clearTempObject();

    void won(QString msg);
    void lost(QString msg);
    void showAlert(QString msg);
    void phaseChange(AH::GamePhase ph);
    void playerChange(QString id);

    void on_btnSkipOptions_clicked();

private:
    Ui::AhMainGui *ui;
    AhBoardScene *m_scene;
    ConnectionHandler *m_conn;
    ObjectRegistry *m_registry;
    QTimer *m_dismissTimer;
    AH::Common::PlayerData::AutoSkipData m_skipOption;

    //QString m_pendingDisplayId;
};

class InventoryListItem : public QListWidgetItem, public AsyncObjectReceiver
{
public:
    explicit InventoryListItem(QString objectId);

    virtual void objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc);
};

#include <QAbstractAnimation>
class VisibilityAnimation : public QAbstractAnimation {
public:
    VisibilityAnimation(QWidget *target, bool vis, QObject *parent = 0)
        : QAbstractAnimation(parent), m_target(target), m_vis(vis) {}

    int duration() const { return 0; }

protected:
    void updateCurrentTime(int currentTime) {
        Q_UNUSED(currentTime)
        if (this->direction() == Backward)
            m_target->setVisible(!m_vis);
        else
            m_target->setVisible(m_vis);
    }

private:
    QWidget *m_target;
    bool m_vis;
};

#endif // AHMAINGUI_H
