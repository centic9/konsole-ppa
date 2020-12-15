/*
    Copyright 2010 by Kurt Hindenburg <kurt.hindenburg@gmail.com>

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
#include "RenameTabDialog.h"

// Konsole
#include "ui_RenameTabDialog.h"
#include "Shortcut_p.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

using Konsole::RenameTabDialog;

RenameTabDialog::RenameTabDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(i18n("Rename Tab"));
    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    auto mainWidget = new QWidget(this);
    auto mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    mainLayout->addWidget(mainWidget);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setAutoDefault(true);
    okButton->setShortcut(Konsole::ACCEL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &RenameTabDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &RenameTabDialog::reject);
    mainLayout->addWidget(buttonBox);

    setWindowModality(Qt::WindowModal);

    _ui = new Ui::RenameTabDialog();
    _ui->setupUi(mainWidget);
}

RenameTabDialog::~RenameTabDialog()
{
    delete _ui;
}

void RenameTabDialog::focusTabTitleText()
{
    _ui->renameTabWidget->focusTabTitleText();
}

void RenameTabDialog::focusRemoteTabTitleText()
{
    _ui->renameTabWidget->focusRemoteTabTitleText();
}

void RenameTabDialog::setTabTitleText(const QString &text)
{
    _ui->renameTabWidget->setTabTitleText(text);
}

void RenameTabDialog::setRemoteTabTitleText(const QString &text)
{
    _ui->renameTabWidget->setRemoteTabTitleText(text);
}

QString RenameTabDialog::tabTitleText() const
{
    return _ui->renameTabWidget->tabTitleText();
}

QString RenameTabDialog::remoteTabTitleText() const
{
    return _ui->renameTabWidget->remoteTabTitleText();
}
