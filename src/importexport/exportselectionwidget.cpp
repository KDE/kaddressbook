/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportselectionwidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>

#include <KConfig>
#include <KConfigGroup>
#include <QCheckBox>
#include <QGridLayout>
#include <QGroupBox>

using namespace KAddressBookImportExport;

ExportSelectionWidget::ExportSelectionWidget(QWidget *parent)
    : QWidget(parent)
    , mPrivateBox(new QCheckBox(i18nc("@option:check", "Private fields"), this))
    , mBusinessBox(new QCheckBox(i18nc("@option:check", "Business fields"), this))
    , mOtherBox(new QCheckBox(i18nc("@option:check", "Other fields"), this))
    , mEncryptionKeys(new QCheckBox(i18nc("@option:check", "Encryption keys"), this))
    , mPictureBox(new QCheckBox(i18nc("@option:check", "Pictures"), this))
    , mDisplayNameBox(new QCheckBox(i18nc("@option:check", "Display name as full name"), this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    auto gbox = new QGroupBox(i18nc("@title:group", "Fields to be exported"), this);
    mainLayout->addWidget(gbox);
    auto layout = new QGridLayout;
    gbox->setLayout(layout);
    gbox->setFlat(true);

    mPrivateBox->setToolTip(i18nc("@info:tooltip", "Export private fields"));
    mPrivateBox->setWhatsThis(i18nc("@info:whatsthis",
                                    "Check this box if you want to export the contact's "
                                    "private fields to the vCard output file."));
    layout->addWidget(mPrivateBox, 1, 0);

    mBusinessBox->setToolTip(i18nc("@info:tooltip", "Export business fields"));
    mBusinessBox->setWhatsThis(i18nc("@info:whatsthis",
                                     "Check this box if you want to export the contact's "
                                     "business fields to the vCard output file."));
    layout->addWidget(mBusinessBox, 2, 0);

    mOtherBox->setToolTip(i18nc("@info:tooltip", "Export other fields"));
    mOtherBox->setWhatsThis(i18nc("@info:whatsthis",
                                  "Check this box if you want to export the contact's "
                                  "other fields to the vCard output file."));
    layout->addWidget(mOtherBox, 3, 0);

    mEncryptionKeys->setToolTip(i18nc("@info:tooltip", "Export encryption keys"));
    mEncryptionKeys->setWhatsThis(i18nc("@info:whatsthis",
                                        "Check this box if you want to export the contact's "
                                        "encryption keys to the vCard output file."));
    layout->addWidget(mEncryptionKeys, 1, 1);

    mPictureBox->setToolTip(i18nc("@info:tooltip", "Export pictures"));
    mPictureBox->setWhatsThis(i18nc("@info:whatsthis",
                                    "Check this box if you want to export the contact's "
                                    "picture to the vCard output file."));
    layout->addWidget(mPictureBox, 2, 1);

    gbox = new QGroupBox(i18nc("@title:group", "Export options"), this);
    gbox->setFlat(true);
    mainLayout->addWidget(gbox);
    auto gbLayout = new QHBoxLayout;
    gbox->setLayout(gbLayout);

    mDisplayNameBox->setToolTip(i18nc("@info:tooltip", "Export display name as full name"));
    mDisplayNameBox->setWhatsThis(i18nc("@info:whatsthis",
                                        "Check this box if you want to export the contact's display name "
                                        "in the vCard's full name field.  This may be required to get the "
                                        "name shown correctly in GMail or Android."));
    gbLayout->addWidget(mDisplayNameBox);

    readSettings();
}

ExportSelectionWidget::~ExportSelectionWidget()
{
    writeSettings();
}

void ExportSelectionWidget::readSettings()
{
    KConfig config(u"kaddressbookrc"_s);
    const KConfigGroup group(&config, u"XXPortVCard"_s);

    mPrivateBox->setChecked(group.readEntry("ExportPrivateFields", true));
    mBusinessBox->setChecked(group.readEntry("ExportBusinessFields", true));
    mOtherBox->setChecked(group.readEntry("ExportOtherFields", true));
    mEncryptionKeys->setChecked(group.readEntry("ExportEncryptionKeys", true));
    mPictureBox->setChecked(group.readEntry("ExportPictureFields", true));
    mDisplayNameBox->setChecked(group.readEntry("ExportDisplayName", false));
}

void ExportSelectionWidget::writeSettings()
{
    KConfig config(u"kaddressbookrc"_s);
    KConfigGroup group(&config, u"XXPortVCard"_s);

    group.writeEntry("ExportPrivateFields", mPrivateBox->isChecked());
    group.writeEntry("ExportBusinessFields", mBusinessBox->isChecked());
    group.writeEntry("ExportOtherFields", mOtherBox->isChecked());
    group.writeEntry("ExportEncryptionKeys", mEncryptionKeys->isChecked());
    group.writeEntry("ExportPictureFields", mPictureBox->isChecked());
    group.writeEntry("ExportDisplayName", mDisplayNameBox->isChecked());
}

ExportSelectionWidget::ExportFields ExportSelectionWidget::exportType() const
{
    ExportFields type = None;
    if (mPrivateBox->isChecked()) {
        type |= Private;
    }
    if (mBusinessBox->isChecked()) {
        type |= Business;
    }
    if (mOtherBox->isChecked()) {
        type |= Other;
    }
    if (mEncryptionKeys->isChecked()) {
        type |= Encryption;
    }
    if (mPictureBox->isChecked()) {
        type |= Picture;
    }
    if (mDisplayNameBox->isChecked()) {
        type |= DiplayName;
    }
    return type;
}

#include "moc_exportselectionwidget.cpp"
