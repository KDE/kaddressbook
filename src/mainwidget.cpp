/*
  This file is part of KAddressBook.

  Copyright (c) 2007 Tobias Koenig <tokoe@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include "mainwidget.h"

#include "contactswitcher.h"
#include "globalcontactmodel.h"
#include "modelcolumnmanager.h"
#include "printing/printingwizard.h"
#include "widgets/quicksearchwidget.h"
#include "settings.h"
#include "kaddressbookadaptor.h"
#include "categoryselectwidget.h"
#include "categoryfilterproxymodel.h"
#include "kaddressbook_options.h"
#include "manageshowcollectionproperties.h"

#include "KaddressbookGrantlee/GrantleeContactFormatter"
#include "KaddressbookGrantlee/GrantleeContactGroupFormatter"
#include "grantleetheme/grantleethememanager.h"

#include "Libkdepim/UiStateSaver"

#include <PimCommonAkonadi/ImapAclAttribute>
#include <PimCommonAkonadi/MailUtil>

#include <AkonadiWidgets/ETMViewStateSaver>
#include <AkonadiCore/CollectionFilterProxyModel>
#include <AkonadiWidgets/ControlGui>
#include <AkonadiCore/EntityMimeTypeFilterModel>
#include <AkonadiWidgets/EntityTreeView>
#include <AkonadiWidgets/ItemView>
#include <AkonadiCore/MimeTypeChecker>
#include <AkonadiCore/AttributeFactory>
#include <AkonadiWidgets/CollectionPropertiesDialog>
#include <AkonadiWidgets/CollectionMaintenancePage>
#include <AkonadiSearch/Debug/akonadisearchdebugdialog.h>
#include <KContacts/Addressee>
#include <QPointer>
#include "PimCommonAkonadi/ManageServerSideSubscriptionJob"
#include "PimCommon/PimUtil"
#include <KAddressBookImportExport/KAddressBookImportExportPluginManager>
#include <KAddressBookImportExport/KAddressBookImportExportPlugin>
#include <KAddressBookImportExport/KAddressBookImportExportPluginInterface>
#include <KAddressBookImportExport/KAddressBookContactSelectionDialog>

#include <Akonadi/Contact/ContactDefaultActions>
#include <Akonadi/Contact/ContactGroupEditorDialog>
#include <Akonadi/Contact/ContactGroupViewer>
#include <Akonadi/Contact/ContactsFilterProxyModel>
#include <Akonadi/Contact/ContactsTreeModel>
#include <Akonadi/Contact/ContactViewer>
#include <Akonadi/Contact/StandardContactActionManager>

#include <KContacts/ContactGroup>
#include "kaddressbook_debug.h"
#include <QAction>
#include <QApplication>
#include <KActionCollection>
#include <KActionMenu>
#include <KCheckableProxyModel>
#include <KDescendantsProxyModel>
#include <KLocalizedString>
#include <KSelectionProxyModel>
#include <QTextBrowser>
#include <KToggleAction>
#include <KCMultiDialog>
#include <KXMLGUIClient>

#include <QActionGroup>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QSplitter>
#include <QStackedWidget>
#include <QDBusConnection>
#include <ItemModifyJob>
#include <KPimPrintPreviewDialog>

#include "plugininterface/kaddressbookplugininterface.h"

namespace {
static bool isStructuralCollection(const Akonadi::Collection &collection)
{
    const QStringList mimeTypes = {KContacts::Addressee::mimeType(), KContacts::ContactGroup::mimeType()};
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
}

MainWidget::MainWidget(KXMLGUIClient *guiClient, QWidget *parent)
    : QWidget(parent)
    , mAllContactsModel(nullptr)
    , mXmlGuiClient(guiClient)
    , mGrantleeThemeManager(nullptr)
    , mQuickSearchAction(nullptr)
    , mServerSideSubscription(nullptr)
{
    (void)new KaddressbookAdaptor(this);
    QDBusConnection::sessionBus().registerObject(QStringLiteral("/KAddressBook"), this);

    mManageShowCollectionProperties = new ManageShowCollectionProperties(this, this);
    mImportExportPluginManager = KAddressBookImportExport::KAddressBookImportExportPluginManager::self();

    Akonadi::AttributeFactory::registerAttribute<PimCommon::ImapAclAttribute>();

    KAddressBookPluginInterface::self()->setActionCollection(guiClient->actionCollection());
    KAddressBookPluginInterface::self()->initializePlugins();
    setupGui();
    setupActions(guiClient->actionCollection());

    /*
    *  The item models, proxies and views have the following structure:
    *
    *                               mItemView
    *                                   ^
    *                                   |
    *                           mContactsFilterModel
    *                                   ^
    *                                   |
    * mCategorySelectWidget --> mCategoryFilterModel
    *                                   ^
    *                                   |
    *                               mItemTree
    *                                   ^
    *                                   |
    *                                   |           mAllContactsModel
    *                                   |                  ^
    *                                   |                  |
    *      mCollectionView     selectionProxyModel  descendantsModel
    *            ^               ^      ^                  ^
    *            |               |      |                  |
    *            |       selectionModel |                  |
    *            |               |      |                  |
    *        proxyModel ---------'      |                  |
    *            ^                      |                  |
    *            |                      |                  |
    *      mCollectionTree              |                  |
    *            ^                      |                  |
    *            |                      |   _______________/
    *             \                    /  /
    *            GlobalContactModel::instance()
    *
    *
    *  GlobalContactModel::instance():  The global contact model (contains collections and items)
    *                 mCollectionTree:  Filters out all items
    *                      proxyModel:  Allows the user to select collections by checkboxes
    *                  selectionModel:  Represents the selected collections that have been
    *                                   selected in proxyModel
    *                 mCollectionView:  Shows the collections (address books) in a view
    *             selectionProxyModel:  Filters out all collections and items that are no children
    *                                   of the collections currently selected in selectionModel
    *                       mItemTree:  Filters out all collections
    *           mCategorySelectWidget:  Selects a list of categories for filtering
    *            mCategoryFilterModel:  Filters the contacts by the selected categories
    *            mContactsFilterModel:  Filters the contacts by the content of mQuickSearchWidget
    *                       mItemView:  Shows the items (contacts and contact groups) in a view
    *
    *                descendantsModel:  Flattens the item/collection tree to a list
    *               mAllContactsModel:  Provides a list of all available contacts from all
    *                                   address books
    */

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
    mCollectionView->setXmlGuiClient(guiClient);
    mCollectionView->header()->setDefaultAlignment(Qt::AlignCenter);
    mCollectionView->header()->setSortIndicatorShown(false);

    connect(mCollectionView->model(), &QAbstractItemModel::rowsInserted,
            this, &MainWidget::slotCheckNewCalendar);

    connect(mCollectionView, QOverload<const Akonadi::Collection &>::of(&Akonadi::EntityTreeView::currentChanged),
            this, &MainWidget::slotCurrentCollectionChanged);

    KSelectionProxyModel *selectionProxyModel
        = new KSelectionProxyModel(mCollectionSelectionModel, this);
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

    for (Akonadi::StandardActionManager::Type standardAction : qAsConst(standardActions)) {
        mActionManager->createAction(standardAction);
    }
    guiClient->actionCollection()->setDefaultShortcut(mActionManager->action(Akonadi::StandardActionManager::DeleteItems), QKeySequence(Qt::Key_Delete));
    QList<Akonadi::StandardContactActionManager::Type> contactActions;
    contactActions << Akonadi::StandardContactActionManager::CreateContact
                   << Akonadi::StandardContactActionManager::CreateContactGroup
                   << Akonadi::StandardContactActionManager::EditItem;

    for (Akonadi::StandardContactActionManager::Type contactAction : qAsConst(contactActions)) {
        mActionManager->createAction(contactAction);
    }

    mActionManager->interceptAction(Akonadi::StandardActionManager::CollectionProperties);
    connect(mActionManager->action(
                Akonadi::StandardActionManager::CollectionProperties), &QAction::triggered, mManageShowCollectionProperties, &ManageShowCollectionProperties::showCollectionProperties);

    connect(mItemView, QOverload<const Akonadi::Item &>::of(&Akonadi::EntityTreeView::currentChanged),
            this, &MainWidget::itemSelected);
    connect(mItemView, QOverload<const Akonadi::Item &>::of(&Akonadi::EntityTreeView::doubleClicked),
            mActionManager->action(Akonadi::StandardContactActionManager::EditItem), &QAction::trigger);
    connect(mItemView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &MainWidget::itemSelectionChanged);

    // show the contact details view as default
    mDetailsViewStack->setCurrentWidget(mContactDetails);

    mContactSwitcher->setView(mItemView);

    Akonadi::ControlGui::widgetNeedsAkonadi(this);

    mModelColumnManager = new ModelColumnManager(GlobalContactModel::instance()->model(), this);
    mModelColumnManager->setWidget(mItemView->header());
    mModelColumnManager->load();

    initializeImportExportPlugin(guiClient->actionCollection());
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    QMetaObject::invokeMethod(this, &MainWidget::delayedInit, Qt::QueuedConnection);
#else
    QMetaObject::invokeMethod(this, "delayedInit", Qt::QueuedConnection);
#endif
    updateQuickSearchText();
}

void MainWidget::setFocusToTreeView()
{
    mItemView->setFocus();
}

void MainWidget::initializeImportExportPlugin(KActionCollection *collection)
{
    const QVector<KAddressBookImportExport::KAddressBookImportExportPlugin *> listPlugins = KAddressBookImportExport::KAddressBookImportExportPluginManager::self()->pluginsList();
    QList<QAction *> importActions;
    QList<QAction *> exportActions;
    for (KAddressBookImportExport::KAddressBookImportExportPlugin *plugin : listPlugins) {
        if (plugin->isEnabled()) {
            KAddressBookImportExport::KAddressBookImportExportPluginInterface *interface
                = static_cast<KAddressBookImportExport::KAddressBookImportExportPluginInterface *>(plugin->createInterface(collection, this));
            interface->setItemSelectionModel(mItemView->selectionModel());
            interface->createAction(collection);
            importActions.append(interface->importActions());
            exportActions.append(interface->exportActions());
            mImportExportPluginInterfaceList.append(interface);
            connect(interface, &PimCommon::AbstractGenericPluginInterface::emitPluginActivated, this, &MainWidget::slotImportExportActivated);
        }
    }

    if (!importActions.isEmpty()) {
        KActionMenu *importMenu = new KActionMenu(i18n("Import"), this);
        collection->addAction(QStringLiteral("import_menu"), importMenu);
        for (QAction *act : qAsConst(importActions)) {
            importMenu->addAction(act);
        }
    }
    if (!exportActions.isEmpty()) {
        KActionMenu *exportMenu = new KActionMenu(i18n("Export"), this);
        collection->addAction(QStringLiteral("export_menu"), exportMenu);
        for (QAction *act : qAsConst(exportActions)) {
            exportMenu->addAction(act);
        }
    }
}

void MainWidget::configure()
{
    QPointer<KCMultiDialog> dlg = new KCMultiDialog(this);
    dlg->addModule(QStringLiteral("akonadicontact_actions.desktop"));
    dlg->addModule(QStringLiteral("kcmldap.desktop"));
    dlg->addModule(QStringLiteral("kaddressbook_config_plugins.desktop"));

    dlg->exec();
    delete dlg;
}

void MainWidget::handleCommandLine(const QStringList &arguments)
{
    QCommandLineParser parser;
    kaddressbook_options(&parser);
    parser.process(arguments);

    if (parser.isSet(QStringLiteral("import"))) {
        for (const QString &urlStr : parser.positionalArguments()) {
            const QUrl url(QUrl::fromUserInput(urlStr));
            for (KAddressBookImportExport::KAddressBookImportExportPluginInterface *interface : qAsConst(mImportExportPluginInterfaceList)) {
                if (interface->canImportFileType(url)) {
                    interface->importFile(url);
                    break;
                }
            }
        }
    } else if (parser.isSet(QStringLiteral("newcontact"))) {
        newContact();
    }
}

void MainWidget::updateQuickSearchText()
{
    mQuickSearchWidget->updateQuickSearchText(i18nc("@label Search contacts in list", "Search... <%1>", mQuickSearchAction->shortcut().toString()));
}

void MainWidget::delayedInit()
{
    setViewMode(0);                                        // get default from settings

    const KConfigGroup group(Settings::self()->config(), "UiState_ContactView");
    KPIM::UiStateSaver::restoreState(mItemView, group);

    mXmlGuiClient->actionCollection()->action(QStringLiteral("options_show_qrcodes"))->setChecked(showQRCodes());

    connect(GlobalContactModel::instance()->model(), &QAbstractItemModel::modelAboutToBeReset,
            this, &MainWidget::saveState);
    connect(GlobalContactModel::instance()->model(), &QAbstractItemModel::modelReset,
            this, &MainWidget::restoreState);
    connect(qApp, &QApplication::aboutToQuit, this, &MainWidget::saveState);

    restoreState();
    updateQuickSearchText();
    initializePluginActions();
}

MainWidget::~MainWidget()
{
    mModelColumnManager->store();
    saveSplitterStates();

    KConfigGroup group(Settings::self()->config(), "UiState_ContactView");
    KPIM::UiStateSaver::saveState(mItemView, group);

    saveState();
    delete mGrantleeThemeManager;
    delete mFormatter;
    delete mGroupFormatter;

    Settings::self()->save();
}

void MainWidget::restoreState()
{
    // collection view
    {
        Akonadi::ETMViewStateSaver *saver = new Akonadi::ETMViewStateSaver;
        saver->setView(mCollectionView);

        const KConfigGroup group(Settings::self()->config(), "CollectionViewState");
        saver->restoreState(group);
    }

    // collection view
    {
        Akonadi::ETMViewStateSaver *saver = new Akonadi::ETMViewStateSaver;
        saver->setSelectionModel(mCollectionSelectionModel);

        const KConfigGroup group(Settings::self()->config(), "CollectionViewCheckState");
        saver->restoreState(group);
    }

    // item view
    {
        Akonadi::ETMViewStateSaver *saver = new Akonadi::ETMViewStateSaver;
        saver->setView(mItemView);
        saver->setSelectionModel(mItemView->selectionModel());

        const KConfigGroup group(Settings::self()->config(), "ItemViewState");
        saver->restoreState(group);
    }
}

void MainWidget::saveState()
{
    // collection view
    {
        Akonadi::ETMViewStateSaver saver;
        saver.setView(mCollectionView);

        KConfigGroup group(Settings::self()->config(), "CollectionViewState");
        saver.saveState(group);
        group.sync();
    }

    // collection view
    {
        Akonadi::ETMViewStateSaver saver;
        saver.setSelectionModel(mCollectionSelectionModel);

        KConfigGroup group(Settings::self()->config(), "CollectionViewCheckState");
        saver.saveState(group);
        group.sync();
    }

    // item view
    {
        Akonadi::ETMViewStateSaver saver;
        saver.setView(mItemView);
        saver.setSelectionModel(mItemView->selectionModel());

        KConfigGroup group(Settings::self()->config(), "ItemViewState");
        saver.saveState(group);
        group.sync();
    }
}

void MainWidget::setupGui()
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

    // the contact switcher for the simple gui mode
    mContactSwitcher = new ContactSwitcher;
    detailsPaneLayout->addWidget(mContactSwitcher);
    mContactSwitcher->setVisible(false);

    // the quick search widget which is embedded in the toolbar action
    mQuickSearchWidget = new QuickSearchWidget;
    mQuickSearchWidget->setMaximumWidth(500);

    // the category filter widget which is embedded in the toolbar action
    mCategorySelectWidget = new CategorySelectWidget;

    // setup the default actions
    Akonadi::ContactDefaultActions *actions = new Akonadi::ContactDefaultActions(this);
    actions->connectToView(mContactDetails);
    actions->connectToView(mContactGroupDetails);
    mFormatter = new KAddressBookGrantlee::GrantleeContactFormatter;

    mContactDetails->setContactFormatter(mFormatter);

    mGroupFormatter = new KAddressBookGrantlee::GrantleeContactGroupFormatter;

    mContactGroupDetails->setContactGroupFormatter(mGroupFormatter);
}

void MainWidget::initializePluginActions()
{
    KAddressBookPluginInterface::self()->initializePluginActions(QStringLiteral("kaddressbook"), mXmlGuiClient);
}

void MainWidget::slotImportExportActivated(PimCommon::AbstractGenericPluginInterface *interface)
{
    KAddressBookImportExport::KAddressBookImportExportPluginInterface *importExportInterface = static_cast<KAddressBookImportExport::KAddressBookImportExportPluginInterface *>(interface);
    if (importExportInterface) {
        importExportInterface->exec();
    }
}

void MainWidget::setupActions(KActionCollection *collection)
{
    KAddressBookPluginInterface::self()->setParentWidget(this);
    KAddressBookPluginInterface::self()->setMainWidget(this);
    KAddressBookPluginInterface::self()->createPluginInterface();

    mGrantleeThemeManager = new GrantleeTheme::ThemeManager(QStringLiteral("addressbook"),
                                                            QStringLiteral("theme.desktop"),
                                                            collection,
                                                            QStringLiteral("kaddressbook/viewertemplates/"), this);
    mGrantleeThemeManager->setDownloadNewStuffConfigFile(QStringLiteral(":/knsrc/data/kaddressbook_themes.knsrc"));
    connect(mGrantleeThemeManager, &GrantleeTheme::ThemeManager::grantleeThemeSelected, this, &MainWidget::slotGrantleeThemeSelected);
    connect(mGrantleeThemeManager, &GrantleeTheme::ThemeManager::updateThemes, this, &MainWidget::slotGrantleeThemesUpdated);

    KActionMenu *themeMenu = new KActionMenu(i18n("&Themes"), this);
    collection->addAction(QStringLiteral("theme_menu"), themeMenu);

    initGrantleeThemeName();
    QActionGroup *group = new QActionGroup(this);
    mGrantleeThemeManager->setThemeMenu(themeMenu);
    mGrantleeThemeManager->setActionGroup(group);

    QAction *action = KStandardAction::print(this, &MainWidget::print, collection);
    action->setWhatsThis(
        i18nc("@info:whatsthis",
              "Print the complete address book or a selected number of contacts."));

    KStandardAction::printPreview(this, &MainWidget::printPreview, collection);

    QWidgetAction *quicksearch = new QWidgetAction(this);
    quicksearch->setText(i18n("Quick search"));
    quicksearch->setDefaultWidget(mQuickSearchWidget);
    collection->addAction(QStringLiteral("quick_search"), quicksearch);

    QWidgetAction *categoryFilter = new QWidgetAction(this);
    categoryFilter->setText(i18n("Category filter"));
    categoryFilter->setDefaultWidget(mCategorySelectWidget);
    collection->addAction(QStringLiteral("category_filter"), categoryFilter);

    action = collection->addAction(QStringLiteral("select_all"));
    action->setText(i18n("Select All"));
    collection->setDefaultShortcut(action, QKeySequence(Qt::CTRL + Qt::Key_A));
    action->setWhatsThis(i18n("Select all contacts in the current address book view."));
    connect(action, &QAction::triggered, mItemView, &Akonadi::EntityTreeView::selectAll);

    KToggleAction *qrtoggleAction = collection->add<KToggleAction>(QStringLiteral("options_show_qrcodes"));
    qrtoggleAction->setText(i18n("Show QR Codes"));
    qrtoggleAction->setWhatsThis(i18n("Show QR Codes in the contact."));
    connect(qrtoggleAction, &KToggleAction::toggled, this, &MainWidget::setQRCodeShow);

    mViewModeGroup = new QActionGroup(this);

    QAction *act = new QAction(i18nc("@action:inmenu", "Simple (one column)"), mViewModeGroup);
    act->setCheckable(true);
    act->setData(1);
    collection->setDefaultShortcut(act, QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_1));
    act->setWhatsThis(i18n("Show a simple mode of the address book view."));
    collection->addAction(QStringLiteral("view_mode_simple"), act);

    act = new QAction(i18nc("@action:inmenu", "Two Columns"), mViewModeGroup);
    act->setCheckable(true);
    act->setData(2);
    collection->addAction(QStringLiteral("view_mode_2columns"), act);
    collection->setDefaultShortcut(act, QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_2));

    act = new QAction(i18nc("@action:inmenu", "Three Columns"), mViewModeGroup);
    act->setCheckable(true);
    act->setData(3);
    collection->setDefaultShortcut(act, QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_3));
    collection->addAction(QStringLiteral("view_mode_3columns"), act);

    connect(mViewModeGroup, QOverload<QAction *>::of(&QActionGroup::triggered), this, QOverload<QAction *>::of(&MainWidget::setViewMode));

    KToggleAction *actTheme = mGrantleeThemeManager->actionForTheme();
    if (actTheme) {
        actTheme->setChecked(true);
    }

    mQuickSearchAction = new QAction(i18n("Set Focus to Quick Search"), this);
    //If change shortcut change in quicksearchwidget->lineedit->setPlaceholderText
    collection->addAction(QStringLiteral("focus_to_quickseach"), mQuickSearchAction);
    connect(mQuickSearchAction, &QAction::triggered, mQuickSearchWidget, &QuickSearchWidget::slotFocusQuickSearch);
    collection->setDefaultShortcut(mQuickSearchAction, QKeySequence(Qt::ALT + Qt::Key_Q));

    if (!qEnvironmentVariableIsEmpty("KDEPIM_DEBUGGING")) {
        action = collection->addAction(QStringLiteral("debug_akonadi_search"));
        //Don't translate it. It's just for debug
        action->setText(QStringLiteral("Debug Akonadi Search..."));
        connect(action, &QAction::triggered, this, &MainWidget::slotDebugAkonadiSearch);
    }

    mServerSideSubscription = new QAction(QIcon::fromTheme(QStringLiteral("folder-bookmarks")), i18n("Serverside Subscription..."), this);
    collection->addAction(QStringLiteral("serverside_subscription"), mServerSideSubscription);
    connect(mServerSideSubscription, &QAction::triggered, this, &MainWidget::slotServerSideSubscription);
}

void MainWidget::printPreview()
{
    QPrinter printer;
    printer.setDocName(i18n("Address Book"));
    printer.setOutputFileName(Settings::self()->defaultFileName());
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setCollateCopies(true);

    QPointer<PimCommon::KPimPrintPreviewDialog> previewdlg = new PimCommon::KPimPrintPreviewDialog(&printer, this);
    KABPrinting::PrintingWizard wizard(&printer, mItemView->selectionModel(), this);
    wizard.setDefaultAddressBook(currentAddressBook());
    connect(previewdlg.data(), &QPrintPreviewDialog::paintRequested, this, [&wizard]() {
        wizard.print();
    });

    const int result = wizard.exec();
    if (result) {
        Settings::self()->setDefaultFileName(printer.outputFileName());
        Settings::self()->setPrintingStyle(wizard.printingStyle());
        Settings::self()->setSortOrder(wizard.sortOrder());
        previewdlg->exec();
    }
    delete previewdlg;
}

void MainWidget::print()
{
    QPrinter printer;
    printer.setDocName(i18n("Address Book"));
    printer.setOutputFileName(Settings::self()->defaultFileName());
    printer.setCollateCopies(true);

    QPointer<QPrintDialog> printDialog = new QPrintDialog(&printer, this);

    printDialog->setWindowTitle(i18n("Print Contacts"));
    if (!printDialog->exec() || !printDialog) {
        delete printDialog;
        return;
    }
    KABPrinting::PrintingWizard wizard(&printer, mItemView->selectionModel(), this);
    wizard.setDefaultAddressBook(currentAddressBook());

    wizard.exec(); //krazy:exclude=crashy

    Settings::self()->setDefaultFileName(printer.outputFileName());
    Settings::self()->setPrintingStyle(wizard.printingStyle());
    Settings::self()->setSortOrder(wizard.sortOrder());
}

void MainWidget::newContact()
{
    mActionManager->action(Akonadi::StandardContactActionManager::CreateContact)->trigger();
}

void MainWidget::newGroup()
{
    mActionManager->action(Akonadi::StandardContactActionManager::CreateContactGroup)->trigger();
}

/**
 * Depending on the mime type of the selected item, this method
 * brings up the right view on the detail view stack and sets the
 * selected item on it.
 */
void MainWidget::itemSelected(const Akonadi::Item &item)
{
    if (Akonadi::MimeTypeChecker::isWantedItem(item, KContacts::Addressee::mimeType())) {
        mDetailsViewStack->setCurrentWidget(mContactDetails);
        mContactDetails->setContact(item);
    } else if (Akonadi::MimeTypeChecker::isWantedItem(item, KContacts::ContactGroup::mimeType())) {
        mDetailsViewStack->setCurrentWidget(mContactGroupDetails);
        mContactGroupDetails->setContactGroup(item);
    }
}

/**
 * Catch when the selection has gone ( e.g. an empty address book has been selected )
 * clear the details view in this case.
 */
void MainWidget::itemSelectionChanged(const QModelIndex &current, const QModelIndex &)
{
    if (!current.isValid()) {
        mDetailsViewStack->setCurrentWidget(mEmptyDetails);
    }
}

void MainWidget::selectFirstItem()
{
    // Whenever the quick search has changed, we select the first item
    // in the item view, so that the detailsview is updated
    if (mItemView && mItemView->selectionModel()) {
        mItemView->selectionModel()->setCurrentIndex(mItemView->model()->index(0, 0),
                                                     QItemSelectionModel::Rows
                                                     |QItemSelectionModel::ClearAndSelect);
    }
}

bool MainWidget::showQRCodes()
{
    KConfig config(QStringLiteral("akonadi_contactrc"));
    KConfigGroup group(&config, QStringLiteral("View"));
    return group.readEntry("QRCodes", true);
}

void MainWidget::setQRCodeShow(bool on)
{
    // must write the configuration setting first before updating the view.
    KConfig config(QStringLiteral("akonadi_contactrc"));
    KConfigGroup group(&config, QStringLiteral("View"));
    group.writeEntry("QRCodes", on);
    group.sync();
    if (mDetailsViewStack->currentWidget() == mContactDetails) {
        mFormatter->setShowQRCode(on);
        mContactDetails->setShowQRCode(on);
    }
}

Akonadi::Item::List MainWidget::selectedItems()
{
    Akonadi::Item::List items;
    QPointer<KAddressBookImportExport::KAddressBookContactSelectionDialog> dlg
        = new KAddressBookImportExport::KAddressBookContactSelectionDialog(mItemView->selectionModel(), false, this);
    dlg->setDefaultAddressBook(currentAddressBook());
    if (!dlg->exec() || !dlg) {
        delete dlg;
        return items;
    }

    items = dlg->selectedItems();
    delete dlg;

    return items;
}

Akonadi::Collection MainWidget::currentAddressBook() const
{
    if (mCollectionView->selectionModel() && mCollectionView->selectionModel()->hasSelection()) {
        const QModelIndex index = mCollectionView->selectionModel()->selectedIndexes().first();
        const Akonadi::Collection collection
            = index.data(Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();

        return collection;
    }

    return Akonadi::Collection();
}

QAbstractItemModel *MainWidget::allContactsModel()
{
    if (!mAllContactsModel) {
        KDescendantsProxyModel *descendantsModel = new KDescendantsProxyModel(this);
        descendantsModel->setSourceModel(GlobalContactModel::instance()->model());

        mAllContactsModel = new Akonadi::EntityMimeTypeFilterModel(this);
        mAllContactsModel->setSourceModel(descendantsModel);
        mAllContactsModel->addMimeTypeExclusionFilter(Akonadi::Collection::mimeType());
        mAllContactsModel->setHeaderGroup(Akonadi::EntityTreeModel::ItemListHeaders);
    }

    return mAllContactsModel;
}

void MainWidget::setViewMode(QAction *action)
{
    setViewMode(action->data().toInt());
}

void MainWidget::setViewMode(int mode)
{
    int currentMode = Settings::self()->viewMode();
    //qCDebug(KADDRESSBOOK_LOG) << "cur" << currentMode << "new" << mode;
    if (mode == currentMode) {
        return;    // nothing to do
    }

    if (mode == 0) {
        mode = currentMode;// initialization, no save
    } else {
        saveSplitterStates();                                // for 2- or 3-column mode
    }
    if (mode == 1) {                                          // simple mode
        mMainWidgetSplitter2->setVisible(false);
        mDetailsPane->setVisible(true);
        mContactSwitcher->setVisible(true);
    } else {
        mMainWidgetSplitter2->setVisible(true);
        mContactSwitcher->setVisible(false);

        if (mode == 2) {                                          // 2 columns
            mMainWidgetSplitter2->setOrientation(Qt::Vertical);
        } else if (mode == 3) {                                // 3 columns
            mMainWidgetSplitter2->setOrientation(Qt::Horizontal);
        }
    }

    Settings::self()->setViewMode(mode);                  // save new mode in settings
    restoreSplitterStates();                                // restore state for new mode
    mViewModeGroup->actions().at(mode - 1)->setChecked(true);

    if (mItemView->model()) {
        mItemView->setCurrentIndex(mItemView->model()->index(0, 0));
    }
}

void MainWidget::saveSplitterStates() const
{
    // The splitter states are saved separately for each column view mode,
    // but only if not in simple mode (1 column).
    int currentMode = Settings::self()->viewMode();
    if (currentMode == 1) {
        return;
    }

    const QString groupName = QStringLiteral("UiState_MainWidgetSplitter_%1").arg(currentMode);
    //qCDebug(KADDRESSBOOK_LOG) << "saving to group" << groupName;
    KConfigGroup group(Settings::self()->config(), groupName);
    KPIM::UiStateSaver::saveState(mMainWidgetSplitter1, group);
    KPIM::UiStateSaver::saveState(mMainWidgetSplitter2, group);
}

void MainWidget::restoreSplitterStates()
{
    // The splitter states are restored as appropriate for the current
    // column view mode, but not for simple mode (1 column).
    int currentMode = Settings::self()->viewMode();
    if (currentMode == 1) {
        return;
    }

    const QString groupName = QStringLiteral("UiState_MainWidgetSplitter_%1").arg(currentMode);
    //qCDebug(KADDRESSBOOK_LOG) << "restoring from group" << groupName;
    KConfigGroup group(Settings::self()->config(), groupName);
    KPIM::UiStateSaver::restoreState(mMainWidgetSplitter1, group);
    KPIM::UiStateSaver::restoreState(mMainWidgetSplitter2, group);
}

void MainWidget::initGrantleeThemeName()
{
    QString themeName = mGrantleeThemeManager->configuredThemeName();
    if (themeName.isEmpty()) {
        themeName = QStringLiteral("default");
    }
    mFormatter->setGrantleeTheme(mGrantleeThemeManager->theme(themeName));
    mGroupFormatter->setGrantleeTheme(mGrantleeThemeManager->theme(themeName));
}

void MainWidget::slotGrantleeThemeSelected()
{
    initGrantleeThemeName();
    if (mItemView->model()) {
        mItemView->setCurrentIndex(mItemView->model()->index(0, 0));
    }
}

void MainWidget::slotGrantleeThemesUpdated()
{
    if (mItemView->model()) {
        mItemView->setCurrentIndex(mItemView->model()->index(0, 0));
    }
}

Akonadi::EntityTreeModel *MainWidget::entityTreeModel() const
{
    QAbstractProxyModel *proxy = qobject_cast<QAbstractProxyModel *>(mCollectionView->model());
    while (proxy) {
        Akonadi::EntityTreeModel *etm = qobject_cast<Akonadi::EntityTreeModel *>(proxy->sourceModel());
        if (etm) {
            return etm;
        }
        proxy = qobject_cast<QAbstractProxyModel *>(proxy->sourceModel());
    }

    qCWarning(KADDRESSBOOK_LOG) << "Couldn't find EntityTreeModel";
    return nullptr;
}

void MainWidget::slotCheckNewCalendar(const QModelIndex &parent, int begin, int end)
{
    // HACK: Check newly created calendars

    if (begin < end) {
        return;
    }

    Akonadi::EntityTreeModel *etm = entityTreeModel();
    QAbstractItemModel *model = mCollectionView->model();
    if (etm && etm->isCollectionTreeFetched()) {
        for (int row = begin; row <= end; ++row) {
            QModelIndex index = model->index(row, 0, parent);
            if (index.isValid()) {
                model->setData(index, Qt::Checked, Qt::CheckStateRole);
                slotCheckNewCalendar(index, 0, model->rowCount(index) - 1);
            }
        }
        if (parent.isValid()) {
            mCollectionView->setExpanded(parent, true);
        }
    }
}

const Akonadi::Item::List MainWidget::collectSelectedAllContactsItem()
{
    return collectSelectedAllContactsItem(mItemView->selectionModel());
}

void MainWidget::slotDebugAkonadiSearch()
{
    const Akonadi::Item::List lst = collectSelectedAllContactsItem(mItemView->selectionModel());
    if (lst.isEmpty()) {
        return;
    }
    QPointer<Akonadi::Search::AkonadiSearchDebugDialog> dlg = new Akonadi::Search::AkonadiSearchDebugDialog;
    dlg->setAkonadiId(lst.at(0).id());
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setSearchType(Akonadi::Search::AkonadiSearchDebugSearchPathComboBox::Contacts);
    dlg->doSearch();
    dlg->show();
}

const Akonadi::Item::List MainWidget::collectSelectedAllContactsItem(QItemSelectionModel *model)
{
    Akonadi::Item::List lst;

    const QModelIndexList indexes = model->selectedRows(0);
    for (int i = 0; i < indexes.count(); ++i) {
        const QModelIndex index = indexes.at(i);
        if (index.isValid()) {
            const Akonadi::Item item
                = index.data(Akonadi::EntityTreeModel::ItemRole).value<Akonadi::Item>();
            if (item.isValid()) {
                if (item.hasPayload<KContacts::Addressee>() || item.hasPayload<KContacts::ContactGroup>()) {
                    lst.append(item);
                }
            }
        }
    }
    return lst;
}

void MainWidget::slotServerSideSubscription()
{
    Akonadi::Collection collection = currentAddressBook();
    if (collection.isValid()) {
        PimCommon::ManageServerSideSubscriptionJob *job = new PimCommon::ManageServerSideSubscriptionJob(this);
        job->setCurrentCollection(collection);
        job->setParentWidget(this);
        job->start();
    }
}

void MainWidget::slotCurrentCollectionChanged(const Akonadi::Collection &col)
{
    for (KAddressBookImportExport::KAddressBookImportExportPluginInterface *interface : qAsConst(mImportExportPluginInterfaceList)) {
        interface->setDefaultCollection(col);
    }
    bool isOnline;
    mServerSideSubscription->setEnabled(PimCommon::MailUtil::isImapFolder(col, isOnline));
}
