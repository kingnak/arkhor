#include "minsizetab.h"
#include <QtWidgets>

class TabFrame : public QWidget
{
public:
    TabFrame(QWidget *p = nullptr) : QWidget(p) {}
protected:
    void paintEvent(QPaintEvent *event) override {
        QStylePainter p(this);
        QStyleOptionTabWidgetFrameV2 opt;
        opt.initFrom(this);
        opt.lineWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth, nullptr, this);
        opt.rect = rect();

        adjustOptions(opt);

        p.drawPrimitive(QStyle::PE_FrameTabWidget, opt);
        Q_UNUSED(event);
    }

private:
    void adjustOptions(QStyleOptionTabWidgetFrameV2 &opt);
};

////////////////////////////////


MinSizeStack::MinSizeStack(QWidget *p) : QWidget(p), m_cur(0)
{
    setMinimumSize(QSize(0,0));
}

void MinSizeStack::setOrientation(Qt::Orientation orient)
{
    if (orient == Qt::Horizontal) new QHBoxLayout(this);
    else new QVBoxLayout(this);
}

void MinSizeStack::addWidget(QWidget *w)
{
    m_wgts << w;
    layout()->addWidget(w);
    showCurrentWidget();
}

int MinSizeStack::count() const
{
    return m_wgts.size();
}

const QWidget *MinSizeStack::currentWidget() const {
    return m_wgts[m_cur];
}

QSize MinSizeStack::sizeHint() const
{
    return currentWidget()->minimumSize().expandedTo(QSize(250,250));
}

int MinSizeStack::indexOf(QWidget *w) const
{
    return m_wgts.indexOf(w);
}

void MinSizeStack::setCurrentIndex(int idx)
{
    m_cur = idx;
    showCurrentWidget();
}

void MinSizeStack::setCurrentWidget(QWidget *w)
{
    setCurrentIndex(m_wgts.indexOf(w));
}

void MinSizeStack::showCurrentWidget()
{
    for (auto w : m_wgts)
        w->hide();
    m_wgts.at(m_cur)->show();
    updateGeometry();
}

////////////////////////

MinSizeTab::MinSizeTab(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *l = new QVBoxLayout(this);
    l->setContentsMargins(0,0,0,0);
    l->setSpacing(0);
    m_bar = new QTabBar;
    m_stack = new MinSizeStack;
    TabFrame *f = new TabFrame;
    QHBoxLayout *h = new QHBoxLayout(f);
    h->addWidget(m_stack);
    l->addWidget(m_bar);
    l->addWidget(f, 1);

    connect(m_bar, SIGNAL(currentChanged(int)), this, SLOT(setCurrentIndex(int)));
}

void MinSizeTab::buildFromTab(QTabWidget *tab)
{
    while (tab->count() > 0) {
        m_bar->addTab(tab->tabIcon(0), tab->tabText(0));
        QWidget *w = tab->widget(0);
        tab->removeTab(0);
        m_stack->addWidget(w);
    }
    tab->setVisible(false);
}

void MinSizeTab::setOrientation(Qt::Orientation orient)
{
    m_stack->setOrientation(orient);
}

void MinSizeTab::setCurrentWidget(QWidget *w)
{
    setCurrentIndex(m_stack->indexOf(w));
}

void MinSizeTab::setCurrentIndex(int idx)
{
    if (idx >= 0 && idx < m_stack->count()) {
        m_stack->setCurrentIndex(idx);
        m_bar->setCurrentIndex(idx);
    }
}


#ifdef Q_OS_WIN
#include <Windows.h>
static bool IsAppThemed()
{
    static bool tried = false;
    static bool isThemed = false;
    if (tried)
        return isThemed;

    tried = true;

    QVarLengthArray<wchar_t, MAX_PATH> fullPath;

    UINT retLen = ::GetSystemDirectory(fullPath.data(), MAX_PATH);
    if (retLen > MAX_PATH) {
        fullPath.resize(retLen);
        retLen = ::GetSystemDirectory(fullPath.data(), retLen);
    }
    // in some rare cases retLen might be 0
    QString dll = QString::fromWCharArray(fullPath.constData(), int(retLen));
    if (!dll.endsWith("\\")) dll += "\\";
    dll += "uxtheme.dll";

    HINSTANCE inst = ::LoadLibrary((const wchar_t *)dll.utf16());
    if (!inst)
        return false;

    using PtrIsAppThemed = bool (*)();
    PtrIsAppThemed pIsAppThemed = (PtrIsAppThemed) ::GetProcAddress(inst, "IsAppThemed");

    if (!pIsAppThemed)
        return false;

    isThemed = pIsAppThemed();
    return isThemed;
}

void TabFrame::adjustOptions(QStyleOptionTabWidgetFrameV2 &opt)
{
    if (IsAppThemed())
        opt.rect.adjust(0,-opt.lineWidth,2,0);
    else
        opt.rect.adjust(0,-opt.lineWidth,0,0);
}

#else
void TabFrame::adjustOptions(QStyleOptionTabWidgetFrameV2 &opt)
{
    opt.rect.adjust(0,-opt.lineWidth,0,0);
}
#endif
