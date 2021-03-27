/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <PimCommon/AbstractGenericPlugin>

#include "kaddressbook_importexport_export.h"

namespace KAddressBookImportExport
{
/** Import/export plugin. */
class KADDRESSBOOK_IMPORTEXPORT_EXPORT Plugin : public PimCommon::AbstractGenericPlugin
{
    Q_OBJECT
public:
    explicit Plugin(QObject *parent = nullptr);
    ~Plugin() override;
};
}

