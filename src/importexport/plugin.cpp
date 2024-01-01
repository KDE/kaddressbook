/*
   SPDX-FileCopyrightText: 2016-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "plugin.h"

using namespace KAddressBookImportExport;

Plugin::Plugin(QObject *parent)
    : PimCommon::AbstractGenericPlugin(parent)
{
}

Plugin::~Plugin() = default;

#include "moc_plugin.cpp"
