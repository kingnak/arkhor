#ifndef STARTFORM_H
#define STARTFORM_H

#include <QWidget>
#include "connectionhandler.h"
#include <playerdata.h>

class AhMainGui;

namespace Ui {
class StartForm;
}

class StartForm : public QWidget
{
    Q_OBJECT

public:
    explicit StartForm(QWidget *parent = 0);
    ~StartForm() override;

private slots:
    void on_btnConnect_clicked();
    void on_btnStart_clicked();
    void on_cmbInvestigators_currentIndexChanged(int);
    void on_btnSelectInv_clicked();

    void connectionEstablished();

    void versionMismatch(quint32 c, quint32 s);

    void promptActive();
    void gameStarted();
    void setPlayerData(const AH::Common::PlayerData &d);
    void setInvestigatorList(const QList<AH::Common::InvestigatorData> &l);
    void characterInstantiated(const QString &playerId, const QString &charaterId);
    void startGame();

private:
    Ui::StartForm *ui;
    ConnectionHandler *m_conn;

    QString m_thisPlayerId;
    AhMainGui *m_mainGui;
};

#endif // STARTFORM_H
