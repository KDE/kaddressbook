/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

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
    explicit KCMKaddressbookPluginsConfig(QWidget *parent, const QVariantList &args);
    void load() override;
    void save() override;
    void defaults() override;

private:
    void slotConfigChanged();
    PimCommon::ConfigurePluginsWidget *mConfigurePluginWidget = nullptr;
};
}
