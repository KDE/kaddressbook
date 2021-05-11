/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2020 Konrad Czapla <kondzio89dev@gmail.com>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "contactinfoproxymodel.h"

#include "kaddressbook_debug.h"

#include <AkonadiCore/Item>
#include <AkonadiCore/ItemFetchJob>
#include <AkonadiCore/ItemFetchScope>
#include <AkonadiCore/Monitor>
#include <KLocalizedString>
#include <kcontacts/addressee.h>

#include <KJob>
#include <QImage>

ContactInfoProxyModel::ContactInfoProxyModel(QObject *parent)
    : QIdentityProxyModel(parent)
    , mMonitor(new Akonadi::Monitor(this))
{
    mMonitor->setTypeMonitored(Akonadi::Monitor::Items);
    mMonitor->itemFetchScope().fetchFullPayload(true);
    connect(mMonitor, &Akonadi::Monitor::itemChanged, this, &ContactInfoProxyModel::slotItemChanged);
    connect(this, &ContactInfoProxyModel::rowsAboutToBeRemoved, this, &ContactInfoProxyModel::slotRowsAboutToBeRemoved);
}

QVariant ContactInfoProxyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
        qCWarning(KADDRESSBOOK_LOG) << "invalid index!";
    }
    if (role >= Roles::PictureRole && role <= Roles::DescriptionRole) {
        const auto item = index.data(Akonadi::EntityTreeModel::ItemRole).value<Akonadi::Item>();
        Q_ASSERT(item.isValid());
        if (item.hasPayload<KContacts::Addressee>()) {
            const auto contact = item.payload<KContacts::Addressee>();
            switch (role) {
            case Roles::PictureRole:
                return contact.photo().data();
            case Roles::InitialsRole:
                return getInitials(contact);
            case Roles::DescriptionRole:
                return getDescription(contact);
            }
        } else if (item.hasPayload<KContacts::ContactGroup>()) {
            const auto groupContacts = item.payload<KContacts::ContactGroup>();
            if (!mPendingGroupItems.contains(item.id())) {
                if (!mGroupsCache.contains(item.id())) {
                    mMonitor->setItemMonitored(item);
                    mGroupsCache[item.id()] = ContactCacheData::List();
                }

                if (groupContacts.contactReferenceCount() > 0 && isCacheItemToFetch(item.id(), groupContacts)) {
                    resolveGroup(item.id(), groupContacts);
                }
            }
            switch (role) {
            case Roles::PictureRole:
                return QVariant();
            case Roles::InitialsRole:
                return mPendingGroupItems.contains(item.id()) ? i18n("...") : getInitials(item.id(), groupContacts);
            case Roles::DescriptionRole:
                return mPendingGroupItems.contains(item.id()) ? i18n("Loading contacts details ...") : getDescription(item.id(), groupContacts);
            }
        }
    }
    return QIdentityProxyModel::data(index, role);
}

QString ContactInfoProxyModel::getInitials(const KContacts::Addressee &contact) const
{
    QString initials;
    if (!contact.givenName().isEmpty()) {
        initials.append(contact.givenName().front());
    }
    if (!contact.familyName().isEmpty()) {
        initials.append(contact.familyName().front());
    }

    if (initials.isEmpty() && !contact.realName().isEmpty()) {
        initials.append(contact.realName().front());
    }

    return initials.toUpper();
}

QString ContactInfoProxyModel::getInitials(const Akonadi::Item::Id groupItemId, const KContacts::ContactGroup &groupContacts) const
{
    QString initials;

    for (int idx = 0; idx < groupContacts.dataCount(); idx++) {
        const QString name = groupContacts.data(idx).name().trimmed();
        if (!name.isEmpty()) {
            initials.append(name.front());
        }
    }

    const auto groupCacheId = mGroupsCache[groupItemId];
    for (const ContactCacheData &cacheContact : groupCacheId) {
        const QString name = cacheContact.name().trimmed();
        if (!name.isEmpty()) {
            initials.append(name.front());
        }
    }

    return initials.toUpper();
}

QString ContactInfoProxyModel::getDescription(const KContacts::Addressee &contact) const
{
    QString dataSeparator;
    QString emailAddress;
    QString phone;

    if (!contact.preferredEmail().isEmpty()) {
        emailAddress = i18n("Email: %1", contact.preferredEmail());
    }
    const QList<KContacts::PhoneNumber> phoneList = contact.phoneNumbers().toList();
    QList<KContacts::PhoneNumber>::const_reverse_iterator itPhone =
        std::find_if(phoneList.rbegin(), phoneList.rend(), [&phoneList](const KContacts::PhoneNumber &phone) {
            return phone.isPreferred() || phoneList.at(0) == phone;
        });
    if (itPhone != phoneList.rend()) {
        phone = i18n("Phone: %1", (*itPhone).number());
    }
    if (!emailAddress.isEmpty() && !phone.isEmpty()) {
        dataSeparator = QStringLiteral(",");
    }

    return i18n("%1%2 %3", emailAddress, dataSeparator, phone).trimmed();
}

QString ContactInfoProxyModel::getDescription(const Akonadi::Item::Id groupItemId, const KContacts::ContactGroup &groupContacts) const
{
    QStringList groupDescription;
    QString contactDescription;

    for (int idx = 0; idx < groupContacts.dataCount(); idx++) {
        QString dataSeparator;
        if (!groupContacts.data(idx).name().isEmpty() && !groupContacts.data(idx).email().isEmpty()) {
            dataSeparator = QStringLiteral("-");
        }
        contactDescription = i18n("%1 %2 %3", groupContacts.data(idx).name(), dataSeparator, groupContacts.data(idx).email());
        groupDescription << contactDescription.trimmed();
        contactDescription.clear();
    }
    for (int idx = 0; idx < groupContacts.contactReferenceCount(); idx++) {
        const KContacts::ContactGroup::ContactReference contactRef = groupContacts.contactReference(idx);

        ContactCacheData::ConstListIterator it = findCacheItem(groupItemId, contactRef);
        if (it != mGroupsCache[groupItemId].end()) {
            QString cacheSeparator, email;
            email = contactRef.preferredEmail().isEmpty() ? it->email() : contactRef.preferredEmail();
            if (it->name().isEmpty() && email.isEmpty()) {
                continue;
            } else if (!it->name().isEmpty() && !email.isEmpty()) {
                cacheSeparator = QStringLiteral("-");
            }
            contactDescription = i18n("%1 %2 %3", it->name(), cacheSeparator, email);
            groupDescription << contactDescription.trimmed();
            contactDescription.clear();
        }
    }
    return groupDescription.join(QStringLiteral(", "));
}

QStringList ContactInfoProxyModel::getIdsContactGroup(const KContacts::ContactGroup &group) const
{
    QStringList groupRefIds;
    groupRefIds.reserve(group.contactReferenceCount());
    for (int idx = 0; idx < group.contactReferenceCount(); idx++) {
        const KContacts::ContactGroup::ContactReference &reference = group.contactReference(idx);

        groupRefIds += reference.gid().isEmpty() ? reference.uid() : reference.gid();
    }
    return groupRefIds;
}

QStringList ContactInfoProxyModel::getIdsCacheContactGroup(const Akonadi::Item::Id groupItemId) const
{
    QStringList groupCacheRefIds;
    groupCacheRefIds.reserve(mGroupsCache[groupItemId].size());
    for (const auto &cacheContact : mGroupsCache[groupItemId]) {
        groupCacheRefIds += cacheContact.gid().isEmpty() ? cacheContact.uid() : cacheContact.gid();
    }
    return groupCacheRefIds;
}

bool ContactInfoProxyModel::isCacheItemToFetch(const Akonadi::Item::Id groupItemId, const KContacts::ContactGroup &group) const
{
    QStringList groupRefIds = getIdsContactGroup(group);
    QStringList groupCacheRefIds = getIdsCacheContactGroup(groupItemId);

    auto sortFunc = [](const QString &lhs, const QString &rhs) -> bool {
        return lhs.toLongLong() < rhs.toLongLong();
    };

    std::sort(groupRefIds.begin(), groupRefIds.end(), sortFunc);
    groupRefIds.erase(std::unique(groupRefIds.begin(), groupRefIds.end()), groupRefIds.end());

    std::sort(groupCacheRefIds.begin(), groupCacheRefIds.end(), sortFunc);

    return !std::equal(groupRefIds.begin(), groupRefIds.end(), groupCacheRefIds.begin(), groupCacheRefIds.end());
}

ContactInfoProxyModel::ContactCacheData::ListIterator ContactInfoProxyModel::findCacheItem(const Akonadi::Item::Id groupItemId,
                                                                                           const ContactInfoProxyModel::ContactCacheData &cacheContact)
{
    ContactCacheData::ListIterator it =
        std::find_if(mGroupsCache[groupItemId].begin(), mGroupsCache[groupItemId].end(), [&cacheContact](const ContactCacheData &contact) -> bool {
            return contact == cacheContact;
        });
    return it;
}

ContactInfoProxyModel::ContactCacheData::ConstListIterator
ContactInfoProxyModel::findCacheItem(const Akonadi::Item::Id groupItemId, const ContactInfoProxyModel::ContactCacheData &cacheContact) const
{
    ContactCacheData::ConstListIterator it =
        std::find_if(mGroupsCache[groupItemId].cbegin(), mGroupsCache[groupItemId].cend(), [&cacheContact](const ContactCacheData &contact) -> bool {
            return contact == cacheContact;
        });
    return it;
}

QMap<const char *, QVariant> ContactInfoProxyModel::buildFetchProperties(const Akonadi::Item::Id groupItemId) const
{
    return QMap<const char *, QVariant>{
        {"groupItemId", QVariant::fromValue((groupItemId))},
    };
}

void ContactInfoProxyModel::resolveGroup(const Akonadi::Item::Id groupItemId, const KContacts::ContactGroup &groupContacts) const
{
    Akonadi::Item::List groupItemsList;

    for (int idx = 0; idx < groupContacts.contactReferenceCount(); idx++) {
        const KContacts::ContactGroup::ContactReference contactRef = groupContacts.contactReference(idx);

        if (findCacheItem(groupItemId, contactRef) == mGroupsCache[groupItemId].cend()) {
            mGroupsCache[groupItemId].push_back(contactRef);
            Akonadi::Item newItem;

            if (contactRef.gid().isEmpty()) {
                newItem.setId(contactRef.uid().toLongLong());
            } else {
                newItem.setGid(contactRef.gid());
            }
            groupItemsList << newItem;
        }
    }
    if (!groupItemsList.isEmpty()) {
        mPendingGroupItems << groupItemId;
        fetchItems(groupItemsList, buildFetchProperties(groupItemId));
    }
}

void ContactInfoProxyModel::fetchItems(const Akonadi::Item::List &items, const QMap<const char *, QVariant> &properties) const
{
    auto job = new Akonadi::ItemFetchJob(items);
    job->fetchScope().fetchFullPayload();
    job->fetchScope().setIgnoreRetrievalErrors(true);

    for (const auto &property : properties.toStdMap()) {
        job->setProperty(property.first, property.second);
    }

    connect(job, &Akonadi::ItemFetchJob::result, this, &ContactInfoProxyModel::slotFetchJobFinished);
}

void ContactInfoProxyModel::slotFetchJobFinished(KJob *job)
{
    if (job->error()) {
        qCWarning(KADDRESSBOOK_LOG) << " error during fetching items" << job->errorString();
        return;
    }
    auto fetchJob = qobject_cast<Akonadi::ItemFetchJob *>(job);

    const auto groupItemId = job->property("groupItemId").value<Akonadi::Item::Id>();

    const auto items = fetchJob->items();
    for (const Akonadi::Item &item : items) {
        ContactCacheData::List::iterator it_contact = findCacheItem(groupItemId, item);
        if (it_contact != mGroupsCache[groupItemId].end()) {
            if (it_contact->setData(item)) {
                mMonitor->setItemMonitored(item);
            } else {
                qCWarning(KADDRESSBOOK_LOG) << QStringLiteral("item with id %1 cannot be saved into cache").arg(item.id());
            }
        }
    }

    if (mPendingGroupItems.contains(groupItemId)) {
        mPendingGroupItems.removeOne(groupItemId);
    }
    const QModelIndex index = Akonadi::EntityTreeModel::modelIndexesForItem(this, Akonadi::Item(groupItemId)).constFirst();
    Q_EMIT dataChanged(index, index, mKrole);
}

void ContactInfoProxyModel::slotItemChanged(const Akonadi::Item &item, const QSet<QByteArray> &partIdentifiers)
{
    Q_UNUSED(partIdentifiers)
    Q_ASSERT(item.isValid());

    if (item.hasPayload<KContacts::Addressee>()) {
        QMapIterator<Akonadi::Item::Id, ContactCacheData::List> it_group(mGroupsCache);
        while (it_group.hasNext()) {
            it_group.next();
            ContactCacheData::ListIterator it_contact = findCacheItem(it_group.key(), item);
            if (it_contact != mGroupsCache[it_group.key()].end()) {
                if (it_contact->setData(item)) {
                    const QModelIndex index = Akonadi::EntityTreeModel::modelIndexesForItem(this, Akonadi::Item(it_group.key())).constFirst();
                    Q_EMIT dataChanged(index, index, mKrole);
                } else {
                    qCWarning(KADDRESSBOOK_LOG) << QStringLiteral("changed item with id %1 cannot be saved into cache").arg(item.id());
                }
            }
        }
    } else if (item.hasPayload<KContacts::ContactGroup>()) {
        if (mGroupsCache.contains(item.id())) {
            const auto groupContacts = item.payload<KContacts::ContactGroup>();
            mGroupsCache[item.id()].clear();
            if (groupContacts.contactReferenceCount() > 0 && isCacheItemToFetch(item.id(), groupContacts)) {
                resolveGroup(item.id(), groupContacts);
            }
            const QModelIndex index = Akonadi::EntityTreeModel::modelIndexesForItem(this, Akonadi::Item(item.id())).constFirst();
            Q_EMIT dataChanged(index, index, mKrole);
        }
    }
}

void ContactInfoProxyModel::slotRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last)
{
    for (int idx = first; idx <= last; idx++) {
        const auto item = this->index(idx, 0, parent).data(Akonadi::EntityTreeModel::ItemRole).value<Akonadi::Item>();
        Q_ASSERT(item.isValid());
        if (item.hasPayload<KContacts::Addressee>()) {
            QMapIterator<Akonadi::Item::Id, ContactCacheData::List> it_group(mGroupsCache);
            while (it_group.hasNext()) {
                it_group.next();
                ContactCacheData::List::iterator it_contact = findCacheItem(it_group.key(), item);
                if (it_contact != mGroupsCache[it_group.key()].end()) {
                    mGroupsCache[it_group.key()].erase(it_contact);
                }
            }
        } else if (item.hasPayload<KContacts::ContactGroup>()) {
            if (mGroupsCache.contains(item.id())) {
                mGroupsCache.remove(item.id());
            }
        }
    }
}

bool ContactInfoProxyModel::ContactCacheData::setData(const Akonadi::Item &item)
{
    bool result(false);
    if (validateItem(item)) {
        const auto contact = item.payload<KContacts::Addressee>();
        mName = contact.realName();
        mEmail = contact.preferredEmail();
        result = true;
    }
    return result;
}

bool ContactInfoProxyModel::ContactCacheData::validateItem(const Akonadi::Item &item) const
{
    bool result(true);
    result |= item.isValid();
    result |= mUid == QString::number(item.id());
    result |= mGid == item.gid();
    result |= item.hasPayload<KContacts::Addressee>();
    return result;
}

bool operator==(const ContactInfoProxyModel::ContactCacheData &lhs, const ContactInfoProxyModel::ContactCacheData &rhs)
{
    return !lhs.gid().isEmpty() ? lhs.gid() == rhs.gid() : !lhs.uid().isEmpty() ? lhs.uid() == rhs.uid() : false;
}
