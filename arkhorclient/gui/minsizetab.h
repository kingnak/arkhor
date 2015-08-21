#ifndef MINSIZETAB_H
#define MINSIZETAB_H

#include <QWidget>

class QTabBar;
class QTabWidget;

class MinSizeStack : public QWidget {
    Q_OBJECT
public:
    MinSizeStack(QWidget *p = NULL);

    void setOrientation(Qt::Orientation orient);

    void addWidget(QWidget *w);
    int count() const;
    const QWidget *currentWidget() const;
    QSize sizeHint() const;

    int indexOf(QWidget *w) const;

    void setCurrentIndex(int idx);
    void setCurrentWidget(QWidget *w);

signals:
    //void currentIndexChanged(int idx);

private:
    void showCurrentWidget();

private:
    QList<QWidget *> m_wgts;
    int m_cur;
};

////////////////////

class MinSizeTab : public QWidget
{
    Q_OBJECT
public:
    explicit MinSizeTab(QWidget *parent = 0);

    void buildFromTab(QTabWidget *tab);

    void setOrientation(Qt::Orientation orient);

signals:

public slots:
    void setCurrentWidget(QWidget *w);
    void setCurrentIndex(int idx);

private:
    QTabBar *m_bar;
    MinSizeStack *m_stack;
};

#endif // MINSIZETAB_H
