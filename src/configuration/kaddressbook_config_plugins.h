/*
   SPDX-FileCopyrightText: 2016-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "kcmutils_version.h"
#include <KCModule>

#include <QVariant>
namespace PimCommon
{
class ConfigurePluginsWidget;
}
namespace KAddressBook
{
class KCMKaddressbookPluginsConfig : public KCModule
{
    Q_OBJECT
public:
#if KCMUTILS_VERSION < QT_VERSION_CHECK(5, 240, 0)
    explicit KCMKaddressbookPluginsConfig(QWidget *parent, const QVariantList &args);
#else
    explicit KCMKaddressbookPluginsConfig(QObject *parent, const KPluginMetaData &data, const QVariantList &args);
#endif
    void load() override;
    void save() override;
    void defaults() override;

private:
    void slotConfigChanged();
    PimCommon::ConfigurePluginsWidget *const mConfigurePluginWidget;
};
}
