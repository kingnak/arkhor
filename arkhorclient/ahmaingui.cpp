#include "ahmaingui.h"
#include "ui_ahmaingui.h"
#include "ahboardscene.h"
#include "connectionhandler.h"
#include "flowlayout.h"
#include "objectregistry.h"
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

}

void AhMainGui::setThisPlayerId(QString id)
{
    m_registry->setThisPlayerId(id);
}

void AhMainGui::setThisCharacterId(QString id)
{
    m_registry->setThisCharacterId(id);
}

QString AhMainGui::stringForProperty(PropertyValueData::Property p)
{
    switch (p) {
    case PropertyValueData::NoProperty: return "";

    case PropertyValueData::Attr_Speed: return "Speed";
    case PropertyValueData::Attr_Sneak: return "Sneak";
    case PropertyValueData::Attr_Fight: return "Fight";
    case PropertyValueData::Attr_Will: return "Will";
    case PropertyValueData::Attr_Lore: return "Lore";
    case PropertyValueData::Attr_Luck: return "Luck";

    case PropertyValueData::Skill_Speed: return "Speed";
    case PropertyValueData::Skill_Sneak: return "Sneak";
    case PropertyValueData::Skill_Fight: return "Fight";
    case PropertyValueData::Skill_Will: return "Will";
    case PropertyValueData::Skill_Lore: return "Lore";
    case PropertyValueData::Skill_Luck: return "Luck";
    case PropertyValueData::Skill_Evade: return "Evade";
    case PropertyValueData::Skill_Combat: return "Combat";
    case PropertyValueData::Skill_Horror: return "Horror";
    case PropertyValueData::Skill_Spell: return "Spell";

    case PropertyValueData::Prop_MaxStamina: return "Maximum Stamina";
    case PropertyValueData::Prop_MaxSanity: return "Maximum Sanity";
    case PropertyValueData::Prop_Focus: return "Focus";
    case PropertyValueData::Prop_Movement: return "Movement";
    case PropertyValueData::Prop_MinSuccessDieRoll: return "Minimal Success Die Roll";
    case PropertyValueData::Prop_HandCount: return "HandCount";

    case PropertyValueData::DieRoll_All: return "All";
    case PropertyValueData::DieRoll_Speed: return "Speed";
    case PropertyValueData::DieRoll_Sneak: return "Sneak";
    case PropertyValueData::DieRoll_Fight: return "Fight";
    case PropertyValueData::DieRoll_Will: return "Will";
    case PropertyValueData::DieRoll_Lore: return "Lore";
    case PropertyValueData::DieRoll_Luck: return "Luck";
    case PropertyValueData::DieRoll_Evade: return "Evade";
    case PropertyValueData::DieRoll_Combat: return "Combat";
    case PropertyValueData::DieRoll_Horror: return "Horror";
    case PropertyValueData::DieRoll_Spell: return "Spell";

    case PropertyValueData::Game_SealClueCost: return "Seal Clue Cost";

    case PropertyValueData::Damage_General: return "General Damage";
    case PropertyValueData::Damage_Magical: return "Magical Damage";
    case PropertyValueData::Damage_Physical: return "Physical Damage";
    }
    return "";
}

void AhMainGui::start()
{
    show();
    m_registry->getObject(m_registry->thisCharacterId(), AH::Common::RequestObjectsData::Character);
}

void AhMainGui::displayItemInfo(const QString &id)
{
    QMessageBox::information(this, "Info", id);
    DescribeObjectsData::ObjectDescription d = m_registry->getObject(id);
    if (d.first == RequestObjectsData::Unknown) {
        m_pendingDisplayId = id;
    } else {
        m_pendingDisplayId = QString::null;
    }
}

void AhMainGui::displayInventoryData(QListWidgetItem *itm)
{
    if (itm) {
        QString id = itm->data(ObjectIdRole).toString();
        displayItemInfo(id);
    }
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
}

void AhMainGui::chooseMove(AH::Common::FieldData::FieldID startId, int movementPoints)
{
    ui->stkInteraction->setCurrentWidget(ui->pageMovementChooser);
    ui->tabIntInfInv->setCurrentWidget(ui->tabInteraction);
    ui->wgtMovmentChooser->chooseMovement(startId, movementPoints);
}

void AhMainGui::movementChosen(QList<FieldData::FieldID> path)
{
    m_conn->selectMovementPath(path);
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
}

void AhMainGui::movementCanceled()
{
    m_conn->selectMovementPath(QList<FieldData::FieldID>());
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
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
}

void AhMainGui::updateObject(DescribeObjectsData::ObjectDescription desc)
{
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
            QListWidgetItem *itm = new QListWidgetItem(id);
            itm->setData(ObjectIdRole, id);
            ui->lstInventory->addItem(itm);
        }
    }
}
