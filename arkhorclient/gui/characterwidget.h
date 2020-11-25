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
    ~CharacterWidget() override;

    static QPixmap drawCharacterWithStats(const AH::Common::CharacterData *c, double scale, QSize s = QSize(200, 200));

    void objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc) override;

public slots:
    void displayCharacterData(const AH::Common::CharacterData *data);
    void updateCharacterData(const AH::Common::CharacterData *data);

private slots:
    void requestCharacterDetail(const QString &id);

signals:
    void characterDetailRequested(const QString &id);
    void requestCenterOnField(AH::Common::FieldData::FieldID id);

private:
    QString m_curCharId;
    Ui::CharacterWidget *ui;
};

#endif // CHARACTERWIDGET_H
