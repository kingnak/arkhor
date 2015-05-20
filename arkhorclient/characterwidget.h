#ifndef CHARACTERWIDGET_H
#define CHARACTERWIDGET_H

#include <QWidget>
#include <characterdata.h>

namespace Ui {
class CharacterWidget;
}

class CharacterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CharacterWidget(QWidget *parent = 0);
    ~CharacterWidget();

public slots:
    void updateCharacterData(AH::Common::CharacterData data);

private:
    Ui::CharacterWidget *ui;
};

#endif // CHARACTERWIDGET_H
