/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2009 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "quicksearchwidget.h"

#include <KLocalizedString>
#include <QLineEdit>

#include <QKeyEvent>
#include <QTimer>
#include <QVBoxLayout>
#include <chrono>

using namespace std::chrono_literals;

QuickSearchWidget::QuickSearchWidget(QWidget *parent)
    : QWidget(parent)
    , mEdit(new QLineEdit(this))
    , mTimer(new QTimer(this))
{
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins({});

    // If change shortcut changes it in mainwidget
    mEdit->setClearButtonEnabled(true);
    mEdit->setToolTip(i18nc("@info:tooltip", "Search contacts in list"));
    mEdit->setWhatsThis(i18nc("@info:whatsthis",
                              "Start typing a search string in this box and the list of contacts "
                              "matching that string will be displayed.  This is a quick way of searching "
                              "for contacts of interest."));
    mEdit->installEventFilter(this);

    layout->addWidget(mEdit);

    connect(mEdit, &QLineEdit::textChanged, this, &QuickSearchWidget::resetTimer);
    connect(mTimer, &QTimer::timeout, this, &QuickSearchWidget::delayedTextChanged);
}

QuickSearchWidget::~QuickSearchWidget() = default;

QSize QuickSearchWidget::sizeHint() const
{
    const QSize size = mEdit->sizeHint();
    return {200, size.height()};
}

void QuickSearchWidget::slotFocusQuickSearch()
{
    mEdit->setFocus();
}

void QuickSearchWidget::resetTimer()
{
    mTimer->stop();
    mTimer->start(500ms);
}

void QuickSearchWidget::delayedTextChanged()
{
    mTimer->stop();
    Q_EMIT filterStringChanged(mEdit->text());
}

void QuickSearchWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Down) {
        event->accept();
        delayedTextChanged();
        Q_EMIT arrowDownKeyPressed();
        return;
    }

    QWidget::keyPressEvent(event);
}

void QuickSearchWidget::updateQuickSearchText(const QString &text)
{
    mEdit->setPlaceholderText(text);
}

#include "moc_quicksearchwidget.cpp"
