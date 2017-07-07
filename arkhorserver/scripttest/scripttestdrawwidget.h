#ifdef TEST_SCRIPT_BUILD

#ifndef SCRIPTTESTDRAWWIDGET_H
#define SCRIPTTESTDRAWWIDGET_H

#include <QDialog>

class QComboBox;
class QPushButton;
class QLabel;
class QHBoxLayout;

class ScriptTestDrawWidget : public QDialog
{
    Q_OBJECT
public:
    explicit ScriptTestDrawWidget(QWidget *parent = nullptr);

	virtual QString askDraw(const QString &title, QStringList lst);
	virtual QString moreData() { return QString::null; }
protected:
	ScriptTestDrawWidget(QWidget *parent, bool);
	void setupUi(QHBoxLayout *l);
    void setupButtons(QHBoxLayout *l);
	void prepareShow(const QString &title, QStringList lst);
	QString doShow();

protected:
    QLabel *m_title;
    QComboBox *m_cmb;
    QPushButton *m_rnd;
};

class ScriptTestDrawMonsterWidget : public ScriptTestDrawWidget
{
	Q_OBJECT
public:
	explicit ScriptTestDrawMonsterWidget(QWidget *parent = nullptr);

	QString askDraw(const QString&, QStringList monsters) override;
    QString moreData() override;

private:
	QComboBox *m_dim;
};

class ScriptTestDrawMythosWidget : public ScriptTestDrawWidget
{
    Q_OBJECT
public:
    explicit ScriptTestDrawMythosWidget(QWidget *parent = nullptr);

    QString askDraw(const QString &title, QStringList lst) override;
    QString moreData() override;

private:
    QComboBox *m_clue;
    QComboBox *m_gate;
    quint32 m_black;
    quint32 m_white;
};

class ScriptTestDrawGateWidget : public ScriptTestDrawWidget
{
    Q_OBJECT
public:
    explicit ScriptTestDrawGateWidget(QWidget *parent = nullptr);

    QString askDraw() { return askDraw("",QStringList()); }
    QString askDraw(const QString &title, QStringList lst) override;
    QString moreData() override;

private:
    quint32 m_dim;
    qint32 m_adj;
};

#endif // SCRIPTTESTDRAWWIDGET_H

#endif
