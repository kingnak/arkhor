#ifndef INVESTIGATORWIDGET_H
#define INVESTIGATORWIDGET_H

#include <QWidget>
#include <investigatordata.h>

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

protected:
    void paintEvent(QPaintEvent *event);

private:
    void drawFixedLineHeightText(QPainter *p, QString t, QRect r, int lineHeight);

private:
    QPixmap m_img;
};

#endif // INVESTIGATORWIDGET_H
