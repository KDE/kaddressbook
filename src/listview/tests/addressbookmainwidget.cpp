/*
    Copyright (C) 2016-2017 Laurent Montel <montel@kde.org>

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
#include "addressbookquickwidget.h"
#include "addressbooklistviewmodel.h"
#include <QVBoxLayout>
#include <QSplitter>
#include <KLocalizedString>
#include <QTextBrowser>
#include <ContactDefaultActions>
#include <KCheckableProxyModel>
#include <KSelectionProxyModel>
#include <kactioncollection.h>
#include "../../globalcontactmodel.h"
#include <Akonadi/Contact/ContactsTreeModel>
#include <QHeaderView>
#include <KContacts/ContactGroup>
#include <KContacts/Addressee>
#include <AkonadiCore/MimeTypeChecker>

static bool isStructuralCollection(const Akonadi::Collection &collection)
{
    QStringList mimeTypes;
    mimeTypes << KContacts::Addressee::mimeType() << KContacts::ContactGroup::mimeType();
    const QStringList collectionMimeTypes = collection.contentMimeTypes();
    for (const QString &mimeType : mimeTypes) {
        if (collectionMimeTypes.contains(mimeType)) {
            return false;
        }
    }
    return true;
}

class StructuralCollectionsNotCheckableProxy : public KCheckableProxyModel
{
public:
    explicit StructuralCollectionsNotCheckableProxy(QObject *parent)
        : KCheckableProxyModel(parent)
    {
    }

    QVariant data(const QModelIndex &index, int role) const override
    {
        if (!index.isValid()) {
            return QVariant();
        }

        if (role == Qt::CheckStateRole) {
            // Don't show the checkbox if the collection can't contain incidences
            const Akonadi::Collection collection
                = index.data(Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
            if (collection.isValid() && isStructuralCollection(collection)) {
                return QVariant();
            }
        }
        return KCheckableProxyModel::data(index, role);
    }
};

AddressBookMainWidget::AddressBookMainWidget(QWidget *parent)
    : QWidget(parent)
{
    setupGui();
    setupAction(new KActionCollection(this));
    initGrantleeThemeName();
    mCollectionTree = new Akonadi::EntityMimeTypeFilterModel(this);
    mCollectionTree->setDynamicSortFilter(true);
    mCollectionTree->setSortCaseSensitivity(Qt::CaseInsensitive);
    mCollectionTree->setSourceModel(GlobalContactModel::instance()->model());
    mCollectionTree->addMimeTypeInclusionFilter(Akonadi::Collection::mimeType());
    mCollectionTree->setHeaderGroup(Akonadi::EntityTreeModel::CollectionTreeHeaders);

    mCollectionSelectionModel = new QItemSelectionModel(mCollectionTree);

    StructuralCollectionsNotCheckableProxy *checkableProxyModel
        = new StructuralCollectionsNotCheckableProxy(this);
    checkableProxyModel->setSelectionModel(mCollectionSelectionModel);
    checkableProxyModel->setSourceModel(mCollectionTree);

    mCollectionView->setModel(checkableProxyModel);
    //mCollectionView->setXmlGuiClient(guiClient);
    mCollectionView->header()->setDefaultAlignment(Qt::AlignCenter);
    mCollectionView->header()->setSortIndicatorShown(false);

#if 0
    connect(mCollectionView->model(), &QAbstractItemModel::rowsInserted,
            this, &MainWidget::slotCheckNewCalendar);

    connect(mCollectionView, SIGNAL(currentChanged(Akonadi::Collection)),
            this, SLOT(slotCurrentCollectionChanged(Akonadi::Collection)));
#endif
    KSelectionProxyModel *selectionProxyModel
        = new KSelectionProxyModel(mCollectionSelectionModel, this);
    selectionProxyModel->setSourceModel(GlobalContactModel::instance()->model());
    selectionProxyModel->setFilterBehavior(KSelectionProxyModel::ChildrenOfExactSelection);

    mItemTree = new Akonadi::EntityMimeTypeFilterModel(this);
    mItemTree->setSourceModel(selectionProxyModel);
    mItemTree->addMimeTypeExclusionFilter(Akonadi::Collection::mimeType());
    mItemTree->setHeaderGroup(Akonadi::EntityTreeModel::ItemListHeaders);
#if 0
    mCategoryFilterModel = new CategoryFilterProxyModel(this);
    mCategoryFilterModel->setSourceModel(mItemTree);
    mCategoryFilterModel->setFilterCategories(mCategorySelectWidget->filterTags());
    mCategoryFilterModel->setFilterEnabled(true);

    connect(mCategorySelectWidget, &CategorySelectWidget::filterChanged,
            mCategoryFilterModel, &CategoryFilterProxyModel::setFilterCategories);

#endif
    mContactsFilterModel = new Akonadi::ContactsFilterProxyModel(this);
    mContactsFilterModel->setSourceModel(/*mCategoryFilterModel*/ mItemTree);
    mItemView->setModel(mContactsFilterModel);
    mAddressbookQuickWidget->addressBookModel()->setSourceModel(mContactsFilterModel);
    //mItemView->setXmlGuiClient(guiClient);
    mItemView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mItemView->setRootIsDecorated(false);
    mItemView->header()->setDefaultAlignment(Qt::AlignCenter);
#if 0
    mActionManager = new Akonadi::StandardContactActionManager(guiClient->actionCollection(), this);
    mActionManager->setCollectionSelectionModel(mCollectionView->selectionModel());
    mActionManager->setItemSelectionModel(mItemView->selectionModel());

    guiClient->actionCollection()->setDefaultShortcut(mActionManager->action(Akonadi::StandardActionManager::DeleteItems), QKeySequence(Qt::Key_Delete));
    QList<Akonadi::StandardContactActionManager::Type> contactActions;
    contactActions << Akonadi::StandardContactActionManager::CreateContact
                   << Akonadi::StandardContactActionManager::CreateContactGroup
                   << Akonadi::StandardContactActionManager::EditItem;

    Q_FOREACH (Akonadi::StandardContactActionManager::Type contactAction, contactActions) {
        mActionManager->createAction(contactAction);
    }

#endif
    connect(mItemView, SIGNAL(currentChanged(Akonadi::Item)),
            this, SLOT(itemSelected(Akonadi::Item)));
#if 0
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

void AddressBookMainWidget::itemSelected(const Akonadi::Item &item)
{
    if (Akonadi::MimeTypeChecker::isWantedItem(item, KContacts::Addressee::mimeType())) {
        mDetailsViewStack->setCurrentWidget(mContactDetails);
        mContactDetails->setContact(item);
    } else if (Akonadi::MimeTypeChecker::isWantedItem(item, KContacts::ContactGroup::mimeType())) {
        mDetailsViewStack->setCurrentWidget(mContactGroupDetails);
        mContactGroupDetails->setContactGroup(item);
    }
}

void AddressBookMainWidget::initGrantleeThemeName()
{
    QString themeName = mGrantleeThemeManager->configuredThemeName();
    if (themeName.isEmpty()) {
        themeName = QStringLiteral("default");
    }
    mFormatter->setGrantleeTheme(mGrantleeThemeManager->theme(themeName));
    mGroupFormatter->setGrantleeTheme(mGrantleeThemeManager->theme(themeName));
}

void AddressBookMainWidget::setupAction(KActionCollection *collection)
{
    mGrantleeThemeManager = new GrantleeTheme::ThemeManager(QStringLiteral("addressbook"),
                                                            QStringLiteral("theme.desktop"),
                                                            collection,
                                                            QStringLiteral("kaddressbook/viewertemplates/"));
    mGrantleeThemeManager->setDownloadNewStuffConfigFile(QStringLiteral("kaddressbook_themes.knsrc"));
    //connect(mGrantleeThemeManager, &GrantleeTheme::ThemeManager::grantleeThemeSelected, this, &MainWidget::slotGrantleeThemeSelected);
    //connect(mGrantleeThemeManager, &GrantleeTheme::ThemeManager::updateThemes, this, &MainWidget::slotGrantleeThemesUpdated);
}

void AddressBookMainWidget::setupGui()
{
    // the horizontal main layout
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    // Splitter 1 contains the two main parts of the GUI:
    //  - collection and item view splitter 2 on the left (see below)
    //  - details pane on the right, that contains
    //   - details view stack on the top
    //   - contact switcher at the bottom
    mMainWidgetSplitter1 = new QSplitter(Qt::Horizontal);
    mMainWidgetSplitter1->setObjectName(QStringLiteral("MainWidgetSplitter1"));
    layout->addWidget(mMainWidgetSplitter1);

    // Splitter 2 contains the remaining parts of the GUI:
    //  - collection view on either the left or the top
    //  - item view on either the right or the bottom
    // The orientation of this splitter is changed for either
    // a three or two column view;  in simple mode it is hidden.
    mMainWidgetSplitter2 = new QSplitter(Qt::Vertical);
    mMainWidgetSplitter2->setObjectName(QStringLiteral("MainWidgetSplitter2"));
    mMainWidgetSplitter1->addWidget(mMainWidgetSplitter2);

    // the collection view
    mCollectionView = new Akonadi::EntityTreeView();
    mMainWidgetSplitter2->addWidget(mCollectionView);

    // the items view
    mItemView = new Akonadi::EntityTreeView();
    mItemView->setObjectName(QStringLiteral("ContactView"));
    mItemView->setDefaultPopupMenu(QStringLiteral("akonadi_itemview_contextmenu"));
    mItemView->setAlternatingRowColors(true);
    mMainWidgetSplitter2->addWidget(mItemView);

    mAddressbookQuickWidget = new AddressBookQuickWidget(this);
    mAddressbookQuickWidget->setSource(QUrl(QStringLiteral("qrc:/qml/data/addressbooklistview.qml")));
    mMainWidgetSplitter2->addWidget(mAddressbookQuickWidget);

    // the details pane that contains the details view stack and contact switcher
    mDetailsPane = new QWidget;
    mMainWidgetSplitter1->addWidget(mDetailsPane);

    mMainWidgetSplitter1->setStretchFactor(1, 9);          // maximum width for detail
    mMainWidgetSplitter2->setStretchFactor(1, 9);          // for intuitive resizing
    mMainWidgetSplitter2->setChildrenCollapsible(false);
    mMainWidgetSplitter1->setChildrenCollapsible(false);

    QVBoxLayout *detailsPaneLayout = new QVBoxLayout(mDetailsPane);
    detailsPaneLayout->setMargin(0);

    // the details view stack
    mDetailsViewStack = new QStackedWidget();
    detailsPaneLayout->addWidget(mDetailsViewStack);

    // the details widget for contacts
    mContactDetails = new Akonadi::ContactViewer(mDetailsViewStack);
    mDetailsViewStack->addWidget(mContactDetails);

    // the details widget for contact groups
    mContactGroupDetails = new Akonadi::ContactGroupViewer(mDetailsViewStack);
    mDetailsViewStack->addWidget(mContactGroupDetails);

    // the details widget for empty items
    mEmptyDetails = new QTextBrowser(mDetailsViewStack);
    mDetailsViewStack->addWidget(mEmptyDetails);
    // setup the default actions
    Akonadi::ContactDefaultActions *actions = new Akonadi::ContactDefaultActions(this);
    actions->connectToView(mContactDetails);
    actions->connectToView(mContactGroupDetails);
    mFormatter = new KAddressBookGrantlee::GrantleeContactFormatter;

    mContactDetails->setContactFormatter(mFormatter);

    mGroupFormatter = new KAddressBookGrantlee::GrantleeContactGroupFormatter;

    mContactGroupDetails->setContactGroupFormatter(mGroupFormatter);
}
