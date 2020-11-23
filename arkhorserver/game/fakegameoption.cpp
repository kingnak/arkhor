#include "FakeGameOption.h"

FakeGameOption::FakeGameOption(const QString &id, const QString &name, const QString &description)
    : GameOption(nullptr, AH::ContinueType::CanContinue, AH::ChooseType::ChooseSupplemental)
    , fga(id, name, description)
{
    m_action = &fga;
}

FakeGameAction::FakeGameAction(const QString &id, const QString &name, const QString &description)
{
    m_id = id;
    m_name = name;
    m_description = description;
}
