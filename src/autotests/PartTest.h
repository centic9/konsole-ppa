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

#ifndef PARTTEST_H
#define PARTTEST_H

#include <QEventLoop>

#include <kde_terminal_interface.h>
#include <KParts/Part>

namespace Konsole
{

class PartTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testFd();

private:
    KParts::Part* createPart();
};

}

#endif // PARTTEST_H

