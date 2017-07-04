#ifdef TEST_SCRIPT_BUILD

#ifndef SCRIPTTESTDIEROLLER_H
#define SCRIPTTESTDIEROLLER_H

#include <QWidget>

class ScriptTestDieRollWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScriptTestDieRollWidget(QWidget *parent = nullptr);

    QList<quint32> roll(const QList<quint32> fixed, int toRoll);

signals:

public slots:
};

#endif // SCRIPTTESTDIEROLLER_H

#endif
