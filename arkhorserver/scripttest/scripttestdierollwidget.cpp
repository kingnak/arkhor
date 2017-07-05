#ifdef TEST_SCRIPT_BUILD

#include "scripttestdierollwidget.h"
#include <QtWidgets>

ScriptTestDieRollWidget::ScriptTestDieRollWidget(QWidget *parent) : QDialog(parent)
{
    QHBoxLayout *l = new QHBoxLayout(this);
    l->addWidget(new QLabel("Die roll"));

    m_txt = new QLineEdit;
    m_txt->setValidator(new QRegExpValidator(QRegExp("[0-6]*")));
    l->addWidget(m_txt);

    QPushButton *b = new QPushButton("OK");
    b->setDefault(true);
    b->setAutoDefault(true);
    connect(b, &QPushButton::clicked, this, &QDialog::accept);
    l->addWidget(b);
    b = new QPushButton("Random");
    connect(b, &QPushButton::clicked, this, &QDialog::reject);
    l->addWidget(b);
}

QString ScriptTestDieRollWidget::roll(const QString &curVals)
{
    m_txt->setText(curVals);
    m_txt->setMaxLength(curVals.length());
    m_txt->selectAll();

    show();
    activateWindow();
    raise();

    // Run a separate event loop so other debug windows respond (non-modal, blocking)
    QEventLoop l;
    connect(this, &QDialog::accepted, &l, &QEventLoop::quit);
    connect(this, &QDialog::rejected, &l, &QEventLoop::quit);
    l.exec();

    if (result() == QDialog::Accepted) {
        return m_txt->text();
    }
    return QString::null;
}

#endif
