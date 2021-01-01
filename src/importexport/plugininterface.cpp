/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "plugininterface.h"

using namespace KAddressBookImportExport;

PluginInterface::PluginInterface(QObject *parent)
    : PimCommon::AbstractGenericPluginInterface(parent)
{
}

PluginInterface::~PluginInterface() = default;

QList<QAction *> PluginInterface::importActions() const
{
    return mImportActions;
}

void PluginInterface::setImportActions(const QList<QAction *> &importAction)
{
    mImportActions = importAction;
}

QList<QAction *> PluginInterface::exportActions() const
{
    return mExportActions;
}

void PluginInterface::setExportActions(const QList<QAction *> &exportAction)
{
    mExportActions = exportAction;
}

PluginInterface::ImportExportAction PluginInterface::importExportAction() const
{
    return mImportExportAction;
}

void PluginInterface::setImportExportAction(ImportExportAction importExportAction)
{
    mImportExportAction = importExportAction;
}

Akonadi::Collection PluginInterface::defaultCollection() const
{
    return mDefaultCollection;
}

void PluginInterface::setDefaultCollection(const Akonadi::Collection &defaultCollection)
{
    mDefaultCollection = defaultCollection;
}

QItemSelectionModel *PluginInterface::itemSelectionModel() const
{
    return mItemSelectionModel;
}

void PluginInterface::setItemSelectionModel(QItemSelectionModel *itemSelectionModel)
{
    mItemSelectionModel = itemSelectionModel;
}

bool PluginInterface::canImportFileType(const QUrl &url)
{
    Q_UNUSED(url)
    return false;
}

void PluginInterface::importFile(const QUrl &url)
{
    Q_UNUSED(url)
}

bool PluginInterface::canImportData() const
{
    return false;
}

void PluginInterface::importData(const QByteArray &data)
{
    Q_UNUSED(data)
}
