/*
  This file is part of KAddressBook.
  SPDX-FileCopyrightText: 1996-2002 Mirko Boehm <mirko@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later WITH Qt-Commercial-exception-1.0
*/

#pragma once

#include <QWidget>

class QProgressBar;
class QString;

class QTextBrowser;

namespace KABPrinting
{
/**
  This defines a simple widget to display print progress
  information. It is provided to all print styles during a print
  process. It displays messages and a progress bar.
 */
class PrintProgress : public QWidget
{
    Q_OBJECT

public:
    explicit PrintProgress(QWidget *parent);
    ~PrintProgress() override;

    /**
      Add a message to the message log. Give the user something to admire :-)
     */
    void addMessage(const QString &);

    /**
      Set the progress to a certain amount. Steps are from 0 to 100.
     */
    void setProgress(int);

private:
    QStringList mMessages;

    QTextBrowser *mLogBrowser = nullptr;
    QProgressBar *mProgressBar = nullptr;
};
}

