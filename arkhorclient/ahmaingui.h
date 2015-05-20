#ifndef AHMAINGUI_H
#define AHMAINGUI_H

#include <QWidget>
#include <gameoptiondata.h>
#include <fielddata.h>
#include <attributesliderdata.h>
#include <diedata.h>
#include <characterdata.h>
#include <objectdata.h>

class ObjectRegistry;
class ConnectionHandler;
class AhBoardScene;
class QListWidgetItem;

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
    void setThisCharacterId(QString id) { m_thisCharacterId = id; }

public slots:
    void start();

private:
    enum {
        ObjectIdRole = Qt::UserRole
    };

private slots:
    void displayItemInfo(const QString &id);

    void displayInventoryData(QListWidgetItem *itm);

    void chooseOption(QList<AH::Common::GameOptionData> opts);
    void optionChosen(QString id);

    void chooseMove(AH::Common::FieldData::FieldID startId, int movementPoints);
    void movementChosen(QList<AH::Common::FieldData::FieldID> path);
    void movementCanceled();

    void chooseFocus(QList<AH::Common::AttributeSliderData> sliders, int focusAmount);
    void focusChosen(QList<int> diffs);

    void showDieRollInfo(AH::Common::DieRollTestData data);
    void dieUpdateChosen(AH::Common::DieTestUpdateData upd);

    void updateObject(AH::Common::DescribeObjectsData::ObjectDescription desc);
    void updateCharacter(AH::Common::CharacterData c);

private:
    Ui::AhMainGui *ui;
    AhBoardScene *m_scene;
    ConnectionHandler *m_conn;
    QString m_thisPlayerId;
    QString m_thisCharacterId;
    ObjectRegistry *m_registry;
    AH::Common::CharacterData m_thisCharacter;

    QString m_pendingDisplayId;
};

#endif // AHMAINGUI_H
