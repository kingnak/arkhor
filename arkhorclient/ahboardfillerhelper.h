#ifndef AHBOARDFILLERHELPER_H
#define AHBOARDFILLERHELPER_H

#include <fielddata.h>

class QGraphicsItem;
class AhBoardScene;

class AhBoardfillerHelper
{
public:
    AhBoardfillerHelper();

    void initBoard(AhBoardScene *scene, QGraphicsItem *parent);

private:
    void initFields(AhBoardScene *scene, QGraphicsItem *parent);
    void initTerror(AhBoardScene *scene, QGraphicsItem *parent);

    AH::Common::FieldData::FieldID parseId(QString id);
    AH::Common::FieldData::FieldType parseType(QString type);
    QRect parseRect(QString r);
};

#endif // AHBOARDFILLERHELPER_H
