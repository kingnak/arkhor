#include "investigatorwidget.h"
#include "resourcepool.h"
#include "utils.h"
#include <QtGui>

InvestigatorWidget::InvestigatorWidget(QWidget *parent) :
    QWidget(parent)
{
}

QSize InvestigatorWidget::sizeHint() const
{
    return minimumSizeHint();
}

QSize InvestigatorWidget::minimumSizeHint() const
{
    return QSize(512, 720);
}

void InvestigatorWidget::displayInvestigator(AH::Common::InvestigatorData inv)
{
    QPixmap base = QPixmap(":/core/images/inv_template");

    QSize s = base.size();
    QPixmap ret(s);
    ret.fill();

    QPainter p(&ret);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawPixmap(0, 0, base);

    // Image
    QPixmap chrImg = ResourcePool::instance()->loadCharacterFigure("CH_"+inv.id());
    chrImg = chrImg.scaled(450,450);
    QMatrix rot;
    rot.rotate(-10);
    chrImg = chrImg.transformed(rot);
    p.drawPixmap(2,0,chrImg);

    QFont f = ResourcePool::instance()->loadMainFont();

    // Name
    f.setBold(true);
    f.setPixelSize(72);
    f.setLetterSpacing(QFont::PercentageSpacing, 80);
    p.setPen(Qt::black);
    p.setFont(f);
    QRect nameRect(540,54,460,184);
    p.drawText(nameRect, Qt::AlignHCenter | Qt::TextWordWrap, inv.name());

    // Occupation
    QRect occRect(540,238,460,80);
    f.setPixelSize(36);
    p.setFont(f);
    p.drawText(occRect, Qt::AlignHCenter, inv.occupation());

    // San / Stam
    f.setPixelSize(96);
    p.setFont(f);
    QRect sanRect(540,334,152,96);
    p.drawText(sanRect, Qt::AlignRight, QString::number(inv.sanity()));
    QRect stamRect(540,480,152,96);
    p.drawText(stamRect, Qt::AlignRight, QString::number(inv.stamina()));

    // Focus
    QRect focRect(180,924,198,44);
    f.setPixelSize(42);
    p.setFont(f);
    p.drawText(focRect, Qt::AlignRight, QString::number(inv.focus()));

    // Attrs
    {
        f.setPixelSize(42);
        p.setFont(f);
        const int attrX = 240;
        QPoint attrTL1(0,1044);
        QPoint attrTL2(0,0);
        const int attrTotWidth = s.width()-attrX;
        int attrCt = 2*qMax(inv.attrSpeedSneak().size(), qMax(inv.attrFightWill().size(), inv.attrLoreLuck().size()));
        QList<QList<AH::Common::InvestigatorData::AttributeValuePair> > attrs;
        attrs << inv.attrSpeedSneak() << inv.attrFightWill() << inv.attrLoreLuck();
        const int addOff = 44/qMax(1,attrCt);
        for (int i = 0; i < 3; ++i) {
            int off = i%2;
            attrTL2.setY(attrTL1.y()+54);
            for (int j = 0; j < attrs[i].size(); ++j) {
                int x = (attrTotWidth/attrCt+addOff)*(j*2+off);
                attrTL1.setX(attrX+x);
                attrTL2.setX(attrX+x);
                p.setPen(qRgb(36,120,209));
                p.drawText(attrTL1, QString::number(attrs[i][j].first));
                p.setPen(qRgb(209,28,10));
                p.drawText(attrTL2, QString::number(attrs[i][j].second));
            }

            attrTL1.setY(attrTL2.y()+92);
        }
    }

    // Home
    QRect homeRect(160,606,318,36);
    QFont ft("Times New Roman", 24);
    p.setPen(Qt::black);
    ft.setLetterSpacing(QFont::PercentageSpacing, 90);
    p.setFont(ft);
    p.drawText(homeRect, Utils::stringForField(inv.startFieldId()));

    // Random Items
    QStringList rndPoss;
    foreach (AH::ObjectTypeCount otc, inv.randomPossesions()) {
        rndPoss << QString("%1 %2").arg(otc.amount).arg(Utils::stringForObjectType(otc.type));
    }
    QRect rndRect(50,826,426,60);
    drawFixedLineHeightText(&p, rndPoss.join(", "), rndRect, 26);

    // Fixed Possessions
    QStringList fixPoss;
    if (inv.money() > 0)
        fixPoss << QString("<b>$</b>%1").arg(inv.money());
    if (inv.clues() > 0)
        fixPoss << QString("%1 Clues").arg(inv.clues());
    foreach (QString oid, inv.fixedPossessionNames()) {
        fixPoss << oid;
    }
    QRect fixRect(50,700,426,90);
    drawFixedLineHeightText(&p, fixPoss.join(", "), fixRect, 26);

    // Special ability
    QRect saNameRect(500,604,510,40);
    f.setPixelSize(36);
    p.setFont(f);
    p.drawText(saNameRect, Qt::AlignHCenter, inv.uniqueAbilityName());

    QRect saDescRect(500,644,510,350);
    p.setFont(ft);
    drawFixedLineHeightText(&p, inv.uniqueAbilityDescription(), saDescRect, 26);

    m_img = ret.scaled(512, 720);
    update();
}

void InvestigatorWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.drawPixmap(0, 0, m_img);
}

void InvestigatorWidget::drawFixedLineHeightText(QPainter *p, QString t, QRect r, int lineHeight)
{
    p->save();

    QTextDocument doc;
    doc.setDefaultFont(p->font());
    doc.setTextWidth(r.width());
    doc.setHtml(t);

    for (QTextBlock it = doc.begin(); it != doc.end(); it = it.next()) {
        QTextCursor c(it);
        QTextBlockFormat f = it.blockFormat();
        f.setLineHeight(lineHeight, QTextBlockFormat::FixedHeight);
        c.setBlockFormat(f);
    }

    QAbstractTextDocumentLayout::PaintContext ctx;
    ctx.clip = QRect(QPoint(), r.size());
    p->translate(r.topLeft());
    p->setClipRect(QRect(QPoint(), r.size()));
    doc.documentLayout()->draw(p,ctx);

    p->restore();
}
