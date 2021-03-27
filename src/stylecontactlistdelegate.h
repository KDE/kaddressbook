/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2020 Konrad Czapla <kondzio89dev@gmail.com>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QStyledItemDelegate>

class StyleContactListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit StyleContactListDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    const QSize mKImageSize;
    const qreal mKMargin = 5.0;
};

