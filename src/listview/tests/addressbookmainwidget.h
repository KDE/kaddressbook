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

#ifndef ADDRESSBOOKMAINWIDGET_H
#define ADDRESSBOOKMAINWIDGET_H

#include <QActionGroup>
#include <QSplitter>
#include <QStackedWidget>
#include <QWidget>

#include <AkonadiCore/entitymimetypefiltermodel.h>

#include <Akonadi/Contact/ContactGroupViewer>
#include <Akonadi/Contact/ContactViewer>
#include <Akonadi/Contact/ContactsFilterProxyModel>
#include <Akonadi/Contact/StandardContactActionManager>

#include <AkonadiWidgets/EntityTreeView>

#include <KaddressbookGrantlee/GrantleeContactFormatter>
#include <KaddressbookGrantlee/GrantleeContactGroupFormatter>

#include <GrantleeTheme/GrantleeThemeManager>
class AddressBookQuickWidget;
class AddressBookMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AddressBookMainWidget(QWidget *parent = nullptr);
    ~AddressBookMainWidget();
private Q_SLOTS:
    void itemSelected(const Akonadi::Item &item);
private:
    void initGrantleeThemeName();
    void setupGui();
    void setupAction(KActionCollection *collection);
    Akonadi::EntityMimeTypeFilterModel *mCollectionTree;
    Akonadi::EntityMimeTypeFilterModel *mItemTree;
    Akonadi::EntityMimeTypeFilterModel *mAllContactsModel;
    Akonadi::ContactsFilterProxyModel *mContactsFilterModel;
    Akonadi::EntityTreeView *mCollectionView;
    Akonadi::EntityTreeView *mItemView;

    QWidget *mDetailsPane;
    QStackedWidget *mDetailsViewStack;
    //ContactSwitcher *mContactSwitcher;

    QSplitter *mMainWidgetSplitter1;
    QSplitter *mMainWidgetSplitter2;
    Akonadi::ContactViewer *mContactDetails;
    Akonadi::ContactGroupViewer *mContactGroupDetails;
    QWidget *mEmptyDetails;
    Akonadi::StandardContactActionManager *mActionManager;
    QItemSelectionModel *mCollectionSelectionModel;

    QActionGroup *mViewModeGroup;

    //ModelColumnManager *mModelColumnManager;
    //KXMLGUIClient *mXmlGuiClient;
    KAddressBookGrantlee::GrantleeContactFormatter *mFormatter;
    KAddressBookGrantlee::GrantleeContactGroupFormatter *mGroupFormatter;
    GrantleeTheme::ThemeManager *mGrantleeThemeManager;

    AddressBookQuickWidget *mAddressbookQuickWidget;
};

#endif // ADDRESSBOOKMAINWIDGET_H
