#include "environmentshortwidget.h"
#include <QtGui>
#include "resourcepool.h"
#include "utils.h"
#include "objectregistry.h"

using namespace AH::Common;

EnvironmentShortWidget::EnvironmentShortWidget(QWidget *parent) :
    QWidget(parent)
{
    m_name = new QLabel;
    m_name->setFont(ResourcePool::instance()->loadMainFont());
    m_name->setAlignment(Qt::AlignHCenter);

    m_type = new QLabel;
    QFont f = m_type->font();
    f.setItalic(true);
    m_type->setFont(f);
    m_type->setAlignment(Qt::AlignHCenter);

    m_mods = new QLabel;

    QVBoxLayout *l = new QVBoxLayout(this);
    l->addWidget(m_name);
    l->addWidget(m_type);
    l->addWidget(m_mods, 1);

    connect(m_name, SIGNAL(linkActivated(QString)), this, SIGNAL(environmentInfoRequested(QString)));
}

void EnvironmentShortWidget::objectDescribed(const DescribeObjectsData::ObjectDescription &desc)
{
    if (desc.type == RequestObjectsData::Mythos) {
        MythosData m;
        desc.data >> m;
        displayEnvironment(&m);
    }
}

void EnvironmentShortWidget::displayEnvironment(QString id)
{
    ObjectRegistry::instance()->asyncGetObject(this, id);
}

void EnvironmentShortWidget::displayEnvironment(MythosData *env)
{
    if (env && env->type() == AH::Common::MythosData::Environment) {
        QString nl = QString("<a href=\"%1\" style=\"text-decoration:none; color:rgb(0,0,0)\">%2</a>");
        m_name->setText(nl.arg(env->id(), env->name()));
        m_type->setText(Utils::stringForMythosEnvironmentType(env->environmenType()));
        QStringList lst;
        foreach (PropertyModificationData d, env->getModificationData()) {
            lst << Utils::stringForPropertyModification(d);
        }
        m_mods->setText(lst.join("\n"));
    } else {
        m_name->setText("");
        m_type->setText("");
        m_mods->setText("");
    }
}
