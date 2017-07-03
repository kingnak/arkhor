#ifdef TEST_SCRIPT_BUILD

#ifndef SCRIPTTESTDRAWWIDGET_H
#define SCRIPTTESTDRAWWIDGET_H

#include <QDialog>

class QComboBox;
class QPushButton;
class QLabel;

class ScriptTestDrawWidget : public QDialog
{
    Q_OBJECT
public:
    explicit ScriptTestDrawWidget(QWidget *parent = nullptr);

    QString askDraw(const QString &title, QStringList lst);

private:
    QLabel *m_title;
    QComboBox *m_cmb;
    QPushButton *m_rnd;
};

#endif // SCRIPTTESTDRAWWIDGET_H

#endif
