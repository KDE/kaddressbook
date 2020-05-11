/*
   Copyright (C) 2016-2020 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "kaddressbookimportexportpluginmanager.h"
#include "kaddressbookimportexportplugin.h"
#include "libkaddressbookexportimport_debug.h"
#include <PimCommon/PluginUtil>

#include <QFileInfo>
#include <QSet>
#include <KPluginLoader>
#include <KPluginMetaData>
#include <KPluginFactory>

using namespace KAddressBookImportExport;

class KAddressBookImportExportPluginManagerPrivate
{
public:
    KAddressBookImportExportPluginManagerPrivate()
        : pluginManager(new KAddressBookImportExportPluginManager)
    {
    }

    ~KAddressBookImportExportPluginManagerPrivate()
    {
        delete pluginManager;
    }

    KAddressBookImportExportPluginManager *pluginManager = nullptr;
};

class KAddressBookImportExportInfo
{
public:
    KAddressBookImportExportInfo()
        : plugin(nullptr)
        , isEnabled(true)
    {
    }

    QString metaDataFileNameBaseName;
    QString metaDataFileName;
    PimCommon::PluginUtilData pluginData;
    KAddressBookImportExportPlugin *plugin = nullptr;
    bool isEnabled;
};

Q_GLOBAL_STATIC(KAddressBookImportExportPluginManagerPrivate, sInstance)

namespace {
QString pluginVersion()
{
    return QStringLiteral("1.0");
}
}

class KAddressBookImportExport::KAddressBookImportExportPrivate
{
public:
    KAddressBookImportExportPrivate(KAddressBookImportExportPluginManager *qq)
        : q(qq)
    {
        initializePlugins();
    }

    void loadPlugin(KAddressBookImportExportInfo *item);
    QVector<KAddressBookImportExportPlugin *> pluginsList() const;
    QVector<PimCommon::PluginUtilData> pluginDataList() const;
    bool initializePlugins();

    QString configGroupName() const;
    QString configPrefixSettingKey() const;
    KAddressBookImportExportPlugin *pluginFromIdentifier(const QString &id);
private:
    QVector<KAddressBookImportExportInfo> mPluginList;
    QVector<PimCommon::PluginUtilData> mPluginDataList;
    KAddressBookImportExportPluginManager *q = nullptr;
};

bool KAddressBookImportExportPrivate::initializePlugins()
{
    const QVector<KPluginMetaData> plugins = KPluginLoader::findPlugins(QStringLiteral("kaddressbook5/importexportplugin"));

    const QPair<QStringList, QStringList> pair = PimCommon::PluginUtil::loadPluginSetting(configGroupName(), configPrefixSettingKey());

    QVectorIterator<KPluginMetaData> i(plugins);
    i.toBack();
    QSet<QString> unique;
    while (i.hasPrevious()) {
        KAddressBookImportExportInfo info;
        const KPluginMetaData data = i.previous();

        //1) get plugin data => name/description etc.
        info.pluginData = PimCommon::PluginUtil::createPluginMetaData(data);
        //2) look at if plugin is activated
        const bool isPluginActivated = PimCommon::PluginUtil::isPluginActivated(pair.first, pair.second, info.pluginData.mEnableByDefault, info.pluginData.mIdentifier);
        info.isEnabled = isPluginActivated;
        info.metaDataFileNameBaseName = QFileInfo(data.fileName()).baseName();
        info.metaDataFileName = data.fileName();
        if (pluginVersion() == data.version()) {
            // only load plugins once, even if found multiple times!
            if (unique.contains(info.metaDataFileNameBaseName)) {
                continue;
            }
            info.plugin = nullptr;
            mPluginList.push_back(info);
            unique.insert(info.metaDataFileNameBaseName);
        } else {
            qCWarning(LIBKADDRESSBOOKIMPORTEXPORT_LOG) << "Plugin " << data.name() << " doesn't have correction plugin version. It will not be loaded.";
        }
    }
    QVector<KAddressBookImportExportInfo>::iterator end(mPluginList.end());
    for (QVector<KAddressBookImportExportInfo>::iterator it = mPluginList.begin(); it != end; ++it) {
        loadPlugin(&(*it));
    }
    return true;
}

void KAddressBookImportExportPrivate::loadPlugin(KAddressBookImportExportInfo *item)
{
    KPluginLoader pluginLoader(item->metaDataFileName);
    if (pluginLoader.factory()) {
        item->plugin = pluginLoader.factory()->create<KAddressBookImportExportPlugin>(q, QVariantList() << item->metaDataFileNameBaseName);
        item->plugin->setIsEnabled(item->isEnabled);
        item->pluginData.mHasConfigureDialog = item->plugin->hasConfigureDialog();
        mPluginDataList.append(item->pluginData);
    }
}

QVector<KAddressBookImportExportPlugin *> KAddressBookImportExportPrivate::pluginsList() const
{
    QVector<KAddressBookImportExportPlugin *> lst;
    QVector<KAddressBookImportExportInfo>::ConstIterator end(mPluginList.constEnd());
    for (QVector<KAddressBookImportExportInfo>::ConstIterator it = mPluginList.constBegin(); it != end; ++it) {
        if (auto plugin = (*it).plugin) {
            lst << plugin;
        }
    }
    return lst;
}

QVector<PimCommon::PluginUtilData> KAddressBookImportExportPrivate::pluginDataList() const
{
    return mPluginDataList;
}

QString KAddressBookImportExportPrivate::configGroupName() const
{
    return QStringLiteral("KAddressBookPluginImportExport");
}

QString KAddressBookImportExportPrivate::configPrefixSettingKey() const
{
    return QStringLiteral("KAddressBookImportExportPlugin");
}

KAddressBookImportExportPlugin *KAddressBookImportExportPrivate::pluginFromIdentifier(const QString &id)
{
    QVector<KAddressBookImportExportInfo>::ConstIterator end(mPluginList.constEnd());
    for (QVector<KAddressBookImportExportInfo>::ConstIterator it = mPluginList.constBegin(); it != end; ++it) {
        if ((*it).pluginData.mIdentifier == id) {
            return (*it).plugin;
        }
    }
    return {};
}

KAddressBookImportExportPluginManager::KAddressBookImportExportPluginManager(QObject *parent)
    : QObject(parent)
    , d(new KAddressBookImportExportPrivate(this))
{
}

KAddressBookImportExportPluginManager::~KAddressBookImportExportPluginManager()
{
    delete d;
}

KAddressBookImportExportPluginManager *KAddressBookImportExportPluginManager::self()
{
    return sInstance->pluginManager;
}

QVector<KAddressBookImportExportPlugin *> KAddressBookImportExportPluginManager::pluginsList() const
{
    return d->pluginsList();
}

QVector<PimCommon::PluginUtilData> KAddressBookImportExportPluginManager::pluginsDataList() const
{
    return d->pluginDataList();
}

QString KAddressBookImportExportPluginManager::configGroupName() const
{
    return d->configGroupName();
}

QString KAddressBookImportExportPluginManager::configPrefixSettingKey() const
{
    return d->configPrefixSettingKey();
}

KAddressBookImportExportPlugin *KAddressBookImportExportPluginManager::pluginFromIdentifier(const QString &id)
{
    return d->pluginFromIdentifier(id);
}
