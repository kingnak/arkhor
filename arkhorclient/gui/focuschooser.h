#ifndef FOCUSCHOOSER_H
#define FOCUSCHOOSER_H

#include <QWidget>
#include <QList>
#include <attributesliderdata.h>

namespace Ui {
class FocusChooser;
}

class FocusChooser : public QWidget
{
    Q_OBJECT

public:
    explicit FocusChooser(QWidget *parent = 0);
    ~FocusChooser();

    QList<int> getChosenFocusSettings();

public slots:
    void chooseFocus(QList<AH::Common::AttributeSliderData> sliders, int totAmount);

signals:
    void focusConfirmed(QList<int> diffs);

private slots:
    void on_btnOk_clicked();
    void focusChange();

private:
    void cleanWidget(QWidget *w);
    void setupWidget(QWidget *w, const AH::Common::AttributeSliderData &data);
    int getDiffValue(QWidget *w);
    QList<int> getAllDiffValues();

private:
    Ui::FocusChooser *ui;
    int m_totFocus;
};

#endif // FOCUSCHOOSER_H
