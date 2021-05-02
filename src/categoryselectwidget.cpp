/*
    SPDX-FileCopyrightText: 2014 Jonathan Marten <jjm@keelhaul.me.uk>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "categoryselectwidget.h"

#include "kaddressbook_debug.h"
#include <AkonadiCore/monitor.h>
#include <AkonadiCore/tagmodel.h>
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QTimer>
#include <QToolButton>

#include <Libkdepim/KCheckComboBox>

using namespace Akonadi;

static const int FILTER_ROLE = Qt::UserRole + 1;

class CategorySelectWidgetPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(CategorySelectWidget)

public:
    explicit CategorySelectWidgetPrivate(CategorySelectWidget *parent);

    Akonadi::TagModel *tagModel = nullptr;
    int rowOffset = 0;
    QTimer *updateTimer = nullptr;
    KPIM::KCheckComboBox *checkCombo = nullptr;

    void init();
    QStandardItemModel *itemModel() const;
    void selectAll(Qt::CheckState state) const;
    QList<Akonadi::Tag::Id> filterTags() const;

public Q_SLOTS:
    void slotSelectAll();
    void slotSelectNone();

    void slotTagsInserted(const QModelIndex &parent, int start, int end);
    void slotTagsRemoved(const QModelIndex &parent, int start, int end);
    void slotTagsChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

    void slotCheckedItemsChanged();
    void slotCheckedItemsTimer();

private:
    CategorySelectWidget *const q_ptr;
};

CategorySelectWidgetPrivate::CategorySelectWidgetPrivate(CategorySelectWidget *parent)
    : QObject()
    , q_ptr(parent)
{
}

void CategorySelectWidgetPrivate::init()
{
    Q_Q(CategorySelectWidget);

    auto hbox = new QHBoxLayout(q);
    hbox->setSpacing(0);
    hbox->setContentsMargins(0, 0, 0, 0);

    checkCombo = new KPIM::KCheckComboBox;
    checkCombo->setMinimumWidth(150);
    checkCombo->setSqueezeText(true);
    connect(checkCombo, &KPIM::KCheckComboBox::checkedItemsChanged, this, &CategorySelectWidgetPrivate::slotCheckedItemsChanged);
    hbox->addWidget(checkCombo);

    auto monitor = new Monitor(this);
    monitor->setTypeMonitored(Monitor::Tags);
    tagModel = new Akonadi::TagModel(monitor, this);

    connect(tagModel, &QAbstractItemModel::rowsInserted, this, &CategorySelectWidgetPrivate::slotTagsInserted);
    connect(tagModel, &QAbstractItemModel::rowsRemoved, this, &CategorySelectWidgetPrivate::slotTagsRemoved);
    connect(tagModel, &QAbstractItemModel::dataChanged, this, &CategorySelectWidgetPrivate::slotTagsChanged);

    updateTimer = new QTimer(this);
    updateTimer->setSingleShot(true);
    updateTimer->setInterval(200);
    connect(updateTimer, &QTimer::timeout, this, &CategorySelectWidgetPrivate::slotCheckedItemsTimer);

    auto but = new QToolButton(q);
    but->setAutoRaise(true);
    but->setIcon(QIcon::fromTheme(QStringLiteral("edit-undo")));
    but->setToolTip(i18nc("@action:button", "Reset category filter"));
    connect(but, &QToolButton::clicked, this, &CategorySelectWidgetPrivate::slotSelectAll);
    hbox->addWidget(but);

    but = new QToolButton(q);
    but->setAutoRaise(true);
    but->setIcon(QIcon::fromTheme(QStringLiteral("edit-clear")));
    but->setToolTip(i18nc("@action:button", "Clear category filter"));
    connect(but, &QToolButton::clicked, this, &CategorySelectWidgetPrivate::slotSelectNone);
    hbox->addWidget(but);

    auto item = new QStandardItem(i18n("(Untagged)"));
    item->setCheckState(Qt::Checked);
    item->setData(CategorySelectWidget::FilterUntagged, FILTER_ROLE);
    itemModel()->appendRow(item);

    item = new QStandardItem(i18n("(Groups)"));
    item->setCheckState(Qt::Checked);
    item->setData(CategorySelectWidget::FilterGroups, FILTER_ROLE);
    itemModel()->appendRow(item);

    checkCombo->setAlwaysShowDefaultText(false);
    checkCombo->setDefaultText(i18n("(All)"));

    rowOffset = itemModel()->rowCount();
}

QStandardItemModel *CategorySelectWidgetPrivate::itemModel() const
{
    auto m = qobject_cast<QStandardItemModel *>(checkCombo->model());
    Q_ASSERT(m != nullptr);
    return m;
}

void CategorySelectWidgetPrivate::slotTagsRemoved(const QModelIndex &parent, int start, int end)
{
    itemModel()->removeRows(start + rowOffset, end + rowOffset, parent);
}

void CategorySelectWidgetPrivate::slotTagsChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
        QStandardItem *it = itemModel()->item(row + rowOffset);
        Q_ASSERT(it != nullptr);

        QModelIndex idx = tagModel->index(row, 0);
        it->setText(tagModel->data(idx, TagModel::NameRole).toString());
        it->setIcon(tagModel->data(idx, Qt::DecorationRole).value<QIcon>());
        it->setData(tagModel->data(idx, TagModel::IdRole), FILTER_ROLE);
    }
}

void CategorySelectWidgetPrivate::slotTagsInserted(const QModelIndex &parent, int start, int end)
{
    for (int row = start; row <= end; ++row) {
        QModelIndex idx = tagModel->index(row, 0, parent);
#if 0
        qCDebug(KADDRESSBOOK_LOG) << "idx" << idx << "=" << tagModel->data(idx, Qt::DisplayRole).toString()
                                  << "name" << tagModel->data(idx, TagModel::NameRole).toString()
                                  << "tag" << tagModel->data(idx, TagModel::TagRole)
                                  << "id" << tagModel->data(idx, TagModel::IdRole).toInt();
#endif
        auto it = new QStandardItem(tagModel->data(idx, TagModel::NameRole).toString());
        it->setIcon(tagModel->data(idx, Qt::DecorationRole).value<QIcon>());
        it->setData(tagModel->data(idx, TagModel::IdRole), FILTER_ROLE);
        it->setCheckState(Qt::Checked);

        // If a tag with a parent arrives from the model, we know that its parent
        // must already have arrived.  So there is no need for a list of pending
        // tags, as is required in Akonadi::TagModel.
        //
        // FIXME: not tested (no way to create hierarchical tags at present)
        if (parent != QModelIndex()) {
            const auto parentId = tagModel->data(idx, TagModel::IdRole).value<Tag::Id>();
            QModelIndexList matchList = itemModel()->match(itemModel()->index(0, 0), FILTER_ROLE, parentId, 1, Qt::MatchExactly | Qt::MatchRecursive);
            if (matchList.count() == 1) { // found the parent tag
                QModelIndex parentIndex = matchList.at(0);
                itemModel()->itemFromIndex(parentIndex)->appendRow(it);
            } else {
                qCWarning(KADDRESSBOOK_LOG) << "Cannot find parent with ID" << parentId;
                itemModel()->insertRow(row + rowOffset, it);
            }
        } else {
            itemModel()->insertRow(row + rowOffset, it);
        }
    }
}

void CategorySelectWidgetPrivate::selectAll(Qt::CheckState state) const
{
    for (int row = 0; row < itemModel()->rowCount(); ++row) {
        QStandardItem *it = itemModel()->item(row);
        it->setCheckState(state);
    }
}

void CategorySelectWidgetPrivate::slotSelectAll()
{
    selectAll(Qt::Checked);
}

void CategorySelectWidgetPrivate::slotSelectNone()
{
    selectAll(Qt::Unchecked);
}

void CategorySelectWidgetPrivate::slotCheckedItemsChanged()
{
    updateTimer->start();
}

void CategorySelectWidgetPrivate::slotCheckedItemsTimer()
{
    Q_Q(CategorySelectWidget);

    bool allOn = true;
    for (int row = 0; row < itemModel()->rowCount(); ++row) {
        const QStandardItem *it = itemModel()->item(row);
        Qt::CheckState rowState = static_cast<Qt::CheckState>(it->data(Qt::CheckStateRole).toInt());
        if (rowState != Qt::Checked) {
            allOn = false;
            break;
        }
    }

    if (allOn) {
        checkCombo->setAlwaysShowDefaultText(true);
        checkCombo->setDefaultText(i18n("(All)"));
    } else {
        checkCombo->setAlwaysShowDefaultText(false);
        checkCombo->setDefaultText(i18n("(None)"));
    }

    const QStringList checkedList = checkCombo->checkedItems();
    if (!checkedList.isEmpty()) {
        checkCombo->setToolTip(i18n("<qt>Category filter: %1", checkedList.join(i18n(", "))));
    } else {
        checkCombo->setToolTip(QString());
    }

    Q_EMIT q->filterChanged(filterTags());
}

QList<Akonadi::Tag::Id> CategorySelectWidgetPrivate::filterTags() const
{
    QList<Tag::Id> filter;
    bool allOn = true;
    for (int row = 0; row < itemModel()->rowCount(); ++row) {
        const QStandardItem *it = itemModel()->item(row);
        Q_ASSERT(it != nullptr);
        if (it->checkState() == Qt::Checked) {
            Tag::Id id = it->data(FILTER_ROLE).toInt();
            if (id != 0) {
                filter.append(id);
            }
        } else {
            allOn = false;
        }
    }

    if (allOn) {
        filter.clear();
        filter.append(CategorySelectWidget::FilterAll);
    }

    // qCDebug(KADDRESSBOOK_LOG) << "filter" << filter;
    return filter;
}

CategorySelectWidget::CategorySelectWidget(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new CategorySelectWidgetPrivate(this))
{
    Q_D(CategorySelectWidget);
    d->init();
}

CategorySelectWidget::~CategorySelectWidget()
{
    delete d_ptr;
}

QList<Akonadi::Tag::Id> CategorySelectWidget::filterTags() const
{
    Q_D(const CategorySelectWidget);
    return d->filterTags();
}

#include "categoryselectwidget.moc"
