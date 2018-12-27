/*
  This file is part of KAddressBook.

  Copyright (C) 2016-2019 Laurent Montel <montel@kde.org>

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

#ifndef AKONADICONTACTGROUPREPOSITORY_H
#define AKONADICONTACTGROUPREPOSITORY_H

#include "contactgrouprepository.h"
#include <QObject>
namespace Domain {
class ContactGroup;
}
class KJob;
class AkonadiContactGroupRepository : public QObject, public Domain::ContactGroupRepository
{
    Q_OBJECT
public:
    AkonadiContactGroupRepository();
    ~AkonadiContactGroupRepository();

    KJob *createContactGroup(Domain::ContactGroup *contactGroup) override;
    KJob *removeContactGroup(Domain::ContactGroup *contactGroup) override;
    KJob *updateContactGroup(Domain::ContactGroup *contactGroup) override;
};

#endif // AKONADICONTACTGROUPREPOSITORY_H
