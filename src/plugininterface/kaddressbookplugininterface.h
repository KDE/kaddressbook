/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KADDRESSBOOKPLUGININTERFACE_H
#define KADDRESSBOOKPLUGININTERFACE_H

#include <QObject>

#include <PimCommonAkonadi/GenericPluginInterface>
#include <pimcommonakonadi/plugininterface.h>
class MainWidget;

#include "kaddressbook_export.h"

class KADDRESSBOOK_EXPORT KAddressBookPluginInterface : public PimCommon::PluginInterface
{
    Q_OBJECT
public:
    explicit KAddressBookPluginInterface(QObject *parent = nullptr);
    ~KAddressBookPluginInterface() override;

    void setMainWidget(MainWidget *mainWidget);

    void initializeInterfaceRequires(PimCommon::AbstractGenericPluginInterface *interface) override;
    static KAddressBookPluginInterface *self();
private:
    MainWidget *mMainWidget = nullptr;
};

#endif // KADDRESSBOOKPLUGININTERFACE_H
