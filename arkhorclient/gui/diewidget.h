#ifndef DIEWIDGET_H
#define DIEWIDGET_H

#include <QWidget>

class DieWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DieWidget(QWidget *parent = 0);

    int dieValue() const { return m_val; }

    QSize sizeHint() const override { return minimumSizeHint(); }
    QSize minimumSizeHint() const override;

signals:

public slots:
    void setDieValue(int val);
    void setSuccess(bool success);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void updateImage();

private:
    int m_val;
    bool m_success;
    QPixmap m_cache;

};

#endif // DIEWIDGET_H
