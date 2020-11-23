#ifndef OBJECTSELECTORWIDGET_H
#define OBJECTSELECTORWIDGET_H

#include <QWidget>
#include <objectdata.h>

class QListWidget;
class QListWidgetItem;

namespace Ui {
class ObjectSelectorWidget;
}

class ObjectSelectorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ObjectSelectorWidget(QWidget *parent = 0);
    ~ObjectSelectorWidget() override;

    QStringList getSelectedItems() const;

public slots:
    void activate();
    void deactivate();
    void setItems(const QStringList &objectIds);
    void clearItems();
    void setMinItems(int min);
    void setMaxItems(int max);
    void showCounts(bool show);

signals:
    void requestObjectInfo(QString id);
    void selectionOk(bool ok);
    void selectionWrong(bool wrong);
    void activateChoice();

private slots:
    void describeItem(const AH::Common::DescribeObjectsData::ObjectDescription &desc);
    void selectItem();
    void deselectItem();
    void displayItem(QListWidgetItem *itm);

private:
    enum {
        IdRole = Qt::UserRole
    };

private:
    void checkItemCount();
    void selectMoney(int row, QListWidgetItem *itm);
    void updateMoney(QListWidget *lst, int add);
    void updateMoney(QListWidgetItem *itm, int add);

private:
    Ui::ObjectSelectorWidget *ui;
    bool m_isActive;
    int m_max;
    int m_min;
    QMap<QString, QListWidgetItem *> m_items;
};

#endif // OBJECTSELECTORWIDGET_H
