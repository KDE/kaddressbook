/*
  This file is part of KAddressBook.
  SPDX-FileCopyrightText: 1996-2002 Mirko Boehm <mirko@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later WITH Qt-Commercial-exception-1.0
*/

#include "printstyle.h"
#include "printingwizard.h"

#include "kaddressbook_debug.h"

#include <QPushButton>
#include <QStandardPaths>

using namespace KABPrinting;
using namespace KAddressBookImportExport;

PrintStyle::PrintStyle(PrintingWizard *parent)
    : QObject(parent)
    , mWizard(parent)
    , mSortField(ContactFields::GivenName)
    , mSortOrder(Qt::AscendingOrder)
{
}

PrintStyle::~PrintStyle()
{
}

const QPixmap &PrintStyle::preview() const
{
    return mPreview;
}

void PrintStyle::setPreview(const QPixmap &image)
{
    mPreview = image;
}

bool PrintStyle::setPreview(const QString &fileName)
{
    QPixmap preview;

    const QString path = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QLatin1String("kaddressbook/printing/") + fileName);
    if (path.isEmpty()) {
        qCDebug(KADDRESSBOOK_LOG) << "cannot locate preview image " << fileName << " in appdata";
        return false;
    } else {
        if (preview.load(path)) {
            setPreview(preview);
            return true;
        } else {
            qCDebug(KADDRESSBOOK_LOG) << "preview at '" << path << "' cannot be loaded.";
            return false;
        }
    }
}

PrintingWizard *PrintStyle::wizard() const
{
    return mWizard;
}

void PrintStyle::addPage(QWidget *page, const QString &title)
{
    if (!mPageList.contains(page)) { // not yet in the list
        mPageList.append(page);
        mPageTitles.append(title);

        auto item = new KPageWidgetItem(page, title);
        mPageItems.insert(page, item);
        mWizard->addPage(item);
        mWizard->setAppropriate(item, false);
    }
}

void PrintStyle::showPages()
{
    QWidget *wdg = nullptr;

    for (QWidget *wdg2 : std::as_const(mPageList)) {
        mWizard->setAppropriate(mPageItems[wdg2], true);
        wdg = wdg2;
    }
    mWizard->nextButton()->setEnabled(wdg);
    mWizard->finishButton()->setEnabled(!wdg);
}

void PrintStyle::hidePages()
{
    for (QWidget *wdg : std::as_const(mPageList)) {
        mWizard->setAppropriate(mPageItems[wdg], false);
    }
}

void PrintStyle::setPreferredSortOptions(ContactFields::Field field, Qt::SortOrder sortOrder)
{
    mSortField = field;
    mSortOrder = sortOrder;
}

ContactFields::Field PrintStyle::preferredSortField() const
{
    return mSortField;
}

Qt::SortOrder PrintStyle::preferredSortOrder() const
{
    return mSortOrder;
}

PrintStyleFactory::PrintStyleFactory(PrintingWizard *parent)
    : mParent(parent)
{
}

PrintStyleFactory::~PrintStyleFactory()
{
}
