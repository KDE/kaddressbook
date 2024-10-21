/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "kaddressbook_export.h"
#include <Akonadi/Item>
#include <QItemSelection>
#include <QWidget>
class ContactEntityMimeTypeFilterModel;

namespace GrantleeTheme
{
class ThemeManager;
}
class QAction;
namespace Akonadi
{
class ContactViewer;
class ContactGroupViewer;
class StandardContactActionManager;
}
namespace Akonadi
{
class Collection;
class ContactsFilterProxyModel;
class EntityMimeTypeFilterModel;
class EntityTreeView;
class Item;
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
    [[nodiscard]] const Akonadi::Item::List collectSelectedAllContactsItem();

    /**
     * Returns the address book collection that is currently
     * selected by the user or an invalid collection if no
     * address book is selected.
     */
    [[nodiscard]] Akonadi::Collection currentAddressBook() const;

    [[nodiscard]] Akonadi::Item::List selectedItems(bool &canceled);
    void initializePluginActions();

    [[nodiscard]] bool canClose() const;

public Q_SLOTS:
    void newContact();
    void newGroup();
    void print();
    void printPreview();
    void configure();

    void handleCommandLine(const QStringList &arguments);

protected:
    [[nodiscard]] bool event(QEvent *e) override;

private:
    KADDRESSBOOK_NO_EXPORT void slotImportExportActivated(PimCommon::AbstractGenericPluginInterface *interface);
    KADDRESSBOOK_NO_EXPORT void delayedInit();

    KADDRESSBOOK_NO_EXPORT void itemSelected(const Akonadi::Item &item);
    KADDRESSBOOK_NO_EXPORT void itemSelectionChanged(const QModelIndex &, const QModelIndex &);
    KADDRESSBOOK_NO_EXPORT void selectFirstItem();

    KADDRESSBOOK_NO_EXPORT void setActivateViewMode(QAction *action);
    KADDRESSBOOK_NO_EXPORT void setQRCodeShow(bool on);

    KADDRESSBOOK_NO_EXPORT void restoreState();
    KADDRESSBOOK_NO_EXPORT void saveState();

    KADDRESSBOOK_NO_EXPORT void slotGrantleeThemeSelected();
    KADDRESSBOOK_NO_EXPORT void slotGrantleeThemesUpdated();
    KADDRESSBOOK_NO_EXPORT void slotCheckNewCalendar(const QModelIndex &parent, int begin, int end);

    KADDRESSBOOK_NO_EXPORT void slotDebugAkonadiSearch();
    KADDRESSBOOK_NO_EXPORT void slotServerSideSubscription();
    KADDRESSBOOK_NO_EXPORT void slotCurrentCollectionChanged(const Akonadi::Collection &col);
    KADDRESSBOOK_NO_EXPORT void setFocusToTreeView();

    KADDRESSBOOK_NO_EXPORT const Akonadi::Item::List collectSelectedAllContactsItem(QItemSelectionModel *model);
    KADDRESSBOOK_NO_EXPORT void initGrantleeThemeName();
    KADDRESSBOOK_NO_EXPORT void setupGui();
    KADDRESSBOOK_NO_EXPORT void setupActions(KActionCollection *);
    KADDRESSBOOK_NO_EXPORT bool showQRCodes();
    KADDRESSBOOK_NO_EXPORT void setViewMode(int mode);
    KADDRESSBOOK_NO_EXPORT void saveSplitterStates() const;
    KADDRESSBOOK_NO_EXPORT void restoreSplitterStates();
    KADDRESSBOOK_NO_EXPORT void initializeImportExportPlugin(KActionCollection *collection);
    KADDRESSBOOK_NO_EXPORT void slotGeneralPaletteChanged();
    KADDRESSBOOK_NO_EXPORT void slotWhatsNew();
    KADDRESSBOOK_NO_EXPORT Akonadi::EntityTreeModel *entityTreeModel() const;

    KADDRESSBOOK_NO_EXPORT QAbstractItemModel *allContactsModel();

    ContactEntityMimeTypeFilterModel *mCollectionTree = nullptr;
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
    KXMLGUIClient *const mXmlGuiClient;
    KAddressBookGrantlee::GrantleeContactFormatter *mFormatter = nullptr;
    KAddressBookGrantlee::GrantleeContactGroupFormatter *mGroupFormatter = nullptr;
    GrantleeTheme::ThemeManager *mGrantleeThemeManager = nullptr;
    QAction *mQuickSearchAction = nullptr;
    QAction *mServerSideSubscription = nullptr;
    QList<KAddressBookImportExport::PluginInterface *> mImportExportPluginInterfaceList;
    ManageShowCollectionProperties *mManageShowCollectionProperties = nullptr;

    Akonadi::Item mPendingSelection;
};
