#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "connectionhandler.h"
#include <playerdata.h>

class AhMainGui;

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

private slots:
    void on_btnConnect_clicked();
    void on_btnStart_clicked();
    void on_cmbInvestigators_currentIndexChanged(int);
    void on_btnSelectInv_clicked();

    void connectionEstablished();

    void versionMismatch(quint32 c, quint32 s);

    void promptActive();
    void gameStarted();
    void setPlayerData(AH::Common::PlayerData d);
    void setInvestigatorList(QList<AH::Common::InvestigatorData> l);
    void characterInstantiated(QString playerId, QString charaterId);
    void startGame();

private:
    Ui::Form *ui;
    ConnectionHandler *m_conn;

    QString m_thisPlayerId;
    AhMainGui *m_mainGui;
};

#endif // FORM_H
