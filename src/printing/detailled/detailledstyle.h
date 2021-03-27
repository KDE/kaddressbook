/*
  This file is part of KAddressBook.
  SPDX-FileCopyrightText: 1996-2002 Mirko Boehm <mirko@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later WITH Qt-Commercial-exception-1.0
*/

#pragma once

#include "printing/printstyle.h"

#include <KContacts/Addressee>

namespace KABPrinting
{
class AppearancePage;

class DetailledPrintStyle : public PrintStyle
{
    Q_OBJECT

public:
    explicit DetailledPrintStyle(PrintingWizard *parent);
    ~DetailledPrintStyle() override;

    void print(const KContacts::Addressee::List &contacts, PrintProgress *) override;

private:
    AppearancePage *mPageAppearance = nullptr;
};

class DetailledPrintStyleFactory : public PrintStyleFactory
{
public:
    explicit DetailledPrintStyleFactory(PrintingWizard *parent);

    PrintStyle *create() const override;
    QString description() const override;
};
}

