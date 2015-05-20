#ifndef CHARACTERSCRIPT_H
#define CHARACTERSCRIPT_H

#include <QObject>
#include "character.h"

class CharacterScript : public QObject, public Character
{
    Q_OBJECT
public:
    explicit CharacterScript(Investigator *i, QObject *parent = 0);

signals:

public slots:

};

#endif // CHARACTERSCRIPT_H
