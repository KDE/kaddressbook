/*
   Copyright (C) 2016 Montel Laurent <montel@kde.org>

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

#include "kaddressbook_config_plugins.h"
#include <KAboutData>
#include <KPluginFactory>
#include <KLocalizedString>

#include <QVBoxLayout>

using namespace KAddressBook;

K_PLUGIN_FACTORY(KCMKaddressbookPluginsConfigFactory, registerPlugin<KCMKaddressbookPluginsConfig>();)

KCMKaddressbookPluginsConfig::KCMKaddressbookPluginsConfig(QWidget *parent, const QVariantList &args)
    : KCModule(parent, args)
{
    QHBoxLayout *lay = new QHBoxLayout(this);
    lay->setMargin(0);

    KAboutData *about = new KAboutData(QStringLiteral("kcmakrpluginsconfig"),
                                       i18n("Configure Plugins"),
                                       QString(), QString(), KAboutLicense::GPL,
                                       i18n("(c), 2016 Laurent Montel"));

    about->addAuthor(i18n("Laurent Montel"), QString(), QStringLiteral("montel@kde.org"));

    setAboutData(about);
    //TODO
}

void KCMKaddressbookPluginsConfig::slotConfigChanged()
{
    Q_EMIT changed(true);
}

void KCMKaddressbookPluginsConfig::save()
{
}

void KCMKaddressbookPluginsConfig::load()
{
}

void KCMKaddressbookPluginsConfig::defaults()
{
}

#include "kaddressbook_config_plugins.moc"
