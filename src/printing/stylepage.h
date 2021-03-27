/*
  This file is part of KAddressBook.
  SPDX-FileCopyrightText: 2002 Anders Lund <anders.lund@lund.tdcadsl.dk>
  SPDX-FileCopyrightText: Tobias Koenig <tokoe@kde.org>
  SPDX-FileCopyrightText: 2009-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later WITH Qt-Commercial-exception-1.0
*/

#pragma once

#include "importexport/contactfields.h"
#include <QWidget>

class QLabel;
class QPixmap;
class QComboBox;

class StylePage : public QWidget
{
    Q_OBJECT

public:
    explicit StylePage(QWidget *parent = nullptr, const QString &name = QString());
    ~StylePage() override;

    /**
     * Set a preview image. If @p pixmap is 'null' a text will
     * be displayed instead.
     */
    void setPreview(const QPixmap &pixmap);

    /**
     * Add a style name.
     */
    void addStyleName(const QString &name);

    /**
     * Clear the style name list.
     */
    void clearStyleNames();

    /**
     * Set the sort criterion field.
     */
    void setSortField(KAddressBookImportExport::ContactFields::Field field);

    /**
     * Returns the sort criterion field.
     */
    KAddressBookImportExport::ContactFields::Field sortField() const;

    /**
     * Sets the sort order.
     */
    void setSortOrder(Qt::SortOrder sortOrder);

    /**
     * Returns the sort order.
     */
    Qt::SortOrder sortOrder() const;

    /**
     * Returns the sort order.
     */
    int printingStyle() const;

    /**
     * Returns the sort order.
     */
    void setPrintingStyle(int index);

Q_SIGNALS:
    /**
     * This signal is emitted when the user selects a new style in the
     * style combo box.
     */
    void styleChanged(int index);

private:
    void initGUI();
    void initFieldCombo();

    QComboBox *mFieldCombo = nullptr;
    QComboBox *mSortTypeCombo = nullptr;
    QComboBox *mStyleCombo = nullptr;
    QLabel *mPreview = nullptr;

    KAddressBookImportExport::ContactFields::Fields mFields;
};

