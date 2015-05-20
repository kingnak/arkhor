#ifndef OBJECTINFOWIDGET_H
#define OBJECTINFOWIDGET_H

#include <QWidget>
#include <objectdata.h>
#include <monsterdata.h>
#include <gatedata.h>
#include <gameobjectdata.h>

class QStackedWidget;
class GateDataWidget;
class MonsterWidget;
class GameObjectWidget;

class ObjectInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ObjectInfoWidget(QWidget *parent = 0);

signals:

public slots:
    void displayItemInfo(const QString &id);
    void displayNothing();

private slots:
    void receivedDescription(AH::Common::DescribeObjectsData::ObjectDescription desc);

private:
    void displayMonsterDetails(const AH::Common::MonsterData *m);
    void displayGateDetails(const AH::Common::GateData *g);
    void displayObjectDetails(const AH::Common::GameObjectData *o);

private:
    QString m_pendingDisplayId;

    QStackedWidget *m_stack;
    GateDataWidget *m_gate;
    MonsterWidget *m_monster;
    GameObjectWidget *m_object;
};

#endif // OBJECTINFOWIDGET_H
