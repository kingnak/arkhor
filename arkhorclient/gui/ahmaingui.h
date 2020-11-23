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
#include <tradedata.h>
#include "asyncobjectreceiver.h"
#include "objectregistry.h"

class ObjectRegistry;
class ConnectionHandler;
class AhBoardScene;
class QListWidgetItem;
class DetailCardWidget;

namespace Ui {
class AhMainGui;
}

class AhMainGui : public QWidget
{
    Q_OBJECT

public:
    explicit AhMainGui(QWidget *parent = 0);
    ~AhMainGui() override;

    void initConnection(ConnectionHandler *conn);

public slots:
    void setThisPlayerId(const QString &id);
    void setThisCharacterId(const QString &id);
    void start();
    void refitGui();
    void dismissInfoPane();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void expandInfoPane();
    void doDismissInfoPane();
    void updateSceneNeighbours(const QList<ObjectRegistry::FieldDescription> &descs);

    void characterInstantiated(const QString &playerId, const QString &characterId);

    void displayItemInfo(const QString &id);
    void fieldInfoRequested(AH::Common::FieldData::FieldID id);
    void displayAncientOne(const QString &id);
    void displayCharacterDetail(const QString &id);

    void displayInventoryData(QListWidgetItem *itm);

    void gameSettingUpdate(const AH::Common::GameSettingData &data);

    void textMessage(const QString &msg);

    void chooseOption(const QList<AH::Common::GameOptionData> &opts);
    void optionChosen(const QString &id);

    void chooseMove(AH::Common::FieldData::FieldID startId, int movementPoints);
    void movementChosen(const QList<AH::Common::FieldData::FieldID> &path);
    void movementCanceled();

    void chooseFocus(const QList<AH::Common::AttributeSliderData> &sliders, int focusAmount);
    void focusChosen(const QList<int> &diffs);

    void chooseSkill(const QList<AH::Common::ModifiedPropertyValueData> &options);
    void skillChoosen(AH::Common::PropertyValueData::Property skill);

    void chooseMonster(const QString &desc, const QList<AH::Common::MonsterData> &monsters);
    void monsterSelected(const QString &id);

    void showDieRollInfo(const AH::Common::DieRollTestData &data);
    void dieUpdateChosen(const AH::Common::DieTestUpdateData &upd);

    //void updateObject(AH::Common::DescribeObjectsData::ObjectDescription desc);
    void updateCharacter(const AH::Common::CharacterData &c);

    void chooseWeapons(const QList<AH::Common::GameObjectData> &weapons, const AH::Common::ModifiedPropertyValueData &hands);
    void weaponsCanceled();
    void weaponsSelected(const QStringList &weaponIds);

    void chooseEncounter(const AH::Common::EncounterData &encounter);
    void encounterSelected(const QString &id);

    void displayMythos(const AH::Common::MythosData &mythos);
    void acknowledgeMythos();
    void finishMythos();

    void displayMonsterMovement(const AH::Common::MonsterData &monster);
    void acknowledgeMonsterMovement();
    void finishMonsterMovement();

    void offerChoice(const AH::Common::ChoiceData &choice);
    void choiceSelected(const AH::Common::ChoiceResponseData &resp);
    void choiceCanceled();

    void offerTrade(const AH::Common::TradeData &trade);
    void tradeSelected(const AH::Common::TradeData &trade);
    void tradeCanceled(const QString &name);
    void cancelTrade();

    void clearTempObject();

    void won(const QString &msg);
    void lost(const QString &msg);
    void showAlert(const QString &msg);
    void phaseChange(AH::GamePhase ph);
    void playerChange(const QString &id);

    void on_btnSkipOptions_clicked();

private:
    void readjustDetailCard();

private:
    Ui::AhMainGui *ui;
    AhBoardScene *m_scene;
    ConnectionHandler *m_conn;
    ObjectRegistry *m_registry;
    QTimer *m_dismissTimer;
    AH::Common::PlayerData::AutoSkipData m_skipOption;

    DetailCardWidget *m_cardWidget;
    //QString m_pendingDisplayId;
};

#include <QAbstractAnimation>
class VisibilityAnimation : public QAbstractAnimation {
public:
    VisibilityAnimation(QWidget *target, bool vis, QObject *parent = 0)
        : QAbstractAnimation(parent), m_target(target), m_vis(vis) {}

    int duration() const override { return 0; }

protected:
    void updateCurrentTime(int currentTime) override {
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
