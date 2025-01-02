/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kaddressbookplugininterface.h"

#include "kaddressbook_debug.h"
#include "mainwidget.h"

#include <PimCommon/GenericPlugin>

KAddressBookPluginInterface::KAddressBookPluginInterface(QObject *parent)
    : PimCommon::PluginInterface(parent)
{
    setPluginName(QStringLiteral("kaddressbook"));
    setPluginDirectory(QStringLiteral("pim6/kaddressbook/mainview"));
}

KAddressBookPluginInterface::~KAddressBookPluginInterface() = default;

KAddressBookPluginInterface *KAddressBookPluginInterface::self()
{
    static KAddressBookPluginInterface s_self;
    return &s_self;
}

bool KAddressBookPluginInterface::initializeInterfaceRequires(PimCommon::AbstractGenericPluginInterface *abstractInterface)
{
    if (!mMainWidget) {
        qCCritical(KADDRESSBOOK_LOG) << "Main windows pointer not defined";
        return false;
    }
    bool canceled = false;
    auto interface = static_cast<PimCommon::GenericPluginInterface *>(abstractInterface);
    const PimCommon::GenericPluginInterface::RequireTypes requiresFeatures = interface->requiresFeatures();
    if (requiresFeatures & PimCommon::GenericPluginInterface::CurrentItems) {
        interface->setCurrentItems(mMainWidget->collectSelectedAllContactsItem());
    }
    if (requiresFeatures & PimCommon::GenericPluginInterface::Items) {
        interface->setItems(mMainWidget->selectedItems(canceled));
    }
    if (requiresFeatures & PimCommon::GenericPluginInterface::CurrentCollection) {
        interface->setCurrentCollection(mMainWidget->currentAddressBook());
    }
    if (requiresFeatures & PimCommon::GenericPluginInterface::Collections) {
        qCDebug(KADDRESSBOOK_LOG) << "PimCommon::GenericPluginInterface::Collections not implemented";
    }
    if (canceled) {
        return false;
    } else {
        return true;
    }
}

void KAddressBookPluginInterface::setMainWidget(MainWidget *mainWidget)
{
    mMainWidget = mainWidget;
}

#include "moc_kaddressbookplugininterface.cpp"
