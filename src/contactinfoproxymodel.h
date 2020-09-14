/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2020 Konrad Czapla <kondzio89dev@gmail.com>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef CONTACTINFOPROXYMODEL_H
#define CONTACTINFOPROXYMODEL_H

#include <AkonadiCore/EntityTreeModel>
#include <kcontacts/contactgroup.h>
#include <QObject>
#include <QIdentityProxyModel>
#include <QPersistentModelIndex>

namespace Akonadi {
class Item;
class Monitor;
}

namespace KContacts {
class Addressee;
class ContactGroup;
}
class ContactInfoProxyModel : public QIdentityProxyModel
{
    Q_OBJECT
public:
    enum Roles {
        PictureRole = Akonadi::EntityTreeModel::UserRole,
        InitialsRole,
        DescriptionRole,
    };

    explicit ContactInfoProxyModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role) const override;

private:

    class ContactCacheData
    {
    public:
        ContactCacheData() = default;
        ~ContactCacheData() = default;
        ContactCacheData(const ContactCacheData &) = default;

        ContactCacheData(const KContacts::ContactGroup::ContactReference &other)
            : uid(other.uid())
            , gid(other.gid())
        {
        }

        ContactCacheData(const Akonadi::Item &other)
        {
            uid = QString::number(other.id());
            gid = other.gid();
        }

        friend bool operator==(const ContactCacheData &lhs, const ContactCacheData &rhs);

        using List = QVector<ContactCacheData>;
        using ListIterator = ContactCacheData::List::iterator;
        using ConstListIterator = ContactCacheData::List::ConstIterator;

        QString uid;
        QString gid;
        QString name;
        QString email;
    };

    Q_REQUIRED_RESULT QString getInitials(const KContacts::Addressee &contact) const;
    Q_REQUIRED_RESULT QString getInitials(const QModelIndex &index, const KContacts::ContactGroup &groupContacts) const;
    Q_REQUIRED_RESULT QString getDescription(const KContacts::Addressee &contact) const;
    Q_REQUIRED_RESULT QString getDescription(const QModelIndex &index, const KContacts::ContactGroup &groupContacts) const;

    void updateCache(const QModelIndex &index, const KContacts::ContactGroup &groupContacts) const;
    Q_REQUIRED_RESULT bool groupFetchDone(const QModelIndex &index, const KContacts::ContactGroup &groupContacts) const;
    Q_REQUIRED_RESULT ContactCacheData::ListIterator findCacheItem(const QModelIndex &index, const ContactCacheData &cacheContact);
    Q_REQUIRED_RESULT ContactCacheData::ConstListIterator findCacheItem(const QModelIndex &index, const ContactCacheData &cacheContact) const;
    void fetchItems(const Akonadi::Item::List &items, const QMap<const char *, QVariant> &properties) const;
    void slotFetchJobFinished(KJob *job);
    void slotItemChanged(const Akonadi::Item &item, const QSet<QByteArray> &partIdentifiers);
    void slotItemRemoved(const Akonadi::Item &item);

    friend bool operator==(const ContactCacheData &lhs, const ContactCacheData &rhs);
    using Cache = QMap<QPersistentModelIndex, ContactCacheData::List>;
    mutable Cache mGroupsCache;
    mutable QStringList mPendingGroups;
    const QVector<int> mKrole {PictureRole, InitialsRole, DescriptionRole};
    Akonadi::Monitor *const mMonitor;
};

#endif // CONTACTINFOPROXYMODEL_H
