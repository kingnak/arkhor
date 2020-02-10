#include "ahmaingui.h"
#include "ui_ahmaingui.h"
#include "ahboardscene.h"
#include "connectionhandler.h"
#include "objectregistry.h"
#include "monsterdata.h"
#include "gatedata.h"
#include "resourcepool.h"
#include <QtWidgets>
#include "utils.h"
#include "objectlistitem.h"
#include "detailcardwidget.h"

#define MAXIMUM_WIDGET_WIDTH 16777215

Q_DECLARE_METATYPE(AH::Common::MonsterData);
Q_DECLARE_METATYPE(QList<AH::Common::MonsterData>)

using namespace AH::Common;

AhMainGui::AhMainGui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AhMainGui),
    m_conn(NULL),
    m_registry(NULL),
    m_skipOption(PlayerData::NoAutoChoose)
{
    qRegisterMetaType<AH::Common::MonsterData>();
    qRegisterMetaType<QList<AH::Common::MonsterData>>();

    ui->setupUi(this);
    ui->tabInteract->setOrientation(Qt::Horizontal);
    ui->tabInteract->buildFromTab(ui->tabInteract_base);
    addAction(ui->actionDismissInfo);

    m_dismissTimer = new QTimer(this);
    m_dismissTimer->setInterval(150);
    m_dismissTimer->setSingleShot(true);

    ui->wgtObjectInfo->setVisible(false);

    m_scene = new AhBoardScene(this);
    ui->grvBoard->setScene(m_scene);
    m_scene->initBoard();

    connect(ui->grvBoard, SIGNAL(receivedFocus()), this, SLOT(dismissInfoPane()));
    connect(m_dismissTimer, SIGNAL(timeout()), this, SLOT(doDismissInfoPane()));

    ui->wgtMovmentChooser->setBoard(m_scene, ui->grvBoard);

    ui->lblPhase->setText("Setup");

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
    connect(ui->wgtAncientOne, SIGNAL(ancientOneInfoRequested(QString)), this, SLOT(displayAncientOne(QString)));
    connect(ui->wgtRumor, SIGNAL(rumorInfoRequested(QString)), this, SLOT(displayItemInfo(QString)));
    connect(ui->wgtEnvironment, SIGNAL(environmentInfoRequested(QString)), this, SLOT(displayItemInfo(QString)));
    connect(ui->wgtChoice, SIGNAL(objectInfoRequested(QString)), this, SLOT(displayItemInfo(QString)));
    connect(ui->wgtWeaponChooser, SIGNAL(itemInfoRequested(QString)), this, SLOT(displayItemInfo(QString)));
    connect(ui->wgtOptionChooser, SIGNAL(objectDescriptionRequested(QString)), this, SLOT(displayItemInfo(QString)));
    connect(ui->wgtTrade, SIGNAL(itemInfoRequested(QString)), this, SLOT(displayItemInfo(QString)));
    connect(ui->wgtObjectInfo, &ObjectInfoWidget::objectInfoRequested, this, &AhMainGui::displayItemInfo);
    connect(ui->wgtCharShortInfo, &CharShortInfoWidget::characterDetailRequested, this, &AhMainGui::displayCharacterDetail);
    connect(ui->wgtCharacter, &CharacterWidget::characterDetailRequested, this, &AhMainGui::displayCharacterDetail);
    connect(ui->wgtObjectInfo, &ObjectInfoWidget::characterDetailRequested, this, &AhMainGui::displayCharacterDetail);
    connect(ui->wgtCharacter, &CharacterWidget::requestCenterOnField, ui->grvBoard, &AhGraphicsView::centerOnField);
    connect(ui->wgtObjectInfo, &ObjectInfoWidget::requestCenterOnField, ui->grvBoard, &AhGraphicsView::centerOnField);

    m_cardWidget = new DetailCardWidget(this);
    m_cardWidget->setVisible(false);
    m_cardWidget->clear();
}

AhMainGui::~AhMainGui()
{
    delete ui;
}

void AhMainGui::initConnection(ConnectionHandler *conn)
{
    if (m_conn) return;
    m_conn = conn;
    m_registry = ObjectRegistry::instance();
    m_registry->init(conn);

    connect(m_conn, &ConnectionHandler::overridePlayerId, this, &AhMainGui::setThisPlayerId);
    connect(m_conn, &ConnectionHandler::overrideCharacterId, this, &AhMainGui::setThisCharacterId);

    //connect(m_registry, SIGNAL(objectDescribed(AH::Common::DescribeObjectsData::ObjectDescription)), this, SLOT(updateObject(AH::Common::DescribeObjectsData::ObjectDescription)));
    connect(m_registry, SIGNAL(thisCharacterUpdated(AH::Common::CharacterData)), this, SLOT(updateCharacter(AH::Common::CharacterData)));

    connect(m_conn, SIGNAL(playerCharacterInstantiated(QString,QString)), this, SLOT(characterInstantiated(QString,QString)));

    connect(m_conn, SIGNAL(textMessage(QString)), this, SLOT(textMessage(QString)));

    // BOARD
    connect(m_conn, SIGNAL(boardContent(QVariantMap)), m_scene, SLOT(updateBoardFromData(QVariantMap)));
    connect(m_registry, &ObjectRegistry::boardDescriptionUpdated, this, &AhMainGui::updateSceneNeighbours);
    connect(m_scene, &AhBoardScene::fieldInfoRequested, this, &AhMainGui::fieldInfoRequested);

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

    // MONSTER
    connect(m_conn, SIGNAL(chooseMonster(QString, QList<AH::Common::MonsterData>)), this, SLOT(chooseMonster(QString, QList<AH::Common::MonsterData>)));
    connect(ui->wgtOptionChooser, SIGNAL(monsterChosen(QString)), this, SLOT(monsterSelected(QString)));

    // MYTHOS
    connect(m_conn, SIGNAL(acknowledgeMythos(AH::Common::MythosData)), this, SLOT(displayMythos(AH::Common::MythosData)));
    connect(m_conn, SIGNAL(finishAcknowledge()), this, SLOT(finishMythos()));
    connect(ui->wgtMythos, SIGNAL(acknowledgeMythos()), this, SLOT(acknowledgeMythos()));

    // MOVEMENT
    connect(m_conn, SIGNAL(acknowledgeMonsterMovement(AH::Common::MonsterData)), this, SLOT(displayMonsterMovement(AH::Common::MonsterData)));
    connect(m_conn, SIGNAL(finishAcknowledge()), this, SLOT(finishMonsterMovement()));
    connect(ui->wgtMonsterMovement, SIGNAL(acknowledgeMovement()), this, SLOT(acknowledgeMonsterMovement()));

    // CHOICE
    connect(m_conn, SIGNAL(offerChoice(AH::Common::ChoiceData)), this, SLOT(offerChoice(AH::Common::ChoiceData)));
    connect(ui->wgtChoice, SIGNAL(choiceSelected(AH::Common::ChoiceResponseData)), this, SLOT(choiceSelected(AH::Common::ChoiceResponseData)));
    connect(ui->wgtChoice, SIGNAL(choiceCanceled()), this, SLOT(choiceCanceled()));

    // TRADE
    connect(m_conn, SIGNAL(offerTrade(AH::Common::TradeData)), this, SLOT(offerTrade(AH::Common::TradeData)));
    connect(m_conn, SIGNAL(canceledTrade(QString)), this, SLOT(tradeCanceled(QString)));
    connect(ui->wgtTrade, SIGNAL(cancelTrade()), this, SLOT(cancelTrade()));
    connect(ui->wgtTrade, SIGNAL(selectedTrade(AH::Common::TradeData)), this, SLOT(tradeSelected(AH::Common::TradeData)));


    connect(m_conn, SIGNAL(died(QString)), this, SLOT(showAlert(QString)));
    connect(m_conn, SIGNAL(won(QString)), this, SLOT(won(QString)));
    connect(m_conn, SIGNAL(lost(QString)), this, SLOT(lost(QString)));
    connect(m_conn, SIGNAL(gameAlert(QString)), this, SLOT(showAlert(QString)));
    connect(m_conn, SIGNAL(phaseChange(AH::GamePhase)), this, SLOT(phaseChange(AH::GamePhase)));
    connect(m_conn, SIGNAL(playerChange(QString)), this, SLOT(playerChange(QString)));

    connect(m_conn, SIGNAL(clearTempData()), this, SLOT(clearTempObject()));

    m_skipOption = PlayerData::AutoSkipData::AutoChooseAlways;
    m_conn->setSkipOption(m_skipOption);
}

void AhMainGui::setThisPlayerId(QString id)
{
    m_registry->setThisPlayerId(id);
}

void AhMainGui::setThisCharacterId(QString id)
{
    m_registry->setThisCharacterId(id);
    updateCharacter(m_registry->thisCharacter());
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

void AhMainGui::dismissInfoPane()
{
    m_dismissTimer->start();
    m_cardWidget->setVisible(false);
}

void AhMainGui::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    if (m_cardWidget->isVisible())
        readjustDetailCard();
}

void AhMainGui::expandInfoPane()
{
    m_dismissTimer->stop();
    if (!ui->wgtObjectInfo->isVisible()) {
        ui->wgtObjectInfo->setMaximumWidth(MAXIMUM_WIDGET_WIDTH);
        ui->wgtObjectInfo->setAttribute(Qt::WA_DontShowOnScreen);
        ui->wgtObjectInfo->setVisible(true);
        QSize s = ui->wgtObjectInfo->size();
        ui->wgtObjectInfo->setVisible(false);
        ui->wgtObjectInfo->setAttribute(Qt::WA_DontShowOnScreen, false);

        QSequentialAnimationGroup *grp = new QSequentialAnimationGroup;
        QPropertyAnimation *anim = new QPropertyAnimation(ui->wgtObjectInfo, "maximumWidth");
        anim->setStartValue(0);
        anim->setEndValue(s.width());
        anim->setDuration(150);
        grp->addAnimation(anim);
        anim = new QPropertyAnimation(ui->wgtObjectInfo, "maximumWidth");
        anim->setStartValue(s.width());
        anim->setEndValue(MAXIMUM_WIDGET_WIDTH);
        anim->setDuration(0);
        grp->addAnimation(anim);

        ui->wgtObjectInfo->setMaximumWidth(0);
        ui->wgtObjectInfo->setVisible(true);
        grp->start(QAbstractAnimation::DeleteWhenStopped);
    }
    //ui->wgtInfo->displayItemInfo(id);
    //ui->tabIntInfInv->setCurrentWidget(ui->tabInfo);
}

void AhMainGui::doDismissInfoPane()
{
    if (ui->wgtObjectInfo->isVisible()) {
        QSize s = ui->wgtObjectInfo->size();
        QSequentialAnimationGroup *grp = new QSequentialAnimationGroup;
        QPropertyAnimation *anim = new QPropertyAnimation(ui->wgtObjectInfo, "maximumWidth");
        anim->setStartValue(s.width());
        anim->setEndValue(0);
        anim->setDuration(150);
        grp->addAnimation(anim);

        VisibilityAnimation *v = new VisibilityAnimation(ui->wgtObjectInfo, false);
        grp->addAnimation(v);
        grp->start(QAbstractAnimation::DeleteWhenStopped);
    }
    //ui->wgtObjectInfo->setVisible(false);
}

void AhMainGui::updateSceneNeighbours(QList<ObjectRegistry::FieldDescription> descs)
{
    QList<AH::Common::GameFieldData> fd;
    for (auto f : descs) {
        fd << f.fieldData;
    }
    m_scene->initNeighbourHoodFromBoardData(fd);
}

void AhMainGui::characterInstantiated(QString playerId, QString characterId)
{
    if (playerId == m_registry->thisPlayerId()) {
        setThisCharacterId(characterId);
    }
}

void AhMainGui::displayItemInfo(const QString &id)
{
    ui->wgtObjectInfo->displayItemInfo(id);
    expandInfoPane();
}

void AhMainGui::fieldInfoRequested(FieldData::FieldID id)
{
    m_dismissTimer->stop();
    auto fd = m_registry->getFieldDescription(id);
    ui->wgtObjectInfo->displayFieldInfo(fd.fieldData);
    expandInfoPane();
}

void AhMainGui::displayAncientOne(const QString &id)
{
    auto ao = m_registry->getObject<AH::Common::AncientOneData>(id);
    if (!ao.id().isEmpty()) {
        m_cardWidget->displayAncientOne(&ao);
        readjustDetailCard();
        m_cardWidget->setVisible(true);
    }
}

void AhMainGui::displayCharacterDetail(const QString &id)
{
    auto chr = m_registry->getObject<AH::Common::CharacterData>(id);
    if (!chr.id().isEmpty()) {
        m_cardWidget->displayCharacter(&chr);
        readjustDetailCard();
        m_cardWidget->setVisible(true);
    }
}

void AhMainGui::displayInventoryData(QListWidgetItem *itm)
{
    if (itm) {
        QString id = itm->data(ObjectListItem::ObjectIdRole).toString();
        displayItemInfo(id);
    }
}

void AhMainGui::gameSettingUpdate(GameSettingData data)
{
    ui->wgtAncientOne->displayAncientOne(data.ancientOneId());
    ui->wgtRumor->displayRumor(data.rumorId());
    ui->wgtEnvironment->displayEnvironment(data.environmentId());
    ui->lblTerrorLevel->setText(QString::number(data.terrorLevel()));

    m_scene->setTerrorLevel(data.terrorLevel());
}

void AhMainGui::textMessage(const QString &msg)
{
    ui->txtLog->append(msg);
}

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
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
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
        ui->wgtCharacter->updateCharacterData(&c);
        ui->wgtCharShortInfo->displayCharacter(&c);

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
    ui->wgtOptionChooser->setEncounter(encounter);
    ui->stkInteraction->setCurrentWidget(ui->pageOptionChooser);
    ui->tabInteract->setCurrentWidget(ui->tabInteraction);
}

void AhMainGui::encounterSelected(QString id)
{
    m_conn->selectEncounterOption(id);
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
    refitGui();
}

void AhMainGui::chooseMonster(QString desc, QList<AH::Common::MonsterData> monsters)
{
    ui->wgtOptionChooser->setMonsters(desc, monsters);
    ui->stkInteraction->setCurrentWidget(ui->pageOptionChooser);
    ui->tabInteract->setCurrentWidget(ui->tabInteraction);
}

void AhMainGui::monsterSelected(QString id)
{
    m_conn->selectMonster(id);
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
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

void AhMainGui::displayMonsterMovement(MonsterData monster)
{
    ui->wgtMonsterMovement->displayMonsterMovement(&monster);
    ui->stkInteraction->setCurrentWidget(ui->pageMonsterMovement);
    ui->tabInteract->setCurrentWidget(ui->tabInteraction);
}

void AhMainGui::acknowledgeMonsterMovement()
{
    m_conn->acknowledge();
    finishMonsterMovement();
}

void AhMainGui::finishMonsterMovement()
{
    ui->wgtMonsterMovement->displayMonsterMovement(NULL);
    if (ui->stkInteraction->currentWidget() == ui->pageMonsterMovement) {
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
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
    refitGui();
}

void AhMainGui::choiceCanceled()
{
     m_conn->choiceCanceled();
     ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
     refitGui();
}

void AhMainGui::offerTrade(TradeData trade)
{
    ui->wgtTrade->showTrade(trade);
    ui->stkInteraction->setCurrentWidget(ui->pageTrade);
    ui->tabInteract->setCurrentWidget(ui->tabInteraction);
}

void AhMainGui::tradeSelected(TradeData trade)
{
    m_conn->tradeSelected(trade);
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
    refitGui();
}

void AhMainGui::tradeCanceled(QString name)
{
    QMessageBox::information(this, "Trade", name + " canceled the trade");
    m_conn->acknowledge();
    /*
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
    refitGui();
    */
}

void AhMainGui::cancelTrade()
{
    m_conn->tradeCanceled();
    ui->stkInteraction->setCurrentWidget(ui->pageEmptyInteraction);
    refitGui();
}

void AhMainGui::clearTempObject()
{
    if (ui->wgtObjectInfo->isDisplayingTempData()) {
        doDismissInfoPane();
        ui->wgtObjectInfo->displayNothing();
    }
}

void AhMainGui::won(QString msg)
{
    QMessageBox::information(this, "WON!", msg);
}

void AhMainGui::lost(QString msg)
{
    QMessageBox::critical(this, "LOST!", msg);
}

void AhMainGui::showAlert(QString msg)
{
    QMessageBox::information(this, "Arkham Horror", msg);
}

void AhMainGui::phaseChange(AH::GamePhase ph)
{
    textMessage(QString("New phase: %1").arg(Utils::stringForPhase(ph)));
    ui->lblPhase->setText(Utils::stringForPhase(ph));
}

void AhMainGui::playerChange(QString id)
{
    if (id == m_registry->thisPlayerId()) {
        this->updateCharacter(m_registry->thisCharacter());
        ui->stkWhichPlayer->setCurrentWidget(ui->pageThisPlayer);
        //textMessage("You are the current player");
        textMessage(QString("The current player is %1").arg(id));
    } else {
        ui->stkWhichPlayer->setCurrentWidget(ui->pageOtherPlayer);
        textMessage(QString("The current player is %1").arg(id));
    }
}

void AhMainGui::on_btnSkipOptions_clicked()
{
    bool ok = true;
    QStringList itms = QStringList()
            << "Show all options"
            << "Automatically choose some options"
            << "Automatically choose all basic options"
            << "Automatically choose option if there  is only 1";
    QString sel = QInputDialog::getItem(this, "Skip Option", "Choose skip options", itms, m_skipOption, false, &ok);

    if (!ok) return;
    int idx = itms.indexOf(sel);
    if (idx < 0) return;

    if (idx != m_skipOption) {
        m_skipOption = static_cast<PlayerData::AutoSkipData> (idx);
        m_conn->setSkipOption(m_skipOption);
    }
}

void AhMainGui::readjustDetailCard()
{
    m_cardWidget->adjustSize();
    auto ss = m_cardWidget->size();

    auto ms = m_cardWidget->minimumSizeHint();
    auto as = this->size();

    auto dx = double(as.width())/ss.width();
    auto dy = double(as.height())/ss.height();
    if (dx < 1 || dy < 1) {
        auto dd = qMin(dx, dy);
        as = ss*dd;
        if (as.width()<ms.width() || as.height()<ms.height()) as = ms;
    } else {
        as = ss;
    }
    m_cardWidget->setGeometry((ui->grvBoard->width()-as.width())/2, (this->height()-as.height())/2, as.width(), as.height());
}
