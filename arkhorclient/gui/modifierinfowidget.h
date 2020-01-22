#ifndef MODIFIERINFOWIDGET_H
#define MODIFIERINFOWIDGET_H

#include <QScrollArea>
#include <propertyvaluedata.h>

class QLabel;

class ModifierInfoWidget : public QScrollArea
{
    Q_OBJECT
public:
    ModifierInfoWidget(QWidget *parent = nullptr);
    void addStretch();

public slots:
    void cleanModifiers();
    void setModifiers(const QList<AH::Common::PropertyModificationData> &mods, bool appendStretch = true);
    QLabel *appendModifier(const QString &id, const QString &name, const QString &display);

signals:
    void objectDescriptionRequested(QString id);
};

#endif // MODIFIERINFOWIDGET_H
