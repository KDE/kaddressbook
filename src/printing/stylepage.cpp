/*
  This file is part of KAddressBook.
  SPDX-FileCopyrightText: 2002 Anders Lund <anders.lund@lund.tdcadsl.dk>
  SPDX-FileCopyrightText: Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later WITH Qt-Commercial-exception-1.0
*/

#include "stylepage.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>

#include <KLocalizedString>
#include <QComboBox>

using namespace KAddressBookImportExport;

// helper method to sort contact fields by field label
static bool contactFieldsNameLesser(ContactFields::Field field, ContactFields::Field otherField)
{
    return QString::localeAwareCompare(ContactFields::label(field), ContactFields::label(otherField)) < 0;
}

StylePage::StylePage(QWidget *parent, const QString &name)
    : QWidget(parent)
{
    setObjectName(name);
    initGUI();

    initFieldCombo();

    mSortTypeCombo->addItem(i18nc("@item:inlistbox Ascending sort order", "Ascending"));
    mSortTypeCombo->addItem(i18nc("@item:inlistbox Descending sort order", "Descending"));

    connect(mStyleCombo, qOverload<int>(&QComboBox::activated), this, &StylePage::styleChanged);
}

StylePage::~StylePage()
{
}

void StylePage::setPreview(const QPixmap &pixmap)
{
    if (pixmap.isNull()) {
        mPreview->setText(i18nc("@label", "(No preview available.)"));
    } else {
        mPreview->setPixmap(pixmap);
    }
}

void StylePage::addStyleName(const QString &name)
{
    mStyleCombo->addItem(name);
}

void StylePage::clearStyleNames()
{
    mStyleCombo->clear();
}

void StylePage::setSortField(ContactFields::Field field)
{
    mFieldCombo->setCurrentIndex(mFields.indexOf(field));
}

void StylePage::setSortOrder(Qt::SortOrder sortOrder)
{
    if (sortOrder == Qt::AscendingOrder) {
        mSortTypeCombo->setCurrentIndex(0);
    } else {
        mSortTypeCombo->setCurrentIndex(1);
    }
}

ContactFields::Field StylePage::sortField() const
{
    if (mFieldCombo->currentIndex() == -1) {
        return ContactFields::GivenName;
    }

    return mFields[mFieldCombo->currentIndex()];
}

Qt::SortOrder StylePage::sortOrder() const
{
    return mSortTypeCombo->currentIndex() == 0 ? Qt::AscendingOrder : Qt::DescendingOrder;
}

void StylePage::initFieldCombo()
{
    mFieldCombo->clear();

    mFields = ContactFields::allFields();
    mFields.remove(0); // remove ContactFields::Undefined

    std::sort(mFields.begin(), mFields.end(), contactFieldsNameLesser);

    ContactFields::Fields::ConstIterator it;
    const ContactFields::Fields::ConstIterator end(mFields.constEnd());
    for (it = mFields.constBegin(); it != end; ++it) {
        mFieldCombo->addItem(ContactFields::label(*it));
    }
}

void StylePage::initGUI()
{
    setWindowTitle(i18nc("@title:window", "Choose Printing Style"));

    auto topLayout = new QGridLayout(this);

    auto label = new QLabel(i18nc("@label:textbox",
                                  "What should the print look like?\n"
                                  "KAddressBook has several printing styles, designed "
                                  "for different purposes.\n"
                                  "Choose the style that suits your needs below."),
                            this);
    topLayout->addWidget(label, 0, 0, 1, 2);

    auto group = new QGroupBox(i18nc("@title:group", "Sorting"), this);
    auto sortLayout = new QGridLayout();
    group->setLayout(sortLayout);
    sortLayout->setAlignment(Qt::AlignTop);

    label = new QLabel(i18nc("@label:listbox", "Criterion:"), group);
    sortLayout->addWidget(label, 0, 0);

    mFieldCombo = new QComboBox(group);
    mFieldCombo->setToolTip(i18nc("@info:tooltip", "Select the primary sort field"));
    mFieldCombo->setWhatsThis(i18nc("@info:whatsthis",
                                    "From this list you can select the field on which your contacts are sorted "
                                    "in the print output.  Use the sorting order option to determine if the "
                                    "sort will be in ascending or descending order."));
    sortLayout->addWidget(mFieldCombo, 0, 1);

    label = new QLabel(i18nc("@label:listbox", "Order:"), group);
    sortLayout->addWidget(label, 1, 0);

    mSortTypeCombo = new QComboBox(group);
    mSortTypeCombo->setToolTip(i18nc("@info:tooltip", "Select the sorting order"));
    mSortTypeCombo->setWhatsThis(i18nc("@info:whatsthis",
                                       "Choose if you want to sort your contacts in ascending or descending order. "
                                       "Use the sorting criterion option to specify on which contact field the sorting "
                                       "will be performed."));
    sortLayout->addWidget(mSortTypeCombo, 1, 1);

    topLayout->addWidget(group, 1, 0);

    group = new QGroupBox(i18nc("@title:group", "Print Style"), this);
    auto styleLayout = new QVBoxLayout();
    group->setLayout(styleLayout);

    mStyleCombo = new QComboBox(group);
    mStyleCombo->setToolTip(i18nc("@info:tooltip", "Select the print style"));
    mStyleCombo->setWhatsThis(i18nc("@info:whatsthis", "Choose your desired printing style. See the preview image to help you decide."));

    styleLayout->addWidget(mStyleCombo);

    mPreview = new QLabel(group);
    QFont font(mPreview->font());
    font.setPointSize(20);
    mPreview->setFont(font);
    mPreview->setScaledContents(true);
    mPreview->setAlignment(Qt::AlignCenter);
    mPreview->setWordWrap(true);
    styleLayout->addWidget(mPreview);

    topLayout->addWidget(group, 1, 1);
    topLayout->setRowStretch(1, 1);
}

int StylePage::printingStyle() const
{
    return mStyleCombo->currentIndex();
}

void StylePage::setPrintingStyle(int index)
{
    mStyleCombo->setCurrentIndex(index);
}
