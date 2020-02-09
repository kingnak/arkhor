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

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

signals:

public slots:
    void displayInvestigator(AH::Common::InvestigatorData inv);
    void displayCharacter(AH::Common::CharacterData chr);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawFixedLineHeightText(QPainter *p, QString t, QRect r, int lineHeight);
    void paintInvestigator();
    void paintCharacter();
    void paintBase(QPainter &p);

    QPainter *createBase();

private:
    QPixmap m_img;
    AH::Common::InvestigatorData m_inv;
    AH::Common::CharacterData m_char;
};

#endif // INVESTIGATORWIDGET_H
