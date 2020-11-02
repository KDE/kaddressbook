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
#include <QJsonObject>
#include <QRegularExpression>

ContactInfoProxyModel::ContactInfoProxyModel(QObject *parent)
    : QIdentityProxyModel(parent)
    , mMonitor(new Akonadi::Monitor(this))
{
    mMonitor->setTypeMonitored(Akonadi::Monitor::Items);
    mMonitor->itemFetchScope().fetchFullPayload(true);
    connect(mMonitor, &Akonadi::Monitor::itemChanged, this, &ContactInfoProxyModel::slotItemChanged);
    connect(mMonitor, &Akonadi::Monitor::itemRemoved, this, &ContactInfoProxyModel::slotItemRemoved);
}

QVariant ContactInfoProxyModel::data(const QModelIndex &index, int role) const
{
    if (role >= Roles::PictureRole && role <= Roles::DescriptionRole) {
        const Akonadi::Item item = index.data(Akonadi::EntityTreeModel::ItemRole).value<Akonadi::Item>();
        if (item.isValid()) {
            if (item.hasPayload<KContacts::Addressee>()) {
                const KContacts::Addressee contact = item.payload<KContacts::Addressee>();
                switch (role) {
                case Roles::PictureRole:
                    return contact.photo().data();
                case Roles::InitialsRole:
                    return getInitials(contact);
                case Roles::DescriptionRole:
                    return getDescription(contact);
                }
            } else if (item.hasPayload<KContacts::ContactGroup>()) {
                const KContacts::ContactGroup groupContacts = item.payload<KContacts::ContactGroup>();
                if (!mGroupsCache.contains(index)) {
                    mGroupsCache.insert(index, ContactCacheData::List());
                }
                updateCache(index, groupContacts);
                if (groupFetchDone(index, groupContacts)) {
                    switch (role) {
                    case Roles::PictureRole:
                        return QVariant();
                    case Roles::InitialsRole:
                        return getInitials(index, groupContacts);
                    case Roles::DescriptionRole:
                        return getDescription(index, groupContacts);
                    }
                } else if (role == Roles::DescriptionRole) {
                    return i18n("Loading contacts details ...");
                }
                if (!mPendingGroups.contains(groupContacts.id())) {
                    QMap<const char *, QVariant> properties{
                        {"groupPersistentModelIndex", QVariant::fromValue(index)},
                        {"groupId", QVariant::fromValue(groupContacts.id())},
                    };

                    Akonadi::Item::List groupItemsList;
                    QList<QJsonObject> groupRefIdsList;

                    for (int idx = 0; idx < groupContacts.contactReferenceCount(); ++idx) {
                        const KContacts::ContactGroup::ContactReference contactRef = groupContacts.contactReference(idx);

                        if (findCacheItem(index, contactRef) == mGroupsCache[index].cend()) {
                            Akonadi::Item newItem;

                            if (contactRef.gid().isEmpty()) {
                                newItem.setId(contactRef.uid().toLongLong());
                            } else {
                                newItem.setGid(contactRef.gid());
                            }
                            if (!mPendingGroups.contains(groupContacts.id())) {
                                mPendingGroups << groupContacts.id();
                            }

                            groupItemsList << newItem;
                            const QJsonObject refId {
                                {QStringLiteral("uid"), contactRef.uid()},
                                {QStringLiteral("gid"), contactRef.gid()}
                            };
                            if (!groupRefIdsList.contains(refId)) {
                                groupRefIdsList << refId;
                            }
                        }
                    }
                    if (!groupItemsList.isEmpty() && !groupRefIdsList.isEmpty()) {
                        properties.insert("groupRefIdsList", QVariant::fromValue(groupRefIdsList));
                        fetchItems(groupItemsList, properties);
                    }
                }
                return QVariant();
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

QString ContactInfoProxyModel::getInitials(const QModelIndex &index, const KContacts::ContactGroup &groupContacts) const
{
    QString initials;

    for (int idx = 0; idx < groupContacts.dataCount(); idx++) {
        if (!groupContacts.data(idx).name().isEmpty()) {
            initials.append(groupContacts.data(idx).name().front());
        }
    }

    for (const ContactCacheData &cacheContact : mGroupsCache[index]) {
        if (!cacheContact.name.isEmpty()) {
            initials.append(cacheContact.name.front());
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
    QList<KContacts::PhoneNumber>::const_reverse_iterator itPhone = std::find_if(phoneList.rbegin(),
                                                                                 phoneList.rend(),
                                                                                 [&phoneList](const KContacts::PhoneNumber &phone) {
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

QString ContactInfoProxyModel::getDescription(const QModelIndex &index, const KContacts::ContactGroup &groupContacts) const
{
    QStringList groupDescription;
    QString contactDescription;

    for (int idx = 0; idx < groupContacts.dataCount(); idx++) {
        QString dataSeparator;
        if (!groupContacts.data(idx).name().isEmpty()
            && !groupContacts.data(idx).email().isEmpty()) {
            dataSeparator = QStringLiteral("-");
        }
        contactDescription = i18n("%1 %2 %3", groupContacts.data(idx).name(), dataSeparator, groupContacts.data(idx).email());
        groupDescription << contactDescription.trimmed();
        contactDescription.clear();
    }
    for (int idx = 0; idx < groupContacts.contactReferenceCount(); ++idx) {
        const KContacts::ContactGroup::ContactReference contactRef = groupContacts.contactReference(idx);

        ContactCacheData::ConstListIterator it = findCacheItem(index, contactRef);
        if (it != mGroupsCache[index].end()) {
            QString cacheSeparator, email;
            email = contactRef.preferredEmail().isEmpty() ? it->email : contactRef.preferredEmail();
            if (it->name.isEmpty() && email.isEmpty()) {
                continue;
            } else if (!it->name.isEmpty() && !email.isEmpty()) {
                cacheSeparator = QStringLiteral("-");
            }
            contactDescription = i18n("%1 %2 %3", it->name, cacheSeparator, email);
            groupDescription << contactDescription.trimmed();
            contactDescription.clear();
        }
    }
    return groupDescription.join(QStringLiteral(", "));
}

void ContactInfoProxyModel::updateCache(const QModelIndex &index, const KContacts::ContactGroup &groupContacts) const
{
    mGroupsCache[index].erase(std::remove_if(mGroupsCache[index].begin(), mGroupsCache[index].end(),
                                             [&groupContacts](const ContactCacheData &cacheContact) -> bool {
        for (int idx = 0; idx < groupContacts.contactReferenceCount(); ++idx) {
            const KContacts::ContactGroup::ContactReference &reference = groupContacts.contactReference(idx);

            if (cacheContact == reference) {
                return false;
            }
        }
        return true;
    }), mGroupsCache[index].end());
}

bool ContactInfoProxyModel::groupFetchDone(const QModelIndex &index, const KContacts::ContactGroup &groupContacts) const
{
    QStringList contactRefIds;
    QStringList contactCacheIds;

    auto sortFunc = [](const QString &lhs, const QString &rhs) -> bool {
                        return lhs.toLongLong() < rhs.toLongLong();
                    };

    for (int idx = 0; idx < groupContacts.contactReferenceCount(); ++idx) {
        const KContacts::ContactGroup::ContactReference &reference = groupContacts.contactReference(idx);

        contactRefIds += reference.gid().isEmpty() ? reference.uid() : reference.gid();
    }
    std::sort(contactRefIds.begin(), contactRefIds.end(), sortFunc);
    contactRefIds.erase(std::unique(contactRefIds.begin(), contactRefIds.end()), contactRefIds.end());

    for (const auto &cacheContact : mGroupsCache[index]) {
        contactCacheIds += cacheContact.gid.isEmpty() ? cacheContact.uid : cacheContact.gid;
    }
    std::sort(contactCacheIds.begin(), contactCacheIds.end(), sortFunc);

    return std::equal(contactRefIds.begin(), contactRefIds.end(), contactCacheIds.begin(), contactCacheIds.end());
}

ContactInfoProxyModel::ContactCacheData::ListIterator ContactInfoProxyModel::findCacheItem(const QModelIndex &index, const ContactInfoProxyModel::ContactCacheData &cacheContact)
{
    ContactCacheData::ListIterator it = std::find_if(mGroupsCache[index].begin(), mGroupsCache[index].end(),
                                                     [&cacheContact](const ContactCacheData &contact) -> bool
    {
        return contact == cacheContact;
    });
    return it;
}

ContactInfoProxyModel::ContactCacheData::ConstListIterator ContactInfoProxyModel::findCacheItem(const QModelIndex &index, const ContactInfoProxyModel::ContactCacheData &cacheContact) const
{
    ContactCacheData::ConstListIterator it = std::find_if(mGroupsCache[index].cbegin(), mGroupsCache[index].cend(),
                                                          [&cacheContact](const ContactCacheData &contact) -> bool
    {
        return contact == cacheContact;
    });
    return it;
}

void ContactInfoProxyModel::fetchItems(const Akonadi::Item::List &items, const QMap<const char *, QVariant> &properties) const
{
    auto *job = new Akonadi::ItemFetchJob(items);
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
    auto *fetchJob = qobject_cast<Akonadi::ItemFetchJob *>(job);

    const QPersistentModelIndex index = job->property("groupPersistentModelIndex").value<QPersistentModelIndex>();
    const QString groupId = job->property("groupId").value<QString>();
    const QList<QJsonObject> groupRefIdsList = job->property("groupRefIdsList").value<QList<QJsonObject> >();

    for (const QJsonObject &refId : groupRefIdsList) {
        ContactCacheData cacheContact;
        cacheContact.gid = refId[QStringLiteral("gid")].toString();
        cacheContact.uid = refId[QStringLiteral("uid")].toString();

        for (const Akonadi::Item &item : fetchJob->items()) {
            if (item.isValid()) {
                if ((!item.gid().isEmpty() && refId[QStringLiteral("gid")].toString() == item.gid())
                    || QString::number(item.id()) == refId[QStringLiteral("uid")].toString()) {
                    if (item.hasPayload<KContacts::Addressee>()) {
                        mMonitor->setItemMonitored(item);
                        const KContacts::Addressee contact = item.payload<KContacts::Addressee>();
                        cacheContact.name = contact.realName();
                        cacheContact.email = contact.preferredEmail();
                    }
                }
            }
        }
        if (mGroupsCache.contains(index)) {
            mGroupsCache[index].append(cacheContact);
        }
    }

    Q_EMIT dataChanged(index, index, mKrole);
    if (mPendingGroups.contains(groupId)) {
        mPendingGroups.removeOne(groupId);
    }
}

void ContactInfoProxyModel::slotItemChanged(const Akonadi::Item &item, const QSet<QByteArray> &partIdentifiers)
{
    Q_UNUSED(partIdentifiers)
    for (const QPersistentModelIndex &index : mGroupsCache.keys()) {
        ContactCacheData::ListIterator it = findCacheItem(index, item);
        if (it != mGroupsCache[index].end()) {
            if (item.isValid()) {
                if (item.hasPayload<KContacts::Addressee>()) {
                    const KContacts::Addressee contact = item.payload<KContacts::Addressee>();
                    it->uid = QString::number(item.id());
                    it->gid = item.gid();
                    it->name = contact.realName();
                    it->email = contact.preferredEmail();
                    Q_EMIT dataChanged(index, index, mKrole);
                }
            }
        }
    }
}

void ContactInfoProxyModel::slotItemRemoved(const Akonadi::Item &item)
{
    if (item.isValid()) {
        for (const QPersistentModelIndex &index : mGroupsCache.keys()) {
            ContactCacheData::List::iterator it = findCacheItem(index, item);

            if (it != mGroupsCache[index].end()) {
                mGroupsCache[index].erase(it);
                Q_EMIT dataChanged(index, index, mKrole);
            }
        }
    }
}

bool operator==(const ContactInfoProxyModel::ContactCacheData &lhs, const ContactInfoProxyModel::ContactCacheData &rhs)
{
    return !lhs.gid.isEmpty() ? lhs.gid == rhs.gid : !lhs.uid.isEmpty() ? lhs.uid == rhs.uid : false;
}
