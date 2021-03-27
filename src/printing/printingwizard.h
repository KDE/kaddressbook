/*
  This file is part of KAddressBook.
  SPDX-FileCopyrightText: 1996-2002 Mirko Boehm <mirko@kde.org>
  SPDX-FileCopyrightText: Tobias Koenig <tokoe@kde.org>
  SPDX-FileCopyrightText: 2009-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later WITH Qt-Commercial-exception-1.0
*/

#pragma once

#include <KAssistantDialog>
#include <KContacts/Addressee>

namespace KAddressBookImportExport
{
class ContactSelectionWidget;
}
class StylePage;

class QItemSelectionModel;
class QPrinter;

namespace Akonadi
{
class Collection;
}

namespace KABPrinting
{
class PrintProgress;
class PrintStyle;
class PrintStyleFactory;

/**
 * The PrintingWizard combines pages common for all print styles
 * and those provided by the respective style.
 */
class PrintingWizard : public KAssistantDialog
{
    Q_OBJECT

public:
    /**
     * Creates a new printing wizard.
     *
     * @param printer The configured printer.
     * @param selectionModel The selection model to get the selected contacts from.
     * @param parent The parent widget.
     */
    PrintingWizard(QPrinter *printer, QItemSelectionModel *selectionModel, QWidget *parent = nullptr);

    /**
     * Destroys the printing wizard.
     */
    ~PrintingWizard() override;

    /**
     * Sets the default addressbook of the contact selection.
     */
    void setDefaultAddressBook(const Akonadi::Collection &addressBook);

    /**
     * Registers all available printing styles.
     */
    void registerStyles();

    /**
     * Performs the actual printing.
     */
    void print();

    /**
     * Returns the printer to use for printing.
     */
    QPrinter *printer() const;

    /**
     * Returns the index of the selected style
     */
    int printingStyle() const;

    /**
     * Returns the sort order of addressBook
     */
    int sortOrder() const;

protected Q_SLOTS:
    /**
     * A print style has been selected. The argument is the index
     * in the cbStyle combo and in styles.
     */
    void slotStyleSelected(int);

protected:
    class PrintStyleDefinition
    {
    public:
        PrintStyleDefinition(PrintStyleFactory *factory = nullptr, PrintStyle *style = nullptr)
            : printstyleFactory(factory)
            , printStyle(style)
        {
        }

        PrintStyleFactory *printstyleFactory = nullptr;
        PrintStyle *printStyle = nullptr;
    };

    QList<PrintStyleDefinition *> mPrintStyleDefinition;
    QPrinter *mPrinter = nullptr;
    PrintStyle *mStyle = nullptr;
    PrintProgress *mProgress = nullptr;

    StylePage *mStylePage = nullptr;
    KAddressBookImportExport::ContactSelectionWidget *mSelectionPage = nullptr;

    /**
     * Overloaded accept slot. This is used to do the actual
     * printing without having the wizard disappearing
     * before. What happens is actually up to the print style,
     * since it does the printing. It could display a progress
     * window, for example (hint, hint).
     */
    void accept() override;

private:
    void writeConfig();
    void readConfig();
    void loadGrantleeStyle();
};
}

