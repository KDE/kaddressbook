/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

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
    setPluginDirectory(QStringLiteral("kaddressbook/mainview"));
}

KAddressBookPluginInterface::~KAddressBookPluginInterface()
{
}

KAddressBookPluginInterface *KAddressBookPluginInterface::self()
{
    static KAddressBookPluginInterface s_self;
    return &s_self;
}

void KAddressBookPluginInterface::initializeInterfaceRequires(PimCommon::AbstractGenericPluginInterface *abstractInterface)
{
    if (!mMainWidget) {
        qCCritical(KADDRESSBOOK_LOG) << "Main windows pointer not defined";
        return;
    }
    auto interface = static_cast<PimCommon::GenericPluginInterface *>(abstractInterface);
    PimCommon::GenericPluginInterface::RequireTypes requiresFeatures = interface->requiresFeatures();
    if (requiresFeatures & PimCommon::GenericPluginInterface::CurrentItems) {
        interface->setCurrentItems(mMainWidget->collectSelectedAllContactsItem());
    }
    if (requiresFeatures & PimCommon::GenericPluginInterface::Items) {
        interface->setItems(mMainWidget->selectedItems());
    }
    if (requiresFeatures & PimCommon::GenericPluginInterface::CurrentCollection) {
        interface->setCurrentCollection(mMainWidget->currentAddressBook());
    }
    if (requiresFeatures & PimCommon::GenericPluginInterface::Collections) {
        qCDebug(KADDRESSBOOK_LOG) << "PimCommon::GenericPluginInterface::Collections not implemented";
    }
}

void KAddressBookPluginInterface::setMainWidget(MainWidget *mainWidget)
{
    mMainWidget = mainWidget;
}
