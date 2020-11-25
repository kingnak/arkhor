#ifndef FIELDINFOWIDGET_H
#define FIELDINFOWIDGET_H

#include <QWidget>
#include <gamefielddata.h>

namespace Ui {
class FieldInfoWidget;
}

class QScrollArea;

class FieldInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FieldInfoWidget(QWidget *parent = nullptr);
    ~FieldInfoWidget() override;

public slots:
    void clear();
    void displayField(const AH::Common::GameFieldData &fd);

signals:
    void objectDescriptionRequested(QString id);

private:
    void cleanList(QScrollArea *list);
    void addToList(QScrollArea *list, const QString &name, const QString &id);
    void addStretch(QScrollArea *list);

private:
    Ui::FieldInfoWidget *ui;
};

#endif // FIELDINFOWIDGET_H
