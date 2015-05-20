#ifndef AHBOARDSCENE_H
#define AHBOARDSCENE_H

#include <QGraphicsScene>

class AhBoardScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit AhBoardScene(QObject *parent = 0);

    void initBoard();

signals:
    void itemInfoRequested(QString id);

public slots:

};

#endif // AHBOARDSCENE_H
