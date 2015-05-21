#include "monsterwidget.h"
#include "ui_monsterwidget.h"
#include "resourcepool.h"
#include "utils.h"
#include <QtGui>

MonsterFrontWidget::MonsterFrontWidget(QWidget *parent) :
    QWidget(parent)
{

}

MonsterFrontWidget::~MonsterFrontWidget()
{

}

QPixmap MonsterFrontWidget::drawMonster(const AH::Common::MonsterData *m, QSize s)
{
    QPixmap ret(s);
    ret.fill();
    if (m) {
        // Main image
        QPainter p(&ret);
        p.setRenderHint(QPainter::Antialiasing);
        QPixmap img = ResourcePool::instance()->loadMonster(m->id()).scaled(s);
        p.drawPixmap(0, 0, img);
        QPixmap overlay = QPixmap(":/core/images/monster_overlay").scaled(s);
        p.drawPixmap(0, 0, overlay);

        QFont f = ResourcePool::instance()->loadMainFont();
        // Name
        f.setBold(true);
        f.setPixelSize(10);
        p.setBrush(Qt::black);
        p.setFont(f);
        QRect nameRect(6,6,154,14);
        p.drawText(nameRect, m->name());

        // Awareness
        f.setPixelSize(28);
        f.setBold(false);
        QRect awareRect(160, 6, 40, 28);
        p.setClipRect(awareRect);
        QPainterPath txtPath;
        txtPath.addText(160, 30, f, Utils::fullNumberString(m->awareness()));
        p.fillPath(txtPath, QColor(0xD2363A));
        QPainterPathStroker txtStroke;
        txtPath = txtStroke.createStroke(txtPath);
        p.drawPath(txtPath);
        p.setClipping(false);

        // Dimension
        QPixmap dimSym = ResourcePool::instance()->loadDimensionSymbol(m->dimension());
        QRect dimRect(162,162,36,36);
        p.drawPixmap(dimRect, dimSym);

        // Movment border
        p.setBrush(Qt::NoBrush);
        p.setPen(QPen(MonsterWidget::getMovementTypeColor(m->movementType()), 8));
        p.drawRect(0, 0, s.width(), s.height());
    }
    return ret;
}

void MonsterFrontWidget::displayMonster(const AH::Common::MonsterData *m)
{
    QSize s = sizeHint();
    m_cache = MonsterFrontWidget::drawMonster(m, s);
    update();
}

QSize MonsterFrontWidget::sizeHint() const
{
    return QSize(200,200);
}

QSize MonsterFrontWidget::minimumSizeHint() const
{
    return QSize(200, 200);
}

void MonsterFrontWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.drawPixmap(0, 0, m_cache);
}

//////////////////////////////////


MonsterBackWidget::MonsterBackWidget(QWidget *parent) :
    QWidget(parent)
{

}

MonsterBackWidget::~MonsterBackWidget()
{

}

void MonsterBackWidget::displayMonster(const AH::Common::MonsterData *m)
{
    QSize s = sizeHint();
    m_cache = QPixmap(s);
    m_cache.fill();

    if (m) {
        // Main image
        QPainter p(&m_cache);
        p.setRenderHint(QPainter::Antialiasing);
        QPixmap img = ResourcePool::instance()->loadMonster(m->id()).scaled(s);
        p.drawPixmap(0, 0, img);
        QPixmap overlay = QPixmap(":/core/images/monster_overlay").scaled(s);
        p.drawPixmap(0, 0, overlay);
        p.fillRect(0, 0, s.width(), s.height(), QBrush(QColor(244, 244, 244, 225)));

        QFont f = ResourcePool::instance()->loadMainFont();
        f.setPixelSize(24);

        // Horror
        {
            QPixmap sanity = QPixmap(":/core/marker/sanity").scaled(30,30, Qt::KeepAspectRatio);
            QPainterPath pathAdj;

            if (m->horrorDamage() == 0) {
                pathAdj.addText(8, 165, f, "-");
            } else {
                pathAdj.addText(8, 165, f, Utils::fullNumberString(m->horrorAdjustment()));

                p.drawPixmap(8, 175, sanity);
                QPainterPath pathDmg;
                pathDmg.addText(16, 190, f, QString::number(m->horrorDamage()));
                p.fillPath(pathDmg, QColor(0x3672AE));
                QPainterPathStroker strDmg;
                pathDmg = strDmg.createStroke(pathDmg);
                //p.setPen(Qt::white);
                p.drawPath(pathDmg);
            }
            p.fillPath(pathAdj, QColor(0x3672AE));
            QPainterPathStroker strAdj;
            pathAdj = strAdj.createStroke(pathAdj);
            //p.setPen(Qt::black);
            p.drawPath(pathAdj);
        }

        // Combat
        {
            QPixmap combat = QPixmap(":/core/marker/stamina").scaled(30,30, Qt::KeepAspectRatio);
            p.drawPixmap(162, 170, combat);
            QPainterPath pathDmg;
            pathDmg.addText(170, 190, f, QString::number(m->combatDamage()));
            p.fillPath(pathDmg, QColor(0xD2363A));
            QPainterPathStroker strDmg;
            pathDmg = strDmg.createStroke(pathDmg);
            //p.setPen(Qt::white);
            p.drawPath(pathDmg);

            QPainterPath pathAdj;
            pathAdj.addText(160, 165, f, Utils::fullNumberString(m->combatAdjustment()));
            p.fillPath(pathAdj, QColor(0xD2363A));
            QPainterPathStroker strAdj;
            pathAdj = strAdj.createStroke(pathAdj);
            //p.setPen(Qt::black);
            p.drawPath(pathAdj);
        }

        // Toughness
        {
            QPixmap tough = QPixmap(":/core/marker/toughness").scaled(25,25, Qt::KeepAspectRatio);
            p.drawPixmap(75, 170, tough);
            QPainterPath path;
            path.addText(100, 190, f, QString::number(m->toughness()));
            p.fillPath(path, QColor(0xD2363A));
            QPainterPathStroker str;
            path = str.createStroke(path);
            //p.setPen(Qt::white);
            p.drawPath(path);
        }

        // Text
        QRect textClip(10, 10, 180, 128);
        p.setClipRect(textClip);

        QStringList l = Utils::stringsForMonsterAttributes(m->attributes());
        QString str = l.join("\n");
        QFont ftxt("Times New Roman", 8);
        p.setFont(ftxt);

        if (!l.isEmpty()) {
            ftxt.setBold(true);
            p.setFont(ftxt);
            QRect rTxt;
            p.drawText(textClip, 0, str, &rTxt);
            textClip.setTop(rTxt.bottom() + 2);

            ftxt.setBold(false);
            p.setFont(ftxt);
        }

        QString desc = m->description();
        if (!desc.isEmpty()) {
            QRect rTxt;
            p.drawText(textClip, Qt::TextWordWrap, desc, &rTxt);
            textClip.setTop(rTxt.bottom() + 2);
        }

        QString myth = m->mythText();
        if (!myth.isEmpty()) {
            QRect rTxt;
            ftxt.setItalic(true);
            p.setFont(ftxt);
            p.drawText(textClip, Qt::TextWordWrap, myth, &rTxt);
            textClip.setTop(rTxt.bottom() + 2);
        }

        // Movment border
        p.setClipping(false);
        p.setBrush(Qt::NoBrush);
        p.setPen(QPen(MonsterWidget::getMovementTypeColor(m->movementType()), 8));
        p.drawRect(0, 0, s.width(), s.height());
    }
    update();
}

QSize MonsterBackWidget::sizeHint() const
{
    return QSize(200,200);
}

QSize MonsterBackWidget::minimumSizeHint() const
{
    return QSize(200, 200);
}

void MonsterBackWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.drawPixmap(0, 0, m_cache);
}

//////////////////////////////////

MonsterWidget::MonsterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MonsterWidget)
{
    ui->setupUi(this);
    displayMonster(NULL);
}

MonsterWidget::~MonsterWidget()
{
    delete ui;
}

void MonsterWidget::displayMonster(const AH::Common::MonsterData *m)
{
    ui->wgtBackImg->displayMonster(m);
    ui->wgtFrontImg->displayMonster(m);

    if (m) {
        ui->lblName->setText(m->name());
        ui->lblDimension->setText(Utils::stringForDimension(m->dimension()));
        ui->lblAwareness->setText(Utils::fullNumberString(m->awareness()));
        ui->lblMovement->setText(Utils::stringForMovement(m->movementType()));
        if (m->horrorDamage() == 0) {
            ui->lblHorror->setText("-");
        } else {
            ui->lblHorror->setText(QString("%1 / %2").arg(Utils::fullNumberString(m->horrorAdjustment())).arg(m->horrorDamage()));
        }
        ui->lblCombat->setText(QString("%1 / %2").arg(Utils::fullNumberString(m->combatAdjustment())).arg(m->combatDamage()));
        ui->lblToughness->setText(QString::number(m->toughness()));
        QStringList l = Utils::stringsForMonsterAttributes(m->attributes());
        ui->lblAttributes->setText(l.join("\n"));
    } else {
        ui->lblName->setText("");
        ui->lblDimension->setText("");
        ui->lblAwareness->setText("");
        ui->lblMovement->setText("");
        ui->lblHorror->setText("");
        ui->lblCombat->setText("");
        ui->lblToughness->setText("");
        ui->lblAttributes->setText("");
    }
}

void MonsterWidget::showFront()
{
    ui->wgtBackImg->setVisible(false);
    ui->wgtBackText->setVisible(false);
    ui->wgtFrontImg->setVisible(true);
}

void MonsterWidget::showBack()
{
    ui->wgtBackImg->setVisible(true);
    ui->wgtBackText->setVisible(true);
    ui->wgtFrontImg->setVisible(false);
}

void MonsterWidget::showBoth()
{
    ui->wgtBackImg->setVisible(true);
    ui->wgtBackText->setVisible(true);
    ui->wgtFrontImg->setVisible(true);
}

QColor MonsterWidget::getMovementTypeColor(AH::Common::MonsterData::MovementType type)
{
    switch (type) {
    case AH::Common::MonsterData::Normal:
        return Qt::black;
    case AH::Common::MonsterData::Fast:
        return QColor(0xEE3439);
    case AH::Common::MonsterData::Stationary:
        return QColor(0xFBDF26);
    case AH::Common::MonsterData::Flying:
        return QColor(0x97C7EB);
    case AH::Common::MonsterData::Special:
        return QColor(0x63BB53);
    default:
        return qRgba(0,0,0,0);
    }
}
