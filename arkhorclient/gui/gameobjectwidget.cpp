#include "gameobjectwidget.h"
#include "ui_gameobjectwidget.h"
#include "resourcepool.h"
#include "utils.h"

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

void GameObjectWidget::displayGameObject(const AH::Common::GameObjectData *obj)
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
        foreach (AH::Common::DynamicPropertyData dp, obj->dynamicProperties()) {
            QString s = QString("%1: %2").arg(dp.name(), dp.display());
            props << s;
        }
        if (props.size() > 0) {
            desc += "\n\n" + props.join("\n");
        }
        ui->lblDesc->setText(desc);

        QStringList l;
        foreach (AH::Common::PropertyModificationData mod, obj->getModificationData()) {
            l << Utils::stringForPropertyModification(mod);
        }
        ui->lblModifications->setText(l.join("\n"));
        ui->lblAttributes->setText("");
        ui->lblPrice->setText(QString("$ %1").arg(obj->price()));
        ui->lblHands->setText(QString("%1 hands").arg(obj->handCount()));
        ui->lblImage->setPixmap(ResourcePool::instance()->loadObjectImage(obj->typeId(), obj->type()));


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
    foreach (QLabel *l, this->findChildren<QLabel*>()) {
        if (l != ui->lblImage && l != ui->lblExhausted) {
            if (l->text() == "") {
                l->setVisible(false);
            } else {
                l->setVisible(true);
            }
        }
    }
}
