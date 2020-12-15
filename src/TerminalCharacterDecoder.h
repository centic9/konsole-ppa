/*
    This file is part of Konsole, an X terminal.

    Copyright 2006-2008 by Robert Knight <robertknight@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301  USA.
*/

#ifndef TERMINAL_CHARACTER_DECODER_H
#define TERMINAL_CHARACTER_DECODER_H

// Qt
#include <QList>

// Konsole
#include "Character.h"
#include "konsoleprivate_export.h"

class QTextStream;

namespace Konsole {
/**
 * Base class for terminal character decoders
 *
 * The decoder converts lines of terminal characters which consist of a unicode character, foreground
 * and background colors and other appearance-related properties into text strings.
 *
 * Derived classes may produce either plain text with no other color or appearance information, or
 * they may produce text which incorporates these additional properties.
 */
class KONSOLEPRIVATE_EXPORT TerminalCharacterDecoder
{
public:
    virtual ~TerminalCharacterDecoder()
    {
    }

    /** Begin decoding characters.  The resulting text is appended to @p output. */
    virtual void begin(QTextStream *output) = 0;
    /** End decoding. */
    virtual void end() = 0;

    /**
     * Converts a line of terminal characters with associated properties into a text string
     * and writes the string into an output QTextStream.
     *
     * @param characters An array of characters of length @p count.
     * @param count The number of characters
     * @param properties Additional properties which affect all characters in the line
     */
    virtual void decodeLine(const Character * const characters, int count,
                            LineProperty properties) = 0;
};

/**
 * A terminal character decoder which produces plain text, ignoring colors and other appearance-related
 * properties of the original characters.
 */
class KONSOLEPRIVATE_EXPORT PlainTextDecoder : public TerminalCharacterDecoder
{
public:
    PlainTextDecoder();

    /**
     * Set whether leading whitespace at the end of lines should be included
     * in the output.
     * Defaults to true.
     */
    void setLeadingWhitespace(bool enable);
    /**
     * Returns whether leading whitespace at the end of lines is included
     * in the output.
     */
    bool leadingWhitespace() const;
    /**
     * Set whether trailing whitespace at the end of lines should be included
     * in the output.
     * Defaults to true.
     */
    void setTrailingWhitespace(bool enable);
    /**
     * Returns whether trailing whitespace at the end of lines is included
     * in the output.
     */
    bool trailingWhitespace() const;
    /**
     * Returns of character positions in the output stream
     * at which new lines where added.  Returns an empty if setTrackLinePositions() is false or if
     * the output device is not a string.
     */
    QList<int> linePositions() const;
    /** Enables recording of character positions at which new lines are added.  See linePositions() */
    void setRecordLinePositions(bool record);

    void begin(QTextStream *output) Q_DECL_OVERRIDE;
    void end() Q_DECL_OVERRIDE;

    void decodeLine(const Character * const characters, int count,
                    LineProperty properties) Q_DECL_OVERRIDE;

private:
    QTextStream *_output;
    bool _includeLeadingWhitespace;
    bool _includeTrailingWhitespace;

    bool _recordLinePositions;
    QList<int> _linePositions;
};

/**
 * A terminal character decoder which produces pretty HTML markup
 */
class KONSOLEPRIVATE_EXPORT HTMLDecoder : public TerminalCharacterDecoder
{
public:
    /**
     * Constructs an HTML decoder using a default black-on-white color scheme.
     */
    HTMLDecoder();

    /**
     * Sets the color table which the decoder uses to produce the HTML color codes in its
     * output
     */
    void setColorTable(const ColorEntry *table);

    void decodeLine(const Character * const characters, int count,
                    LineProperty properties) Q_DECL_OVERRIDE;

    void begin(QTextStream *output) Q_DECL_OVERRIDE;
    void end() Q_DECL_OVERRIDE;

private:
    void openSpan(QString &text, const QString &style);
    void closeSpan(QString &text);

    QTextStream *_output;
    const ColorEntry *_colorTable;
    bool _innerSpanOpen;
    RenditionFlags _lastRendition;
    CharacterColor _lastForeColor;
    CharacterColor _lastBackColor;
};
}

#endif
