/*
   SPDX-FileCopyrightText: 2016-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pluginmanager.h"
#include "libkaddressbookexportimport_debug.h"
#include "plugin.h"

#include <KPluginFactory>
#include <KPluginMetaData>
#include <QFileInfo>

using namespace KAddressBookImportExport;

class PluginManagerHolder
{
public:
    PluginManagerHolder()
        : pluginManager(new PluginManager)
    {
    }

    const QScopedPointer<PluginManager> pluginManager;
};

class ImportExportInfo
{
public:
    QString metaDataFileNameBaseName;
    PimCommon::PluginUtilData pluginData;
    KPluginMetaData data;
    Plugin *plugin = nullptr;
    bool isEnabled = true;
};

Q_GLOBAL_STATIC(PluginManagerHolder, sInstance)

namespace
{
QString pluginVersion()
{
    return QStringLiteral("1.0");
}
}

namespace KAddressBookImportExport
{
class PluginManagerPrivate
{
public:
    PluginManagerPrivate(PluginManager *qq)
        : q(qq)
    {
        initializePlugins();
    }

    void loadPlugin(ImportExportInfo *item);
    Q_REQUIRED_RESULT QVector<Plugin *> pluginsList() const;
    Q_REQUIRED_RESULT QVector<PimCommon::PluginUtilData> pluginDataList() const;
    bool initializePlugins();

    Q_REQUIRED_RESULT QString configGroupName() const;
    Q_REQUIRED_RESULT QString configPrefixSettingKey() const;
    Plugin *pluginFromIdentifier(const QString &id);

private:
    QVector<ImportExportInfo> mPluginList;
    QVector<PimCommon::PluginUtilData> mPluginDataList;
    PluginManager *const q = nullptr;
};
}

bool PluginManagerPrivate::initializePlugins()
{
    const QVector<KPluginMetaData> plugins = KPluginMetaData::findPlugins(QStringLiteral("pim6/kaddressbook/importexportplugin"));

    const QPair<QStringList, QStringList> pair = PimCommon::PluginUtil::loadPluginSetting(configGroupName(), configPrefixSettingKey());

    QListIterator<KPluginMetaData> i(plugins);
    i.toBack();
    while (i.hasPrevious()) {
        ImportExportInfo info;
        const KPluginMetaData data = i.previous();

        // 1) get plugin data => name/description etc.
        info.pluginData = PimCommon::PluginUtil::createPluginMetaData(data);
        // 2) look at if plugin is activated
        const bool isPluginActivated =
            PimCommon::PluginUtil::isPluginActivated(pair.first, pair.second, info.pluginData.mEnableByDefault, info.pluginData.mIdentifier);
        info.isEnabled = isPluginActivated;
        info.metaDataFileNameBaseName = QFileInfo(data.fileName()).baseName();
        info.data = data;
        if (pluginVersion() == data.version()) {
            info.plugin = nullptr;
            mPluginList.push_back(info);
        } else {
            qCWarning(LIBKADDRESSBOOKIMPORTEXPORT_LOG) << "Plugin " << data.name() << " doesn't have correction plugin version. It will not be loaded.";
        }
    }
    QVector<ImportExportInfo>::iterator end(mPluginList.end());
    for (QVector<ImportExportInfo>::iterator it = mPluginList.begin(); it != end; ++it) {
        loadPlugin(&(*it));
    }
    return true;
}

void PluginManagerPrivate::loadPlugin(ImportExportInfo *item)
{
    if (auto plugin = KPluginFactory::instantiatePlugin<Plugin>(item->data, q, QVariantList() << item->metaDataFileNameBaseName).plugin) {
        item->plugin = plugin;
        item->plugin->setIsEnabled(item->isEnabled);
        item->pluginData.mHasConfigureDialog = item->plugin->hasConfigureDialog();
        mPluginDataList.append(item->pluginData);
    }
}

QVector<Plugin *> PluginManagerPrivate::pluginsList() const
{
    QVector<Plugin *> lst;
    QVector<ImportExportInfo>::ConstIterator end(mPluginList.constEnd());
    for (QVector<ImportExportInfo>::ConstIterator it = mPluginList.constBegin(); it != end; ++it) {
        if (auto plugin = (*it).plugin) {
            lst << plugin;
        }
    }
    return lst;
}

QVector<PimCommon::PluginUtilData> PluginManagerPrivate::pluginDataList() const
{
    return mPluginDataList;
}

QString PluginManagerPrivate::configGroupName() const
{
    return QStringLiteral("KAddressBookPluginImportExport");
}

QString PluginManagerPrivate::configPrefixSettingKey() const
{
    return QStringLiteral("KAddressBookImportExportPlugin");
}

Plugin *PluginManagerPrivate::pluginFromIdentifier(const QString &id)
{
    QVector<ImportExportInfo>::ConstIterator end(mPluginList.constEnd());
    for (QVector<ImportExportInfo>::ConstIterator it = mPluginList.constBegin(); it != end; ++it) {
        if ((*it).pluginData.mIdentifier == id) {
            return (*it).plugin;
        }
    }
    return {};
}

PluginManager::PluginManager(QObject *parent)
    : QObject(parent)
    , d(new PluginManagerPrivate(this))
{
}

PluginManager::~PluginManager() = default;

PluginManager *PluginManager::self()
{
    return sInstance->pluginManager.get();
}

QVector<Plugin *> PluginManager::pluginsList() const
{
    return d->pluginsList();
}

QVector<PimCommon::PluginUtilData> PluginManager::pluginsDataList() const
{
    return d->pluginDataList();
}

QString PluginManager::configGroupName() const
{
    return d->configGroupName();
}

QString PluginManager::configPrefixSettingKey() const
{
    return d->configPrefixSettingKey();
}

Plugin *PluginManager::pluginFromIdentifier(const QString &id)
{
    return d->pluginFromIdentifier(id);
}
