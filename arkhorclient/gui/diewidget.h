#ifndef DIEWIDGET_H
#define DIEWIDGET_H

#include <QWidget>

class DieWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DieWidget(QWidget *parent = 0);

    int dieValue() const { return m_val; }

    QSize sizeHint() const { return minimumSizeHint(); }
    QSize minimumSizeHint() const;
    QSizePolicy sizePolicy() const;

signals:

public slots:
    void setDieValue(int val);

protected:
    void paintEvent(QPaintEvent *event);

private:
    void updateImage();

private:
    int m_val;
    QPixmap m_cache;

};

#endif // DIEWIDGET_H
