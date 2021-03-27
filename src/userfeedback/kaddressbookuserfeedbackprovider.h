/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "kaddressbook_export.h"
#include <KUserFeedback/Provider>

class KADDRESSBOOK_EXPORT KAddressBookUserFeedbackProvider : public KUserFeedback::Provider
{
    Q_OBJECT
public:
    explicit KAddressBookUserFeedbackProvider(QObject *parent = nullptr);
    ~KAddressBookUserFeedbackProvider() override;
};

