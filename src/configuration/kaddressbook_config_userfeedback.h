/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KCModule>

#include <QVariant>
namespace KUserFeedback
{
class FeedbackConfigWidget;
}

namespace KAddressBook
{
class KCMKaddressbookUserFeedBackConfig : public KCModule
{
    Q_OBJECT
public:
    explicit KCMKaddressbookUserFeedBackConfig(QObject *parent, const KPluginMetaData &data);
    void load() override;
    void save() override;
    void defaults() override;

private:
    KUserFeedback::FeedbackConfigWidget *const mUserFeedbackWidget;
};
}
