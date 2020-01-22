#ifndef MODIFIEDPROPERTYINFOWIDGET_H
#define MODIFIEDPROPERTYINFOWIDGET_H

#include <QWidget>
#include <propertyvaluedata.h>

namespace Ui {
class ModifiedPropertyInfoWidget;
}

class ModifiedPropertyInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ModifiedPropertyInfoWidget(QWidget *parent = nullptr);
    ~ModifiedPropertyInfoWidget();

public slots:
    void cleanValue();
    void displayPropertyValue(AH::Common::ModifiedPropertyValueData value);

private slots:
    void requestObjectInfo(QString id);

signals:
    void objectInfoRequested(QString id);

private:
    Ui::ModifiedPropertyInfoWidget *ui;
};

#endif // MODIFIEDPROPERTYINFOWIDGET_H
