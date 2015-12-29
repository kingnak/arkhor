#ifndef OBJECTINFOWIDGET_H
#define OBJECTINFOWIDGET_H

#include <QWidget>
#include <objectdata.h>

namespace AH { namespace Common {
class AncientOneData;
class MonsterData;
class MythosData;
class GateData;
class GameObjectData;
class CharacterData;
} }

class QStackedWidget;
class GateDataWidget;
class MonsterWidget;
class GameObjectWidget;
class MythosCardWidget;
class AncientOneWidget;
class CharacterWidget;

class MinSizeStack;

class ObjectInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ObjectInfoWidget(QWidget *parent = 0);

    QSize sizeHint() const;

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
    void displayMythosDetails(const AH::Common::MythosData *m);
    void displayAncientOneDetails(const AH::Common::AncientOneData *ao);
    void displayCharacterDetails(const AH::Common::CharacterData *c);

private:
    QString m_pendingDisplayId;

    MinSizeStack *m_stack;
    GateDataWidget *m_gate;
    MonsterWidget *m_monster;
    GameObjectWidget *m_object;
    MythosCardWidget *m_mythos;
    AncientOneWidget *m_ancientOne;
    CharacterWidget *m_character;
};

#endif // OBJECTINFOWIDGET_H
