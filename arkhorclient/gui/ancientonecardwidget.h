#ifndef ANCIENTONECARDWIDGET_H
#define ANCIENTONECARDWIDGET_H

#include <QWidget>
#include <ancientonedata.h>

class AncientOneCardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AncientOneCardWidget(QWidget *parent = nullptr);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

public slots:
    void displayAncientOne(AH::Common::AncientOneData *ao);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawAncientOne();

signals:

private:
    QPixmap m_cache;
    AH::Common::AncientOneData m_ao;
};

#endif // ANCIENTONECARDWIDGET_H
