/*
    SPDX-FileCopyrightText: 2008 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uistatesaver.h"

#include <KConfigGroup>

#include <QComboBox>
#include <QHeaderView>
#include <QSplitter>
#include <QTabWidget>
#include <QTreeView>

using namespace KAddressBook;

struct Saver {
    static void process(QSplitter *splitter, KConfigGroup &config)
    {
        if (splitter->sizes().count(0) == splitter->sizes().count()) {
            return;
        }
        config.writeEntry(splitter->objectName(), splitter->sizes());
    }

    static void process(QTabWidget *tab, KConfigGroup &config)
    {
        config.writeEntry(tab->objectName(), tab->currentIndex());
    }

    static void process(QTreeView *tv, KConfigGroup &config)
    {
        config.writeEntry(tv->objectName(), tv->header()->saveState());
    }

    static void process(QComboBox *cb, KConfigGroup &config)
    {
        config.writeEntry(cb->objectName(), cb->currentIndex());
    }
};

struct Restorer {
    static void process(QSplitter *splitter, const KConfigGroup &config)
    {
        const QList<int> sizes = config.readEntry(splitter->objectName(), QList<int>());
        if (!sizes.isEmpty() && splitter->count() == sizes.count() && sizes.count() != sizes.count(0)) {
            splitter->setSizes(sizes);
        }
    }

    static void process(QTabWidget *tab, const KConfigGroup &config)
    {
        const int index = config.readEntry(tab->objectName(), -1);
        if (index >= 0 && index < tab->count()) {
            tab->setCurrentIndex(index);
        }
    }

    static void process(QTreeView *tv, const KConfigGroup &config)
    {
        const QByteArray state = config.readEntry(tv->objectName(), QByteArray());
        if (!state.isEmpty()) {
            tv->header()->restoreState(state);
        }
    }

    static void process(QComboBox *cb, const KConfigGroup &config)
    {
        const int index = config.readEntry(cb->objectName(), -1);
        if (index >= 0 && index < cb->count()) {
            cb->setCurrentIndex(index);
        }
    }
};

#define PROCESS_TYPE(T)                                                                                                                                        \
    {                                                                                                                                                          \
        T *obj = qobject_cast<T *>(w);                                                                                                                         \
        if (obj) {                                                                                                                                             \
            Op::process(obj, config);                                                                                                                          \
            continue;                                                                                                                                          \
        }                                                                                                                                                      \
    }

template<typename Op, typename Config> static void processWidgets(QWidget *widget, Config config)
{
    QList<QWidget *> widgets = widget->findChildren<QWidget *>();
    widgets << widget;
    for (QWidget *w : std::as_const(widgets)) {
        if (w->objectName().isEmpty()) {
            continue;
        }
        PROCESS_TYPE(QSplitter);
        PROCESS_TYPE(QTabWidget);
        PROCESS_TYPE(QTreeView);
        PROCESS_TYPE(QComboBox);
    }
}

#undef PROCESS_TYPE

void UiStateSaver::saveState(QWidget *widget, KConfigGroup &config)
{
    processWidgets<Saver, KConfigGroup &>(widget, config);
}

void UiStateSaver::restoreState(QWidget *widget, const KConfigGroup &config)
{
    processWidgets<Restorer, const KConfigGroup &>(widget, config);
}
