/*
    Copyright 2013 by Kurt Hindenburg <kurt.hindenburg@gmail.com>

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
#include "KeyboardTranslatorTest.h"

// KDE
#include <qtest.h>

using namespace Konsole;

Q_DECLARE_METATYPE(Qt::KeyboardModifiers)

void KeyboardTranslatorTest::testEntryTextWildcards_data()
{
    // Shift   = 1 + (1 << 0) = 2
    // Alt     = 1 + (1 << 2) = 3
    // Control = 1 + (1 << 4) = 5

    QTest::addColumn<QByteArray>("text");
    QTest::addColumn<QByteArray>("result");
    QTest::addColumn<bool>("wildcards");
    QTest::addColumn<Qt::KeyboardModifiers>("modifiers");

    QTest::newRow("Home no wildcards no modifiers")<< QByteArray("Home") << QByteArray("Home") << false << Qt::KeyboardModifiers(Qt::NoModifier);
    QTest::newRow("Home no wildcards Shift modifiers")<< QByteArray("Home") << QByteArray("Home") << false << Qt::KeyboardModifiers(Qt::ShiftModifier);
    QTest::newRow("Home no wildcards Alt modifiers")<< QByteArray("Home") << QByteArray("Home") << false << Qt::KeyboardModifiers(Qt::AltModifier);
    QTest::newRow("Home no wildcards Control modifiers")<< QByteArray("Home") << QByteArray("Home") << false << Qt::KeyboardModifiers(Qt::ControlModifier);

    QTest::newRow("Home yes wildcards no modifiers")<< QByteArray("Home") << QByteArray("Home") << true << Qt::KeyboardModifiers(Qt::NoModifier);
    QTest::newRow("Home yes wildcards Shift modifiers")<< QByteArray("Home") << QByteArray("Home") << true << Qt::KeyboardModifiers(Qt::ShiftModifier);
    QTest::newRow("Home yes wildcards Alt modifiers")<< QByteArray("Home") << QByteArray("Home") << true << Qt::KeyboardModifiers(Qt::AltModifier);
    QTest::newRow("Home yes wildcards Control modifiers")<< QByteArray("Home") << QByteArray("Home") << true << Qt::KeyboardModifiers(Qt::ControlModifier);


    // text, results: no mod, shift, alt, control
    QList<QByteArray> entry;
    entry << QByteArray("E*") << QByteArray("E1") << QByteArray("E2") << QByteArray("E3") << QByteArray("E5");
    QTest::newRow("E* yes wildcards no modifiers")<< entry[0] << entry[1] << true << Qt::KeyboardModifiers(Qt::NoModifier);
    QTest::newRow("E* yes wildcards Shift modifiers")<< entry[0] << entry[2] << true << Qt::KeyboardModifiers(Qt::ShiftModifier);
    QTest::newRow("E* yes wildcards Alt modifiers")<< entry[0] << entry[3] << true << Qt::KeyboardModifiers(Qt::AltModifier);
    QTest::newRow("E* yes wildcards Control modifiers")<< entry[0] << entry[4] << true << Qt::KeyboardModifiers(Qt::ControlModifier);

    // combinations
    entry.clear();;
    entry << QByteArray("E*") << QByteArray("E4") << QByteArray("E6") << QByteArray("E8") << QByteArray("E7");
    QTest::newRow("E* yes wildcards Shift+Alt modifiers")<< entry[0] << entry[1] << true << Qt::KeyboardModifiers(Qt::ShiftModifier | Qt::AltModifier);
    QTest::newRow("E* yes wildcards Shift+Control modifiers")<< entry[0] << entry[2] << true << Qt::KeyboardModifiers(Qt::ShiftModifier | Qt::ControlModifier);
    QTest::newRow("E* yes wildcards Shift+Alt+Control modifiers")<< entry[0] << entry[3] << true << Qt::KeyboardModifiers(Qt::ShiftModifier | Qt::AltModifier | Qt::ControlModifier);
    QTest::newRow("E* yes wildcards Alt+Control modifiers")<< entry[0] << entry[4] << true << Qt::KeyboardModifiers(Qt::AltModifier | Qt::ControlModifier);

    // text, results: no mod, shift, alt, control
    entry.clear();;
    entry << QByteArray("\033[24;*~") << QByteArray("\033[24;1~") << QByteArray("\033[24;2~") << QByteArray("\033[24;3~") << QByteArray("\033[24;5~");
    QTest::newRow("\033[24;*~ yes wildcards no modifiers")<< entry[0] << entry[1] << true << Qt::KeyboardModifiers(Qt::NoModifier);
    QTest::newRow("\033[24;*~ yes wildcards Shift modifiers")<< entry[0] << entry[2] << true << Qt::KeyboardModifiers(Qt::ShiftModifier);
    QTest::newRow("\033[24;*~ yes wildcards Alt modifiers")<< entry[0] << entry[3] << true << Qt::KeyboardModifiers(Qt::AltModifier);
    QTest::newRow("\033[24;*~ yes wildcards Control modifiers")<< entry[0] << entry[4] << true << Qt::KeyboardModifiers(Qt::ControlModifier);

    // combinations
    entry.clear();;
    entry << QByteArray("\033[24;*~") << QByteArray("\033[24;4~") << QByteArray("\033[24;6~") << QByteArray("\033[24;8~") << QByteArray("\033[24;7~");
    QTest::newRow("\033[24;*~ yes wildcards Shift+Alt modifiers")<< entry[0] << entry[1] << true << Qt::KeyboardModifiers(Qt::ShiftModifier | Qt::AltModifier);
    QTest::newRow("\033[24;*~ yes wildcards Shift+Control modifiers")<< entry[0] << entry[2] << true << Qt::KeyboardModifiers(Qt::ShiftModifier | Qt::ControlModifier);
    QTest::newRow("\033[24;*~ yes wildcards Shift+Alt+Control modifiers")<< entry[0] << entry[3] << true << Qt::KeyboardModifiers(Qt::ShiftModifier | Qt::AltModifier | Qt::ControlModifier);
    QTest::newRow("\033[24;*~ yes wildcards Alt+Control modifiers")<< entry[0] << entry[4] << true << Qt::KeyboardModifiers(Qt::AltModifier | Qt::ControlModifier);

}

void KeyboardTranslatorTest::testEntryTextWildcards()
{
    QFETCH(QByteArray, text);
    QFETCH(QByteArray, result);
    QFETCH(bool, wildcards);
    QFETCH(Qt::KeyboardModifiers, modifiers);

    KeyboardTranslator::Entry entry;
    entry.setText(text);

    QCOMPARE(entry.text(wildcards, modifiers), result);
}

QTEST_GUILESS_MAIN(KeyboardTranslatorTest)

