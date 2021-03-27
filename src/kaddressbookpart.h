/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2009-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KParts/Event>
#include <KParts/Part>
#include <kparts/readonlypart.h>

class MainWidget;

class KAddressBookPart : public KParts::Part
{
    Q_OBJECT

public:
    KAddressBookPart(QWidget *parentWidget, QObject *parent, const QVariantList &);
    ~KAddressBookPart() override;

public Q_SLOTS:
    void newContact();
    void newGroup();
    void updateQuickSearchText();

protected:
    void guiActivateEvent(KParts::GUIActivateEvent *) override;
    void initAction();

private:
    MainWidget *mMainWidget = nullptr;
};

