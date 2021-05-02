/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2020 Konrad Czapla <kondzio89dev@gmail.com>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "stylecontactlistdelegate.h"
#include "contactinfoproxymodel.h"

#include <Akonadi/Contact/ContactsTreeModel>

#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QPainterPath>

StyleContactListDelegate::StyleContactListDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
    , mKImageSize(50, 50)
{
}

void StyleContactListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_ASSERT(index.isValid());

    if (Akonadi::ContactsTreeModel::Column::FullName == index.column()) {
        QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter);

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

        QPainterPath path;
        path.addEllipse(pictureRect);
        painter->save();
        painter->setClipPath(path);
        painter->setPen(QPen(Qt::darkGray, qreal(4)));
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

        const auto name = index.data(Qt::DisplayRole).value<QString>();
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
