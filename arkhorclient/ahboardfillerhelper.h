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
    double initScale(AhBoardScene *scene);
    void initFields(AhBoardScene *scene, QGraphicsItem *parent, double scale);
    void initTerror(AhBoardScene *scene, QGraphicsItem *parent, double scale);

    AH::Common::FieldData::FieldID parseId(const QString &id);
    AH::Common::FieldData::FieldType parseType(const QString &type);
    QRect parseRect(const QString &r, double scale);
};

#endif // AHBOARDFILLERHELPER_H
