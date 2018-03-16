/*
  This file is part of KAddressBook Kontact Plugin.

  Copyright (c) 2009-2018 Laurent Montel <montel@kde.org>

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

#ifndef KADDRESSBOOK_PLUGIN_H
#define KADDRESSBOOK_PLUGIN_H

#include <KontactInterface/UniqueAppHandler>

namespace KontactInterface {
class Plugin;
}

class KAddressBookUniqueAppHandler : public KontactInterface::UniqueAppHandler
{
    Q_OBJECT
public:
    explicit KAddressBookUniqueAppHandler(KontactInterface::Plugin *plugin)
        : KontactInterface::UniqueAppHandler(plugin)
    {
    }

    void loadCommandLineOptions(QCommandLineParser *parser) override;
    int activate(const QStringList &args, const QString &workingDir) override;
};

class KAddressBookPlugin : public KontactInterface::Plugin
{
    Q_OBJECT

public:
    KAddressBookPlugin(KontactInterface::Core *core, const QVariantList &);
    ~KAddressBookPlugin() override;

    bool isRunningStandalone() const override;
    int weight() const override;

    QStringList invisibleToolbarActions() const override;
    void shortcutChanged() override;

protected:
    KParts::ReadOnlyPart *createPart() override;

private:
    void slotNewContact();
    void slotNewContactGroup();

    KontactInterface::UniqueAppWatcher *mUniqueAppWatcher = nullptr;
};

#endif
