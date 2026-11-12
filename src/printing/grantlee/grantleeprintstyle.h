/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "printstyle.h"

namespace KABPrinting
{
class GrantleePrintStyle : public PrintStyle
{
    Q_OBJECT
public:
    explicit GrantleePrintStyle(QString themePath, PrintingWizard *parent);
    ~GrantleePrintStyle() override;

    void print(const KContacts::Addressee::List &, PrintProgress *) override;

private:
    const QString m_themePath;
};

class GrantleeStyleFactory : public PrintStyleFactory
{
public:
    explicit GrantleeStyleFactory(QString name, QString themePath, PrintingWizard *parent);

    PrintStyle *create() const override;
    QString description() const override;

private:
    QString mThemePath;
    QString mName;
};
}
