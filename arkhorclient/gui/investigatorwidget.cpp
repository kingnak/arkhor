#include "investigatorwidget.h"
#include "resourcepool.h"
#include "utils.h"
#include "objectregistry.h"
#include <QtWidgets>

InvestigatorWidget::InvestigatorWidget(QWidget *parent) :
    QWidget(parent)
{
}

QSize InvestigatorWidget::sizeHint() const
{
    return {768, 1080};
}

QSize InvestigatorWidget::minimumSizeHint() const
{
    return {512, 720};
}

void InvestigatorWidget::displayInvestigator(const AH::Common::InvestigatorData &inv)
{
    m_inv = inv;
    paintInvestigator();
}

void InvestigatorWidget::displayCharacter(const AH::Common::CharacterData &chr)
{
    m_char = chr;
    m_inv = m_char.investigatorData();
    paintCharacter();
}

void InvestigatorWidget::paintInvestigator()
{
    QScopedPointer<QPainter> pp(paintBase());
    auto inv = m_inv;

    QPainter &p = *pp;

    QFont f = ResourcePool::instance()->loadMainFont();
    QFont ft("Times New Roman", 24);

    f.setPixelSize(36);
    f.setLetterSpacing(QFont::PercentageSpacing, 75);
    f.setWeight(12);
    // Home
    {
        QRect hr(48, 600, 122, 36);
        p.setFont(f);
        p.setPen(Qt::black);
        p.drawText(hr, "Home:");

        QRect homeRect(160,606,318,36);
        p.setPen(Qt::black);
        ft.setLetterSpacing(QFont::PercentageSpacing, 90);
        p.setFont(ft);
        p.drawText(homeRect, Utils::stringForField(inv.startFieldId()));
    }

    // Random Items
    {
        QRect hr(48, 780, 486, 36);
        p.setFont(f);
        p.drawText(hr, "Random Possessions:");
        QStringList rndPoss;
        for (auto otc : inv.randomPossesions()) {
            rndPoss << QString("%1 %2").arg(otc.amount).arg(Utils::stringForObjectType(otc.type));
        }
        QRect rndRect(50,826,426,60);
        p.setFont(ft);
        drawFixedLineHeightText(&p, rndPoss.join(", "), rndRect, 26);
    }

    // Fixed Possessions
    {
        QRect hr(48, 652, 486, 36);
        p.setFont(f);
        p.drawText(hr, "Fixed Possessions:");

        QStringList fixPoss;
        if (inv.money() > 0)
            fixPoss << QString("<b>$</b>%1").arg(inv.money());
        if (inv.clues() > 0)
            fixPoss << QString("%1 Clues").arg(inv.clues());
        for (const auto &oid : inv.fixedPossessionNames()) {
            fixPoss << oid;
        }
        QRect fixRect(50,700,426,90);
        p.setFont(ft);
        drawFixedLineHeightText(&p, fixPoss.join(", "), fixRect, 26);
    }

    // Attrs
    {
        QPixmap base = QPixmap(":/core/images/inv_template");
        paintAttributeSliders(p, base.width());
    }

    update();
}

void InvestigatorWidget::paintCharacter()
{
    QScopedPointer<QPainter> pp(paintBase());

    QPainter &p = *pp;

    QFont f = ResourcePool::instance()->loadMainFont();
    QFont ft("Times New Roman", 24);

    f.setPixelSize(36);
    f.setLetterSpacing(QFont::PercentageSpacing, 75);
    f.setWeight(12);

    // Location
    {
        QRect hr(48, 600, 122, 36);
        p.setFont(f);
        p.setPen(Qt::black);
        p.drawText(hr, "Field:");

        QRect homeRect(160,606,318,36);
        p.setPen(Qt::black);
        ft.setLetterSpacing(QFont::PercentageSpacing, 90);
        p.setFont(ft);
        p.drawText(homeRect, Utils::stringForField(m_char.fieldId()));
    }

    // Possessions
    {
        QRect hr(48, 652, 486, 36);
        p.setFont(f);
        p.drawText(hr, "Possessions:");

        auto reg = ObjectRegistry::instance();
        QStringList poss;
        poss << QString("<b>$</b>%1").arg(m_char.money());
        poss << QString("%1 Clues").arg(m_char.clues());
        for (const auto &oid : m_char.inventoryIds()) {
            auto o = reg->getObject<AH::Common::GameObjectData>(oid);
            if (o.id().isEmpty()) {
                poss << oid;
            } else {
                poss << o.name();
            }
        }
        QRect possRect(50,700,426,166);
        p.setFont(ft);
        drawFixedLineHeightText(&p, poss.join(", "), possRect, 26);
    }

    // Attrs
    {
        QPixmap sl = QPixmap(":/core/marker/slider");
        QPixmap base = QPixmap(":/core/images/inv_template");
        auto attrPos = paintAttributeSliders(p, base.width());

        for (int i = 0; i < attrPos.size(); ++i) {
            int pos = m_char.attrSettings()[i];
            QRect r = attrPos[i][pos];
            r.adjust(-28, -20, 20, 30);
            p.drawPixmap(r, sl);
        }
    }

    update();
}

QPainter *InvestigatorWidget::createBase(const QSize base)
{
    QSize s = base;
    s.scale(this->size(), Qt::KeepAspectRatio);
    m_img = QPixmap(s);
    m_img.fill();

    QPainter *p = new QPainter(&m_img);
    p->scale(double(s.width())/base.width(), double(s.height())/base.height());
    p->setRenderHint(QPainter::Antialiasing);
    return p;
}

QPainter *InvestigatorWidget::paintBase()
{
    auto &inv = m_inv;

    QPixmap base = QPixmap(":/core/images/inv_template");
    QPainter *pp = createBase(base.size());
    QPainter &p = *pp;

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
    p.setPen(Qt::black);
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
    {
        QRect hr(48, 924, 150, 44);
        f.setPixelSize(36);
        p.setFont(f);
        p.setPen(Qt::black);
        p.drawText(hr, "Focus:");

        QRect focRect(180,924,198,44);
        f.setPixelSize(42);
        p.setFont(f);
        p.drawText(focRect, Qt::AlignRight, QString::number(inv.focus()));
    }

    // Special ability
    QRect saNameRect(500,604,510,40);
    f.setPixelSize(36);
    p.setFont(f);
    p.drawText(saNameRect, Qt::AlignHCenter, inv.uniqueAbilityName());

    QFont ft("Times New Roman", 24);
    QRect saDescRect(500,644,510,350);
    p.setFont(ft);
    drawFixedLineHeightText(&p, inv.uniqueAbilityDescription(), saDescRect, 26);

    return pp;
}

QVector<QVector<QRect>> InvestigatorWidget::paintAttributeSliders(QPainter &p, int totalWidth)
{
    QVector<QVector<QRect>> ret;

    QFont f = ResourcePool::instance()->loadMainFont();
    f.setPixelSize(42);

    QFontMetrics fm(f);

    p.setFont(f);
    const int attrX = 240;
    QPoint attrTL1(0,1044);
    QPoint attrTL2(0,0);
    const int attrTotWidth = totalWidth-attrX;
    int attrCt = 2*qMax(m_inv.attrSpeedSneak().size(), qMax(m_inv.attrFightWill().size(), m_inv.attrLoreLuck().size()));
    QList<QList<AH::Common::InvestigatorData::AttributeValuePair> > attrs;
    attrs << m_inv.attrSpeedSneak() << m_inv.attrFightWill() << m_inv.attrLoreLuck();
    const int addOff = 44/qMax(1,attrCt);
    for (int i = 0; i < 3; ++i) {
        ret << QVector<QRect>();
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

            QRect bound;
            bound.setTopLeft(QPoint(attrTL1.x(), attrTL1.y()-fm.height()));
            bound.setBottomRight(QPoint(attrTL2.x()+qMax(fm.width(QString::number(attrs[i][j].first)), fm.width(QString::number(attrs[i][j].second))), attrTL2.y()));

            ret[i] << bound;
        }

        attrTL1.setY(attrTL2.y()+92);
    }

    return ret;
}

void InvestigatorWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawPixmap(0, 0, m_img.scaledToHeight(this->height()));
}

void InvestigatorWidget::resizeEvent(QResizeEvent *event)
{
    QPixmap base = QPixmap(":/core/images/inv_template");
    QWidget::resizeEvent(event);
    QSize s = base.size();
    s.scale(this->size(), Qt::KeepAspectRatio);
    if (m_img.size() != s) {
        if (m_char.id().isEmpty())
            paintInvestigator();
        else
            paintCharacter();
    }
}

void InvestigatorWidget::drawFixedLineHeightText(QPainter *p, const QString &t, QRect r, int lineHeight)
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
