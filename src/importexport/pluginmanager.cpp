/*
   SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pluginmanager.h"
using namespace Qt::Literals::StringLiterals;

#include "libkaddressbookexportimport_debug.h"
#include "plugin.h"
#include <PimCommon/PluginUtil>

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
    TextAddonsWidgets::PluginUtilData pluginData;
    KPluginMetaData data;
    Plugin *plugin = nullptr;
    bool isEnabled = true;
};

Q_GLOBAL_STATIC(PluginManagerHolder, sInstance)

namespace
{
QString pluginVersion()
{
    return u"1.0"_s;
}
}

namespace KAddressBookImportExport
{
class PluginManagerPrivate
{
public:
    explicit PluginManagerPrivate(PluginManager *qq)
        : q(qq)
    {
        initializePlugins();
    }

    void loadPlugin(ImportExportInfo *item);
    [[nodiscard]] QList<Plugin *> pluginsList() const;
    [[nodiscard]] QList<TextAddonsWidgets::PluginUtilData> pluginDataList() const;
    bool initializePlugins();

    [[nodiscard]] QString configGroupName() const;
    [[nodiscard]] QString configPrefixSettingKey() const;
    Plugin *pluginFromIdentifier(const QString &id);

private:
    QList<ImportExportInfo> mPluginList;
    QList<TextAddonsWidgets::PluginUtilData> mPluginDataList;
    PluginManager *const q = nullptr;
};
}

bool PluginManagerPrivate::initializePlugins()
{
    const QList<KPluginMetaData> plugins = KPluginMetaData::findPlugins(u"pim6/kaddressbook/importexportplugin"_s);

    const TextAddonsWidgets::PluginUtil::PluginsStateList pair = PimCommon::PluginUtil::loadPluginSetting(configGroupName(), configPrefixSettingKey());

    QListIterator<KPluginMetaData> i(plugins);
    i.toBack();
    while (i.hasPrevious()) {
        ImportExportInfo info;
        const KPluginMetaData data = i.previous();

        // 1) get plugin data => name/description etc.
        info.pluginData = TextAddonsWidgets::PluginUtil::createPluginMetaData(data);
        // 2) look at if plugin is activated
        const bool isPluginActivated = TextAddonsWidgets::PluginUtil::isPluginActivated(pair.enabledPluginList,
                                                                                        pair.disabledPluginList,
                                                                                        info.pluginData.mEnableByDefault,
                                                                                        info.pluginData.mIdentifier);
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
    QList<ImportExportInfo>::iterator end(mPluginList.end());
    for (QList<ImportExportInfo>::iterator it = mPluginList.begin(); it != end; ++it) {
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

QList<Plugin *> PluginManagerPrivate::pluginsList() const
{
    QList<Plugin *> lst;
    QList<ImportExportInfo>::ConstIterator end(mPluginList.constEnd());
    for (QList<ImportExportInfo>::ConstIterator it = mPluginList.constBegin(); it != end; ++it) {
        if (auto plugin = (*it).plugin) {
            lst << plugin;
        }
    }
    return lst;
}

QList<TextAddonsWidgets::PluginUtilData> PluginManagerPrivate::pluginDataList() const
{
    return mPluginDataList;
}

QString PluginManagerPrivate::configGroupName() const
{
    return u"KAddressBookPluginImportExport"_s;
}

QString PluginManagerPrivate::configPrefixSettingKey() const
{
    return u"KAddressBookImportExportPlugin"_s;
}

Plugin *PluginManagerPrivate::pluginFromIdentifier(const QString &id)
{
    QList<ImportExportInfo>::ConstIterator end(mPluginList.constEnd());
    for (QList<ImportExportInfo>::ConstIterator it = mPluginList.constBegin(); it != end; ++it) {
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

QList<Plugin *> PluginManager::pluginsList() const
{
    return d->pluginsList();
}

QList<TextAddonsWidgets::PluginUtilData> PluginManager::pluginsDataList() const
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

#include "moc_pluginmanager.cpp"
