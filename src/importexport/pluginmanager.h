/*
   SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "kaddressbook_importexport_export.h"
#include <QObject>
#include <TextAddonsWidgets/PluginUtil>

namespace KAddressBookImportExport
{
class Plugin;
class PluginManagerPrivate;

/** Contact import/export plugin manager. */
class KADDRESSBOOK_IMPORTEXPORT_EXPORT PluginManager : public QObject
{
    Q_OBJECT
public:
    explicit PluginManager(QObject *parent = nullptr);
    ~PluginManager() override;

    static PluginManager *self();

    [[nodiscard]] QList<Plugin *> pluginsList() const;

    [[nodiscard]] QList<TextAddonsWidgets::PluginUtilData> pluginsDataList() const;

    [[nodiscard]] QString configGroupName() const;
    [[nodiscard]] QString configPrefixSettingKey() const;
    Plugin *pluginFromIdentifier(const QString &id);

private:
    QScopedPointer<PluginManagerPrivate> const d;
};
}
