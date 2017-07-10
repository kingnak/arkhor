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
    ~MonsterFrontWidget();

    static QPixmap drawMonster(const AH::Common::MonsterData *m, QSize s = QSize(200,200));

public slots:
    void displayMonster(const AH::Common::MonsterData *m);

public:
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);

private:
    QPixmap m_cache;
};

//////////////////////////////////

class MonsterBackWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MonsterBackWidget(QWidget *parent = 0);
    ~MonsterBackWidget();

public slots:
    void displayMonster(const AH::Common::MonsterData *m, bool minimal = false);

public:
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);

private:
    QPixmap m_cache;
};

//////////////////////////////////

class MonsterWidget : public QWidget, public AsyncObjectReceiver
{
    Q_OBJECT

public:
    explicit MonsterWidget(QWidget *parent = 0);
    ~MonsterWidget();

    virtual void objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc);

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
