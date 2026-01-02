/*
  This file is part of KAddressBook.
  SPDX-FileCopyrightText: 1996-2002 Mirko Boehm <mirko@kde.org>
  SPDX-FileCopyrightText: 2002 Mike Pilone <mpilone@slack.com>

  SPDX-License-Identifier: GPL-2.0-or-later WITH LicenseRef-Qt-Commercial-exception-1.0
*/

#pragma once

#include "printstyle.h"

namespace KABPrinting
{
class PrintProgress;

class MikesStyle : public PrintStyle
{
    Q_OBJECT

public:
    explicit MikesStyle(PrintingWizard *parent);
    ~MikesStyle() override;

    void print(const KContacts::Addressee::List &, PrintProgress *) override;
};

class MikesStyleFactory : public PrintStyleFactory
{
public:
    explicit MikesStyleFactory(PrintingWizard *parent);

    PrintStyle *create() const override;
    QString description() const override;
};
}
