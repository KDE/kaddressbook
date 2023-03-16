/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KCModule>

#include "kcmutils_version.h"
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
#if KCMUTILS_VERSION < QT_VERSION_CHECK(5, 240, 0)
    explicit KCMKaddressbookUserFeedBackConfig(QWidget *parent, const QVariantList &args);
#else
    explicit KCMKaddressbookUserFeedBackConfig(QObject *parent, const KPluginMetaData &data, const QVariantList &args);
#endif
    void load() override;
    void save() override;
    void defaults() override;

private:
    KUserFeedback::FeedbackConfigWidget *const mUserFeedbackWidget;
};
}
