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

#include "kaddressbookimportexportplugininterface.h"

using namespace KAddressBookImportExport;

KAddressBookImportExportPluginInterface::KAddressBookImportExportPluginInterface(QObject *parent)
    : PimCommon::AbstractGenericPluginInterface(parent)
    , mImportExportAction(Import)
{
}

KAddressBookImportExportPluginInterface::~KAddressBookImportExportPluginInterface()
{
}

QList<QAction *> KAddressBookImportExportPluginInterface::importActions() const
{
    return mImportActions;
}

void KAddressBookImportExportPluginInterface::setImportActions(const QList<QAction *> &importAction)
{
    mImportActions = importAction;
}

QList<QAction *> KAddressBookImportExportPluginInterface::exportActions() const
{
    return mExportActions;
}

void KAddressBookImportExportPluginInterface::setExportActions(const QList<QAction *> &exportAction)
{
    mExportActions = exportAction;
}

KAddressBookImportExportPluginInterface::ImportExportAction KAddressBookImportExportPluginInterface::importExportAction() const
{
    return mImportExportAction;
}

void KAddressBookImportExportPluginInterface::setImportExportAction(ImportExportAction importExportAction)
{
    mImportExportAction = importExportAction;
}

Akonadi::Collection KAddressBookImportExportPluginInterface::defaultCollection() const
{
    return mDefaultCollection;
}

void KAddressBookImportExportPluginInterface::setDefaultCollection(const Akonadi::Collection &defaultCollection)
{
    mDefaultCollection = defaultCollection;
}

QItemSelectionModel *KAddressBookImportExportPluginInterface::itemSelectionModel() const
{
    return mItemSelectionModel;
}

void KAddressBookImportExportPluginInterface::setItemSelectionModel(QItemSelectionModel *itemSelectionModel)
{
    mItemSelectionModel = itemSelectionModel;
}

bool KAddressBookImportExportPluginInterface::canImportFileType(const QUrl &url)
{
    Q_UNUSED(url);
    return false;
}

void KAddressBookImportExportPluginInterface::importFile(const QUrl &url)
{
    Q_UNUSED(url);
}

bool KAddressBookImportExportPluginInterface::canImportData() const
{
    return false;
}

void KAddressBookImportExportPluginInterface::importData(const QByteArray &data)
{
    Q_UNUSED(data);
}
