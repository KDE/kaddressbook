/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "configuredialog.h"

#include <KConfigGroup>
#include <KSharedConfig>

namespace
{
static const char myConfigureDialogConfigGroupName[] = "ConfigureDialog";
}

ConfigureDialog::ConfigureDialog(QWidget *parent)
    : KCMultiDialog(parent)
{
}

ConfigureDialog::~ConfigureDialog()
{
    writeConfig();
}

void ConfigureDialog::showEvent(QShowEvent *ev)
{
    KPageDialog::showEvent(ev);
    readConfig();
}

void ConfigureDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigureDialogConfigGroupName));
    const QSize size = group.readEntry("Size", QSize(600, 400));
    if (size.isValid()) {
        resize(size);
    }
}

void ConfigureDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigureDialogConfigGroupName));
    group.writeEntry("Size", size());
    group.sync();
}
