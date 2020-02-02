#include "FakeGameOption.h"

FakeGameOption::FakeGameOption(QString id, QString name, QString description)
    : GameOption(nullptr, AH::ContinueType::CanContinue, AH::ChooseType::ChooseSupplemental)
    , fga(id, name, description)
{
    m_action = &fga;
}

FakeGameAction::FakeGameAction(QString id, QString name, QString description)
{
    m_id = id;
    m_name = name;
    m_description = description;
}
