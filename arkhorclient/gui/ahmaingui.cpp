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

    m_scene = new AhBoardScene(this);
    ui->grvBoard->setScene(m_scene);
    m_scene->initBoard();

    ui->wgtMovmentChooser->setBoard(m_scene, ui->grvBoard);

    connect(m_scene, SIGNAL(itemInfoRequested(QString)), this, SLOT(displayItemInfo(QString)));
    connect(ui->wgtDieRoll, SIGNAL(itemInfoRequested(QString)), this, SLOT(displayItemInfo(QString)));
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

    connect(m_registry, SIGNAL(objectDescribed(AH::Common::DescribeObjectsData::ObjectDescription)), this, SLOT(updateObject(AH::Common::DescribeObjectsData::ObjectDescription)));
    connect(m_registry, SIGNAL(thisCharacterUpdated(AH::Common::CharacterData)), this, SLOT(updateCharacter(AH::Common::CharacterData)));

    // BOARD
    connect(m_conn, SIGNAL(boardContent(QVariantMap)), m_scene, SLOT(updateBoardFromData(QVariantMap)));

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
    ui->tabIntInfInv->resize(ui->tabIntInfInv->minimumSizeHint());
}

void AhMainGui::displayItemInfo(const QString &id)
{
    DescribeObjectsData::ObjectDescription d = m_registry->getObject(id);
    if (d.type == RequestObjectsData::Unknown) {
        m_pendingDisplayId = id;
    } else {
        m_pendingDisplayId = QString::null;
        switch (d.type) {
        case AH::Common::RequestObjectsData::Monster:
        {
            AH::Common::MonsterData m;
            d.data >> m;
            displayMonsterDetails(&m);
            break;
        }
        case AH::Common::RequestObjectsData::Gate:
        {
            AH::Common::GateData g;
            d.data >> g;
            displayGateDetails(&g);
            break;
        }
        case AH::Common::RequestObjectsData::Object:
        {
            AH::Common::GameObjectData o;
            d.data >> o;
            displayObjectDetails(&o);
            break;
        }
        default:
            QMessageBox::information(this, "Info", id);
        }
    }
}

void AhMainGui::displayInventoryData(QListWidgetItem *itm)
{
    if (itm) {
        QString id = itm->data(ObjectIdRole).toString();
        displayItemInfo(id);
    }
}

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

void AhMainGui::chooseOption(QList<GameOptionData> opts)
{
    ui->stkInteraction->setCurrentWidget(ui->pageOptionChooser);
    ui->tabIntInfInv->setCurrentWidget(ui->tabInteraction);
    ui->wgtOptionChooser->setOptions(opts);
}

void AhMainGui::optionChosen(QString id)
{
    m_conn->selectOption(id);
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
    refitGui();
}

void AhMainGui::chooseMove(AH::Common::FieldData::FieldID startId, int movementPoints)
{
    ui->stkInteraction->setCurrentWidget(ui->pageMovementChooser);
    ui->tabIntInfInv->setCurrentWidget(ui->tabInteraction);
    ui->wgtMovmentChooser->chooseMovement(startId, movementPoints);
    refitGui();
}

void AhMainGui::movementChosen(QList<FieldData::FieldID> path)
{
    m_conn->selectMovementPath(path);
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
    refitGui();
}

void AhMainGui::movementCanceled()
{
    m_conn->selectMovementPath(QList<FieldData::FieldID>());
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
    refitGui();
}

void AhMainGui::chooseFocus(QList<AttributeSliderData> sliders, int focusAmount)
{
    ui->stkInteraction->setCurrentWidget(ui->pageFocusChooser);
    ui->tabIntInfInv->setCurrentWidget(ui->tabInteraction);
    ui->wgtFocusChooser->chooseFocus(sliders, focusAmount);
}

void AhMainGui::focusChosen(QList<int> diffs)
{
    m_conn->selectFocus(diffs);
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
    refitGui();
}

void AhMainGui::chooseSkill(QList<ModifiedPropertyValueData> options)
{
    ui->stkInteraction->setCurrentWidget(ui->pageOptionChooser);
    ui->tabIntInfInv->setCurrentWidget(ui->tabInteraction);
    ui->wgtOptionChooser->setSkills(options);
}

void AhMainGui::skillChoosen(PropertyValueData::Property skill)
{
    m_conn->selectSkill(skill);
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
    refitGui();
}

void AhMainGui::showDieRollInfo(DieRollTestData data)
{
    ui->stkInteraction->setCurrentWidget(ui->pageDieRoll);
    ui->tabIntInfInv->setCurrentWidget(ui->tabInteraction);
    ui->wgtDieRoll->displayDieRoll(data);
}

void AhMainGui::dieUpdateChosen(DieTestUpdateData upd)
{
    m_conn->chooseDieRollUpdate(upd);
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
    refitGui();
}

void AhMainGui::updateObject(DescribeObjectsData::ObjectDescription desc)
{
    Q_UNUSED(desc)
    /*
    if (desc.first == RequestObjectsData::Character) {
        CharacterData c;
        desc.second >> c;
        updateCharacter(c);
    }
    */
    if (!m_pendingDisplayId.isEmpty()) {
        // TODO: Check if this is good this way...
        displayItemInfo(m_pendingDisplayId);
    }
}

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
    ui->stkInteraction->setCurrentWidget(ui->pageWeaponChooser);
    ui->tabIntInfInv->setCurrentWidget(ui->tabInteraction);
    ui->wgtWeaponChooser->chooseWeapons(weapons, hands);
}

void AhMainGui::weaponsCanceled()
{
    m_conn->cancelWeapons();
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
    refitGui();
}

void AhMainGui::weaponsSelected(QStringList weaponIds)
{
    m_conn->selectWeapons(weaponIds);
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
    refitGui();
}

void AhMainGui::chooseEncounter(EncounterData encounter)
{
    ui->stkInteraction->setCurrentWidget(ui->pageOptionChooser);
    ui->tabIntInfInv->setCurrentWidget(ui->tabInteraction);
    ui->wgtOptionChooser->setEncounter(encounter);
}

void AhMainGui::encounterSelected(QString id)
{
    m_conn->selectEncounterOption(id);
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
    refitGui();
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
