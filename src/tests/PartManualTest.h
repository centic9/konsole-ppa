/*
    Copyright 2008 by Robert Knight <robertknight@gmail.com>

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

#ifndef PARTMANUALTEST_H
#define PARTMANUALTEST_H

#include <QEventLoop>

#include <kde_terminal_interface.h>
#include <KParts/Part>

class QKeyEvent;

namespace Konsole
{

class PartManualTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testShortcutOverride();

// marked as protected so they are not treated as test cases
protected Q_SLOTS:
    void overrideShortcut(QKeyEvent* event, bool& override);
    void shortcutTriggered();

private:
    KParts::Part* createPart();

    // variables for testShortcutOverride()
    bool _shortcutTriggered;
    bool _overrideCalled;
    bool _override;
    QEventLoop* _shortcutEventLoop;
};

}

#endif // PARTMANUALTEST_H

