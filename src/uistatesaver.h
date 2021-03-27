/*
    SPDX-FileCopyrightText: 2008 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kaddressbook_export.h"

class QWidget;
class KConfigGroup;

namespace KAddressBook
{
/**
 * @short Methods to save and restore the UI state of an application.
 *
 * This namespace provides methods that automatically save and restore
 * the state of various UI elements to/from a configuration group.
 *
 * The following widgets are supported so far:
 * - QSplitter
 * - QTabWidget
 * - QTreeView
 * - QComboBox
 *
 * @note The widgets need to have set an objectName, otherwise they are ignored
 *       on processing.
 *
 * @author Volker Krause <vkrause@kde.org>
 * @since 4.5
 */
namespace UiStateSaver
{
/**
 * Saves the state of @p widget and all its sub-widgets to @p config.
 * @param widget The top-level widget which state should be saved.
 * @param config The config group the settings should be written to.
 */
KADDRESSBOOK_EXPORT void saveState(QWidget *widget, KConfigGroup &config);

/**
 * Restores the UI state of @p widget and all its sub-widgets from @p config.
 * @param widget The top-level widget which state should be restored.
 * @param config The config gorup the settings should be read from.
 */
KADDRESSBOOK_EXPORT void restoreState(QWidget *widget, const KConfigGroup &config);
}
}

