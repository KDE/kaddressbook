/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "kaddressbook_importexport_export.h"
#include <PimCommon/PluginUtil>
#include <QObject>

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

    Q_REQUIRED_RESULT QVector<Plugin *> pluginsList() const;

    Q_REQUIRED_RESULT QVector<PimCommon::PluginUtilData> pluginsDataList() const;

    Q_REQUIRED_RESULT QString configGroupName() const;
    Q_REQUIRED_RESULT QString configPrefixSettingKey() const;
    Plugin *pluginFromIdentifier(const QString &id);

private:
    QScopedPointer<PluginManagerPrivate> const d;
};
}

