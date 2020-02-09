#ifndef DETAILCARDWIDGET_H
#define DETAILCARDWIDGET_H

#include <QStackedWidget>
#include <ancientonedata.h>

class AncientOneCardWidget;

class DetailCardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DetailCardWidget(QWidget *parent = nullptr);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

public slots:
    void clear();
    void displayAncientOne(AH::Common::AncientOneData *ao);

signals:

private:
    QStackedWidget *m_stack;
    AncientOneCardWidget *m_ao;
};

#endif // DETAILCARDWIDGET_H
