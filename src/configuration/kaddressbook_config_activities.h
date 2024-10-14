/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KCModule>

#include <QVariant>
namespace KAddressBook
{
class KCMKaddressbookActivitiesConfig : public KCModule
{
    Q_OBJECT
public:
    explicit KCMKaddressbookActivitiesConfig(QObject *parent, const KPluginMetaData &data);
    void load() override;
    void save() override;
    void defaults() override;

private:
    void slotConfigChanged();
};
}
