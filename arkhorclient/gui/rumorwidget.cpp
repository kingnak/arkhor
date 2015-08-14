#include "rumorwidget.h"
#include <QtGui>
#include "resourcepool.h"
#include "utils.h"
#include "objectregistry.h"

using namespace AH::Common;

RumorWidget::RumorWidget(bool shortDisplay, QWidget *parent) :
    QWidget(parent), m_shortDisplay(shortDisplay)
{
    m_name = new QLabel;
    m_name->setFont(ResourcePool::instance()->loadMainFont());
    m_name->setAlignment(Qt::AlignHCenter);
    m_name->setWordWrap(true);

    m_desc = new QLabel;
    m_desc->setWordWrap(true);
    m_dynProps = new QLabel;

    QVBoxLayout *l = new QVBoxLayout(this);
    l->addWidget(m_name);
    l->addWidget(m_desc);
    l->addWidget(m_dynProps);

    connect(m_name, SIGNAL(linkActivated(QString)), this, SIGNAL(rumorInfoRequested(QString)));

    displayRumor(NULL);
}

void RumorWidget::objectDescribed(const DescribeObjectsData::ObjectDescription &desc)
{
    if (desc.type == RequestObjectsData::Mythos) {
        MythosData m;
        desc.data >> m;
        displayRumor(&m);
    }
}

void RumorWidget::displayRumor(QString id)
{
    if (id.isEmpty()) {
        ObjectRegistry::instance()->unsubscribe(this);
        displayRumor(NULL);
    } else {
        if (!m_subscribedId.isEmpty()) {
            if (m_subscribedId != id) {
                ObjectRegistry::instance()->unsubscribe(this);
                ObjectRegistry::instance()->asyncSubscribeObject(this, id);
            }
        } else {
            ObjectRegistry::instance()->asyncSubscribeObject(this, id);
        }
    }
    m_subscribedId = id;
}

void RumorWidget::setShortDisplay(bool shortDisplay)
{
    m_shortDisplay = shortDisplay;
    displayRumor(NULL);
}

void RumorWidget::setLongDisplay(bool longDisplay)
{
    setShortDisplay(!longDisplay);
}

void RumorWidget::displayRumor(const AH::Common::MythosData *rumor)
{
    if (rumor && rumor->type() == AH::Common::MythosData::Rumor) {
        QString nl = QString("<a href=\"%1\" style=\"text-decoration:none; color:rgb(0,0,0)\">%2</a>");
        m_name->setText(nl.arg(rumor->id(), rumor->name()));
        if (m_shortDisplay) {
            m_desc->setText(rumor->shortDescription());
        } else {
            m_desc->setText(rumor->description());
        }
        QStringList lst;
        foreach (DynamicPropertyData d, rumor->dynamicProperties()) {
            lst << QString("<b>%1:</b> %2").arg(d.name(), d.display());
        }
        m_dynProps->setText(lst.join("<br/>"));
    } else {
        m_name->setText("");
        m_desc->setText("");
        m_dynProps->setText("");
    }
}
