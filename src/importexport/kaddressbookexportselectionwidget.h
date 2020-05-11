/*
   Copyright (C) 2015-2020 Laurent Montel <montel@kde.org>

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

#ifndef KADDRESSBOOKEXPORTSELECTIONWIDGET_H
#define KADDRESSBOOKEXPORTSELECTIONWIDGET_H

#include <QWidget>
#include "kaddressbook_export.h"
class QCheckBox;
namespace KAddressBookImportExport {

/** Addressbool export selection widget. */
class KADDRESSBOOK_EXPORT KAddressBookExportSelectionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KAddressBookExportSelectionWidget(QWidget *parent = nullptr);
    ~KAddressBookExportSelectionWidget();
    enum ExportField {
        None = 0,
        Private = 1,
        Business = 2,
        Other = 4,
        Encryption = 8,
        Picture = 16,
        DiplayName = 32
    };
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
#endif // KADDRESSBOOKEXPORTSELECTIONWIDGET_H
