#include "focuschooser.h"
#include "ui_focuschooser.h"
#include <QtWidgets>

static const char *FOCUS_DIFF_PROPERTY = "FOCUS_DIFF";

FocusChooser::FocusChooser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FocusChooser)
{
    ui->setupUi(this);
    ui->wgtSpeedSneak->setLayout(new QVBoxLayout);
    ui->wgtFightWill->setLayout(new QVBoxLayout);
    ui->wgtLoreLuck->setLayout(new QVBoxLayout);
}

FocusChooser::~FocusChooser()
{
    delete ui;
}

void FocusChooser::chooseFocus(QList<AH::Common::AttributeSliderData> sliders, int totAmount)
{
    cleanWidget(ui->wgtSpeedSneak);
    cleanWidget(ui->wgtFightWill);
    cleanWidget(ui->wgtLoreLuck);

    setupWidget(ui->wgtSpeedSneak, sliders.value(0));
    setupWidget(ui->wgtFightWill, sliders.value(1));
    setupWidget(ui->wgtLoreLuck, sliders.value(2));

    m_totFocus = totAmount;
    if (totAmount > 50 && false) {
        // Signalling free move
        ui->lblFocusRemaining->setVisible(false);
    } else {
        ui->lblFocusRemaining->setVisible(true);
        focusChange();
    }
}

void FocusChooser::on_btnOk_clicked()
{
    QList<int> diffs = getAllDiffValues();
    emit focusConfirmed(diffs);
}

QList<int> FocusChooser::getAllDiffValues()
{
    return QList<int>()
        << getDiffValue(ui->wgtSpeedSneak)
        << getDiffValue(ui->wgtFightWill)
        << getDiffValue(ui->wgtLoreLuck);
}

void FocusChooser::focusChange()
{
    QList<int> diffs = getAllDiffValues();
    int rem = m_totFocus;
    foreach (int d, diffs) {
        rem -= qAbs(d);
    }

    if (rem < 0) {
        ui->lblFocusRemaining->setStyleSheet("color: rgb(255,0,0,);");
    } else {
        ui->lblFocusRemaining->setStyleSheet("");
    }
    ui->lblFocusRemaining->setText(QString::number(rem));
}

void FocusChooser::cleanWidget(QWidget *w)
{
    QLayout *l = w->layout();
    if (l) {
        QLayoutItem *child;
        while ((child = l->takeAt(0)) != 0) {
            delete child;
        }
    }
    foreach (QWidget *c, w->findChildren<QWidget*>()) {
        delete c;
    }
}

void FocusChooser::setupWidget(QWidget *w, const AH::Common::AttributeSliderData &data)
{
    for (int i = 0; i < data.availableSettings().size(); ++i)
    {
        int diff = i-data.currentSettingPos();
        AH::Common::AttributePair p = data.availableSettings()[i];

        QRadioButton *btn = new QRadioButton(QString("%1 / %2").arg(p.first().value).arg(p.second().value));
        btn->setProperty(FOCUS_DIFF_PROPERTY, QVariant::fromValue(diff));
        w->layout()->addWidget(btn);

        if (i == data.currentSettingPos()) {
            btn->setChecked(true);
        }

        connect(btn, SIGNAL(toggled(bool)), this, SLOT(focusChange()));
    }
}

int FocusChooser::getDiffValue(QWidget *w)
{
    foreach (QRadioButton *btn, w->findChildren<QRadioButton*>()) {
        if (btn->isChecked()) {
            return btn->property(FOCUS_DIFF_PROPERTY).toInt();
        }
    }
    return 0;
}
