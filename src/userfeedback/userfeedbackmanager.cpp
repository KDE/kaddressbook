/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "userfeedbackmanager.h"
using namespace Qt::Literals::StringLiterals;

#include "kaddressbookuserfeedbackprovider.h"

UserFeedBackManager::UserFeedBackManager(QObject *parent)
    : QObject(parent)
    , mUserFeedbackProvider(new KAddressBookUserFeedbackProvider(this))
{
}

UserFeedBackManager *UserFeedBackManager::self()
{
    static UserFeedBackManager s_self;
    return &s_self;
}

KUserFeedback::Provider *UserFeedBackManager::userFeedbackProvider() const
{
    return mUserFeedbackProvider;
}

#include "moc_userfeedbackmanager.cpp"
