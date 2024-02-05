/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2020 Konrad Czapla <kondzio89dev@gmail.com>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "stylecontactlistdelegate.h"
#include "contactinfoproxymodel.h"

#include <Akonadi/ContactsTreeModel>

#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QPainterPath>

namespace
{
// SPDX-SnippetBegin
// SPDX-FileCopyrightText: 2020 Carson Black <uhhadd@gmail.com>
// SPDX-License-Identifier: LGPL-2.0-or-later

/* clang-format off */
const QList<QColor> c_colors = {
    QColor("#e93a9a"),
    QColor("#e93d58"),
    QColor("#e9643a"),
    QColor("#ef973c"),
    QColor("#e8cb2d"),
    QColor("#b6e521"),
    QColor("#3dd425"),
    QColor("#00d485"),
    QColor("#00d3b8"),
    QColor("#3daee9"),
    QColor("#b875dc"),
    QColor("#926ee4"),
};
/* clang-format on */

QColor colorsFromString(const QString &string)
{
    // We use a hash to get a "random" number that's always the same for
    // a given string.
    auto hash = qHash(string);
    // hash modulo the length of the colors list minus one will always get us a valid
    // index
    auto index = hash % (c_colors.length() - 1);
    // return a colour
    return c_colors[index];
}

// SPDX-SnippetEnd

}

StyleContactListDelegate::StyleContactListDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
    , mKImageSize(50, 50)
{
}

void StyleContactListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_ASSERT(index.isValid());

    if (Akonadi::ContactsTreeModel::Column::FullName == index.column()) {
        QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, option.widget);

        const QRectF optionRect = option.rect.marginsRemoved(QMargins() + static_cast<int>(mKMargin));

        QRectF pictureRect = QRectF(optionRect.topLeft(), mKImageSize);
        if (mKImageSize.width() > optionRect.size().width()) {
            const qreal width = option.rect.size().width();
            const qreal height = option.rect.size().height();
            const QMargins fitMargins(0, (qMax(width, height) - qMin(width, height)) / qreal(2), 0, (qMax(width, height) - qMin(width, height)) / qreal(2));

            pictureRect = optionRect.marginsRemoved(fitMargins);
        }
        QRectF nameTextRect(optionRect.topLeft(), QSize(optionRect.width(), optionRect.height() / qreal(2)));
        QRectF descriptionTextRect = nameTextRect;
        descriptionTextRect.moveBottomLeft(optionRect.bottomLeft());

        QMargins textMargin;
        switch (static_cast<int>(option.widget->layoutDirection())) {
        case Qt::LayoutDirection::LeftToRight:
            textMargin.setLeft(mKMargin);

            nameTextRect.setLeft(pictureRect.bottomRight().x());
            nameTextRect = nameTextRect.marginsRemoved(textMargin);

            descriptionTextRect.setLeft(pictureRect.bottomRight().x());
            descriptionTextRect = descriptionTextRect.marginsRemoved(textMargin);
            break;
        case Qt::LayoutDirection::RightToLeft:
            pictureRect.moveRight(optionRect.bottomRight().x());
            textMargin.setRight(mKMargin);

            nameTextRect.setRight(pictureRect.bottomLeft().x());
            nameTextRect = nameTextRect.marginsRemoved(textMargin);

            descriptionTextRect.setRight(pictureRect.bottomLeft().x());
            descriptionTextRect = descriptionTextRect.marginsRemoved(textMargin);
            break;
        }

        const auto name = index.data(Qt::DisplayRole).value<QString>();

        QPainterPath path;
        path.addEllipse(pictureRect);
        painter->save();
        painter->setClipPath(path);
        painter->setPen(QPen(colorsFromString(name), qreal(4)));
        painter->setRenderHint(QPainter::Antialiasing);
        painter->drawPath(path);
        painter->setFont(QFont(option.font.family(), 12, QFont::Bold, true));

        auto image(index.data(ContactInfoProxyModel::Roles::PictureRole).value<QImage>());
        if (image.isNull()) {
            const auto initials = index.data(ContactInfoProxyModel::Roles::InitialsRole).value<QString>();
            painter->drawText(pictureRect, Qt::AlignCenter, painter->fontMetrics().elidedText(initials, Qt::ElideRight, pictureRect.width() - qreal(10)));
        } else {
            const qreal dpr = qApp->devicePixelRatio();
            image = image.scaled(mKImageSize * dpr, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            image.setDevicePixelRatio(dpr);
            painter->drawImage(pictureRect, image);
        }

        painter->restore();

        if (!name.isEmpty()) {
            painter->setFont(QFont(option.font.family(), 11));
            painter->drawText(nameTextRect, Qt::AlignLeft | Qt::AlignVCenter, painter->fontMetrics().elidedText(name, Qt::ElideRight, nameTextRect.width()));
        }

        const auto description = index.data(ContactInfoProxyModel::Roles::DescriptionRole).value<QString>();
        if (!description.isEmpty()) {
            painter->setFont(QFont(option.font.family(), 8, -1, true));
            painter->drawText(descriptionTextRect,
                              Qt::AlignLeft | Qt::AlignVCenter,
                              painter->fontMetrics().elidedText(description, Qt::ElideRight, descriptionTextRect.width()));
        }
        return;
    }
    QStyledItemDelegate::paint(painter, option, index);
}

QSize StyleContactListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(mKImageSize.height() + (qreal(2) * mKMargin));

    return size;
}

#include "moc_stylecontactlistdelegate.cpp"
