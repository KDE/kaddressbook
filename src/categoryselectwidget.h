/*
    SPDX-FileCopyrightText: 2014 Jonathan Marten <jjm@keelhaul.me.uk>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <AkonadiCore/tag.h>
#include <QWidget>

class CategorySelectWidgetPrivate;

/**
 * @short A widget to specify a category (tag) filter.
 *
 * @since 4.14
 * @author Jonathan Marten
 **/

class CategorySelectWidget : public QWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(CategorySelectWidget)

public:
    /**
     * Constructor.
     *
     * @param parent The parent widget
     **/
    explicit CategorySelectWidget(QWidget *parent = nullptr);

    /**
     * Destructor.
     **/
    ~CategorySelectWidget() override;

    /**
     * Special @c Akonadi::Tag::Id values for filtering.
     **/
    enum FilterTag {
        FilterAll = -2, /**< All items */
        FilterUntagged = -3, /**< Untagged items */
        FilterGroups = -4 /**< Contact groups */
    };

    /**
     * Get the current tag filter list.
     *
     * @return The filter list, as would be sent by @c filterChanged()
     *
     * @see filterChanged
     **/
    QList<Akonadi::Tag::Id> filterTags() const;

Q_SIGNALS:
    /**
     * The tag filter selection has changed.
     *
     * @param idList A list of @c Akonadi::Tag::Id's of the tags which
     * are to be included in the filter.
     *
     * @see CategorySelectModel::filterChanged
     **/
    void filterChanged(const QList<Akonadi::Tag::Id> &idList);

private:
    CategorySelectWidgetPrivate *const d_ptr;
};

