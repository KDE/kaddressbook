/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KADDRESSBOOKUSERFEEDBACKPROVIDER_H
#define KADDRESSBOOKUSERFEEDBACKPROVIDER_H

#include <KUserFeedback/Provider>
#include "kaddressbook_export.h"

class KADDRESSBOOK_EXPORT KAddressBookUserFeedbackProvider : public KUserFeedback::Provider
{
    Q_OBJECT
public:
    explicit KAddressBookUserFeedbackProvider(QObject *parent = nullptr);
    ~KAddressBookUserFeedbackProvider();
};

#endif // KADDRESSBOOKUSERFEEDBACKPROVIDER_H
