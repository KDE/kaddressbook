/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KXmlGuiWindow>

class MainWidget;
class KToggleAction;
class KHamburgerMenu;
class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow() override;
    MainWidget *mainWidget() const;

private:
    void configure();
    void configureKeyBindings();
    void configureToolbars() override;
    void newToolbarConfig();

    void slotToggleMenubar(bool dontShowWarning);

    void initActions();
    void updateHamburgerMenu();

    MainWidget *mMainWidget = nullptr;
    KToggleAction *mShowMenuBarAction = nullptr;
    KHamburgerMenu *mHamburgerMenu = nullptr;
};

