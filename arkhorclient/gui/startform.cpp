#include "startform.h"
#include "ui_startform.h"
#include <utils/cleanupthread.h>
#include <QMessageBox>
#include "ahmaingui.h"

Q_DECLARE_METATYPE(QList<AH::Common::InvestigatorData>)

StartForm::StartForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartForm),
    m_conn(NULL)
{
    ui->setupUi(this);

    AH::registerCommonMetaTypes();

    m_mainGui = new AhMainGui;
}

StartForm::~StartForm()
{
    delete ui;
    delete m_mainGui;
}

void StartForm::on_btnConnect_clicked()
{
    QString h = ui->txtHost->text();
    int p = ui->spnPort->value();

    m_conn = new ConnectionHandler(h, p);
    AH::Common::CleanupThread *t = new AH::Common::CleanupThread;
    m_conn->moveToThread(t);
    connect(t, SIGNAL(started()), m_conn, SLOT(startup()));
    connect(t, SIGNAL(finished()), m_conn, SLOT(deleteLater()));
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    connect(m_conn, SIGNAL(disconnected()), t, SLOT(quit()));
    t->registerCleanupMethod(m_conn, "cleanup");
    connect(m_conn, SIGNAL(connected()), this, SLOT(connectionEstablished()));

    connect(m_conn, SIGNAL(versionMismatch(quint32,quint32)), this, SLOT(versionMismatch(quint32,quint32)));
    connect(m_conn, SIGNAL(promptActive()), this, SLOT(promptActive()));
    connect(m_conn, SIGNAL(setPlayerData(AH::Common::PlayerData)), this, SLOT(setPlayerData(AH::Common::PlayerData)));
    connect(m_conn, SIGNAL(setInvestigatorList(QList<AH::Common::InvestigatorData>)), this, SLOT(setInvestigatorList(QList<AH::Common::InvestigatorData>)));
    connect(m_conn, SIGNAL(gameStarted()), this, SLOT(gameStarted()));

    connect(m_conn, SIGNAL(playerCharacterInstantiated(QString,QString)), this, SLOT(characterInstantiated(QString,QString)));
    connect(m_conn, SIGNAL(gameStart()), this, SLOT(startGame()));
    t->start();

}

void StartForm::on_btnStart_clicked()
{
    m_conn->startGame();
}

void StartForm::on_cmbInvestigators_currentIndexChanged(int)
{
    QVariant v = ui->cmbInvestigators->itemData(ui->cmbInvestigators->currentIndex());
    AH::Common::InvestigatorData d;
    v >> d;
    //ui->txtInvDetail->setText(v.toString());
    ui->wgtInvestigator->displayInvestigator(d);
}

void StartForm::on_btnSelectInv_clicked()
{
    QVariant v = ui->cmbInvestigators->itemData(ui->cmbInvestigators->currentIndex());
    AH::Common::InvestigatorData d;
    v >> d;
    m_conn->chooseInvestigator(d);
}

void StartForm::connectionEstablished()
{
    ui->grpConnection->setEnabled(false);
    m_conn->registerPlayer();
}

void StartForm::versionMismatch(quint32 c, quint32 s)
{
    QString cstr = QString("%1.%2.%3.%4").arg((c>>24)&0xFF).arg((c>>16)&0xFF).arg((c>>8)&0xFF).arg((c>>0)&0xFF);
    QString sstr = QString("%1.%2.%3.%4").arg((s>>24)&0xFF).arg((s>>16)&0xFF).arg((s>>8)&0xFF).arg((s>>0)&0xFF);

    QMessageBox::critical(this, "Version Mismatch",
        QString("This client cannot communicate with the server.\nClient version: %1\nServer version: %2\nUse the appropriate client!")
                          .arg(cstr).arg(sstr)
    );
}

void StartForm::promptActive()
{
    QMessageBox::information(this, "Active?", "It's your turn. You have 15 seconds to verify that you are still here!\nClick on OK!");
    m_conn->confirmActive();
}

void StartForm::gameStarted()
{
    ui->btnStart->setEnabled(false);
}

void StartForm::setPlayerData(AH::Common::PlayerData d)
{
    m_thisPlayerId = d.id();
    ui->lblId->setText(d.id());
    ui->btnStart->setEnabled(true);

    m_mainGui->initConnection(m_conn);
}

void StartForm::setInvestigatorList(QList<AH::Common::InvestigatorData> l)
{
    ui->grpInvestigator->setEnabled(true);
    ui->cmbInvestigators->clear();
    ui->cmbInvestigators->addItem(QString::null, QVariant());
    foreach (AH::Common::InvestigatorData i, l) {
        QVariant v;
        v << i;
        ui->cmbInvestigators->addItem(i.name(), v);
    }
}

void StartForm::characterInstantiated(QString playerId, QString charaterId)
{
    if (playerId == this->m_thisPlayerId) {
        int idx = ui->cmbInvestigators->findData(charaterId);
        if (idx >= 0) ui->cmbInvestigators->setCurrentIndex(idx);
        ui->cmbInvestigators->setEnabled(false);
        m_mainGui->setThisCharacterId(charaterId);
        m_mainGui->setThisPlayerId(playerId);
    }
    ui->btnSelectInv->setEnabled(false);
}

void StartForm::startGame()
{
    m_mainGui->start();
    this->close();
    //this->deleteLater();
}

