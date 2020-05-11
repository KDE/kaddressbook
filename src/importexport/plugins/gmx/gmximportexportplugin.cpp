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

#include "gmximportexportplugin.h"
#include "gmximportexportplugininterface.h"

#include <KPluginFactory>

K_PLUGIN_CLASS_WITH_JSON(GMXImportExportPlugin, "kaddressbook_importexportgmxplugin.json")

GMXImportExportPlugin::GMXImportExportPlugin(QObject *parent, const QList<QVariant> &)
    : KAddressBookImportExport::KAddressBookImportExportPlugin(parent)
{
}

GMXImportExportPlugin::~GMXImportExportPlugin()
{
}

PimCommon::AbstractGenericPluginInterface *GMXImportExportPlugin::createInterface(QObject *parent)
{
    GMXImportExportPluginInterface *interface = new GMXImportExportPluginInterface(parent);
    return interface;
}

bool GMXImportExportPlugin::hasPopupMenuSupport() const
{
    return true;
}

#include "gmximportexportplugin.moc"
