/*
   SPDX-FileCopyrightText: 2016-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <AkonadiCore/Collection>
#include <QObject>
#include <QPointer>
namespace Akonadi
{
class CollectionPropertiesDialog;
}
class MainWidget;
class KJob;
class ManageShowCollectionProperties : public QObject
{
    Q_OBJECT
public:
    explicit ManageShowCollectionProperties(MainWidget *mainWidget, QObject *parent = nullptr);
    ~ManageShowCollectionProperties() override;

public Q_SLOTS:
    void showCollectionProperties();

private:
    void slotCollectionPropertiesContinued(KJob *job);
    void slotCollectionPropertiesFinished(KJob *job);
    QHash<Akonadi::Collection::Id, QPointer<Akonadi::CollectionPropertiesDialog>> mHashDialogBox;
    QStringList mPages;
    MainWidget *const mMainWidget;
};

