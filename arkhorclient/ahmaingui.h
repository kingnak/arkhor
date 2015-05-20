#ifndef AHMAINGUI_H
#define AHMAINGUI_H

#include <QWidget>
#include <gameoptiondata.h>
#include <fielddata.h>
#include <attributesliderdata.h>
#include <diedata.h>

class ConnectionHandler;
class AhBoardScene;

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
    void setThisPlayerId(QString id) { m_thisPlayerId = id; }

private slots:
    void displayItemInfo(const QString &id);

    void chooseOption(QList<AH::Common::GameOptionData> opts);
    void optionChosen(QString id);

    void chooseMove(AH::Common::FieldData::FieldID startId, int movementPoints);
    void movementChosen(QList<AH::Common::FieldData::FieldID> path);
    void movementCanceled();

    void chooseFocus(QList<AH::Common::AttributeSliderData> sliders, int focusAmount);
    void focusChosen(QList<int> diffs);

    void showDieRollInfo(AH::Common::DieRollTestData data);

private:
    Ui::AhMainGui *ui;

    AhBoardScene *m_scene;

    ConnectionHandler *m_conn;

    QString m_thisPlayerId;
};

#endif // AHMAINGUI_H
