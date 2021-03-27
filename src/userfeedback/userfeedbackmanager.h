/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "kaddressbook_export.h"
#include <QObject>
namespace KUserFeedback
{
class Provider;
}

class KADDRESSBOOK_EXPORT UserFeedBackManager : public QObject
{
    Q_OBJECT
public:
    explicit UserFeedBackManager(QObject *parent = nullptr);
    static UserFeedBackManager *self();

    KUserFeedback::Provider *userFeedbackProvider() const;

private:
    KUserFeedback::Provider *const mUserFeedbackProvider;
};

