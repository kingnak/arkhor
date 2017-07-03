#ifdef TEST_SCRIPT_BUILD

#include "scripttestdrawwidget.h"
#include <QtWidgets>

ScriptTestDrawWidget::ScriptTestDrawWidget(QWidget *parent) : QDialog(parent)
{
    QHBoxLayout *l = new QHBoxLayout(this);
    m_title = new QLabel;
    l->addWidget(m_title);

    m_cmb = new QComboBox;
    m_cmb->setEditable(true);
    m_cmb->setInsertPolicy(QComboBox::NoInsert);
    l->addWidget(m_cmb);

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
    m_title->setText("Draw " + title);
    qSort(lst);
    m_cmb->clear();
    m_cmb->addItems(lst);
    QCompleter *c = new QCompleter(lst, this);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    c->setFilterMode(Qt::MatchContains);

    m_cmb->setCompleter(c);

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

#endif
