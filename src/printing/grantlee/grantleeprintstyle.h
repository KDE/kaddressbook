/*
   Copyright (C) 2013-2020 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef GRANTLEEPRINTSTYLE_H
#define GRANTLEEPRINTSTYLE_H

#include "printstyle.h"

namespace KAddressBookGrantlee {
class GrantleePrint;
}

namespace KABPrinting {
class PrintProgress;

class GrantleePrintStyle : public PrintStyle
{
    Q_OBJECT
public:
    explicit GrantleePrintStyle(const QString &themePath, PrintingWizard *parent);
    ~GrantleePrintStyle() override;

    void print(const KContacts::Addressee::List &, PrintProgress *) override;
private:
    QString m_themePath;
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

#endif // GRANTLEEPRINTSTYLE_H
