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
    ~WeaponChooser();

public slots:
    void chooseWeapons(QList<AH::Common::GameObjectData> weapons, AH::Common::ModifiedPropertyValueData hands);

signals:
    void weaponsCanceled();
    void weaponsSelected(QStringList weaponIds);

private slots:
    void updateHandCount();
    void on_btnOk_clicked();
    void on_btnCancel_clicked();

private:
    void cleanWeapons();

private:
    Ui::WeaponChooser *ui;

    int m_totHands;
};

#endif // WEAPONCHOOSER_H
