/*
    SPDX-FileCopyrightText: 2014 Jonathan Marten <jjm@keelhaul.me.uk>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QSortFilterProxyModel>

#include <AkonadiCore/tag.h>

class CategoryFilterProxyModelPrivate;

/**
 * @short A proxy model to filter contacts by categories (tags).
 *
 * @since 4.14
 * @author Jonathan Marten
 **/

class CategoryFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(CategoryFilterProxyModel)

public:
    /**
     * Constructor.
     *
     * @param parent The parent object
     **/
    explicit CategoryFilterProxyModel(QObject *parent = nullptr);

    /**
     * Destructor.
     **/
    ~CategoryFilterProxyModel() override;

public Q_SLOTS:
    /**
     * Set the categories to be accepted by the filter.
     *
     * @param idList A list of @c Akonadi::Tag::Id's of the categories
     * which are to be accepted by the filter.
     * @see CategorySelectModel::filterChanged
     **/
    void setFilterCategories(const QList<Akonadi::Tag::Id> &idList);

    /**
     * Enable or disable the filter.
     *
     * @param enable If @c true, enable the filter to accept only those categories
     * set by @c setFilterCategories().  If false, disable the filter so that all
     * entries are accepted.
     *
     * The default state is that the filter is disabled.
     **/
    void setFilterEnabled(bool enable);

protected:
    /**
     * @reimp
     **/
    bool filterAcceptsRow(int row, const QModelIndex &parent) const override;

private:
    CategoryFilterProxyModelPrivate *const d_ptr;
};

