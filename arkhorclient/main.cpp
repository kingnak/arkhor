
#include <QApplication>
#include "form.h"
#include "ahmaingui.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    Form f;
    f.show();
    //AhMainGui gui;
    //gui.show();
    app.exec();
}

