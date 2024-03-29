#ifndef ITEMSTACKER_H
#define ITEMSTACKER_H

#include <QWidget>
#include <QPixmap>
#include <QVariant>

class QPushButton;
class QToolButton;
class ItemStacker;
class QLabel;

class StackItem
{
public:
    StackItem() : m_stacker(NULL) {}
    virtual ~StackItem() {}
    StackItem(const QString &desc)
        : m_desc(desc), m_stacker(nullptr) {}
    StackItem(const QPixmap &pixmap, const QString &desc)
        : m_desc(desc), m_pixmap(pixmap), m_stacker(nullptr)  {}
    StackItem(const QPixmap &pixmap, const QString &desc, const QVariant &data)
        : m_desc(desc), m_pixmap(pixmap), m_data(data), m_stacker(nullptr)  {}

    virtual void wasAdded() {}
    virtual void wasRemoved() {}
    QString description() const { return m_desc; }
    QPixmap pixmap() const { return m_pixmap; }
    QVariant data() const { return m_data; }
    QString id() const { return m_id; }

    void setDescription(const QString &desc) { m_desc = desc; }
    void setPixmap(const QPixmap &pixmap);
    void setData(const QVariant &data) { m_data = data; }
    void setId(const QString &id) { m_id = id; }

    void setStacker(ItemStacker *s) { m_stacker = s; }

protected:
    QString m_desc;
    QPixmap m_pixmap;
    QVariant m_data;
    QString m_id;
    ItemStacker *m_stacker;
};

class ItemStacker : public QWidget
{
    Q_OBJECT
public:
    explicit ItemStacker(QWidget *parent = 0);
    ~ItemStacker() override;

    int count() const { return m_items.size(); }
    const StackItem *topItem() const;
    QList<StackItem *> items() const { return m_items; }

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    void setDisplayOffset(int off);
    void setDisplayCount(bool on = true);
    void setFont(const QFont &f);

    bool removeById(const QString &id);

signals:
    void itemAdded(const StackItem *item);
    void itemRemoved();
    void itemActivated(const StackItem *item);

public slots:
    void addItem(StackItem *itm);
    void removeAt(int idx);
    void clear();
    void setPicSize(QSize s);
    void itemUpdated(StackItem *itm);

protected:
    void resizeEvent(QResizeEvent *ev) override;

private slots:
    void itemClicked();
    void rotateNext();
    void rotatePrev();

private:
    int fixedWidth() const;
    void updateItemVisibility();
    void updateItemPixmaps();

private:
    static const int DISPLAY_COUNT = 3;
    int m_displayOffset;

    QToolButton *m_next;
    QToolButton *m_prev;
    QPushButton *m_displays[DISPLAY_COUNT];
    QList<StackItem *> m_items;
    int m_cur;
    QFont m_font;
    QLabel *m_lblCount;
};

#endif // ITEMSTACKER_H
