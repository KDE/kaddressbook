/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2009 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "modelcolumnmanager.h"
#include "settings.h"

#include <KContacts/Addressee>

#include <KLocalizedString>

#include <QContextMenuEvent>
#include <QEvent>
#include <QHeaderView>
#include <QMenu>
#include <QTimer>
#include <QWidget>

ModelColumnManager::ModelColumnManager(Akonadi::ContactsTreeModel *model, QObject *parent)
    : QObject(parent)
    , mModel(model)
{
}

void ModelColumnManager::load()
{
    const QList<int> settingsColumns = Settings::contactModelColumns();
    Akonadi::ContactsTreeModel::Columns columns;
    columns.reserve(settingsColumns.count());

    for (int column : settingsColumns) {
        columns.append(static_cast<Akonadi::ContactsTreeModel::Column>(column));
    }

    mModel->setColumns(columns);
}

void ModelColumnManager::store()
{
    const Akonadi::ContactsTreeModel::Columns columns = mModel->columns();
    QList<int> settingsColumns;
    settingsColumns.reserve(columns.count());

    for (int column : columns) {
        settingsColumns.append(static_cast<int>(column));
    }

    Settings::setContactModelColumns(settingsColumns);
}

void ModelColumnManager::setWidget(QWidget *widget)
{
    mWidget = widget;
    mWidget->installEventFilter(this);
}

bool ModelColumnManager::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == mWidget) {
        if (event->type() == QEvent::ContextMenu) {
            QMenu menu;

            const Akonadi::ContactsTreeModel::Columns modelColumns = mModel->columns();

            QAction *fullNameAction = menu.addAction(i18n("Full Name"));
            fullNameAction->setCheckable(true);
            fullNameAction->setChecked(modelColumns.contains(Akonadi::ContactsTreeModel::FullName));
            fullNameAction->setEnabled(false);

            QAction *familyNameAction = menu.addAction(i18n("Family Name"));
            familyNameAction->setCheckable(true);
            familyNameAction->setChecked(modelColumns.contains(Akonadi::ContactsTreeModel::FamilyName));

            QAction *givenNameAction = menu.addAction(i18n("Given Name"));
            givenNameAction->setCheckable(true);
            givenNameAction->setChecked(modelColumns.contains(Akonadi::ContactsTreeModel::GivenName));

            QAction *birthdayAction = menu.addAction(KContacts::Addressee::birthdayLabel());
            birthdayAction->setCheckable(true);
            birthdayAction->setChecked(modelColumns.contains(Akonadi::ContactsTreeModel::Birthday));

            QAction *homeAddressAction = menu.addAction(i18n("Home Address"));
            homeAddressAction->setCheckable(true);
            homeAddressAction->setChecked(modelColumns.contains(Akonadi::ContactsTreeModel::HomeAddress));

            QAction *businessAddressAction = menu.addAction(i18n("Business Address"));
            businessAddressAction->setCheckable(true);
            businessAddressAction->setChecked(modelColumns.contains(Akonadi::ContactsTreeModel::BusinessAddress));

            QAction *phoneNumbersAction = menu.addAction(i18n("Phone Numbers"));
            phoneNumbersAction->setCheckable(true);
            phoneNumbersAction->setChecked(modelColumns.contains(Akonadi::ContactsTreeModel::PhoneNumbers));

            QAction *preferredEmailAction = menu.addAction(i18n("Preferred EMail"));
            preferredEmailAction->setCheckable(true);
            preferredEmailAction->setChecked(modelColumns.contains(Akonadi::ContactsTreeModel::PreferredEmail));

            QAction *allEmailsAction = menu.addAction(i18n("All EMails"));
            allEmailsAction->setCheckable(true);
            allEmailsAction->setChecked(modelColumns.contains(Akonadi::ContactsTreeModel::AllEmails));

            QAction *organizationAction = menu.addAction(KContacts::Addressee::organizationLabel());
            organizationAction->setCheckable(true);
            organizationAction->setChecked(modelColumns.contains(Akonadi::ContactsTreeModel::Organization));

            QAction *roleAction = menu.addAction(KContacts::Addressee::roleLabel());
            roleAction->setCheckable(true);
            roleAction->setChecked(modelColumns.contains(Akonadi::ContactsTreeModel::Role));

            QAction *homepageAction = menu.addAction(KContacts::Addressee::urlLabel());
            homepageAction->setCheckable(true);
            homepageAction->setChecked(modelColumns.contains(Akonadi::ContactsTreeModel::Homepage));

            QAction *noteAction = menu.addAction(KContacts::Addressee::noteLabel());
            noteAction->setCheckable(true);
            noteAction->setChecked(modelColumns.contains(Akonadi::ContactsTreeModel::Note));

            if (menu.exec((static_cast<QContextMenuEvent *>(event))->globalPos())) {
                Akonadi::ContactsTreeModel::Columns columns;

                if (fullNameAction->isChecked()) {
                    columns << Akonadi::ContactsTreeModel::FullName;
                }
                if (familyNameAction->isChecked()) {
                    columns << Akonadi::ContactsTreeModel::FamilyName;
                }
                if (givenNameAction->isChecked()) {
                    columns << Akonadi::ContactsTreeModel::GivenName;
                }
                if (birthdayAction->isChecked()) {
                    columns << Akonadi::ContactsTreeModel::Birthday;
                }
                if (homeAddressAction->isChecked()) {
                    columns << Akonadi::ContactsTreeModel::HomeAddress;
                }
                if (businessAddressAction->isChecked()) {
                    columns << Akonadi::ContactsTreeModel::BusinessAddress;
                }
                if (phoneNumbersAction->isChecked()) {
                    columns << Akonadi::ContactsTreeModel::PhoneNumbers;
                }
                if (preferredEmailAction->isChecked()) {
                    columns << Akonadi::ContactsTreeModel::PreferredEmail;
                }
                if (allEmailsAction->isChecked()) {
                    columns << Akonadi::ContactsTreeModel::AllEmails;
                }
                if (organizationAction->isChecked()) {
                    columns << Akonadi::ContactsTreeModel::Organization;
                }
                if (roleAction->isChecked()) {
                    columns << Akonadi::ContactsTreeModel::Role;
                }
                if (homepageAction->isChecked()) {
                    columns << Akonadi::ContactsTreeModel::Homepage;
                }
                if (noteAction->isChecked()) {
                    columns << Akonadi::ContactsTreeModel::Note;
                }

                mModel->setColumns(columns);
                QTimer::singleShot(0, this, &ModelColumnManager::adaptHeaderView);
            }

            return true;
        } else {
            return false;
        }
    }

    return false;
}

void ModelColumnManager::adaptHeaderView()
{
    auto view = qobject_cast<QHeaderView *>(mWidget);
    if (view) {
        view->resizeSections(QHeaderView::Stretch);

        view->setDefaultAlignment(mModel->columns().count() == 1 ? Qt::AlignCenter : Qt::AlignLeft);
    }
}
