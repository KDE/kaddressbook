/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once
#include <KCMultiDialog>

class ConfigureDialog : public KCMultiDialog
{
public:
    explicit ConfigureDialog(QWidget *parent = nullptr);
    ~ConfigureDialog() override;

protected:
    void showEvent(QShowEvent *ev) override;

private:
    void writeConfig();
    void readConfig();
};
