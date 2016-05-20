#include "notificationformatter.h"
#include "game/game.h"
#include "game/player.h"
#include "character.h"
#include "monster.h"
#include "game/gameaction.h"
#include "game/actions/arkhamencounteryaction.h"
#include "game/actions/castspellaction.h"
#include "game/actions/chooseskillhelper.h"
#include "game/actions/closegateaction.h"
#include "game/actions/dierollaction.h"
#include "game/actions/entergateaction.h"
#include "game/actions/focusaction.h"
#include "game/actions/moveaction.h"
#include "game/actions/otherworldencounteryaction.h"
#include "game/actions/placeonfieldaction.h"
#include "game/actions/setoutaction.h"
#include "game/actions/undelayaction.h"
#include "game/actions/untapaction.h"

NotificationFormatter::NotificationFormatter()
    : m_curAction(NULL)
{
}

QString NotificationFormatter::formatActionStart(const GameAction *action, const QString &desc)
{
    if (m_curAction != NULL) {
        qWarning("Starting new action before finishing current");
        m_bufList.clear();
    }
    m_curAction = action;
    return getBasicActionString(action, desc, GameAction::Start);
}

QString NotificationFormatter::formatActionUpdate(const GameAction *action, const QString &desc)
{
    if (action != m_curAction) {
        qWarning("Getting update for wrong action");
        return QString::null;
    }
    m_bufList << desc;
    return QString::null;
}

QString NotificationFormatter::formatActionFinish(const GameAction *action, const QString &desc)
{
    Q_UNUSED(desc);
    QString ret;
    if (m_curAction != action) {
        qWarning("Finishing wrong action");
    } else {
        ret = getBasicActionString(action, desc, GameAction::Finish);
    }
    m_bufList.clear();
    m_curAction = NULL;
    return ret;
}

QString NotificationFormatter::formatActionExecute(const GameAction *action, const QString &desc)
{
    return getBasicActionString(action, desc, GameAction::Execute);
}

QString NotificationFormatter::formatSimple(const QString &str, const QString &desc)
{
    return formatBasic(str, desc);
}

QString NotificationFormatter::getBasicActionString(const GameAction *action, const QString &desc, GameAction::NotificationPart part)
{
    QString s = action->notificationString(part, desc);
    s = formatBasic(s, desc);
    if (part == GameAction::Finish) {
        s.replace("{B}", m_bufList.join(", "));
    }
    return s;
}

QString NotificationFormatter::formatBasic(QString str, const QString &desc)
{
    str.replace("{P}", gGame->context().player()->id());
    str.replace("{C}", gGame->context().player()->getCharacter()->investigator()->name());
    str.replace("{D}", desc);
    QString m = gGame->context().monster() ? gGame->context().monster()->name() : "";
    str.replace("{M}", m);
    QString f = gGame->context().player()->getCharacter()->field() ? gGame->context().player()->getCharacter()->field()->name() : "";
    str.replace("{F}", f);
    return str;
}
