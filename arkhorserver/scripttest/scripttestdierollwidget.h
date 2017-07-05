#ifdef TEST_SCRIPT_BUILD

#ifndef SCRIPTTESTDIEROLLER_H
#define SCRIPTTESTDIEROLLER_H

#include <QDialog>

class QLineEdit;

class ScriptTestDieRollWidget : public QDialog
{
    Q_OBJECT
public:
    explicit ScriptTestDieRollWidget(QWidget *parent = nullptr);

    QString roll(const QString &curVals);

private:
    QLineEdit *m_txt;
};

#endif // SCRIPTTESTDIEROLLER_H

#endif
