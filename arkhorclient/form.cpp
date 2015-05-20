#include "form.h"
#include "ui_form.h"
#include <utils/cleanupthread.h>
#include <QMessageBox>

Q_DECLARE_METATYPE(QList<AH::Common::InvestigatorData>)

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form),
    m_conn(NULL)
{
    ui->setupUi(this);

    AH::registerCommonMetaTypes();
}

Form::~Form()
{
    delete ui;
}

void Form::on_btnConnect_clicked()
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

    connect(m_conn, SIGNAL(promptActive()), this, SLOT(promptActive()));
    connect(m_conn, SIGNAL(setPlayerData(AH::Common::PlayerData)), this, SLOT(setPlayerData(AH::Common::PlayerData)));
    connect(m_conn, SIGNAL(setInvestigatorList(QList<AH::Common::InvestigatorData>)), this, SLOT(setInvestigatorList(QList<AH::Common::InvestigatorData>)));
    connect(m_conn, SIGNAL(gameStarted()), this, SLOT(gameStarted()));
    t->start();

}

void Form::on_btnStart_clicked()
{
    m_conn->startGame();
}

void Form::on_cmbInvestigators_currentIndexChanged(int)
{
    QVariant v = ui->cmbInvestigators->itemData(ui->cmbInvestigators->currentIndex());
    AH::Common::InvestigatorData d;
    v >> d;
    ui->txtInvDetail->setText(v.toString());
}

void Form::on_btnSelectInv_clicked()
{
    QVariant v = ui->cmbInvestigators->itemData(ui->cmbInvestigators->currentIndex());
    AH::Common::InvestigatorData d;
    v >> d;
    m_conn->chooseInvestigator(d);
}

void Form::connectionEstablished()
{
    ui->grpConnection->setEnabled(false);
    m_conn->registerPlayer();
}

void Form::promptActive()
{
    QMessageBox::information(this, "Active?", "It's your turn. You have 15 seconds to verify that you are still here!\nClick on OK!");
    m_conn->confirmActive();
}

void Form::gameStarted()
{
    ui->btnStart->setEnabled(false);
}

void Form::setPlayerData(AH::Common::PlayerData d)
{
    ui->lblId->setText(d.id());
    ui->btnStart->setEnabled(true);
}

void Form::setInvestigatorList(QList<AH::Common::InvestigatorData> l)
{
    ui->grpInvestigator->setEnabled(true);
    ui->cmbInvestigators->clear();
    foreach (AH::Common::InvestigatorData i, l) {
        QVariant v;
        v << i;
        ui->cmbInvestigators->addItem(i.id(), v);
    }
}
