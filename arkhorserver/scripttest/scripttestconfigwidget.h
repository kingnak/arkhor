#ifdef TEST_SCRIPT_BUILD

#ifndef SCRIPTTESTCONFIGWIDGET_H
#define SCRIPTTESTCONFIGWIDGET_H

#include <QWidget>

class QGroupBox;

class ScriptTestConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScriptTestConfigWidget(QWidget *parent = nullptr);

private:
    QWidget *buildAsk();
    QWidget *buildDebug();
    void toggleAll(QGroupBox *b);
    void activateAll(QGroupBox *b);
    void deactivateAll(QGroupBox *b);
};

#endif // SCRIPTTESTCONFIGWIDGET_H

#endif
