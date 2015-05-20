#include "optionchooser.h"
#include "ui_optionchooser.h"
#include <QtGui>
#include "flowlayout.h"

using namespace AH::Common;

static const char *OPTION_DESCRIPTION_PROPERTY = "DESCRIPTION";
static const char *OPTION_ID_PROPERTY = "ID";

OptionChooser::OptionChooser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OptionChooser)
{
    ui->setupUi(this);
}

OptionChooser::~OptionChooser()
{
    delete ui;
}

void OptionChooser::setOptions(QList<AH::Common::GameOptionData> opts)
{
    cleanupOptions();

    FlowLayout *l = new FlowLayout(ui->wgtOptionsList);
    foreach (GameOptionData o, opts) {
        QPushButton *b = new QPushButton(o.name());
        b->setProperty(OPTION_DESCRIPTION_PROPERTY, o.description());
        b->setProperty(OPTION_ID_PROPERTY, o.id());
        connect(b, SIGNAL(clicked()), this, SLOT(showOption()));
        l->addWidget(b);
    }

    //ui->wgtOptionsList->setLayout(l);
}

void OptionChooser::cleanupOptions()
{
    QLayout *l = ui->wgtOptionsList->layout();
    if (l) {
        QLayoutItem *child;
        while ((child = l->takeAt(0)) != 0) {
            delete child;
        }
        ui->wgtOptionsList->setLayout(NULL);
        delete l;
    }
    foreach (QWidget *w, ui->wgtOptionsList->findChildren<QWidget*>()) {
        delete w;
    }

    ui->lblOptionDescription->setText("");
    ui->btnOptionActivate->setEnabled(false);
}

void OptionChooser::showOption()
{
    ui->lblOptionDescription->setText(sender()->property(OPTION_DESCRIPTION_PROPERTY).toString());
    ui->btnOptionActivate->setProperty(OPTION_ID_PROPERTY, sender()->property(OPTION_ID_PROPERTY));
    ui->btnOptionActivate->setEnabled(true);
}

void OptionChooser::on_btnOptionActivate_clicked()
{
    QString id = ui->btnOptionActivate->property(OPTION_ID_PROPERTY).toString();
    cleanupOptions();
    emit optionChosen(id);
}
