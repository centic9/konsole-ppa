/*
    Copyright 2007-2008 by Robert Knight <robertknight@gmail.com>
    Copyright 2012 by Jekyll Wu <adaptee@gmail.com>

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
#include "HistorySizeWidget.h"

// Qt
#include <QButtonGroup>
#include <QAbstractButton>

#include <KLocalizedString>

// Konsole
#include "ui_HistorySizeWidget.h"

using namespace Konsole;

HistorySizeWidget::HistorySizeWidget(QWidget *parent) :
    QWidget(parent)
{
    _ui = new Ui::HistorySizeWidget();
    _ui->setupUi(this);

    _ui->fixedSizeWarningWidget->setVisible(false);
    _ui->fixedSizeWarningWidget->setWordWrap(true);
    _ui->fixedSizeWarningWidget->setCloseButtonVisible(false);
    _ui->fixedSizeWarningWidget->setMessageType(KMessageWidget::Information);
    _ui->fixedSizeWarningWidget->setText(i18nc("@info:status",
                                               "When using this option, the scrollback data will be saved "
                                               "to RAM.  If you choose a huge value, your system may run out "
                                               "of free RAM and cause serious issues with your system."));

    _ui->unlimitedWarningWidget->setVisible(false);
    _ui->unlimitedWarningWidget->setWordWrap(true);
    _ui->unlimitedWarningWidget->setCloseButtonVisible(false);
    _ui->unlimitedWarningWidget->setMessageType(KMessageWidget::Information);
    _ui->unlimitedWarningWidget->setText(i18nc("@info:status",
                                               "When using this option, the scrollback data will be written "
                                               "unencrypted to temporary files. Those temporary files will be "
                                               "deleted automatically when Konsole is closed in a normal manner.\n"
                                               "Use Settings->Configure Konsole->File Location to select the "
                                               "location of the temporary files."));

    // focus and select the spinner automatically when appropriate
    _ui->fixedSizeHistoryButton->setFocusProxy(_ui->historyLineSpinner);
    connect(_ui->fixedSizeHistoryButton, &QRadioButton::clicked,
            _ui->historyLineSpinner,
            &KPluralHandlingSpinBox::selectAll);

    auto modeGroup = new QButtonGroup(this);
    modeGroup->addButton(_ui->noHistoryButton);
    modeGroup->addButton(_ui->fixedSizeHistoryButton);
    modeGroup->addButton(_ui->unlimitedHistoryButton);
    connect(modeGroup,
            static_cast<void (QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonClicked),
            this, &Konsole::HistorySizeWidget::buttonClicked);

    _ui->historyLineSpinner->setSuffix(ki18ncp("@label:textbox Unit of scrollback", " line", " lines"));
    this->setLineCount(HistorySizeWidget::DefaultLineCount);

    connect(_ui->historyLineSpinner,
            static_cast<void (KPluralHandlingSpinBox::*)(int)>(&KPluralHandlingSpinBox::valueChanged),
            this, &Konsole::HistorySizeWidget::historySizeChanged);
}

HistorySizeWidget::~HistorySizeWidget()
{
    delete _ui;
}

void HistorySizeWidget::buttonClicked(QAbstractButton *)
{
    Enum::HistoryModeEnum selectedMode = mode();
    _ui->fixedSizeWarningWidget->setVisible(Enum::FixedSizeHistory == selectedMode);
    _ui->unlimitedWarningWidget->setVisible(Enum::UnlimitedHistory == selectedMode);
    emit historyModeChanged(selectedMode);
}

void HistorySizeWidget::setMode(Enum::HistoryModeEnum aMode)
{
    if (aMode == Enum::NoHistory) {
        _ui->noHistoryButton->setChecked(true);
    } else if (aMode == Enum::FixedSizeHistory) {
        _ui->fixedSizeHistoryButton->setChecked(true);
    } else if (aMode == Enum::UnlimitedHistory) {
        _ui->unlimitedHistoryButton->setChecked(true);
    }
    _ui->fixedSizeWarningWidget->setVisible(Enum::FixedSizeHistory == aMode);
    _ui->unlimitedWarningWidget->setVisible(Enum::UnlimitedHistory == aMode);
}

Enum::HistoryModeEnum HistorySizeWidget::mode() const
{
    if (_ui->noHistoryButton->isChecked()) {
        return Enum::NoHistory;
    } else if (_ui->fixedSizeHistoryButton->isChecked()) {
        return Enum::FixedSizeHistory;
    } else if (_ui->unlimitedHistoryButton->isChecked()) {
        return Enum::UnlimitedHistory;
    }

    Q_ASSERT(false);
    return Enum::NoHistory;
}

void HistorySizeWidget::setLineCount(int lines)
{
    _ui->historyLineSpinner->setValue(lines);
    _ui->historyLineSpinner->setSingleStep(lines / 10);
}

int HistorySizeWidget::lineCount() const
{
    return _ui->historyLineSpinner->value();
}
