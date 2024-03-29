#include "gatescript.h"

GateScript::GateScript(Gate *g, QObject *parent) :
    QObject(parent), m_gate(g)
{
}

void GateScript::drawThrough(CharacterScript *c)
{
    m_gate->drawThrough(c);
}


bool GateScript::seal(CharacterScript *byChar)
{
    m_gate->seal(byChar);
    return true;
}

bool GateScript::close(CharacterScript *byChar)
{
    m_gate->close(byChar);
    return true;
}
