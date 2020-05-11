/*
  This file is part of KAddressBook.
  Copyright (c) 2009 Tobias Koenig <tokoe@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#ifndef CSVIMPORTDIALOG_H
#define CSVIMPORTDIALOG_H

#include <kaddressbookimportexport/kaddressbookimportexportcontactfields.h>

#include <QDialog>

#include <QList>

class QComboBox;
class QLineEdit;
class KUrlRequester;

class QButtonGroup;
class QCheckBox;
class QCsvModel;
class QTableView;

class CSVImportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CSVImportDialog(QWidget *parent = nullptr);
    ~CSVImportDialog();

    KContacts::AddresseeList contacts() const;

private:
    void setFile(const QString &);
    void setUrl(const QUrl &);
    void urlChanged(const QString &);

    void customDelimiterChanged();
    void customDelimiterChanged(const QString &, bool reload = true);
    void delimiterClicked(int, bool reload = true);
    void textQuoteChanged(const QString &, bool reload = true);
    void skipFirstRowChanged(bool, bool reload = true);
    void codecChanged(bool reload = true);

    void modelFinishedLoading();
    void finalizeApplyTemplate();

    void slotSaveTemplate();
    void slotApplyTemplate();
    void slotOk();
private:
    void applyTemplate();
    void saveTemplate();

    QTableView *mTable = nullptr;
    QButtonGroup *mDelimiterGroup = nullptr;
    QLineEdit *mDelimiterEdit = nullptr;
    QLineEdit *mDatePatternEdit = nullptr;
    QComboBox *mComboQuote = nullptr;
    QComboBox *mCodecCombo = nullptr;
    QCheckBox *mSkipFirstRow = nullptr;
    KUrlRequester *mUrlRequester = nullptr;
    QCsvModel *mModel = nullptr;

    void initGUI();

    void reloadCodecs();
    QTextCodec *currentCodec();
    QList<QTextCodec *> mCodecs;

    QChar mTextQuote;
    QString mDelimiter;
    QMap<QString, uint> mTypeMap;
    QIODevice *mDevice = nullptr;
    KAddressBookImportExport::KAddressBookImportExportContactFields::Fields mFieldSelection;
    QPushButton *mUser1Button = nullptr;
    QPushButton *mUser2Button = nullptr;
    QPushButton *mOkButton = nullptr;
};

#endif
