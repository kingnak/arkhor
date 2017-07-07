#ifdef TEST_SCRIPT_BUILD

#include <ahglobal.h>
#include "scripttestdrawwidget.h"
#include <QtWidgets>

QString string4dim(AH::Dimension d)
{
	switch (d) {
	case AH::NoDimension: return "No Dimension";
	case AH::Dim_Square: return "Square";
	case AH::Dim_Circle: return "Circle";
	case AH::Dim_Moon: return "Moon";
	case AH::Dim_Slash: return "Slash";
	case AH::Dim_Star: return "Star";
	case AH::Dim_Diamond: return "Diamond";
	case AH::Dim_Hexagon: return "Hexagon";
	case AH::Dim_Plus: return "Plus";
	case AH::Dim_Triangle: return "Triangle";
	default: return "??? " + QString::number(int(d));
	}
}

void forDimensions(std::function<void(AH::Dimension,QString)> cb, bool withNone = true)
{
	if (withNone) {
		cb(AH::NoDimension, string4dim(AH::NoDimension));
	}
	for (int i = AH::Dim_Square; i <= AH::Dim_Triangle; i<<=1) {
		cb(static_cast<AH::Dimension>(i), string4dim(static_cast<AH::Dimension>(i)));
	}
}

ScriptTestDrawWidget::ScriptTestDrawWidget(QWidget *parent) : QDialog(parent)
{
	setupUi(new QHBoxLayout(this));
}

ScriptTestDrawWidget::ScriptTestDrawWidget(QWidget *parent, bool) : QDialog(parent)
{
}

void ScriptTestDrawWidget::setupUi(QHBoxLayout *l)
{
	m_title = new QLabel;
    l->addWidget(m_title);

    m_cmb = new QComboBox;
    m_cmb->setEditable(true);
    m_cmb->setInsertPolicy(QComboBox::NoInsert);
    l->addWidget(m_cmb, 1);

    QPushButton *b = new QPushButton("OK");
    b->setDefault(true);
    b->setAutoDefault(true);
    connect(b, &QPushButton::clicked, this, &QDialog::accept);
    l->addWidget(b);
    b = new QPushButton("Random");
    connect(b, &QPushButton::clicked, this, &QDialog::reject);
	l->addWidget(b);
}

QString ScriptTestDrawWidget::askDraw(const QString &title, QStringList lst)
{
	prepareShow(title, lst);
	return doShow();
}

void ScriptTestDrawWidget::prepareShow(const QString &title, QStringList lst)
{
    m_title->setText("Draw " + title);
    qSort(lst);
    m_cmb->clear();
    m_cmb->addItems(lst);

    QCompleter *c = new QCompleter(lst, this);
    c->setCaseSensitivity(Qt::CaseInsensitive);
	c->setFilterMode(Qt::MatchContains);
    m_cmb->setCompleter(c);

	m_cmb->lineEdit()->selectAll();
}

QString ScriptTestDrawWidget::doShow()
{
    show();
    activateWindow();
    raise();

    // Run a separate event loop so other debug windows respond (non-modal, blocking)
    QEventLoop l;
    connect(this, &QDialog::accepted, &l, &QEventLoop::quit);
    connect(this, &QDialog::rejected, &l, &QEventLoop::quit);
    l.exec();

    if (result() == QDialog::Accepted) {
        return m_cmb->currentText();
    }
    return QString::null;
}

/////////////////////////////////////

ScriptTestDrawMonsterWidget::ScriptTestDrawMonsterWidget(QWidget *parent)
	: ScriptTestDrawWidget(parent, true)
{
	QHBoxLayout *l = new QHBoxLayout;
	ScriptTestDrawWidget::setupUi(l);

	QVBoxLayout *v = new QVBoxLayout(this);
	v->addLayout(l);

	l = new QHBoxLayout;
	m_dim = new QComboBox;
	forDimensions([=](AH::Dimension d, QString s){
		m_dim->addItem(s, QVariant(int(d)));
	});

	l->addWidget(new QLabel("Dimension: "));
	l->addWidget(m_dim, 1);

	v->addLayout(l);
	v->addStretch(1);
}

QString ScriptTestDrawMonsterWidget::askDraw(const QString&, QStringList monsters)
{
	return ScriptTestDrawWidget::askDraw("Monster", monsters);
}

QString ScriptTestDrawMonsterWidget::moreData()
{
	int i = m_dim->currentData().toInt();
	return QString::number(i);
}

#endif
