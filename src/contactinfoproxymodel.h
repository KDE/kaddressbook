/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2020 Konrad Czapla <kondzio89dev@gmail.com>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <AkonadiCore/EntityTreeModel>
#include <QIdentityProxyModel>
#include <QObject>
#include <kcontacts/contactgroup.h>

namespace Akonadi
{
class Item;
class Monitor;
}

namespace KContacts
{
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
        using List = QVector<ContactCacheData>;
        using ListIterator = ContactCacheData::List::iterator;
        using ConstListIterator = ContactCacheData::List::ConstIterator;

        ContactCacheData() = default;
        ~ContactCacheData() = default;
        ContactCacheData(const ContactCacheData &) = default;

        ContactCacheData(const KContacts::ContactGroup::ContactReference &other)
            : mUid(other.uid())
            , mGid(other.gid())
        {
        }

        ContactCacheData(const Akonadi::Item &other)
            : mUid(QString::number(other.id()))
            , mGid(other.gid())
        {
        }

        Q_REQUIRED_RESULT bool setData(const Akonadi::Item &item);

        friend bool operator==(const ContactCacheData &lhs, const ContactCacheData &rhs);

        Q_REQUIRED_RESULT QString uid() const
        {
            return mUid;
        }
        Q_REQUIRED_RESULT QString gid() const
        {
            return mGid;
        }
        Q_REQUIRED_RESULT QString name() const
        {
            return mName;
        }
        Q_REQUIRED_RESULT QString email() const
        {
            return mEmail;
        }

    private:
        Q_REQUIRED_RESULT bool validateItem(const Akonadi::Item &item) const;
        QString mUid;
        QString mGid;
        QString mName;
        QString mEmail;
    };

    Q_REQUIRED_RESULT QString getInitials(const KContacts::Addressee &contact) const;
    Q_REQUIRED_RESULT QString getInitials(const Akonadi::Item::Id groupItemId, const KContacts::ContactGroup &groupContacts) const;
    Q_REQUIRED_RESULT QString getDescription(const KContacts::Addressee &contact) const;
    Q_REQUIRED_RESULT QString getDescription(const Akonadi::Item::Id groupItemId, const KContacts::ContactGroup &groupContacts) const;
    Q_REQUIRED_RESULT QStringList getIdsContactGroup(const KContacts::ContactGroup &group) const;
    Q_REQUIRED_RESULT QStringList getIdsCacheContactGroup(const Akonadi::Item::Id groupItemId) const;
    Q_REQUIRED_RESULT bool isCacheItemToFetch(const Akonadi::Item::Id groupItemId, const KContacts::ContactGroup &group) const;
    Q_REQUIRED_RESULT ContactCacheData::ListIterator findCacheItem(const Akonadi::Item::Id groupItemId, const ContactCacheData &cacheContact);
    Q_REQUIRED_RESULT ContactCacheData::ConstListIterator findCacheItem(const Akonadi::Item::Id groupItemId, const ContactCacheData &cacheContact) const;
    Q_REQUIRED_RESULT QMap<const char *, QVariant> buildFetchProperties(const Akonadi::Item::Id groupItemId) const;

    void resolveGroup(const Akonadi::Item::Id groupItemId, const KContacts::ContactGroup &groupContacts) const;
    void fetchItems(const Akonadi::Item::List &items, const QMap<const char *, QVariant> &properties) const;

    void slotFetchJobFinished(KJob *job);
    void slotItemChanged(const Akonadi::Item &item, const QSet<QByteArray> &partIdentifiers);
    void slotRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last);

    friend bool operator==(const ContactCacheData &lhs, const ContactCacheData &rhs);
    using Cache = QMap<Akonadi::Item::Id, ContactCacheData::List>;
    mutable Cache mGroupsCache;
    mutable QList<Akonadi::Item::Id> mPendingGroupItems;
    const QVector<int> mKrole{PictureRole, InitialsRole, DescriptionRole};
    Akonadi::Monitor *const mMonitor;
};

