#include "characterwidget.h"
#include "ui_characterwidget.h"
#include "../utils.h"
#include "objectregistry.h"
#include "resourcepool.h"
#include <QtGui>

CharacterWidget::CharacterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CharacterWidget)
{
    ui->setupUi(this);
}

CharacterWidget::~CharacterWidget()
{
    delete ui;
}

void CharacterWidget::objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc)
{
    if (desc.id == m_curCharId && desc.type == AH::Common::RequestObjectsData::Character) {
        AH::Common::CharacterData c;
        desc.data >> c;
        updateCharacterData(&c);
    }
}

QPixmap CharacterWidget::drawCharacterWithStats(const AH::Common::CharacterData *c, double scale, QSize s)
{
    s.setWidth(s.width()*scale);
    s.setHeight(s.height()*scale);
    QPixmap ret(s);
    ret.fill();
    if (c) {
        // Main image
        QPainter p(&ret);
        p.setRenderHint(QPainter::Antialiasing);
        QPixmap img = ResourcePool::instance()->loadCharacterFigure(c->id()).scaled(s);
        p.drawPixmap(0, 0, img);

        QFont f = ResourcePool::instance()->loadMainFont();
        f.setPixelSize(46*scale);

        // Sanity
        QPixmap sanity = QPixmap(":/core/marker/sanity").scaled(50*scale,50*scale, Qt::KeepAspectRatio);
        p.drawPixmap(8*scale, 12*scale, sanity);
        QPainterPath pathSan;
        pathSan.addText(20*scale, 36*scale, f, QString::number(c->curSanity()));
        p.fillPath(pathSan, QColor(0x3672AE));
        QPainterPathStroker strSan;
        pathSan = strSan.createStroke(pathSan);
        //p.setPen(Qt::white);
        p.drawPath(pathSan);

        // Stamina
        QPixmap stamina = QPixmap(":/core/marker/stamina").scaled(50*scale,50*scale, Qt::KeepAspectRatio);
        p.drawPixmap(142*scale, 7*scale, stamina);
        QPainterPath pathSta;
        pathSta.addText(154*scale, 36*scale, f, QString::number(c->curStamina()));
        p.fillPath(pathSta, QColor(0xD2363A));
        QPainterPathStroker strSta;
        pathSta = strSta.createStroke(pathSta);
        //p.setPen(Qt::white);
        p.drawPath(pathSta);
    }

    return ret;
}

void CharacterWidget::displayCharacterData(const AH::Common::CharacterData *data)
{
    m_curCharId = QString::null;
    ObjectRegistry::instance()->unsubscribe(this);
    updateCharacterData(data);
    if (data) {
        m_curCharId = data->id();
        ObjectRegistry::instance()->asyncSubscribeObject(this, data->id(), AH::Common::RequestObjectsData::Character);
    }
}

void CharacterWidget::updateCharacterData(const AH::Common::CharacterData *data)
{
    if (data) {
        ui->lblName->setText(QString("<a href=\"%1\">%2</a>").arg(data->id(), data->investigatorData().name()));
        ui->lblClue->setText(QString::number(data->clues()));
        ui->lblStamina->setText(QString("%1 / %2").arg(data->curStamina()).arg(data->maxStamina()));
        ui->lblSanity->setText(QString("%1 / %2").arg(data->curSanity()).arg(data->maxSanity()));
        ui->lblMoney->setText(QString::number(data->money()));
        AH::Common::InvestigatorData::AttributeValuePair ss = data->investigatorData().attrSpeedSneak().value(data->attrSettings().value(0));
        ui->lblSpeedSneak->setText(QString("%1 / %2").arg(ss.first).arg(ss.second));
        AH::Common::InvestigatorData::AttributeValuePair fw = data->investigatorData().attrFightWill().value(data->attrSettings().value(1));
        ui->lblFightWill->setText(QString("%1 / %2").arg(fw.first).arg(fw.second));
        AH::Common::InvestigatorData::AttributeValuePair ll = data->investigatorData().attrLoreLuck().value(data->attrSettings().value(2));
        ui->lblLuckLore->setText(QString("%1 / %2").arg(ll.first).arg(ll.second));
        ui->lblFocus->setText(QString::number(data->curFocus()));
        ui->lblMonsterTrophies->setText(QString::number(data->monsterMarkerIds().count()));
        ui->lblGateTrophies->setText(QString::number(data->gateMarkerIds().count()));
        ui->lblMonsterToughness->setText(QString::number(data->getMonsterToughness()));
        ui->lblField->setText(Utils::stringForField(data->fieldId()));
    } else {
        ui->lblName->setText("");
        ui->lblClue->setText("");
        ui->lblStamina->setText("");
        ui->lblSanity->setText("");
        ui->lblMoney->setText("");
        ui->lblSpeedSneak->setText("");
        ui->lblFightWill->setText("");
        ui->lblLuckLore->setText("");
        ui->lblFocus->setText("");
        ui->lblMonsterTrophies->setText("");
        ui->lblGateTrophies->setText("");
        ui->lblMonsterToughness->setText("");
        ui->lblField->setText("");
    }
}

void CharacterWidget::requestCharacterDetail(const QString &id)
{
    emit characterDetailRequested(id);
}
