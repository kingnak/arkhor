#ifndef WEAPONCHOOSER_H
#define WEAPONCHOOSER_H

#include <QWidget>

#include <gameobjectdata.h>
#include <propertyvaluedata.h>

namespace Ui {
class WeaponChooser;
}

class WeaponChooser : public QWidget
{
    Q_OBJECT

public:
    explicit WeaponChooser(QWidget *parent = 0);
    ~WeaponChooser() override;

public slots:
    void chooseWeapons(const QList<AH::Common::GameObjectData> &weapons, const AH::Common::ModifiedPropertyValueData &hands);

signals:
    void weaponsCanceled();
    void weaponsSelected(QStringList weaponIds);
    void itemInfoRequested(const QString &itemId);

private slots:
    void updateHandCount();
    void on_btnOk_clicked();
    void on_btnCancel_clicked();
    void on_lstWeapons_currentRowChanged(int row);

private:
    void cleanWeapons();

    enum {
        IDRole = Qt::UserRole,
        HandsRole
    };

private:
    Ui::WeaponChooser *ui;

    int m_totHands;
};

#endif // WEAPONCHOOSER_H
