#include "modifierinfowidget.h"
#include <QtWidgets>
#include <objectdata.h>
#include "objectregistry.h"
#include "utils.h"

ModifierInfoWidget::ModifierInfoWidget(QWidget *parent)
    : QScrollArea(parent)
{
    setLayout(new QVBoxLayout);
}

void ModifierInfoWidget::addStretch()
{
    static_cast<QBoxLayout*>(layout())->addStretch(1);
}

void ModifierInfoWidget::cleanModifiers()
{
    QLayout *l = layout();
    if (l) {
        QLayoutItem *child;
        while ((child = l->takeAt(0)) != nullptr) {
            delete child;
        }
    }
    for (QWidget *w : findChildren<QLabel*>()) {
        delete w;
    }
}

void ModifierInfoWidget::setModifiers(const QList<AH::Common::PropertyModificationData> &mods, bool appendStretch)
{
    cleanModifiers();

    for (AH::Common::PropertyModificationData mod : mods) {
        // TODO: This assumes modifier is already known
        QString id = mod.modifierId();
        QString name = id;
        if (ObjectRegistry::instance()->hasObject(id)) {
            AH::Common::GameOptionData obj;
            ObjectRegistry::instance()->getObject(id).data >> obj;
            name = obj.name();
        }

        appendModifier(id, name, Utils::stringForPropertyModification(mod));
    }

    if (appendStretch) {
        addStretch();
    }
}

QLabel *ModifierInfoWidget::appendModifier(const QString &id, const QString &name, const QString &display)
{
    QLabel *l = new QLabel(QString("<a href=\"%1\">%2</a>: %3").arg(id, name.toHtmlEscaped(), display));
    //l->setProperty(PROPERTY_MODIFIER_ID, mod.modifierId());
    l->setWordWrap(true);
    connect(l, SIGNAL(linkActivated(QString)), this, SIGNAL(objectDescriptionRequested(QString)));
    layout()->addWidget(l);

    return l;
}
