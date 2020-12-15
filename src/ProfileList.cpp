/*
    Copyright 2006-2008 by Robert Knight <robertknight@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301  USA.
*/

// Own
#include "ProfileList.h"

// Qt
#include <QAction>
#include <QActionGroup>

// KDE
#include <KLocalizedString>

// Konsole
#include "ProfileManager.h"

using Konsole::Profile;
using Konsole::ProfileList;

ProfileList::ProfileList(bool addShortcuts , QObject* parent)
    : QObject(parent)
    , _addShortcuts(addShortcuts)
    , _emptyListAction(nullptr)
{
    // construct the list of favorite profiles
    _group = new QActionGroup(this);

    // Even when there are no favorite profiles, allow user to
    // create new tabs using the default profile from the menu
    _emptyListAction = new QAction(i18n("Default profile"), _group);

    // TODO - Handle re-sorts when user changes profile names
    ProfileManager* manager = ProfileManager::instance();
    QList<Profile::Ptr> favoriteProfiles = manager->sortedFavorites();

    foreach(const Profile::Ptr& profile, favoriteProfiles) {
        favoriteChanged(profile, true);
    }

    connect(_group, &QActionGroup::triggered, this, &Konsole::ProfileList::triggered);

    // listen for future changes to the profiles
    connect(manager, &Konsole::ProfileManager::favoriteStatusChanged, this, &Konsole::ProfileList::favoriteChanged);
    connect(manager, &Konsole::ProfileManager::shortcutChanged, this, &Konsole::ProfileList::shortcutChanged);
    connect(manager, &Konsole::ProfileManager::profileChanged, this, &Konsole::ProfileList::profileChanged);
}
void ProfileList::updateEmptyAction()
{
    Q_ASSERT(_group);
    Q_ASSERT(_emptyListAction);

    // show this action only when it is the only action in the group
    const bool showEmptyAction = (_group->actions().count() == 1);

    if (showEmptyAction != _emptyListAction->isVisible())
        _emptyListAction->setVisible(showEmptyAction);
}
QAction* ProfileList::actionForProfile(Profile::Ptr profile) const
{
    foreach(QAction* action, _group->actions()) {
        if (action->data().value<Profile::Ptr>() == profile)
            return action;
    }
    return nullptr; // not found
}

void ProfileList::profileChanged(Profile::Ptr profile)
{
    QAction* action = actionForProfile(profile);
    if (action != nullptr)
        updateAction(action, profile);
}

void ProfileList::updateAction(QAction* action , Profile::Ptr profile)
{
    Q_ASSERT(action);
    Q_ASSERT(profile);

    action->setText(profile->name());
    action->setIcon(QIcon::fromTheme(profile->icon()));
}
void ProfileList::shortcutChanged(Profile::Ptr profile, const QKeySequence& sequence)
{
    if (!_addShortcuts)
        return;

    QAction* action = actionForProfile(profile);

    if (action != nullptr) {
        action->setShortcut(sequence);
    }
}
void ProfileList::syncWidgetActions(QWidget* widget, bool sync)
{
    if (!sync) {
        _registeredWidgets.remove(widget);
        return;
    }

    _registeredWidgets.insert(widget);

    const QList<QAction*> currentActions = widget->actions();
    foreach(QAction * currentAction, currentActions) {
        widget->removeAction(currentAction);
    }

    widget->addActions(_group->actions());
}
void ProfileList::favoriteChanged(Profile::Ptr profile, bool isFavorite)
{
    ProfileManager* manager = ProfileManager::instance();

    if (isFavorite) {
        auto action = new QAction(_group);
        action->setData(QVariant::fromValue(profile));

        if (_addShortcuts) {
            action->setShortcut(manager->shortcut(profile));
        }

        updateAction(action, profile);

        foreach(QWidget * widget, _registeredWidgets) {
            widget->addAction(action);
        }
        emit actionsChanged(_group->actions());
    } else {
        QAction* action = actionForProfile(profile);

        if (action != nullptr) {
            _group->removeAction(action);
            foreach(QWidget * widget, _registeredWidgets) {
                widget->removeAction(action);
            }
            emit actionsChanged(_group->actions());
        }
    }

    updateEmptyAction();
}
void ProfileList::triggered(QAction* action)
{
    emit profileSelected(action->data().value<Profile::Ptr>());
}

QList<QAction*> ProfileList::actions()
{
    return _group->actions();
}

