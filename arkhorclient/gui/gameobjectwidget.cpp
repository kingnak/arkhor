#include "gameobjectwidget.h"
#include "ui_gameobjectwidget.h"
#include "resourcepool.h"
#include "utils.h"
#include "objectregistry.h"

GameObjectWidget::GameObjectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameObjectWidget)
{
    ui->setupUi(this);
    QFont f = ResourcePool::instance()->loadMainFont();
    f.setBold(true);
    ui->lblName->setFont(f);
}

GameObjectWidget::~GameObjectWidget()
{
    delete ui;
}

void GameObjectWidget::objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc)
{
    if (desc.id == m_curObjId && desc.type == AH::Common::RequestObjectsData::Object) {
        AH::Common::GameObjectData o;
        desc.data >> o;
        updateGameObject(&o);
    }
}

void GameObjectWidget::displayGameObject(const AH::Common::GameObjectData *obj)
{
    m_curObjId = QString::null;
    ObjectRegistry::instance()->unsubscribe(this);
    updateGameObject(obj);
    if (obj) {
        m_curObjId = obj->id();
        ObjectRegistry::instance()->asyncSubscribeObject(this, obj->id(), AH::Common::RequestObjectsData::Object);
    }
}

void GameObjectWidget::updateGameObject(const AH::Common::GameObjectData *obj)
{
    if (obj) {
        ui->lblName->setText(obj->name());
        ui->lblType->setText(Utils::stringForObjectType(obj->type()));
        ui->lblExhausted->setVisible(obj->isExhausted());
        //ui->lblPhases->setText(Utils::stringsForPhases(obj->));
        if (obj->type() == AH::Obj_Spell) {
            ui->lblSpellCost->setText(QString("Cast Sanity Const: %1").arg(obj->castCost()));
            QString s = Utils::fullNumberString(obj->castAdjustment());
            if (obj->castTarget() != 1) s += QString(" [%1]").arg(obj->castAdjustment());
            ui->lblSpellAdjust->setText("Cast Adjustment: "+s);
        } else {
            ui->lblSpellAdjust->setText("");
            ui->lblSpellCost->setText("");
        }
        QString desc = obj->description();
        QStringList props;
        for (const auto &dp : obj->dynamicProperties()) {
            QString s = QString("%1: %2").arg(dp.name(), dp.display());
            props << s;
        }
        if (!props.empty()) {
            desc += "\n\n" + props.join("\n");
        }
        ui->lblDesc->setText(desc);

        QStringList l;
        for (const auto &mod : obj->getModificationData()) {
            l << Utils::stringForPropertyModification(mod);
        }
        ui->lblModifications->setText(l.join("\n"));
        ui->lblAttributes->setText(Utils::stringsForObjectAttributes(obj->getAttributes()).join("\n"));
        ui->lblPrice->setText(QString("$ %1").arg(obj->price()));
        ui->lblHands->setText(QString("%1 hands").arg(obj->handCount()));
        QPixmap img = ResourcePool::instance()->loadObjectImage(obj->typeId(), obj->type());
        if (img.isNull()) {
            ui->lblImage->setVisible(false);
        } else {
            ui->lblImage->setPixmap(img);
            ui->lblImage->setVisible(true);
        }
    } else {
        ui->lblName->setText("");
        ui->lblType->setText("");
        ui->lblExhausted->setVisible(false);
        ui->lblSpellAdjust->setText("");
        ui->lblSpellCost->setText("");
        ui->lblDesc->setText("");
        ui->lblModifications->setText("");
        ui->lblAttributes->setText("");
        ui->lblPrice->setText("");
        ui->lblHands->setText("");
        ui->lblImage->setPixmap(QPixmap());
    }

    // Hide labels without text to preserve space
    for (auto l : this->findChildren<QLabel*>()) {
        if (l != ui->lblImage && l != ui->lblExhausted) {
            if (l->text().isEmpty()) {
                l->setVisible(false);
            } else {
                l->setVisible(true);
            }
        }
    }
}
