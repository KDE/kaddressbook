/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "kaddressbook_export.h"
#include <AkonadiCore/Item>
#include <QItemSelection>
#include <QWidget>

namespace GrantleeTheme
{
class ThemeManager;
}
class QAction;
namespace Akonadi
{
class Collection;
class ContactGroupViewer;
class ContactViewer;
class ContactsFilterProxyModel;
class EntityMimeTypeFilterModel;
class EntityTreeView;
class Item;
class StandardContactActionManager;
class EntityTreeModel;
}
namespace PimCommon
{
class AbstractGenericPluginInterface;
}

namespace KAddressBookGrantlee
{
class GrantleeContactFormatter;
class GrantleeContactGroupFormatter;
}
namespace KAddressBookImportExport
{
class PluginInterface;
}
class ContactSwitcher;
class KActionCollection;
class KXMLGUIClient;
class ModelColumnManager;
class QAbstractItemModel;
class QItemSelectionModel;
class QModelIndex;
class QSplitter;
class QStackedWidget;
class QuickSearchWidget;
class QActionGroup;
class CategorySelectWidget;
class CategoryFilterProxyModel;
class ManageShowCollectionProperties;
class KADDRESSBOOK_EXPORT MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(KXMLGUIClient *guiClient, QWidget *parent = nullptr);
    ~MainWidget() override;

    void updateQuickSearchText();
    const Akonadi::Item::List collectSelectedAllContactsItem();

    /**
     * Returns the address book collection that is currently
     * selected by the user or an invalid collection if no
     * address book is selected.
     */
    Akonadi::Collection currentAddressBook() const;

    Akonadi::Item::List selectedItems();
    void initializePluginActions();

public Q_SLOTS:
    void newContact();
    void newGroup();
    void print();
    void printPreview();
    void configure();

    void handleCommandLine(const QStringList &arguments);

private Q_SLOTS:
    void slotImportExportActivated(PimCommon::AbstractGenericPluginInterface *interface);
    void delayedInit();

    void itemSelected(const Akonadi::Item &item);
    void itemSelectionChanged(const QModelIndex &, const QModelIndex &);
    void selectFirstItem();

    void setViewMode(QAction *action);
    void setQRCodeShow(bool on);

    void restoreState();
    void saveState();

    void slotGrantleeThemeSelected();
    void slotGrantleeThemesUpdated();
    void slotCheckNewCalendar(const QModelIndex &parent, int begin, int end);

    void slotDebugAkonadiSearch();
    void slotServerSideSubscription();
    void slotCurrentCollectionChanged(const Akonadi::Collection &col);
    void setFocusToTreeView();

private:
    const Akonadi::Item::List collectSelectedAllContactsItem(QItemSelectionModel *model);
    void initGrantleeThemeName();
    void setupGui();
    void setupActions(KActionCollection *);
    bool showQRCodes();
    void setViewMode(int mode);
    void saveSplitterStates() const;
    void restoreSplitterStates();
    void initializeImportExportPlugin(KActionCollection *collection);
    Akonadi::EntityTreeModel *entityTreeModel() const;

    QAbstractItemModel *allContactsModel();

    Akonadi::EntityMimeTypeFilterModel *mCollectionTree = nullptr;
    Akonadi::EntityMimeTypeFilterModel *mItemTree = nullptr;
    Akonadi::EntityMimeTypeFilterModel *mAllContactsModel = nullptr;
    Akonadi::ContactsFilterProxyModel *mContactsFilterModel = nullptr;
    CategoryFilterProxyModel *mCategoryFilterModel = nullptr;

    QuickSearchWidget *mQuickSearchWidget = nullptr;
    Akonadi::EntityTreeView *mCollectionView = nullptr;
    Akonadi::EntityTreeView *mItemView = nullptr;
    CategorySelectWidget *mCategorySelectWidget = nullptr;

    QWidget *mDetailsPane = nullptr;
    QStackedWidget *mDetailsViewStack = nullptr;
    ContactSwitcher *mContactSwitcher = nullptr;

    QSplitter *mMainWidgetSplitter1 = nullptr;
    QSplitter *mMainWidgetSplitter2 = nullptr;
    Akonadi::ContactViewer *mContactDetails = nullptr;
    Akonadi::ContactGroupViewer *mContactGroupDetails = nullptr;
    QWidget *mEmptyDetails = nullptr;
    Akonadi::StandardContactActionManager *mActionManager = nullptr;
    QItemSelectionModel *mCollectionSelectionModel = nullptr;

    QActionGroup *mViewModeGroup = nullptr;

    ModelColumnManager *mModelColumnManager = nullptr;
    KXMLGUIClient *mXmlGuiClient = nullptr;
    KAddressBookGrantlee::GrantleeContactFormatter *mFormatter = nullptr;
    KAddressBookGrantlee::GrantleeContactGroupFormatter *mGroupFormatter = nullptr;
    GrantleeTheme::ThemeManager *mGrantleeThemeManager = nullptr;
    QAction *mQuickSearchAction = nullptr;
    QAction *mServerSideSubscription = nullptr;
    QList<KAddressBookImportExport::PluginInterface *> mImportExportPluginInterfaceList;
    ManageShowCollectionProperties *mManageShowCollectionProperties = nullptr;

    Akonadi::Item mPendingSelection;
};

