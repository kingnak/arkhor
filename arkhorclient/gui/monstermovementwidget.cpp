#include "monstermovementwidget.h"
#include <QtWidgets>
#include "monsterwidget.h"

MonsterMovementWidget::MonsterMovementWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *v = new QVBoxLayout(this);

    m_monsterName = new QLabel;
    QPushButton *ok = new QPushButton("&Ok");
    connect(ok, &QPushButton::clicked, [this](){
        emit this->acknowledgeMovement();
        this->displayMonsterMovement(nullptr);
    });

    QHBoxLayout *l = new QHBoxLayout;
    l->addWidget(m_monsterName, 1);
    l->addWidget(ok);

    v->addLayout(l);

    m_monster = new MonsterWidget;
    v->addWidget(m_monster);

    m_monster->displayMonster(nullptr);
}

void MonsterMovementWidget::displayMonsterMovement(const AH::Common::MonsterData *m)
{
    m_monster->displayMonster(m);
    if (m) {
        m_monsterName->setText(QString("%1 moves").arg(m->name()));
    } else {
        m_monsterName->setText("");
    }
}
