/*
    SPDX-FileCopyrightText: 2014 Jonathan Marten <jjm@keelhaul.me.uk>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "categoryfilterproxymodel.h"
using namespace Qt::Literals::StringLiterals;

#include "kaddressbook_debug.h"

#include <Akonadi/EntityTreeModel>
#include <Akonadi/Item>

#include <KContacts/Addressee>
#include <KContacts/ContactGroup>

#include "categoryselectwidget.h"

using namespace Akonadi;

class CategoryFilterProxyModelPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(CategoryFilterProxyModel)

public:
    explicit CategoryFilterProxyModelPrivate(CategoryFilterProxyModel *parent);

    QList<Tag> filterTagList;
    bool filterEnabled = false;

    bool containsId(Akonadi::Tag::Id id) const
    {
        for (const Tag &tag : filterTagList) {
            if (tag.id() == id) {
                return true; // a category matches filter
            }
        }
        return false;
    }
    bool containsName(const QString &cat) const
    {
        for (const Tag &tag : filterTagList) {
            if (tag.name() == cat) {
                return true; // a category matches filter
            }
        }
        return false;
    }

private:
    CategoryFilterProxyModel *const q_ptr;
};

CategoryFilterProxyModelPrivate::CategoryFilterProxyModelPrivate(CategoryFilterProxyModel *parent)
    : q_ptr(parent)
{
}

CategoryFilterProxyModel::CategoryFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , d_ptr(new CategoryFilterProxyModelPrivate(this))
{
}

CategoryFilterProxyModel::~CategoryFilterProxyModel() = default;

bool CategoryFilterProxyModel::filterAcceptsRow(int row, const QModelIndex &parent) const
{
    Q_D(const CategoryFilterProxyModel);

    const QModelIndex index = sourceModel()->index(row, 0, parent);
    const auto item = index.data(EntityTreeModel::ItemRole).value<Akonadi::Item>();

    if (!d->filterEnabled) {
        return true; // filter not enabled
    }
    if (d->filterTagList.isEmpty()) {
        return false; // nothing accepted
    }
    // all accepted
    if (d->filterTagList.at(0).id() == CategorySelectWidget::FilterAll) {
        return true;
    }

    if (item.hasPayload<KContacts::Addressee>()) {
        const auto contact = item.payload<KContacts::Addressee>();

        const QStringList categories = contact.categories();

        int validCategories = 0;
        int count = categories.count();
        for (int i = 0; i < count; ++i) {
            const QString cat = categories.at(i);
            if (cat.startsWith("akonadi:"_L1)) {
                const int idx = cat.indexOf("?tag="_L1);
                if (idx >= 0) {
                    ++validCategories;
                    Tag::Id id = QStringView(cat).mid(idx + 5).toInt();
                    if (d->containsId(id)) {
                        return true; // a category matches filter
                    }
                }
            } else {
                ++validCategories;
                if (d->containsName(cat)) {
                    return true; // a category matches filter
                }
            }
        }

        if (validCategories > 0) {
            return false; // categorised but no match
        } else {
            return d->containsId(CategorySelectWidget::FilterUntagged);
        }
    } else if (item.hasPayload<KContacts::ContactGroup>()) { // a contact group item
        return d->containsId(CategorySelectWidget::FilterGroups);
    }

    return true; // not a recognised item
}

void CategoryFilterProxyModel::setFilterCategories(const QList<Akonadi::Tag> &tagList)
{
    Q_D(CategoryFilterProxyModel);

    if (tagList != d->filterTagList) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
        beginFilterChange();
#endif
        d->filterTagList = tagList;
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
        endFilterChange(QSortFilterProxyModel::Direction::Rows);
#else
        invalidateFilter();
#endif
    }
}

void CategoryFilterProxyModel::setFilterEnabled(bool enable)
{
    Q_D(CategoryFilterProxyModel);

    if (enable != d->filterEnabled) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
        beginFilterChange();
#endif
        d->filterEnabled = enable;
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
        endFilterChange(QSortFilterProxyModel::Direction::Rows);
#else
        invalidateFilter();
#endif
    }
}

#include "categoryfilterproxymodel.moc"

#include "moc_categoryfilterproxymodel.cpp"
