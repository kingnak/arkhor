#include "objectinfowidget.h"
#include <QtGui>
#include <objectdata.h>
#include "objectregistry.h"
#include "gatedatawidget.h"
#include "monsterwidget.h"
#include "gameobjectwidget.h"

using namespace AH::Common;

ObjectInfoWidget::ObjectInfoWidget(QWidget *parent) :
    QWidget(parent)
{
    m_stack = new QStackedWidget(this);
    m_gate = new GateDataWidget;
    m_monster = new MonsterWidget;
    m_object = new GameObjectWidget;

    m_stack->addWidget(new QWidget);
    m_stack->addWidget(m_gate);
    m_stack->addWidget(m_monster);
    m_stack->addWidget(m_object);

    connect(ObjectRegistry::instance(), SIGNAL(objectDescribed(AH::Common::DescribeObjectsData::ObjectDescription)), this, SLOT(receivedDescription(AH::Common::DescribeObjectsData::ObjectDescription)));
}

void ObjectInfoWidget::displayItemInfo(const QString &id)
{
    displayNothing();
    DescribeObjectsData::ObjectDescription d = ObjectRegistry::instance()->getObject(id);
    if (d.type == RequestObjectsData::Unknown) {
        m_pendingDisplayId = id;
    } else {
        m_pendingDisplayId = QString::null;
        switch (d.type) {
        case AH::Common::RequestObjectsData::Monster:
        {
            AH::Common::MonsterData m;
            d.data >> m;
            displayMonsterDetails(&m);
            break;
        }
        case AH::Common::RequestObjectsData::Gate:
        {
            AH::Common::GateData g;
            d.data >> g;
            displayGateDetails(&g);
            break;
        }
        case AH::Common::RequestObjectsData::Object:
        {
            AH::Common::GameObjectData o;
            d.data >> o;
            displayObjectDetails(&o);
            break;
        }
        default:
            QMessageBox::information(this, "Info", id);
        }
    }
}

void ObjectInfoWidget::displayNothing()
{
    m_stack->setCurrentIndex(0);
}

void ObjectInfoWidget::receivedDescription(DescribeObjectsData::ObjectDescription desc)
{
    if (desc.id == m_pendingDisplayId) {
        m_pendingDisplayId = QString::null;
        if (desc.type != RequestObjectsData::Unknown) {
            displayItemInfo(desc.id);
        }
    }
}

void ObjectInfoWidget::displayMonsterDetails(const AH::Common::MonsterData *m)
{
    m_monster->displayMonster(m);
    m_stack->setCurrentWidget(m_monster);
}

void ObjectInfoWidget::displayGateDetails(const AH::Common::GateData *g)
{
    m_gate->displayGate(g);
    m_stack->setCurrentWidget(m_gate);
}

void ObjectInfoWidget::displayObjectDetails(const AH::Common::GameObjectData *o)
{
    m_object->displayGameObject(o);
    m_stack->setCurrentWidget(m_object);
}

