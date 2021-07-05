/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "mainwidget.h"

#include "categoryfilterproxymodel.h"
#include "categoryselectwidget.h"
#include "contactinfoproxymodel.h"
#include "contactswitcher.h"
#include "globalcontactmodel.h"
#include "kaddressbook_options.h"
#include "kaddressbookadaptor.h"
#include "kcmutils_version.h"
#include "manageshowcollectionproperties.h"
#include "modelcolumnmanager.h"
#include "printing/printingwizard.h"
#include "settings.h"
#include "stylecontactlistdelegate.h"
#include "widgets/quicksearchwidget.h"

#include "importexport/contactselectiondialog.h"
#include "importexport/plugin.h"
#include "importexport/plugininterface.h"
#include "importexport/pluginmanager.h"

#include <Akonadi/Contact/GrantleeContactFormatter>
#include <Akonadi/Contact/GrantleeContactGroupFormatter>
#include <GrantleeTheme/GrantleeThemeManager>

#include "uistatesaver.h"

#include <PimCommonAkonadi/ImapAclAttribute>
#include <PimCommonAkonadi/MailUtil>

#include <AkonadiCore/AttributeFactory>
#include <AkonadiCore/CollectionFilterProxyModel>
#include <AkonadiCore/EntityMimeTypeFilterModel>
#include <AkonadiCore/MimeTypeChecker>
#include <AkonadiSearch/Debug/akonadisearchdebugdialog.h>
#include <AkonadiWidgets/CollectionMaintenancePage>
#include <AkonadiWidgets/CollectionPropertiesDialog>
#include <AkonadiWidgets/ControlGui>
#include <AkonadiWidgets/ETMViewStateSaver>
#include <AkonadiWidgets/EntityTreeView>
#include <AkonadiWidgets/ItemView>
#include <KContacts/Addressee>
#include <PimCommon/PimUtil>
#include <PimCommonAkonadi/ManageServerSideSubscriptionJob>
#include <QPointer>

#include <Akonadi/Contact/ContactDefaultActions>
#include <Akonadi/Contact/ContactGroupEditorDialog>
#include <Akonadi/Contact/ContactGroupViewer>
#include <Akonadi/Contact/ContactViewer>
#include <Akonadi/Contact/ContactsFilterProxyModel>
#include <Akonadi/Contact/ContactsTreeModel>
#include <Akonadi/Contact/StandardContactActionManager>

#include "kaddressbook_debug.h"
#include <KActionCollection>
#include <KActionMenu>
#include <KCMultiDialog>
#include <KCheckableProxyModel>
#include <KContacts/ContactGroup>
#include <KDescendantsProxyModel>
#include <KLocalizedString>
#include <KPluginLoader>
#include <KPluginMetaData>
#include <KSelectionProxyModel>
#include <KToggleAction>
#include <KXMLGUIClient>
#include <QAction>
#include <QApplication>
#include <QTextBrowser>

#include <ItemModifyJob>
#include <KPimPrintPreviewDialog>
#include <QActionGroup>
#include <QDBusConnection>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QSplitter>
#include <QStackedWidget>

#include "plugininterface/kaddressbookplugininterface.h"

namespace
{
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
            const auto collection = index.data(Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
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
    auto checkableProxyModel = new StructuralCollectionsNotCheckableProxy(this);
    checkableProxyModel->setSelectionModel(mCollectionSelectionModel);
    checkableProxyModel->setSourceModel(mCollectionTree);

    mCollectionView->setModel(checkableProxyModel);
    mCollectionView->setXmlGuiClient(guiClient);
    mCollectionView->header()->setDefaultAlignment(Qt::AlignCenter);
    mCollectionView->header()->setSortIndicatorShown(false);

    connect(mCollectionView->model(), &QAbstractItemModel::rowsInserted, this, &MainWidget::slotCheckNewCalendar);

    connect(mCollectionView, qOverload<const Akonadi::Collection &>(&Akonadi::EntityTreeView::currentChanged), this, &MainWidget::slotCurrentCollectionChanged);

    auto selectionProxyModel = new KSelectionProxyModel(mCollectionSelectionModel, this);
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

    connect(mCategorySelectWidget, &CategorySelectWidget::filterChanged, mCategoryFilterModel, &CategoryFilterProxyModel::setFilterCategories);

    mContactsFilterModel = new Akonadi::ContactsFilterProxyModel(this);
    mContactsFilterModel->setSourceModel(mCategoryFilterModel);

    auto contactInfoProxyModel = new ContactInfoProxyModel(this);
    contactInfoProxyModel->setSourceModel(mContactsFilterModel);

    connect(mQuickSearchWidget, &QuickSearchWidget::filterStringChanged, mContactsFilterModel, &Akonadi::ContactsFilterProxyModel::setFilterString);
    connect(mQuickSearchWidget, &QuickSearchWidget::filterStringChanged, this, &MainWidget::selectFirstItem);
    connect(mQuickSearchWidget, &QuickSearchWidget::arrowDownKeyPressed, this, &MainWidget::setFocusToTreeView);
    mItemView->setModel(contactInfoProxyModel);
    mItemView->setItemDelegate(new StyleContactListDelegate(this));
    mItemView->setXmlGuiClient(guiClient);
    mItemView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mItemView->setRootIsDecorated(false);
    mItemView->header()->setDefaultAlignment(Qt::AlignCenter);

    mActionManager = new Akonadi::StandardContactActionManager(guiClient->actionCollection(), this);
    mActionManager->setCollectionSelectionModel(mCollectionView->selectionModel());
    mActionManager->setItemSelectionModel(mItemView->selectionModel());

    QList<Akonadi::StandardActionManager::Type> standardActions;
    standardActions << Akonadi::StandardActionManager::CreateCollection << Akonadi::StandardActionManager::DeleteCollections
                    << Akonadi::StandardActionManager::SynchronizeCollections << Akonadi::StandardActionManager::CollectionProperties
                    << Akonadi::StandardActionManager::CopyItems << Akonadi::StandardActionManager::Paste << Akonadi::StandardActionManager::DeleteItems
                    << Akonadi::StandardActionManager::CutItems << Akonadi::StandardActionManager::CreateResource
                    << Akonadi::StandardActionManager::DeleteResources << Akonadi::StandardActionManager::ResourceProperties
                    << Akonadi::StandardActionManager::SynchronizeResources << Akonadi::StandardActionManager::SynchronizeCollectionsRecursive
                    << Akonadi::StandardActionManager::MoveItemToMenu << Akonadi::StandardActionManager::CopyItemToMenu;

    for (Akonadi::StandardActionManager::Type standardAction : std::as_const(standardActions)) {
        mActionManager->createAction(standardAction);
    }
    guiClient->actionCollection()->setDefaultShortcut(mActionManager->action(Akonadi::StandardActionManager::DeleteItems), QKeySequence(Qt::Key_Delete));
    QList<Akonadi::StandardContactActionManager::Type> contactActions;
    contactActions << Akonadi::StandardContactActionManager::CreateContact << Akonadi::StandardContactActionManager::CreateContactGroup
                   << Akonadi::StandardContactActionManager::EditItem;

    for (Akonadi::StandardContactActionManager::Type contactAction : std::as_const(contactActions)) {
        mActionManager->createAction(contactAction);
    }

    mActionManager->interceptAction(Akonadi::StandardActionManager::CollectionProperties);
    connect(mActionManager->action(Akonadi::StandardActionManager::CollectionProperties),
            &QAction::triggered,
            mManageShowCollectionProperties,
            &ManageShowCollectionProperties::showCollectionProperties);

    connect(mItemView, qOverload<const Akonadi::Item &>(&Akonadi::EntityTreeView::currentChanged), this, &MainWidget::itemSelected);
    connect(mItemView,
            qOverload<const Akonadi::Item &>(&Akonadi::EntityTreeView::doubleClicked),
            mActionManager->action(Akonadi::StandardContactActionManager::EditItem),
            &QAction::trigger);
    connect(mItemView->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWidget::itemSelectionChanged);

    // show the contact details view as default
    mDetailsViewStack->setCurrentWidget(mContactDetails);

    mContactSwitcher->setView(mItemView);

    Akonadi::ControlGui::widgetNeedsAkonadi(this);

    mModelColumnManager = new ModelColumnManager(GlobalContactModel::instance()->model(), this);
    mModelColumnManager->setWidget(mItemView->header());
    mModelColumnManager->load();

    initializeImportExportPlugin(guiClient->actionCollection());
    QMetaObject::invokeMethod(this, &MainWidget::delayedInit, Qt::QueuedConnection);
    updateQuickSearchText();
}

void MainWidget::setFocusToTreeView()
{
    mItemView->setFocus();
}

void MainWidget::initializeImportExportPlugin(KActionCollection *collection)
{
    const QVector<KAddressBookImportExport::Plugin *> listPlugins = KAddressBookImportExport::PluginManager::self()->pluginsList();
    QList<QAction *> importActions;
    QList<QAction *> exportActions;
    for (KAddressBookImportExport::Plugin *plugin : listPlugins) {
        if (plugin->isEnabled()) {
            auto interface = static_cast<KAddressBookImportExport::PluginInterface *>(plugin->createInterface(this));
            interface->setItemSelectionModel(mItemView->selectionModel());
            interface->setParentWidget(this);
            interface->createAction(collection);
            importActions.append(interface->importActions());
            exportActions.append(interface->exportActions());
            mImportExportPluginInterfaceList.append(interface);
            connect(interface, &PimCommon::AbstractGenericPluginInterface::emitPluginActivated, this, &MainWidget::slotImportExportActivated);
        }
    }

    if (!importActions.isEmpty()) {
        auto importMenu = new KActionMenu(i18n("Import"), this);
        collection->addAction(QStringLiteral("import_menu"), importMenu);
        for (QAction *act : std::as_const(importActions)) {
            importMenu->addAction(act);
        }
    }
    if (!exportActions.isEmpty()) {
        auto exportMenu = new KActionMenu(i18n("Export"), this);
        collection->addAction(QStringLiteral("export_menu"), exportMenu);
        for (QAction *act : std::as_const(exportActions)) {
            exportMenu->addAction(act);
        }
    }
}

void MainWidget::configure()
{
    QPointer<KCMultiDialog> dlg = new KCMultiDialog(this);
    const QVector<KPluginMetaData> availablePlugins = KPluginLoader::findPlugins(QStringLiteral("pim/kcms/kaddressbook"));
    for (const KPluginMetaData &metaData : availablePlugins) {
#if KCMUTILS_VERSION >= QT_VERSION_CHECK(5, 84, 0)
        dlg->addModule(metaData);
#else
        dlg->addModule(metaData.pluginId());
#endif
    }
    dlg->exec();
    delete dlg;
}

void MainWidget::handleCommandLine(const QStringList &arguments)
{
    QCommandLineParser parser;
    kaddressbook_options(&parser);
    parser.process(arguments);

    if (parser.isSet(QStringLiteral("import"))) {
        const QStringList lst = parser.positionalArguments();
        for (const QString &urlStr : lst) {
            const QUrl url(QUrl::fromUserInput(urlStr));
            for (KAddressBookImportExport::PluginInterface *interface : std::as_const(mImportExportPluginInterfaceList)) {
                if (interface->canImportFileType(url)) {
                    interface->importFile(url);
                    break;
                }
            }
        }
    } else if (parser.isSet(QStringLiteral("newcontact"))) {
        newContact();
    } else if (parser.isSet(QStringLiteral("view"))) {
        const auto url = QUrl{parser.value(QStringLiteral("view"))};
        mPendingSelection = Akonadi::Item::fromUrl(url);
    }
}

void MainWidget::updateQuickSearchText()
{
    mQuickSearchWidget->updateQuickSearchText(i18nc("@label Search contacts in list", "Search... <%1>", mQuickSearchAction->shortcut().toString()));
}

void MainWidget::delayedInit()
{
    setViewMode(0); // get default from settings

    const KConfigGroup group(Settings::self()->config(), "UiState_ContactView");
    KAddressBook::UiStateSaver::restoreState(mItemView, group);

    mXmlGuiClient->actionCollection()->action(QStringLiteral("options_show_qrcodes"))->setChecked(showQRCodes());

    connect(GlobalContactModel::instance()->model(), &QAbstractItemModel::modelAboutToBeReset, this, &MainWidget::saveState);
    connect(GlobalContactModel::instance()->model(), &QAbstractItemModel::modelReset, this, &MainWidget::restoreState);
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
    KAddressBook::UiStateSaver::saveState(mItemView, group);

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
        auto saver = new Akonadi::ETMViewStateSaver;
        saver->setView(mCollectionView);

        const KConfigGroup group(Settings::self()->config(), "CollectionViewState");
        saver->restoreState(group);
    }

    // collection view
    {
        auto saver = new Akonadi::ETMViewStateSaver;
        saver->setSelectionModel(mCollectionSelectionModel);

        const KConfigGroup group(Settings::self()->config(), "CollectionViewCheckState");
        saver->restoreState(group);
    }

    // item view
    {
        auto saver = new Akonadi::ETMViewStateSaver;
        saver->setView(mItemView);
        saver->setSelectionModel(mItemView->selectionModel());

        if (mPendingSelection.isValid()) {
            saver->selectItems({mPendingSelection});
            saver->setCurrentItem(mPendingSelection);
            mPendingSelection = {};
        } else {
            const KConfigGroup group(Settings::self()->config(), "ItemViewState");
            saver->restoreState(group);
        }
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
    auto layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

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

    mMainWidgetSplitter1->setStretchFactor(1, 9); // maximum width for detail
    mMainWidgetSplitter2->setStretchFactor(1, 9); // for intuitive resizing
    mMainWidgetSplitter2->setChildrenCollapsible(false);
    mMainWidgetSplitter1->setChildrenCollapsible(false);

    auto detailsPaneLayout = new QVBoxLayout(mDetailsPane);
    detailsPaneLayout->setContentsMargins(0, 0, 0, 0);

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
    auto actions = new Akonadi::ContactDefaultActions(this);
    actions->connectToView(mContactDetails);
    actions->connectToView(mContactGroupDetails);
    mFormatter = new KAddressBookGrantlee::GrantleeContactFormatter;
    mFormatter->setApplicationDomain("kaddressbook");

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
    auto importExportInterface = static_cast<KAddressBookImportExport::PluginInterface *>(interface);
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
                                                            QStringLiteral("kaddressbook/viewertemplates/"),
                                                            this);
    mGrantleeThemeManager->setDownloadNewStuffConfigFile(QStringLiteral(":/knsrc/data/kaddressbook_themes.knsrc"));
    connect(mGrantleeThemeManager, &GrantleeTheme::ThemeManager::grantleeThemeSelected, this, &MainWidget::slotGrantleeThemeSelected);
    connect(mGrantleeThemeManager, &GrantleeTheme::ThemeManager::updateThemes, this, &MainWidget::slotGrantleeThemesUpdated);

    auto themeMenu = new KActionMenu(i18n("&Themes"), this);
    collection->addAction(QStringLiteral("theme_menu"), themeMenu);

    initGrantleeThemeName();
    auto group = new QActionGroup(this);
    mGrantleeThemeManager->setThemeMenu(themeMenu);
    mGrantleeThemeManager->setActionGroup(group);

    QAction *action = KStandardAction::print(this, &MainWidget::print, collection);
    action->setWhatsThis(i18nc("@info:whatsthis", "Print the complete address book or a selected number of contacts."));

    KStandardAction::printPreview(this, &MainWidget::printPreview, collection);

    auto quicksearch = new QWidgetAction(this);
    quicksearch->setText(i18n("Quick search"));
    quicksearch->setDefaultWidget(mQuickSearchWidget);
    collection->addAction(QStringLiteral("quick_search"), quicksearch);

    auto categoryFilter = new QWidgetAction(this);
    categoryFilter->setText(i18n("Category filter"));
    categoryFilter->setDefaultWidget(mCategorySelectWidget);
    collection->addAction(QStringLiteral("category_filter"), categoryFilter);

    action = collection->addAction(QStringLiteral("select_all"));
    action->setText(i18n("Select All"));
    collection->setDefaultShortcut(action, QKeySequence(Qt::CTRL | Qt::Key_A));
    action->setWhatsThis(i18n("Select all contacts in the current address book view."));
    connect(action, &QAction::triggered, mItemView, &Akonadi::EntityTreeView::selectAll);

    auto qrtoggleAction = collection->add<KToggleAction>(QStringLiteral("options_show_qrcodes"));
    qrtoggleAction->setText(i18n("Show QR Codes"));
    qrtoggleAction->setWhatsThis(i18n("Show QR Codes in the contact."));
    connect(qrtoggleAction, &KToggleAction::toggled, this, &MainWidget::setQRCodeShow);

    mViewModeGroup = new QActionGroup(this);

    auto act = new QAction(i18nc("@action:inmenu", "Simple (one column)"), mViewModeGroup);
    act->setCheckable(true);
    act->setData(1);
    collection->setDefaultShortcut(act, QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_1));
    act->setWhatsThis(i18n("Show a simple mode of the address book view."));
    collection->addAction(QStringLiteral("view_mode_simple"), act);

    act = new QAction(i18nc("@action:inmenu", "Two Columns"), mViewModeGroup);
    act->setCheckable(true);
    act->setData(2);
    collection->addAction(QStringLiteral("view_mode_2columns"), act);
    collection->setDefaultShortcut(act, QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_2));

    act = new QAction(i18nc("@action:inmenu", "Three Columns"), mViewModeGroup);
    act->setCheckable(true);
    act->setData(3);
    collection->setDefaultShortcut(act, QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_3));
    collection->addAction(QStringLiteral("view_mode_3columns"), act);

    connect(mViewModeGroup, qOverload<QAction *>(&QActionGroup::triggered), this, qOverload<QAction *>(&MainWidget::setViewMode));

    KToggleAction *actTheme = mGrantleeThemeManager->actionForTheme();
    if (actTheme) {
        actTheme->setChecked(true);
    }

    mQuickSearchAction = new QAction(i18n("Set Focus to Quick Search"), this);
    // If change shortcut change in quicksearchwidget->lineedit->setPlaceholderText
    collection->addAction(QStringLiteral("focus_to_quickseach"), mQuickSearchAction);
    connect(mQuickSearchAction, &QAction::triggered, mQuickSearchWidget, &QuickSearchWidget::slotFocusQuickSearch);
    collection->setDefaultShortcut(mQuickSearchAction, QKeySequence(Qt::ALT | Qt::Key_Q));

    if (!qEnvironmentVariableIsEmpty("KDEPIM_DEBUGGING")) {
        action = collection->addAction(QStringLiteral("debug_akonadi_search"));
        // Don't translate it. It's just for debug
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

    printDialog->setWindowTitle(i18nc("@title:window", "Print Contacts"));
    if (!printDialog->exec() || !printDialog) {
        delete printDialog;
        return;
    }
    KABPrinting::PrintingWizard wizard(&printer, mItemView->selectionModel(), this);
    wizard.setDefaultAddressBook(currentAddressBook());

    wizard.exec(); // krazy:exclude=crashy

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
        mItemView->selectionModel()->setCurrentIndex(mItemView->model()->index(0, 0), QItemSelectionModel::Rows | QItemSelectionModel::ClearAndSelect);
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
    QPointer<KAddressBookImportExport::ContactSelectionDialog> dlg =
        new KAddressBookImportExport::ContactSelectionDialog(mItemView->selectionModel(), false, this);
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
        const auto collection = index.data(Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();

        return collection;
    }

    return Akonadi::Collection();
}

QAbstractItemModel *MainWidget::allContactsModel()
{
    if (!mAllContactsModel) {
        auto descendantsModel = new KDescendantsProxyModel(this);
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
    // qCDebug(KADDRESSBOOK_LOG) << "cur" << currentMode << "new" << mode;
    if (mode == currentMode) {
        return; // nothing to do
    }

    if (mode == 0) {
        mode = currentMode; // initialization, no save
    } else {
        saveSplitterStates(); // for 2- or 3-column mode
    }
    if (mode == 1) { // simple mode
        mMainWidgetSplitter2->setVisible(false);
        mDetailsPane->setVisible(true);
        mContactSwitcher->setVisible(true);
    } else {
        mMainWidgetSplitter2->setVisible(true);
        mContactSwitcher->setVisible(false);

        if (mode == 2) { // 2 columns
            mMainWidgetSplitter2->setOrientation(Qt::Vertical);
        } else if (mode == 3) { // 3 columns
            mMainWidgetSplitter2->setOrientation(Qt::Horizontal);
        }
    }

    Settings::self()->setViewMode(mode); // save new mode in settings
    restoreSplitterStates(); // restore state for new mode
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
    // qCDebug(KADDRESSBOOK_LOG) << "saving to group" << groupName;
    KConfigGroup group(Settings::self()->config(), groupName);
    KAddressBook::UiStateSaver::saveState(mMainWidgetSplitter1, group);
    KAddressBook::UiStateSaver::saveState(mMainWidgetSplitter2, group);
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
    // qCDebug(KADDRESSBOOK_LOG) << "restoring from group" << groupName;
    KConfigGroup group(Settings::self()->config(), groupName);
    KAddressBook::UiStateSaver::restoreState(mMainWidgetSplitter1, group);
    KAddressBook::UiStateSaver::restoreState(mMainWidgetSplitter2, group);
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
    auto proxy = qobject_cast<QAbstractProxyModel *>(mCollectionView->model());
    while (proxy) {
        auto etm = qobject_cast<Akonadi::EntityTreeModel *>(proxy->sourceModel());
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
            const auto item = index.data(Akonadi::EntityTreeModel::ItemRole).value<Akonadi::Item>();
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
        auto job = new PimCommon::ManageServerSideSubscriptionJob(this);
        job->setCurrentCollection(collection);
        job->setParentWidget(this);
        job->start();
    }
}

void MainWidget::slotCurrentCollectionChanged(const Akonadi::Collection &col)
{
    for (auto interface : std::as_const(mImportExportPluginInterfaceList)) {
        interface->setDefaultCollection(col);
    }
    bool isOnline;
    mServerSideSubscription->setEnabled(PimCommon::MailUtil::isImapFolder(col, isOnline));
}
