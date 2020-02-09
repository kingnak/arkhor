#ifndef DETAILCARDWIDGET_H
#define DETAILCARDWIDGET_H

#include <QStackedWidget>
#include <ancientonedata.h>
#include <characterdata.h>

class AncientOneCardWidget;
class InvestigatorWidget;

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
    void displayCharacter(AH::Common::CharacterData *chr);

signals:

private:
    QStackedWidget *m_stack;
    AncientOneCardWidget *m_ao;
    InvestigatorWidget *m_inv;
};

#endif // DETAILCARDWIDGET_H
