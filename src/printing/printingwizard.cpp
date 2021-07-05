/*
  This file is part of KAddressBook.
  SPDX-FileCopyrightText: 1996-2002 Mirko Boehm <mirko@kde.org>
  SPDX-FileCopyrightText: Tobias Koenig <tokoe@kde.org>
  SPDX-FileCopyrightText: 2009-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later WITH Qt-Commercial-exception-1.0
*/

#include "printingwizard.h"
#include "settings.h"

#include "contactsorter.h"
#include "importexport/contactselectionwidget.h"
#include "printprogress.h"
#include "printstyle.h"
#include "stylepage.h"

// including the styles
#include "compact/compactstyle.h"
#include "detailled/detailledstyle.h"
#include "mike/mikesstyle.h"
#include "printing/grantlee/grantleeprintstyle.h"
#include "ringbinder/ringbinderstyle.h"

#include "kaddressbook_debug.h"
#include <KLocalizedString>
#include <QApplication>

#include <KSharedConfig>
#include <QDirIterator>
#include <QPrinter>
#include <QPushButton>
#include <QStandardPaths>

using namespace KABPrinting;

PrintingWizard::PrintingWizard(QPrinter *printer, QItemSelectionModel *selectionModel, QWidget *parent)
    : KAssistantDialog(parent)
    , mPrinter(printer)
    , mStyle(nullptr)
{
    setWindowTitle(i18nc("@title:window", "Print Contacts"));

    mSelectionPage = new KAddressBookImportExport::ContactSelectionWidget(selectionModel, this);
    mSelectionPage->setMessageText(i18n("Which contacts do you want to print?"));

    auto mSelectionPageItem = new KPageWidgetItem(mSelectionPage, i18n("Choose Contacts to Print"));
    addPage(mSelectionPageItem);
    setAppropriate(mSelectionPageItem, true);

    mStylePage = new StylePage(this);
    connect(mStylePage, &StylePage::styleChanged, this, &PrintingWizard::slotStyleSelected);
    addPage(mStylePage, i18n("Choose Printing Style"));

    registerStyles();

    if (mPrintStyleDefinition.count() > Settings::self()->printingStyle()) {
        mStylePage->setPrintingStyle(Settings::self()->printingStyle()); // should Q_EMIT styleChanged
        slotStyleSelected(Settings::self()->printingStyle());
    }

    mStylePage->setSortOrder(Settings::self()->sortOrder() == 0 ? Qt::AscendingOrder : Qt::DescendingOrder);
    readConfig();
}

PrintingWizard::~PrintingWizard()
{
    writeConfig();
}

void PrintingWizard::readConfig()
{
    KConfigGroup grp(KSharedConfig::openConfig(), "PrintingWizard");
    const QSize size = grp.readEntry("Size", QSize(300, 200));
    if (size.isValid()) {
        resize(size);
    }
}

void PrintingWizard::writeConfig()
{
    KConfigGroup grp(KSharedConfig::openConfig(), "PrintingWizard");
    grp.writeEntry("Size", size());
    grp.sync();
}

void PrintingWizard::setDefaultAddressBook(const Akonadi::Collection &addressBook)
{
    mSelectionPage->setDefaultAddressBook(addressBook);
}

void PrintingWizard::accept()
{
    print();
    close();
    setResult(QDialog::Accepted);
}

void PrintingWizard::loadGrantleeStyle()
{
    const QString relativePath = QStringLiteral("kaddressbook/printing/themes/");
    QStringList themesDirectories = QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, relativePath, QStandardPaths::LocateDirectory);
    if (themesDirectories.count() < 2) {
        // Make sure to add local directory
        const QString localDirectory = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1Char('/') + relativePath;
        if (!themesDirectories.contains(localDirectory)) {
            themesDirectories.append(localDirectory);
        }
    }

    for (const QString &directory : std::as_const(themesDirectories)) {
        QDirIterator dirIt(directory, QStringList(), QDir::AllDirs | QDir::NoDotAndDotDot);
        QStringList alreadyLoadedThemeName;
        while (dirIt.hasNext()) {
            dirIt.next();
            const QString themeInfoFile = dirIt.filePath() + QStringLiteral("/theme.desktop");
            KConfig config(themeInfoFile);
            KConfigGroup group(&config, QStringLiteral("Desktop Entry"));
            QString name = group.readEntry("Name", QString());
            if (name.isEmpty()) {
                continue;
            }
            if (alreadyLoadedThemeName.contains(name)) {
                int i = 2;
                const QString originalName(name);
                while (alreadyLoadedThemeName.contains(name)) {
                    name = originalName + QStringLiteral(" (%1)").arg(i);
                    ++i;
                }
            }
            const QString printThemePath(dirIt.filePath() + QLatin1Char('/'));
            if (!printThemePath.isEmpty()) {
                alreadyLoadedThemeName << name;
                mPrintStyleDefinition.append(new PrintStyleDefinition(new GrantleeStyleFactory(name, printThemePath, this)));
            }
        }
    }
}

void PrintingWizard::registerStyles()
{
    mPrintStyleDefinition.append(new PrintStyleDefinition(new DetailledPrintStyleFactory(this)));
    mPrintStyleDefinition.append(new PrintStyleDefinition(new MikesStyleFactory(this)));
    mPrintStyleDefinition.append(new PrintStyleDefinition(new RingBinderPrintStyleFactory(this)));
    mPrintStyleDefinition.append(new PrintStyleDefinition(new CompactStyleFactory(this)));

    loadGrantleeStyle();

    mStylePage->clearStyleNames();
    for (int i = 0; i < mPrintStyleDefinition.count(); ++i) {
        mStylePage->addStyleName(mPrintStyleDefinition.at(i)->printstyleFactory->description());
    }
}

void PrintingWizard::slotStyleSelected(int index)
{
    if (index < 0 || index >= mPrintStyleDefinition.count()) {
        return;
    }
    if (mStyle) {
        mStyle->hidePages();
    }

    mStyle = mPrintStyleDefinition.value(index)->printStyle;
    if (!mStyle) {
        PrintStyleFactory *factory = mPrintStyleDefinition.at(index)->printstyleFactory;
        qCDebug(KADDRESSBOOK_LOG) << "creating print style" << factory->description();

        mStyle = factory->create();
        mPrintStyleDefinition.value(index)->printStyle = mStyle;
    }

    mStyle->showPages();

    mStylePage->setPreview(mStyle->preview());

    mStylePage->setSortField(mStyle->preferredSortField());
    mStylePage->setSortOrder(mStyle->preferredSortOrder());
}

QPrinter *PrintingWizard::printer() const
{
    return mPrinter;
}

int PrintingWizard::printingStyle() const
{
    return mStylePage->printingStyle();
}

int PrintingWizard::sortOrder() const
{
    return mStylePage->sortOrder();
}

void PrintingWizard::print()
{
    // create and show print progress widget:
    mProgress = new PrintProgress(this);
    auto progressItem = new KPageWidgetItem(mProgress, i18n("Print Progress"));
    addPage(progressItem);
    setCurrentPage(progressItem);
    qApp->processEvents();

    KContacts::Addressee::List contacts = mSelectionPage->selectedContacts().addressList();

    const ContactSorter sorter(mStylePage->sortField(), mStylePage->sortOrder());
    sorter.sort(contacts);

    qCDebug(KADDRESSBOOK_LOG) << "printing" << contacts.count() << "contacts.";
    // ... print:
    backButton()->setEnabled(false);

    button(QDialogButtonBox::Cancel)->setEnabled(false);
    mStyle->print(contacts, mProgress);
}
