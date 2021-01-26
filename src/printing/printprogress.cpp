/*
  This file is part of KAddressBook.
  SPDX-FileCopyrightText: 1996-2002 Mirko Boehm <mirko@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later WITH Qt-Commercial-exception-1.0
*/

#include "printprogress.h"

#include <KLocalizedString>
#include <QApplication>
#include <QTextBrowser>

#include <QGridLayout>
#include <QProgressBar>

using namespace KABPrinting;

PrintProgress::PrintProgress(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(i18nc("@title:window", "Printing: Progress"));

    auto topLayout = new QGridLayout(this);

    mLogBrowser = new QTextBrowser(this);
    topLayout->addWidget(mLogBrowser, 0, 0);

    mProgressBar = new QProgressBar(this);
    mProgressBar->setValue(0);
    topLayout->addWidget(mProgressBar, 1, 0);

    resize(QSize(370, 220).expandedTo(minimumSizeHint()));
}

PrintProgress::~PrintProgress()
{
}

void PrintProgress::addMessage(const QString &msg)
{
    mMessages.append(msg);

    QString head = QLatin1String("<qt><b>") + i18n("Progress") + QLatin1String(":</b><ul>");

    const QString foot = QStringLiteral("</ul></qt>");

    QString body;
    QStringList::ConstIterator it;
    QStringList::ConstIterator end(mMessages.constEnd());
    for (it = mMessages.constBegin(); it != end; ++it) {
        body.append(QLatin1String("<li>") + (*it) + QLatin1String("</li>"));
    }

    mLogBrowser->setText(head + body + foot);
    qApp->processEvents();
}

void PrintProgress::setProgress(int step)
{
    mProgressBar->setValue(step);
    qApp->processEvents();
}
