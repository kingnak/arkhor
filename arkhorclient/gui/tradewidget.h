#ifndef TRADEWIDGET_H
#define TRADEWIDGET_H

#include <QWidget>
#include <tradedata.h>

class QListWidget;
class QListWidgetItem;
class ObjectSelectorWidget;

namespace Ui {
class TradeWidget;
}

class TradeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TradeWidget(QWidget *parent = nullptr);
    ~TradeWidget() override;

public slots:
    void showTrade(const AH::Common::TradeData &td);

signals:
    void cancelTrade();
    void selectedTrade(AH::Common::TradeData td);
    void itemInfoRequested(QString id);

private slots:
    void clear();
    void doCancel();
    void doOffer();
    void doAccept();
    void displayItem(QListWidgetItem *itm);
    void displayItem(const QString &id);

private:
    void fillList(QListWidget *lst, const QStringList &items);
    QString getName(QString characterId);

private:
    AH::Common::TradeData m_trade;

private:
    Ui::TradeWidget *ui;
};

#endif // TRADEWIDGET_H
