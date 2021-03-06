#ifndef PAYMENTSELECTORWIDGET_H
#define PAYMENTSELECTORWIDGET_H

#include <QWidget>
#include <cost.h>
#include <QStyle>

class QLabel;

class PaymentSelectorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaymentSelectorWidget(QWidget *parent = 0);

public:
    AH::Common::CostList getSelectedPayment() const;
    int getSelectedPaymentIndex() const;

public slots:
    void displayPayments(const QString &desc, const AH::Common::Cost &costs);
    void clearPayments();

signals:
    void choiceOk(bool ok);
    void activateChoice();
    void objectInfoRequested(QString id);

private slots:
    void alternativeSelected();

private:
    AH::Common::Cost m_cost;
    int m_selIdx;

    QWidget *m_optionsWidget;
    QLabel *m_display;
    QLabel *m_description;
};

#endif // PAYMENTSELECTORWIDGET_H
