/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

#include "pimcommonakonadi/plugininterface.h"
#include <PimCommonAkonadi/GenericPluginInterface>
class MainWidget;

#include "kaddressbook_export.h"

class KADDRESSBOOK_EXPORT KAddressBookPluginInterface : public PimCommon::PluginInterface
{
    Q_OBJECT
public:
    explicit KAddressBookPluginInterface(QObject *parent = nullptr);
    ~KAddressBookPluginInterface() override;

    void setMainWidget(MainWidget *mainWidget);

    bool initializeInterfaceRequires(PimCommon::AbstractGenericPluginInterface *abstractInterface) override;
    static KAddressBookPluginInterface *self();

private:
    MainWidget *mMainWidget = nullptr;
};
