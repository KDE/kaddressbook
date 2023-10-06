/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2020 Konrad Czapla <kondzio89dev@gmail.com>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <Akonadi/EntityTreeModel>
#include <KContacts/ContactGroup>
#include <QIdentityProxyModel>
#include <QObject>

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
        using List = QList<ContactCacheData>;
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

        [[nodiscard]] bool setData(const Akonadi::Item &item);

        friend bool operator==(const ContactCacheData &lhs, const ContactCacheData &rhs);

        [[nodiscard]] QString uid() const
        {
            return mUid;
        }
        [[nodiscard]] QString gid() const
        {
            return mGid;
        }
        [[nodiscard]] QString name() const
        {
            return mName;
        }
        [[nodiscard]] QString email() const
        {
            return mEmail;
        }

    private:
        [[nodiscard]] bool validateItem(const Akonadi::Item &item) const;
        QString mUid;
        QString mGid;
        QString mName;
        QString mEmail;
    };

    [[nodiscard]] QString getInitials(const KContacts::Addressee &contact) const;
    [[nodiscard]] QString getInitials(const Akonadi::Item::Id groupItemId, const KContacts::ContactGroup &groupContacts) const;
    [[nodiscard]] QString getDescription(const KContacts::Addressee &contact) const;
    [[nodiscard]] QString getDescription(const Akonadi::Item::Id groupItemId, const KContacts::ContactGroup &groupContacts) const;
    [[nodiscard]] QStringList getIdsContactGroup(const KContacts::ContactGroup &group) const;
    [[nodiscard]] QStringList getIdsCacheContactGroup(const Akonadi::Item::Id groupItemId) const;
    [[nodiscard]] bool isCacheItemToFetch(const Akonadi::Item::Id groupItemId, const KContacts::ContactGroup &group) const;
    [[nodiscard]] ContactCacheData::ListIterator findCacheItem(const Akonadi::Item::Id groupItemId, const ContactCacheData &cacheContact);
    [[nodiscard]] ContactCacheData::ConstListIterator findCacheItem(const Akonadi::Item::Id groupItemId, const ContactCacheData &cacheContact) const;
    [[nodiscard]] QMap<const char *, QVariant> buildFetchProperties(const Akonadi::Item::Id groupItemId) const;

    void resolveGroup(const Akonadi::Item::Id groupItemId, const KContacts::ContactGroup &groupContacts) const;
    void fetchItems(const Akonadi::Item::List &items, const QMap<const char *, QVariant> &properties) const;

    void slotFetchJobFinished(KJob *job);
    void slotItemChanged(const Akonadi::Item &item, const QSet<QByteArray> &partIdentifiers);
    void slotRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last);

    friend bool operator==(const ContactCacheData &lhs, const ContactCacheData &rhs);
    using Cache = QMap<Akonadi::Item::Id, ContactCacheData::List>;
    mutable Cache mGroupsCache;
    mutable QList<Akonadi::Item::Id> mPendingGroupItems;
    const QList<int> mKrole{PictureRole, InitialsRole, DescriptionRole};
    Akonadi::Monitor *const mMonitor;
};
