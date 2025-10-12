/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "whatsnewtranslations.h"
using namespace Qt::Literals::StringLiterals;

WhatsNewTranslations::WhatsNewTranslations() = default;

WhatsNewTranslations::~WhatsNewTranslations() = default;

// Use by newFeaturesMD5
QList<KLazyLocalizedString> WhatsNewTranslations::lastNewFeatures() const
{
    const QList<KLazyLocalizedString> info{kli18n("Add new whatsnew widget.")};
    return info;
}

#if HAVE_TEXTUTILS_HAS_WHATSNEW_SUPPORT
QList<TextAddonsWidgets::WhatsNewInfo> WhatsNewTranslations::createWhatsNewInfo() const
#else
QList<PimCommon::WhatsNewInfo> WhatsNewTranslations::createWhatsNewInfo() const
#endif
{
#if HAVE_TEXTUTILS_HAS_WHATSNEW_SUPPORT
    QList<TextAddonsWidgets::WhatsNewInfo> listInfo;
#else
    QList<PimCommon::WhatsNewInfo> listInfo;
#endif
    {
#if HAVE_TEXTUTILS_HAS_WHATSNEW_SUPPORT
        TextAddonsWidgets::WhatsNewInfo info;
#else
        PimCommon::WhatsNewInfo info;
#endif
        QStringList lst;
        for (const KLazyLocalizedString &l : lastNewFeatures()) {
            lst += l.toString();
        }
        info.setNewFeatures(lst);
        info.setBugFixings({i18n("Restore configure dialog size.")});
        info.setVersion(u"6.3.0"_s);
        listInfo.append(std::move(info));
    }
    return listInfo;
}
