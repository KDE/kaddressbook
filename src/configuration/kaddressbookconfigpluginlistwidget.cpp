/*
   SPDX-FileCopyrightText: 2016-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kaddressbookconfigpluginlistwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "../importexport/plugin.h"
#include "../importexport/pluginmanager.h"
#include "../plugininterface/kaddressbookplugininterface.h"

#include "kaddressbook_configure_debug.h"
#include <KLocalizedString>
#include <PimCommon/GenericPlugin>

namespace
{
QString kaddressbookPluginToolsGroupName()
{
    return u"kaddressbookplugintoolsgroupname"_s;
}

QString kaddressbookImportExportGroupName()
{
    return u"kaddressbookpluginimportexportgroupname"_s;
}
}

KAddressBookConfigPluginListWidget::KAddressBookConfigPluginListWidget(QWidget *parent)
    : TextAddonsWidgets::ConfigurePluginsWidget(parent)
{
    connect(this, &KAddressBookConfigPluginListWidget::configureClicked, this, &KAddressBookConfigPluginListWidget::slotConfigureClicked);
}

KAddressBookConfigPluginListWidget::~KAddressBookConfigPluginListWidget() = default;

void KAddressBookConfigPluginListWidget::save()
{
    TextAddonsWidgets::ConfigurePluginsWidget::savePlugins(KAddressBookPluginInterface::self()->configGroupName(),
                                                           KAddressBookPluginInterface::self()->configPrefixSettingKey(),
                                                           mPluginGenericItems,
                                                           PimCommon::PluginUtil::pluginConfigFile());

    TextAddonsWidgets::ConfigurePluginsWidget::savePlugins(KAddressBookImportExport::PluginManager::self()->configGroupName(),
                                                           KAddressBookImportExport::PluginManager::self()->configPrefixSettingKey(),
                                                           mPluginImportExportItems,
                                                           PimCommon::PluginUtil::pluginConfigFile());
}

void KAddressBookConfigPluginListWidget::doLoadFromGlobalSettings()
{
    initialize();
    initializeDone();
}

void KAddressBookConfigPluginListWidget::doResetToDefaultsOther()
{
    TextAddonsWidgets::ConfigurePluginsWidget::changeState(mPluginGenericItems);
    TextAddonsWidgets::ConfigurePluginsWidget::changeState(mPluginImportExportItems);
}

void KAddressBookConfigPluginListWidget::initialize()
{
    mTreePluginWidget->clear();
    // Necessary to initialize plugins when we load it outside kaddressbook
    KAddressBookPluginInterface::self()->initializePlugins();
    TextAddonsWidgets::ConfigurePluginsWidget::fillTopItems(KAddressBookPluginInterface::self()->pluginsDataList(),
                                                            i18n("Tools Plugins"),
                                                            KAddressBookPluginInterface::self()->configGroupName(),
                                                            KAddressBookPluginInterface::self()->configPrefixSettingKey(),
                                                            mPluginGenericItems,
                                                            kaddressbookPluginToolsGroupName(),
                                                            true,
                                                            PimCommon::PluginUtil::pluginConfigFile());

    TextAddonsWidgets::ConfigurePluginsWidget::fillTopItems(KAddressBookImportExport::PluginManager::self()->pluginsDataList(),
                                                            i18n("Import/Export Plugins"),
                                                            KAddressBookImportExport::PluginManager::self()->configGroupName(),
                                                            KAddressBookImportExport::PluginManager::self()->configPrefixSettingKey(),
                                                            mPluginImportExportItems,
                                                            kaddressbookImportExportGroupName(),
                                                            true,
                                                            PimCommon::PluginUtil::pluginConfigFile());

    mTreePluginWidget->expandAll();
}

void KAddressBookConfigPluginListWidget::slotConfigureClicked(const QString &configureGroupName, const QString &identifier)
{
    if (!configureGroupName.isEmpty() && !identifier.isEmpty()) {
        if (configureGroupName == kaddressbookPluginToolsGroupName()) {
            PimCommon::GenericPlugin *plugin = KAddressBookPluginInterface::self()->pluginFromIdentifier(identifier);
            plugin->showConfigureDialog(this);
        } else if (configureGroupName == kaddressbookImportExportGroupName()) {
            auto plugin = KAddressBookImportExport::PluginManager::self()->pluginFromIdentifier(identifier);
            plugin->showConfigureDialog(this);
        } else {
            qCWarning(KADDRESSBOOK_CONFIGURE_LOG) << "Unknown configureGroupName" << configureGroupName;
        }
    }
}

#include "moc_kaddressbookconfigpluginlistwidget.cpp"
