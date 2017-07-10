#ifndef OBJECTSELECTORWIDGET_H
#define OBJECTSELECTORWIDGET_H

#include <QWidget>
#include <objectdata.h>

class QListWidgetItem;

namespace Ui {
class ObjectSelectorWidget;
}

class ObjectSelectorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ObjectSelectorWidget(QWidget *parent = 0);
    ~ObjectSelectorWidget();

    QStringList getSelectedItems() const;

public slots:
    void activate();
    void deactivate();
    void setItems(QStringList objectIds);
    void clearItems();
    void setMinItems(int min);
    void setMaxItems(int max);

signals:
    void requestObjectInfo(QString id);
    void selectionOk(bool ok);
    void selectionWrong(bool wrong);
    void activateChoice();

private slots:
    void describeItem(AH::Common::DescribeObjectsData::ObjectDescription desc);
    void selectItem();
    void deselectItem();
    void displayItem(QListWidgetItem *itm);

private:
    enum {
        IdRole = Qt::UserRole
    };

private:
    void checkItemCount();

private:
    Ui::ObjectSelectorWidget *ui;
    bool m_isActive;
    int m_max;
    int m_min;
    QMap<QString, QListWidgetItem *> m_items;
};

#endif // OBJECTSELECTORWIDGET_H
