#include "ancientonecardwidget.h"
#include <QtGui>
#include "../resourcepool.h"
#include "../utils.h"

AncientOneCardWidget::AncientOneCardWidget(QWidget *parent)
    : QWidget(parent)
{

}

QSize AncientOneCardWidget::sizeHint() const
{
    return QSize(700, 980);
}

QSize AncientOneCardWidget::minimumSizeHint() const
{
    return QSize(350, 490);
}

void AncientOneCardWidget::displayAncientOne(AH::Common::AncientOneData *ao)
{
    if (ao) m_ao = *ao;
    else m_ao = {};
    drawAncientOne();
}

void AncientOneCardWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawPixmap(0, 0, m_cache.scaled(this->size()));
}


void AncientOneCardWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    drawAncientOne();
    //update();
}

void AncientOneCardWidget::drawAncientOne()
{
    QPixmap mask = QPixmap(":/core/images/ao_template");
    QSize s = this->size();
    m_cache = QPixmap(s);
    m_cache.fill();
    if (m_ao.id().isEmpty()) {
        update();
        return;
    }

    QPainter p(&m_cache);
    p.scale(double(s.width())/mask.width(), double(s.height())/mask.height());
    p.setRenderHint(QPainter::Antialiasing);
    QPixmap aoImg = ResourcePool::instance()->loadAncientOne(m_ao.id());
    p.drawPixmap(0, 0, aoImg);
    p.drawPixmap(0, 0, mask);

    QFont f = ResourcePool::instance()->loadMainFont();
    f.setBold(true);
    f.setPointSize(24);
    p.setFont(f);
    QTextOption to(Qt::AlignHCenter|Qt::AlignVCenter);
    to.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    p.drawText(QRect(305, 470, 360, 80), m_ao.name(), to);

    f.setPointSize(36);
    p.setFont(f);
    p.drawText(QRect(80, 490, 220, 80), Utils::fullNumberString(m_ao.combatAdjustment()), to);

    f.setPointSize(18);
    p.setFont(f);
    QStringList defs = Utils::stringsForMonsterAttributes(m_ao.defenses());
    if (defs.empty()) defs << "None";
    p.drawText(QRect(680, 490, 220, 80), defs.join(", "), to);

    QFont f2("Times New Roman", 18);
    p.setFont(f2);

    {
        p.save();
        QString str;
        str = "<center><b>Worshippers</b><p>"
                + m_ao.worshippersText() +
              "</p></center>";

        QRect r(4, 603, 290, 350);
        QTextDocument td;
        td.setHtml(str);
        td.setTextWidth(r.width());
        td.setDefaultFont(f2);

        QAbstractTextDocumentLayout::PaintContext ctx;
        ctx.clip = QRect(QPoint(), r.size());
        p.translate(r.topLeft());
        p.setClipRect(QRect(QPoint(), r.size()));
        td.documentLayout()->draw(&p, ctx);
        p.restore();
    }

    {
        p.save();
        QString str =
                "<center><p><b><i>" + m_ao.powerTitle() + "</i></b></p><p>"
                + m_ao.powerText();
        if (!m_ao.battleStartText().isEmpty())
            str += "</p><p><b>Start of Battle</b><br>" + m_ao.battleStartText();
        str += "</p></center>";

        QRect r(305, 560, 360, 400);

        QTextDocument td;
        td.setHtml(str);
        td.setTextWidth(r.width());
        td.setDefaultFont(f2);

        QAbstractTextDocumentLayout::PaintContext ctx;
        ctx.clip = QRect(QPoint(), r.size());
        p.translate(r.topLeft());
        p.setClipRect(QRect(QPoint(), r.size()));
        td.documentLayout()->draw(&p, ctx);
        p.restore();
    }

    {
        p.save();
        QString str =
                "<center><p><b>Attack</b><p>"
                + m_ao.attackText() +
                "</p></center>";

        QRect r(700, 603, 290, 350);

        QTextDocument td;
        td.setHtml(str);
        td.setTextWidth(r.width());
        td.setDefaultFont(f2);

        QAbstractTextDocumentLayout::PaintContext ctx;
        ctx.clip = QRect(QPoint(), r.size());
        p.translate(r.topLeft());
        p.setClipRect(QRect(QPoint(), r.size()));
        td.documentLayout()->draw(&p, ctx);
        p.restore();
    }

    // doom track
    {
        QPixmap dtb = QPixmap(":/core/marker/doom_token");
        QPixmap dtr = QPixmap(":/core/marker/doom_token_red");
        const int tw_ = dtb.width();
        const int th_ = dtb.height();

        const int w_ = mask.size().width() - tw_;
        const int t_ = 970;
        const int h_ = mask.size().height() - t_;

        int doomTrack = m_ao.doomTrack();
        int doomTokens = m_ao.doomValue();
        if (doomTrack < 6 || doomTrack > 17) {
            QString s = QString("%1/%2").arg(doomTrack).arg(doomTokens);
            f.setPointSize(42);

            QFontMetrics fm(f);
            QPoint pt((w_-fm.width(s))/2,t_+h_/2+fm.height());
            QPainterPath path;
            path.addText(pt, f, s);
            p.fillPath(path, Qt::white);
            QPainterPathStroker str;
            path = str.createStroke(path);
            p.setPen(Qt::black);
            p.drawPath(path);
        } else {
            int middle = doomTrack/3;
            int top = middle + (doomTrack%3-1);
            int bottom = (doomTrack%3 != 1) ? middle+1 : middle;
            if (doomTrack%3 == 1) middle++;

            QVector<int> lines = {top, middle, bottom};
            int ct = *std::max_element(lines.begin(), lines.end());

            const int oy_ = (th_*1.2);
            const int ox_ = (w_-tw_)/ct;

            f.setPointSize(42);

            for (int l = 0; l < 3; ++l) {
                const int y = t_+(h_-oy_)/3+oy_*l;
                for (int r = 0; r < lines[l]; ++r) {
                    const int x = ox_ + w_/ct/2*(ct-lines[l]+r*2);

                    int nr = r+lines.value(l-1)+lines.value(l-2)+1;
                    QRect rt(x-tw_/2, y-th_/2, tw_, th_);

                    if (nr <= doomTokens)
                        p.drawPixmap(rt, dtr);
                    else
                        p.drawPixmap(rt, dtb);

                    QString s = QString::number(nr);
                    QFontMetrics fm(f);
                    QPoint pt((rt.width()-fm.width(s))/2,(rt.height()+fm.height())/2);
                    pt += rt.topLeft();
                    QPainterPath path;
                    path.addText(pt, f, s);
                    p.fillPath(path, Qt::white);
                    QPainterPathStroker str;
                    path = str.createStroke(path);
                    p.setPen(Qt::black);
                    p.drawPath(path);
                }
            }
        }
    }

    update();
}
