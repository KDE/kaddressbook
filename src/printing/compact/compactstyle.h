/*
  This file is part of KAddressBook.
  SPDX-FileCopyrightText: 2011 Mario Scheel <zweistein12@gmx.de>

  SPDX-License-Identifier: GPL-2.0-or-later WITH Qt-Commercial-exception-1.0
*/

#pragma once

#include "printstyle.h"

namespace KABPrinting
{
class CompactStyleForm;

class CompactStyle : public PrintStyle
{
    Q_OBJECT

public:
    explicit CompactStyle(PrintingWizard *parent);
    ~CompactStyle() override;

    /**
     * prints the contacts
     */
    void print(const KContacts::Addressee::List &, PrintProgress *) override;

private:
    QString contactsToHtml(const KContacts::Addressee::List &contacts) const;

    CompactStyleForm *mPageSettings = nullptr;

    /**
     * various settings
     */
    bool withAlternating = false;
    bool withBusinessAddress = false;
    bool withHomeAddress = false;
    bool withBirthday = false;
    bool withEMail = false;
    QColor firstColor;
    QColor secondColor;

private:
    /**
     * Enable or disable the controls for color selection. The colors
     * are needed for alternating background color of the rows.
     */
    void setAlternatingColors();
};

class CompactStyleFactory : public PrintStyleFactory
{
public:
    explicit CompactStyleFactory(PrintingWizard *parent);

    PrintStyle *create() const override;
    QString description() const override;
};
}

