/*
  This file is part of KAddressBook.
  SPDX-FileCopyrightText: 1996-2002 Mirko Boehm <mirko@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later WITH Qt-Commercial-exception-1.0
*/

#include "printprogress.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QApplication>
#include <QTextBrowser>

#include <QGridLayout>
#include <QProgressBar>

using namespace KABPrinting;

PrintProgress::PrintProgress(QWidget *parent)
    : QWidget(parent)
    , mLogBrowser(new QTextBrowser(this))
    , mProgressBar(new QProgressBar(this))
{
    setWindowTitle(i18nc("@title:window", "Printing: Progress"));

    auto topLayout = new QGridLayout(this);

    topLayout->addWidget(mLogBrowser, 0, 0);

    mProgressBar->setValue(0);
    mProgressBar->setFormat(i18nc("Percent value; %p is the value, % is the percent sign", "%p%"));
    topLayout->addWidget(mProgressBar, 1, 0);

    resize(QSize(370, 220).expandedTo(minimumSizeHint()));
}

PrintProgress::~PrintProgress() = default;

void PrintProgress::addMessage(const QString &msg)
{
    mMessages.append(msg);

    QString head = QLatin1StringView("<qt><b>") + i18n("Progress") + QLatin1StringView(":</b><ul>");

    const QString foot = u"</ul></qt>"_s;

    QString body;
    QStringList::ConstIterator it;
    QStringList::ConstIterator end(mMessages.constEnd());
    for (it = mMessages.constBegin(); it != end; ++it) {
        body.append(QLatin1StringView("<li>") + (*it) + QLatin1StringView("</li>"));
    }

    mLogBrowser->setText(head + body + foot);
    qApp->processEvents();
}

void PrintProgress::setProgress(int step)
{
    mProgressBar->setValue(step);
    qApp->processEvents();
}

#include "moc_printprogress.cpp"
