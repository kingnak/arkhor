#ifndef STRINGCHOICESELECTORWIDGET_H
#define STRINGCHOICESELECTORWIDGET_H

#include <QWidget>
#include <choicedata.h>

class QLabel;

class StringChoiceSelectorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StringChoiceSelectorWidget(QWidget *parent = 0);

public slots:
    QString getSelectedChoiceId() const;

public slots:
    void displayChoices(const QString &desc, const QList<AH::Common::ChoiceData::OptionData> &opts);
    void clearChoices();

signals:
    void choiceOk(bool ok);
    void activateChoice();
    void objectInfoRequested(const QString &id);

private slots:
    void choiceSelected();

private:
    QList<AH::Common::ChoiceData::OptionData> m_options;
    int m_selIdx;

    QWidget *m_optionsWidget;
    QLabel *m_description;
    QLabel *m_display;

};

#endif // STRINGCHOICESELECTORWIDGET_H
