#ifndef INVESTIGATORWIDGET_H
#define INVESTIGATORWIDGET_H

#include <QWidget>
#include <investigatordata.h>
#include <characterdata.h>

class InvestigatorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InvestigatorWidget(QWidget *parent = 0);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:

public slots:
    void displayInvestigator(const AH::Common::InvestigatorData &inv);
    void displayCharacter(const AH::Common::CharacterData &chr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawFixedLineHeightText(QPainter *p, const QString &t, QRect r, int lineHeight);
    void paintInvestigator();
    void paintCharacter();
    QPainter *paintBase();
    QVector<QVector<QRect> > paintAttributeSliders(QPainter &p, int totalWidth);

    QPainter *createBase(const QSize base);

private:
    QPixmap m_img;
    AH::Common::InvestigatorData m_inv;
    AH::Common::CharacterData m_char;
};

#endif // INVESTIGATORWIDGET_H
