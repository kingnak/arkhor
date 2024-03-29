#ifndef MONSTERWIDGET_H
#define MONSTERWIDGET_H

#include <QWidget>
#include <monsterdata.h>
#include "asyncobjectreceiver.h"

namespace Ui {
class MonsterWidget;
}

class MonsterFrontWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MonsterFrontWidget(QWidget *parent = 0);
    ~MonsterFrontWidget() override;

    static QPixmap drawMonster(const AH::Common::MonsterData *m, double scale = 1.0, QSize s = QSize(200, 200));

public slots:
    void displayMonster(const AH::Common::MonsterData *m);

public:
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap m_cache;
};

//////////////////////////////////

class MonsterBackWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MonsterBackWidget(QWidget *parent = 0);
    ~MonsterBackWidget() override;

public slots:
    void displayMonster(const AH::Common::MonsterData *m, bool minimal = false);

public:
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap m_cache;
};

//////////////////////////////////

class MonsterWidget : public QWidget, public AsyncObjectReceiver
{
    Q_OBJECT

public:
    explicit MonsterWidget(QWidget *parent = 0);
    ~MonsterWidget() override;

    void objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc) override;

public slots:
    void displayMonster(const AH::Common::MonsterData *m);

    /*
    void showFront();
    void showBack();
    void showBoth();
    */

    static QColor getMovementTypeColor(AH::Common::MonsterData::MovementType type);

private:
    void updateMonster(const AH::Common::MonsterData *m);

private slots:
    void turn();

private:
    Ui::MonsterWidget *ui;
    QString m_curMonsterId;
};

#endif // MONSTERWIDGET_H
