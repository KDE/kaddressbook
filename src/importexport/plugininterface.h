/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "kaddressbook_importexport_export.h"
#include <AkonadiCore/Collection>
#include <PimCommon/AbstractGenericPluginInterface>
#include <QList>

class QItemSelectionModel;

namespace KAddressBookImportExport
{
/** Import/export plugin interface. */
class KADDRESSBOOK_IMPORTEXPORT_EXPORT PluginInterface : public PimCommon::AbstractGenericPluginInterface
{
    Q_OBJECT
public:
    explicit PluginInterface(QObject *parent = nullptr);
    ~PluginInterface() override;

    enum ImportExportAction { Import = 0, Export = 1 };

    Q_REQUIRED_RESULT QList<QAction *> importActions() const;
    void setImportActions(const QList<QAction *> &importAction);

    Q_REQUIRED_RESULT QList<QAction *> exportActions() const;
    void setExportActions(const QList<QAction *> &exportAction);

    Q_REQUIRED_RESULT ImportExportAction importExportAction() const;
    void setImportExportAction(ImportExportAction importExportAction);

    Q_REQUIRED_RESULT Akonadi::Collection defaultCollection() const;
    void setDefaultCollection(const Akonadi::Collection &defaultCollection);

    Q_REQUIRED_RESULT QItemSelectionModel *itemSelectionModel() const;
    void setItemSelectionModel(QItemSelectionModel *itemSelectionModel);

    Q_REQUIRED_RESULT virtual bool canImportFileType(const QUrl &url);
    virtual void importFile(const QUrl &url);

    Q_REQUIRED_RESULT virtual bool canImportData() const;
    virtual void importData(const QByteArray &data);

protected:
    QList<QAction *> mImportActions;
    QList<QAction *> mExportActions;
    ImportExportAction mImportExportAction = Import;
    Akonadi::Collection mDefaultCollection;
    QItemSelectionModel *mItemSelectionModel = nullptr;
};
}
