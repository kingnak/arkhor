#ifndef CHARSHORTINFOWIDGET_H
#define CHARSHORTINFOWIDGET_H

#include <QWidget>

#include <characterdata.h>

namespace Ui {
class CharShortInfoWidget;
}

class CharShortInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CharShortInfoWidget(QWidget *parent = 0);
    ~CharShortInfoWidget();

public slots:
    void displayCharacter(const AH::Common::CharacterData *chr);

private slots:
    void requestCharacterDetail(const QString &id);

signals:
    void characterDetailRequested(const QString &id);

private:
    Ui::CharShortInfoWidget *ui;
};

#endif // CHARSHORTINFOWIDGET_H
