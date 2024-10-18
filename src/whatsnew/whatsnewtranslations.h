/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "kaddressbook_export.h"
#include <PimCommon/WhatsNewTranslationsBase>

class KADDRESSBOOK_EXPORT WhatsNewTranslations : public PimCommon::WhatsNewTranslationsBase
{
public:
    WhatsNewTranslations();
    ~WhatsNewTranslations() override;

    [[nodiscard]] QList<PimCommon::WhatsNewInfo> createWhatsNewInfo() const override;
    [[nodiscard]] QList<KLazyLocalizedString> lastNewFeatures() const override;
};
