#ifdef TEST_SCRIPT_BUILD

#ifndef SCRIPTTESTCONFIGWIDGET_H
#define SCRIPTTESTCONFIGWIDGET_H

#include <QWidget>

class ScriptTestConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScriptTestConfigWidget(QWidget *parent = nullptr);

private:
    QWidget *buildAsk();
    QWidget *buildDebug();
};

#endif // SCRIPTTESTCONFIGWIDGET_H

#endif
