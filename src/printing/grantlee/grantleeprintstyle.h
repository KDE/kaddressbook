/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "printstyle.h"

namespace KAddressBookGrantlee
{
class GrantleePrint;
}

namespace KABPrinting
{
class PrintProgress;

class GrantleePrintStyle : public PrintStyle
{
    Q_OBJECT
public:
    explicit GrantleePrintStyle(const QString &themePath, PrintingWizard *parent);
    ~GrantleePrintStyle() override;

    void print(const KContacts::Addressee::List &, PrintProgress *) override;

private:
    const QString m_themePath;
};

class GrantleeStyleFactory : public PrintStyleFactory
{
public:
    explicit GrantleeStyleFactory(const QString &name, const QString &themePath, PrintingWizard *parent);

    PrintStyle *create() const override;
    QString description() const override;

private:
    QString mThemePath;
    QString mName;
};
}

