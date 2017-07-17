#ifndef MONSTERMOVEMENTWIDGET_H
#define MONSTERMOVEMENTWIDGET_H

#include <QWidget>
#include <monsterdata.h>

class QLabel;
class MonsterWidget;

class MonsterMovementWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MonsterMovementWidget(QWidget *parent = nullptr);

signals:
    void acknowledgeMovement();

public slots:
    void displayMonsterMovement(const AH::Common::MonsterData *m);

private:
    QLabel *m_monsterName;
    MonsterWidget *m_monster;
};

#endif // MONSTERMOVEMENTWIDGET_H
