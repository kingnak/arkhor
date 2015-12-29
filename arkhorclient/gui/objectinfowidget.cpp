#include "objectinfowidget.h"
#include <QtGui>
#include "objectregistry.h"
#include "gatedatawidget.h"
#include "monsterwidget.h"
#include "gameobjectwidget.h"
#include "mythoscardwidget.h"
#include "ancientonewidget.h"
#include "characterwidget.h"
#include "minsizetab.h"

using namespace AH::Common;

ObjectInfoWidget::ObjectInfoWidget(QWidget *parent) :
    QWidget(parent)
{
    m_stack = new MinSizeStack;
    m_stack->setOrientation(Qt::Horizontal);
    m_gate = new GateDataWidget;
    m_monster = new MonsterWidget;
    m_object = new GameObjectWidget;
    m_mythos = new MythosCardWidget;
    m_ancientOne = new AncientOneWidget;
    m_character = new CharacterWidget;

    QWidget *nothing = new QWidget;
    nothing->setMinimumSize(QSize(0,0));
    nothing->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_stack->addWidget(nothing);
    m_stack->addWidget(m_gate);
    m_stack->addWidget(m_monster);
    m_stack->addWidget(m_object);
    m_stack->addWidget(m_mythos);
    m_stack->addWidget(m_ancientOne);
    m_stack->addWidget(m_character);

    QHBoxLayout *l = new QHBoxLayout(this);
    l->addWidget(m_stack);

    connect(ObjectRegistry::instance(), SIGNAL(objectDescribed(AH::Common::DescribeObjectsData::ObjectDescription)), this, SLOT(receivedDescription(AH::Common::DescribeObjectsData::ObjectDescription)));
}

QSize ObjectInfoWidget::sizeHint() const
{
    return m_stack->sizeHint().expandedTo(QSize(250,250));
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
        case AH::Common::RequestObjectsData::Mythos:
        {
            AH::Common::MythosData m;
            d.data >> m;
            displayMythosDetails(&m);
            break;
        }
        case AH::Common::RequestObjectsData::AncientOne:
        {
            AH::Common::AncientOneData ao;
            d.data >> ao;
            displayAncientOneDetails(&ao);
            break;
        }
        case AH::Common::RequestObjectsData::Character:
        {
            AH::Common::CharacterData c;
            d.data >> c;
            displayCharacterDetails(&c);
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

void ObjectInfoWidget::displayMythosDetails(const MythosData *m)
{
    m_mythos->displayMythosCard(m);
    m_stack->setCurrentWidget(m_mythos);
}

void ObjectInfoWidget::displayAncientOneDetails(const AncientOneData *ao)
{
    m_ancientOne->displayAncientOne(ao);
    m_stack->setCurrentWidget(m_ancientOne);
}

void ObjectInfoWidget::displayCharacterDetails(const CharacterData *c)
{
    if (c) {
        m_character->updateCharacterData(*c);
        m_stack->setCurrentWidget(m_character);
    }
}

