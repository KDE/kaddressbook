/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "kaddressbook_importexport_export.h"
#include <QWidget>

class QCheckBox;
namespace KAddressBookImportExport
{
/** Addressbool export selection widget. */
class KADDRESSBOOK_IMPORTEXPORT_EXPORT ExportSelectionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExportSelectionWidget(QWidget *parent = nullptr);
    ~ExportSelectionWidget() override;

    enum ExportField { None = 0, Private = 1, Business = 2, Other = 4, Encryption = 8, Picture = 16, DiplayName = 32 };
    Q_ENUM(ExportField)
    Q_DECLARE_FLAGS(ExportFields, ExportField)

    Q_REQUIRED_RESULT ExportFields exportType() const;

private:
    void readSettings();
    void writeSettings();
    QCheckBox *mPrivateBox = nullptr;
    QCheckBox *mBusinessBox = nullptr;
    QCheckBox *mOtherBox = nullptr;
    QCheckBox *mEncryptionKeys = nullptr;
    QCheckBox *mPictureBox = nullptr;
    QCheckBox *mDisplayNameBox = nullptr;
};
}
