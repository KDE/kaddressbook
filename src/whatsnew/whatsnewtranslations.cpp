/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "whatsnewtranslations.h"
using namespace Qt::Literals::StringLiterals;

WhatsNewTranslations::WhatsNewTranslations() = default;

WhatsNewTranslations::~WhatsNewTranslations() = default;

// Use by newFeaturesMD5
QList<KLazyLocalizedString> WhatsNewTranslations::lastNewFeatures() const
{
    const QList<KLazyLocalizedString> info{};
    return info;
}

QList<TextAddonsWidgets::WhatsNewInfo> WhatsNewTranslations::createWhatsNewInfo() const
{
    QList<TextAddonsWidgets::WhatsNewInfo> listInfo;
    {
        TextAddonsWidgets::WhatsNewInfo info;
        info.setNewFeatures({i18n("Add new whatsnew widget.")});
        info.setBugFixings({i18n("Restore configure dialog size.")});
        info.setVersion(u"6.3.0"_s);
        listInfo.append(std::move(info));
    }
    return listInfo;
}
