/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2002 Jost Schenck <jost@schenck.de>
  SPDX-FileCopyrightText: 2009 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later WITH Qt-Commercial-exception-1.0
*/

#pragma once

#include "printstyle.h"

namespace KABPrinting
{
class RingBinderStyleAppearanceForm;

class RingBinderPrintStyle : public PrintStyle
{
public:
    explicit RingBinderPrintStyle(PrintingWizard *parent);
    ~RingBinderPrintStyle() override;

    void print(const KContacts::Addressee::List &contacts, PrintProgress *) override;

private:
    RingBinderStyleAppearanceForm *mPageAppearance = nullptr;
};

class RingBinderPrintStyleFactory : public PrintStyleFactory
{
public:
    explicit RingBinderPrintStyleFactory(PrintingWizard *parent);

    PrintStyle *create() const override;
    QString description() const override;
};
}

