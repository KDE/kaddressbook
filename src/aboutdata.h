/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2009-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef ABOUTDATA_H
#define ABOUTDATA_H

#include "kaddressbook_export.h"

#include <KAboutData>

class KADDRESSBOOK_EXPORT AboutData : public KAboutData
{
public:
    /**
     * Creates a new about data object.
     */
    AboutData();

    /**
     * Destroys the about data object.
     */
    ~AboutData();
};

#endif /* ABOUTDATA_H */
