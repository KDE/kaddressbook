/*
    Copyright (C) 2016 Laurent Montel <montel@kde.org>

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/

#include "addressbookmainwidget.h"
#include <QVBoxLayout>

AddressBookMainWidget::AddressBookMainWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
#if 0
    mCollectionTree = new Akonadi::EntityMimeTypeFilterModel(this);
    mCollectionTree->setDynamicSortFilter(true);
    mCollectionTree->setSortCaseSensitivity(Qt::CaseInsensitive);
    mCollectionTree->setSourceModel(GlobalContactModel::instance()->model());
    mCollectionTree->addMimeTypeInclusionFilter(Akonadi::Collection::mimeType());
    mCollectionTree->setHeaderGroup(Akonadi::EntityTreeModel::CollectionTreeHeaders);

    mCollectionSelectionModel = new QItemSelectionModel(mCollectionTree);
    StructuralCollectionsNotCheckableProxy *checkableProxyModel =
        new StructuralCollectionsNotCheckableProxy(this);
    checkableProxyModel->setSelectionModel(mCollectionSelectionModel);
    checkableProxyModel->setSourceModel(mCollectionTree);

    mCollectionView->setModel(checkableProxyModel);
    mCollectionView->setXmlGuiClient(guiClient);
    mCollectionView->header()->setDefaultAlignment(Qt::AlignCenter);
    mCollectionView->header()->setSortIndicatorShown(false);

    connect(mCollectionView->model(), &QAbstractItemModel::rowsInserted,
            this, &MainWidget::slotCheckNewCalendar);

    connect(mCollectionView, SIGNAL(currentChanged(Akonadi::Collection)),
            this, SLOT(slotCurrentCollectionChanged(Akonadi::Collection)));

    KSelectionProxyModel *selectionProxyModel =
        new KSelectionProxyModel(mCollectionSelectionModel, this);
    selectionProxyModel->setSourceModel(GlobalContactModel::instance()->model());
    selectionProxyModel->setFilterBehavior(KSelectionProxyModel::ChildrenOfExactSelection);

    mItemTree = new Akonadi::EntityMimeTypeFilterModel(this);
    mItemTree->setSourceModel(selectionProxyModel);
    mItemTree->addMimeTypeExclusionFilter(Akonadi::Collection::mimeType());
    mItemTree->setHeaderGroup(Akonadi::EntityTreeModel::ItemListHeaders);

    mCategoryFilterModel = new CategoryFilterProxyModel(this);
    mCategoryFilterModel->setSourceModel(mItemTree);
    mCategoryFilterModel->setFilterCategories(mCategorySelectWidget->filterTags());
    mCategoryFilterModel->setFilterEnabled(true);

    connect(mCategorySelectWidget, &CategorySelectWidget::filterChanged,
            mCategoryFilterModel, &CategoryFilterProxyModel::setFilterCategories);

    mContactsFilterModel = new Akonadi::ContactsFilterProxyModel(this);
    mContactsFilterModel->setSourceModel(mCategoryFilterModel);

    connect(mQuickSearchWidget, &QuickSearchWidget::filterStringChanged,
            mContactsFilterModel, &Akonadi::ContactsFilterProxyModel::setFilterString);
    connect(mQuickSearchWidget, &QuickSearchWidget::filterStringChanged,
            this, &MainWidget::selectFirstItem);
    connect(mQuickSearchWidget, &QuickSearchWidget::arrowDownKeyPressed,
            this, &MainWidget::setFocusToTreeView);
    mItemView->setModel(mContactsFilterModel);
    mItemView->setXmlGuiClient(guiClient);
    mItemView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mItemView->setRootIsDecorated(false);
    mItemView->header()->setDefaultAlignment(Qt::AlignCenter);

    mXXPortManager->setSelectionModel(mItemView->selectionModel());

    mActionManager = new Akonadi::StandardContactActionManager(guiClient->actionCollection(), this);
    mActionManager->setCollectionSelectionModel(mCollectionView->selectionModel());
    mActionManager->setItemSelectionModel(mItemView->selectionModel());

    QList<Akonadi::StandardActionManager::Type> standardActions;
    standardActions << Akonadi::StandardActionManager::CreateCollection
                    << Akonadi::StandardActionManager::DeleteCollections
                    << Akonadi::StandardActionManager::SynchronizeCollections
                    << Akonadi::StandardActionManager::CollectionProperties
                    << Akonadi::StandardActionManager::CopyItems
                    << Akonadi::StandardActionManager::Paste
                    << Akonadi::StandardActionManager::DeleteItems
                    << Akonadi::StandardActionManager::CutItems
                    << Akonadi::StandardActionManager::CreateResource
                    << Akonadi::StandardActionManager::DeleteResources
                    << Akonadi::StandardActionManager::ResourceProperties
                    << Akonadi::StandardActionManager::SynchronizeResources
                    << Akonadi::StandardActionManager::SynchronizeCollectionsRecursive
                    << Akonadi::StandardActionManager::MoveItemToMenu
                    << Akonadi::StandardActionManager::CopyItemToMenu;

    Q_FOREACH (Akonadi::StandardActionManager::Type standardAction, standardActions) {
        mActionManager->createAction(standardAction);
    }
    guiClient->actionCollection()->setDefaultShortcut(mActionManager->action(Akonadi::StandardActionManager::DeleteItems), QKeySequence(Qt::Key_Delete));
    QList<Akonadi::StandardContactActionManager::Type> contactActions;
    contactActions << Akonadi::StandardContactActionManager::CreateContact
                   << Akonadi::StandardContactActionManager::CreateContactGroup
                   << Akonadi::StandardContactActionManager::EditItem;

    Q_FOREACH (Akonadi::StandardContactActionManager::Type contactAction, contactActions) {
        mActionManager->createAction(contactAction);
    }
    static bool pageRegistered = false;

    if (!pageRegistered) {
        Akonadi::CollectionPropertiesDialog::registerPage(new PimCommon::CollectionAclPageFactory);
        pageRegistered = true;
    }

    const QStringList pages =
        QStringList() << QStringLiteral("Akonadi::CollectionGeneralPropertiesPage")
        << QStringLiteral("Akonadi::CachePolicyPage")
        << QStringLiteral("PimCommon::CollectionAclPage");

    mActionManager->setCollectionPropertiesPageNames(pages);

    connect(mItemView, SIGNAL(currentChanged(Akonadi::Item)),
            this, SLOT(itemSelected(Akonadi::Item)));
    connect(mItemView, SIGNAL(doubleClicked(Akonadi::Item)),
            mActionManager->action(Akonadi::StandardContactActionManager::EditItem),
            SLOT(trigger()));
    connect(mItemView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &MainWidget::itemSelectionChanged);
    connect(mItemView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWidget::slotSelectionChanged);

    // show the contact details view as default
    mDetailsViewStack->setCurrentWidget(mContactDetails);

    mContactSwitcher->setView(mItemView);

    Akonadi::ControlGui::widgetNeedsAkonadi(this);

    mModelColumnManager = new ModelColumnManager(GlobalContactModel::instance()->model(), this);
    mModelColumnManager->setWidget(mItemView->header());
    mModelColumnManager->load();

    QMetaObject::invokeMethod(this, "delayedInit", Qt::QueuedConnection);
    updateQuickSearchText();
    slotSelectionChanged();

#endif
}

AddressBookMainWidget::~AddressBookMainWidget()
{

}
