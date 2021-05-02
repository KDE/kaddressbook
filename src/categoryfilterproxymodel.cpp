/*
    SPDX-FileCopyrightText: 2014 Jonathan Marten <jjm@keelhaul.me.uk>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "categoryfilterproxymodel.h"

#include "kaddressbook_debug.h"

#include <AkonadiCore/entitytreemodel.h>
#include <AkonadiCore/item.h>

#include <kcontacts/addressee.h>
#include <kcontacts/contactgroup.h>

#include "categoryselectwidget.h"

using namespace Akonadi;

class CategoryFilterProxyModelPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(CategoryFilterProxyModel)

public:
    CategoryFilterProxyModelPrivate(CategoryFilterProxyModel *parent);

    QList<Tag::Id> filterIdList;
    bool filterEnabled = false;

private:
    CategoryFilterProxyModel *const q_ptr;
};

CategoryFilterProxyModelPrivate::CategoryFilterProxyModelPrivate(CategoryFilterProxyModel *parent)
    : QObject()
    , q_ptr(parent)
{
}

CategoryFilterProxyModel::CategoryFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , d_ptr(new CategoryFilterProxyModelPrivate(this))
{
    setDynamicSortFilter(true);
}

CategoryFilterProxyModel::~CategoryFilterProxyModel()
{
    delete d_ptr;
}

bool CategoryFilterProxyModel::filterAcceptsRow(int row, const QModelIndex &parent) const
{
    Q_D(const CategoryFilterProxyModel);

    const QModelIndex index = sourceModel()->index(row, 0, parent);
    const auto item = index.data(EntityTreeModel::ItemRole).value<Akonadi::Item>();

    if (!d->filterEnabled) {
        return true; // filter not enabled
    }
    if (d->filterIdList.isEmpty()) {
        return false; // nothing accepted
    }
    // all accepted
    if (d->filterIdList.at(0) == CategorySelectWidget::FilterAll) {
        return true;
    }

    if (item.hasPayload<KContacts::Addressee>()) {
        const auto contact = item.payload<KContacts::Addressee>();

        const QStringList categories = contact.categories();

        int validCategories = 0;
        int count = categories.count();
        for (int i = 0; i < count; ++i) {
            const QString cat = categories.at(i);
            if (cat.startsWith(QLatin1String("akonadi:"))) {
                const int idx = cat.indexOf(QLatin1String("?tag="));
                if (idx >= 0) {
                    ++validCategories;
                    Tag::Id id = cat.midRef(idx + 5).toInt();
                    if (d->filterIdList.contains(id)) {
                        return true; // a category matches filter
                    }
                }
            }
        }

        if (validCategories > 0) {
            return false; // categorised but no match
        } else {
            return d->filterIdList.contains(CategorySelectWidget::FilterUntagged);
        }
    } else if (item.hasPayload<KContacts::ContactGroup>()) { // a contact group item
        return d->filterIdList.contains(CategorySelectWidget::FilterGroups);
    }

    return true; // not a recognised item
}

void CategoryFilterProxyModel::setFilterCategories(const QList<Akonadi::Tag::Id> &idList)
{
    Q_D(CategoryFilterProxyModel);

    if (idList != d->filterIdList) {
        d->filterIdList = idList;
        invalidateFilter();
    }
}

void CategoryFilterProxyModel::setFilterEnabled(bool enable)
{
    Q_D(CategoryFilterProxyModel);

    if (enable != d->filterEnabled) {
        d->filterEnabled = enable;
        invalidateFilter();
    }
}

#include "categoryfilterproxymodel.moc"
