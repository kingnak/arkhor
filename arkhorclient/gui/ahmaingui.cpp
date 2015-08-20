#include "ahmaingui.h"
#include "ui_ahmaingui.h"
#include "ahboardscene.h"
#include "connectionhandler.h"
#include "objectregistry.h"
#include "monsterdata.h"
#include "gatedata.h"
#include "resourcepool.h"
#include <QtGui>

using namespace AH::Common;

AhMainGui::AhMainGui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AhMainGui),
    m_conn(NULL),
    m_registry(NULL)
{
    ui->setupUi(this);

    ui->wgtObjectInfo->setVisible(false);

    m_scene = new AhBoardScene(this);
    ui->grvBoard->setScene(m_scene);
    m_scene->initBoard();

    ui->wgtMovmentChooser->setBoard(m_scene, ui->grvBoard);

    //ui->wgtAncientOne->setVisible(false);
    //ui->wgtCharacter->setVisible(false);
    //ui->wgtEnvironment->setVisible(false);
    //ui->wgtFocusChooser->setVisible(false);
    //ui->wgtMovmentChooser->setVisible(false);
    //ui->wgtMythos->setVisible(false);
    //ui->wgtOptionChooser->setVisible(false);
    //ui->wgtRumor->setVisible(false);

    connect(m_scene, SIGNAL(itemInfoRequested(QString)), this, SLOT(displayItemInfo(QString)));
    connect(ui->wgtDieRoll, SIGNAL(itemInfoRequested(QString)), this, SLOT(displayItemInfo(QString)));
    connect(ui->wgtAncientOne, SIGNAL(ancientOneInfoRequested(QString)), this, SLOT(displayItemInfo(QString)));
    connect(ui->wgtRumor, SIGNAL(rumorInfoRequested(QString)), this, SLOT(displayItemInfo(QString)));
    connect(ui->wgtEnvironment, SIGNAL(environmentInfoRequested(QString)), this, SLOT(displayItemInfo(QString)));
}

AhMainGui::~AhMainGui()
{
    delete ui;
}

void AhMainGui::initConnection(ConnectionHandler *conn)
{
    m_conn = conn;
    m_registry = ObjectRegistry::instance();
    m_registry->init(conn);

    //connect(m_registry, SIGNAL(objectDescribed(AH::Common::DescribeObjectsData::ObjectDescription)), this, SLOT(updateObject(AH::Common::DescribeObjectsData::ObjectDescription)));
    connect(m_registry, SIGNAL(thisCharacterUpdated(AH::Common::CharacterData)), this, SLOT(updateCharacter(AH::Common::CharacterData)));

    connect(m_conn, SIGNAL(playerCharacterInstantiated(QString,QString)), this, SLOT(characterInstantiated(QString,QString)));

    // BOARD
    connect(m_conn, SIGNAL(boardContent(QVariantMap)), m_scene, SLOT(updateBoardFromData(QVariantMap)));

    // SETTING
    connect(m_conn, SIGNAL(settingUpdate(AH::Common::GameSettingData)), this, SLOT(gameSettingUpdate(AH::Common::GameSettingData)));

    // OPTION
    connect(m_conn, SIGNAL(chooseOption(QList<AH::Common::GameOptionData>)), this, SLOT(chooseOption(QList<AH::Common::GameOptionData>)));
    connect(ui->wgtOptionChooser, SIGNAL(optionChosen(QString)), this, SLOT(optionChosen(QString)));

    // MOVE
    connect(m_conn, SIGNAL(chooseMovement(AH::Common::FieldData::FieldID,int)), this, SLOT(chooseMove(AH::Common::FieldData::FieldID,int)));
    connect(ui->wgtMovmentChooser, SIGNAL(movementChosen(QList<AH::Common::FieldData::FieldID>)), this, SLOT(movementChosen(QList<AH::Common::FieldData::FieldID>)));
    connect(ui->wgtMovmentChooser, SIGNAL(movementCancelled()), this, SLOT(movementCanceled()));

    // FOCUS
    connect(m_conn, SIGNAL(chooseFocus(QList<AH::Common::AttributeSliderData>,int)), this, SLOT(chooseFocus(QList<AH::Common::AttributeSliderData>,int)));
    connect(ui->wgtFocusChooser, SIGNAL(focusConfirmed(QList<int>)), this, SLOT(focusChosen(QList<int>)));

    // SKILL
    connect(m_conn, SIGNAL(chooseSkill(QList<AH::Common::ModifiedPropertyValueData>)), this, SLOT(chooseSkill(QList<AH::Common::ModifiedPropertyValueData>)));
    connect(ui->wgtOptionChooser, SIGNAL(skillChosen(AH::Common::PropertyValueData::Property)), this, SLOT(skillChoosen(AH::Common::PropertyValueData::Property)));

    // DIE ROLL
    connect(m_conn, SIGNAL(dieRollInfo(AH::Common::DieRollTestData)), this, SLOT(showDieRollInfo(AH::Common::DieRollTestData)));
    connect(ui->wgtDieRoll, SIGNAL(dieUpdateChosen(AH::Common::DieTestUpdateData)), this, SLOT(dieUpdateChosen(AH::Common::DieTestUpdateData)));

    // WEAPONS
    connect(m_conn, SIGNAL(chooseWeapons(QList<AH::Common::GameObjectData>,AH::Common::ModifiedPropertyValueData)), this, SLOT(chooseWeapons(QList<AH::Common::GameObjectData>,AH::Common::ModifiedPropertyValueData)));
    connect(ui->wgtWeaponChooser, SIGNAL(weaponsCanceled()), this, SLOT(weaponsCanceled()));
    connect(ui->wgtWeaponChooser, SIGNAL(weaponsSelected(QStringList)), this, SLOT(weaponsSelected(QStringList)));

    // ENCOUNTER
    connect(m_conn, SIGNAL(chooseEncounterOption(AH::Common::EncounterData)), this, SLOT(chooseEncounter(AH::Common::EncounterData)));
    connect(ui->wgtOptionChooser, SIGNAL(encounterChosen(QString)), this, SLOT(encounterSelected(QString)));

    // MYTHOS
    connect(m_conn, SIGNAL(displayMythos(AH::Common::MythosData)), this, SLOT(displayMythos(AH::Common::MythosData)));
    connect(m_conn, SIGNAL(finishMythos()), this, SLOT(finishMythos()));
    connect(ui->wgtMythos, SIGNAL(acknowledgeMythos()), this, SLOT(acknowledgeMythos()));

    // CHOICE
    connect(m_conn, SIGNAL(offerChoice(AH::Common::ChoiceData)), this, SLOT(offerChoice(AH::Common::ChoiceData)));
    connect(ui->wgtChoice, SIGNAL(choiceSelected(AH::Common::ChoiceResponseData)), this, SLOT(choiceSelected(AH::Common::ChoiceResponseData)));
    connect(ui->wgtChoice, SIGNAL(choiceCanceled()), this, SLOT(choiceCanceled()));

    connect(m_conn, SIGNAL(won(QString)), this, SLOT(won(QString)));
    connect(m_conn, SIGNAL(lost(QString)), this, SLOT(lost(QString)));
}

void AhMainGui::setThisPlayerId(QString id)
{
    m_registry->setThisPlayerId(id);
}

void AhMainGui::setThisCharacterId(QString id)
{
    m_registry->setThisCharacterId(id);
}

void AhMainGui::start()
{
    show();
    m_registry->getObject(m_registry->thisCharacterId(), AH::Common::RequestObjectsData::Character);
}

void AhMainGui::refitGui()
{
    //ui->tabIntInfInv->resize(ui->tabIntInfInv->minimumSizeHint());
}

void AhMainGui::characterInstantiated(QString playerId, QString characterId)
{
    if (playerId == m_registry->thisPlayerId()) {
        setThisCharacterId(characterId);
    }
}

void AhMainGui::displayItemInfo(const QString &id)
{
    //ui->wgtInfo->displayItemInfo(id);
    //ui->tabIntInfInv->setCurrentWidget(ui->tabInfo);
}

void AhMainGui::displayInventoryData(QListWidgetItem *itm)
{
    if (itm) {
        QString id = itm->data(ObjectIdRole).toString();
        displayItemInfo(id);
    }
}

void AhMainGui::gameSettingUpdate(GameSettingData data)
{
    // TODO: Display Terror Level
    ui->wgtAncientOne->displayAncientOne(data.ancientOneId());
    ui->wgtRumor->displayRumor(data.rumorId());
    ui->wgtEnvironment->displayEnvironment(data.environmentId());
}

/*
void AhMainGui::displayMonsterDetails(const MonsterData *m)
{
    ui->wgtInfoMonster->displayMonster(m);
    ui->stkInfo->setCurrentWidget(ui->pageInfoMonster);
    ui->tabIntInfInv->setCurrentWidget(ui->tabInfo);
}

void AhMainGui::displayGateDetails(const GateData *g)
{
    ui->wgtInfoGate->displayGate(g);
    ui->stkInfo->setCurrentWidget(ui->pageInfoGate);
    ui->tabIntInfInv->setCurrentWidget(ui->tabInfo);
}

void AhMainGui::displayObjectDetails(const GameObjectData *o)
{
    ui->wgtInfoObject->displayGameObject(o);
    ui->stkInfo->setCurrentWidget(ui->pageInfoObject);
    ui->tabIntInfInv->setCurrentWidget(ui->tabInfo);
}
*/

void AhMainGui::chooseOption(QList<GameOptionData> opts)
{
    ui->wgtOptionChooser->setOptions(opts);
    ui->stkInteraction->setCurrentWidget(ui->pageOptionChooser);
    ui->tabInteract->setCurrentWidget(ui->tabInteraction);
}

void AhMainGui::optionChosen(QString id)
{
    m_conn->selectOption(id);
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
    refitGui();
}

void AhMainGui::chooseMove(AH::Common::FieldData::FieldID startId, int movementPoints)
{
    ui->wgtMovmentChooser->chooseMovement(startId, movementPoints);
    ui->stkInteraction->setCurrentWidget(ui->pageMovementChooser);
    ui->tabInteract->setCurrentWidget(ui->tabInteraction);
    refitGui();
}

void AhMainGui::movementChosen(QList<FieldData::FieldID> path)
{
    m_conn->selectMovementPath(path);
    //ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
    refitGui();
}

void AhMainGui::movementCanceled()
{
    m_conn->selectMovementPath(QList<FieldData::FieldID>());
    //ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
    refitGui();
}

void AhMainGui::chooseFocus(QList<AttributeSliderData> sliders, int focusAmount)
{
    ui->wgtFocusChooser->chooseFocus(sliders, focusAmount);
    ui->stkInteraction->setCurrentWidget(ui->pageFocusChooser);
    ui->tabInteract->setCurrentWidget(ui->tabInteraction);
}

void AhMainGui::focusChosen(QList<int> diffs)
{
    m_conn->selectFocus(diffs);
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
    refitGui();
}

void AhMainGui::chooseSkill(QList<ModifiedPropertyValueData> options)
{
    ui->wgtOptionChooser->setSkills(options);
    ui->stkInteraction->setCurrentWidget(ui->pageOptionChooser);
    ui->tabInteract->setCurrentWidget(ui->tabInteraction);
}

void AhMainGui::skillChoosen(PropertyValueData::Property skill)
{
    m_conn->selectSkill(skill);
    //ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
    refitGui();
}

void AhMainGui::showDieRollInfo(DieRollTestData data)
{
    ui->wgtDieRoll->displayDieRoll(data);
    ui->stkInteraction->setCurrentWidget(ui->pageDieRoll);
    ui->tabInteract->setCurrentWidget(ui->tabInteraction);
}

void AhMainGui::dieUpdateChosen(DieTestUpdateData upd)
{
    m_conn->chooseDieRollUpdate(upd);
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
    refitGui();
}

//void AhMainGui::updateObject(DescribeObjectsData::ObjectDescription desc)
//{
//    Q_UNUSED(desc)
//    /*
//    if (desc.first == RequestObjectsData::Character) {
//        CharacterData c;
//        desc.second >> c;
//        updateCharacter(c);
//    }
//    */
//    /*
//    if (!m_pendingDisplayId.isEmpty()) {
//        // TODO: Check if this is good this way...
//        displayItemInfo(m_pendingDisplayId);
//    }
//    */
//}

void AhMainGui::updateCharacter(CharacterData c)
{
    if (c.id() == m_registry->thisCharacterId()) {
        ui->wgtCharacter->updateCharacterData(c);

        //m_thisCharacter = c;
        m_registry->getObjectsOfType(c.inventoryIds(), RequestObjectsData::Object);

        ui->lstInventory->clear();
        foreach (QString id, c.inventoryIds()) {
            QListWidgetItem *itm = new InventoryListItem(id);
            ui->lstInventory->addItem(itm);
        }
    }
}

void AhMainGui::chooseWeapons(QList<GameObjectData> weapons, ModifiedPropertyValueData hands)
{
    ui->wgtWeaponChooser->chooseWeapons(weapons, hands);
    ui->stkInteraction->setCurrentWidget(ui->pageWeaponChooser);
    ui->tabInteract->setCurrentWidget(ui->tabInteraction);
}

void AhMainGui::weaponsCanceled()
{
    m_conn->cancelWeapons();
    //ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
    refitGui();
}

void AhMainGui::weaponsSelected(QStringList weaponIds)
{
    m_conn->selectWeapons(weaponIds);
    //ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
    refitGui();
}

void AhMainGui::chooseEncounter(EncounterData encounter)
{
    ui->wgtOptionChooser->setEncounter(encounter);
    ui->stkInteraction->setCurrentWidget(ui->pageOptionChooser);
    ui->tabInteract->setCurrentWidget(ui->tabInteraction);
}

void AhMainGui::encounterSelected(QString id)
{
    m_conn->selectEncounterOption(id);
    //ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
    refitGui();
}

void AhMainGui::displayMythos(MythosData mythos)
{
    ui->wgtMythos->displayMythos(&mythos);
    ui->stkInteraction->setCurrentWidget(ui->pageMythos);
    ui->tabInteract->setCurrentWidget(ui->tabInteraction);
}

void AhMainGui::acknowledgeMythos()
{
    m_conn->acknowledge();
    finishMythos();
}

void AhMainGui::finishMythos()
{
    ui->wgtMythos->displayMythos(NULL);
    if (ui->stkInteraction->currentWidget() == ui->pageMythos) {
        ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
    }
}

void AhMainGui::offerChoice(ChoiceData choice)
{
    ui->wgtChoice->offerChoice(choice);
    ui->stkInteraction->setCurrentWidget(ui->pageChoice);
    ui->tabInteract->setCurrentWidget(ui->tabInteraction);
}

void AhMainGui::choiceSelected(ChoiceResponseData resp)
{
    m_conn->choiceSelected(resp);
    //ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
    refitGui();
}

void AhMainGui::choiceCanceled()
{
     m_conn->choiceCanceled();
     //ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
     refitGui();
}

void AhMainGui::won(QString msg)
{
    QMessageBox::information(this, "WON!", msg);
}

void AhMainGui::lost(QString msg)
{
    QMessageBox::critical(this, "LOST!", msg);
}

//////////////////////////////

InventoryListItem::InventoryListItem(QString objectId)
{
    setData(AhMainGui::ObjectIdRole, objectId);
    ObjectRegistry::instance()->asyncSubscribeObject(this, objectId, AH::Common::RequestObjectsData::Object);
}

void InventoryListItem::objectDescribed(const DescribeObjectsData::ObjectDescription &desc)
{
    AH::Common::GameObjectData o;
    desc.data >> o;
    setText(o.name());
    setIcon(ResourcePool::instance()->loadObjectImage(o.id(), o.type()));
}
