/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2009 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <Akonadi/Contact/ContactsTreeModel>

#include <QObject>

class QWidget;

/**
 * @short A manager for the contacts model columns.
 *
 * This class manages which columns shall be provided by the
 * contacts model. It keeps track of the configuration file
 * and the user configuration.
 *
 * @author Tobias Koenig <tokoe@kde.org>
 */
class ModelColumnManager : public QObject
{
    Q_OBJECT
public:
    /**
     * Creates a new model column manager.
     *
     * @param model The model that shall be managed.
     * @param parent The parent object.
     */
    explicit ModelColumnManager(Akonadi::ContactsTreeModel *model, QObject *parent = nullptr);

    /**
     * Loads the user configuration and applies it to the model.
     */
    void load();

    /**
     * Stores the user configuration.
     */
    void store();

    /**
     * Sets the widget that shall provide a RMB menu to
     * configure the columns to be shown.
     */
    void setWidget(QWidget *view);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void adaptHeaderView();
    Akonadi::ContactsTreeModel *const mModel;
    Akonadi::ContactsTreeModel::Columns mColumns;
    QWidget *mWidget = nullptr;
};

