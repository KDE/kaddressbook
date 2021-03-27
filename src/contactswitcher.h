/*
    SPDX-FileCopyrightText: 2009 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

class QAbstractItemView;
class QLabel;
class QPushButton;

/**
 * @short A widget to switch between the contacts of an contact view.
 *
 * This widgets provides a 'Next' and 'Previous' button to allow the
 * user to switch between the entries of a contact view.
 *
 * @author Tobias Koenig <tokoe@kde.org>
 */
class ContactSwitcher : public QWidget
{
    Q_OBJECT

public:
    /**
     * Creates a new contact switcher.
     *
     * @param parent The parent widget.
     */
    explicit ContactSwitcher(QWidget *parent = nullptr);

    /**
     * Sets the @p view the contact switcher shall work on.
     */
    void setView(QAbstractItemView *view);

private:
    /**
     * This slot is called when the 'Next' button is clicked.
     */
    void nextClicked();

    /**
     * This slot is called when the 'Previous' button is clicked.
     */
    void previousClicked();

    /**
     * This slot is called whenever the layout of the model has changed
     * or the user has clicked a button.
     */
    void updateStatus();

    QAbstractItemView *mView = nullptr;
    QPushButton *mNextButton = nullptr;
    QPushButton *mPreviousButton = nullptr;
    QLabel *mStatusLabel = nullptr;
};

