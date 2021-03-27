/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2009 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

class QLineEdit;

/**
 * @short The quick search widget from the toolbar
 *
 * This widget allows the user to filter for contacts
 * that match a given string criteria.
 * The filter string the user enters here is emitted to
 * the ContactsFilterModel, which does the real filtering.
 *
 * @author Tobias Koenig <tokoe@kde.org>
 */
class QuickSearchWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * Creates the quick search widget.
     *
     * @param parent The parent widget.
     */
    explicit QuickSearchWidget(QWidget *parent = nullptr);

    /**
     * Destroys the quick search widget.
     */
    ~QuickSearchWidget() override;

    /**
     * Returns the size hint of the quick search widget.
     */
    QSize sizeHint() const override;

    void updateQuickSearchText(const QString &text);

public Q_SLOTS:
    void slotFocusQuickSearch();

Q_SIGNALS:
    /**
     * This signal is emitted whenever the user has changed
     * the filter string in the line edit.
     *
     * @param filterString The new filter string.
     */
    void filterStringChanged(const QString &filterString);

    /**
     * This signal is emitted whenever the user pressed the
     * arrow down key. In this case we set the focus on the
     * item view that shows the contacts, so the user can
     * navigate much faster.
     */
    void arrowDownKeyPressed();

protected:
    void keyPressEvent(QKeyEvent *) override;

private:
    void resetTimer();
    void delayedTextChanged();
    QLineEdit *mEdit = nullptr;
    QTimer *mTimer = nullptr;
};

