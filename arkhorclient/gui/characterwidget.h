#ifndef CHARACTERWIDGET_H
#define CHARACTERWIDGET_H

#include <QWidget>
#include <characterdata.h>
#include "asyncobjectreceiver.h"

namespace Ui {
class CharacterWidget;
}

class CharacterWidget : public QWidget, public AsyncObjectReceiver
{
    Q_OBJECT

public:
    explicit CharacterWidget(QWidget *parent = 0);
    ~CharacterWidget();

    virtual void objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc);

public slots:
    void displayCharacterData(const AH::Common::CharacterData *data);
    void updateCharacterData(const AH::Common::CharacterData *data);

private:
    QString m_curCharId;
    Ui::CharacterWidget *ui;
};

#endif // CHARACTERWIDGET_H
