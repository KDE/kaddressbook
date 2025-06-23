/*
  This file is part of KAddressBook.

  SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "aboutdata.h"
using namespace Qt::Literals::StringLiterals;

#include "config-kaddressbook.h"
#include "kaddressbook_options.h"
#include "mainwidget.h"
#include "mainwindow.h"

#include <KCrash>
#include <QCommandLineParser>

#include "kaddressbook_debug.h"
#include <kontactinterface/pimuniqueapplication.h>
#if KADDRESSBOOK_WITH_KUSERFEEDBACK
#include "userfeedback/kaddressbookuserfeedbackprovider.h"
#endif

#include <KIconTheme>

#include <KStyleManager>

//-----------------------------------------------------------------------------

class KAddressBookApplication : public KontactInterface::PimUniqueApplication
{
public:
    KAddressBookApplication(int &argc, char **argv[])
        : KontactInterface::PimUniqueApplication(argc, argv)
    {
    }

    int activate(const QStringList &arguments, const QString &) override;

private:
    MainWindow *mMainWindow = nullptr;
};

int KAddressBookApplication::activate(const QStringList &arguments, const QString &)
{
    if (!mMainWindow) {
        mMainWindow = new MainWindow;
        mMainWindow->show();
    }
    mMainWindow->mainWidget()->handleCommandLine(arguments);
    return 0;
}

int main(int argc, char **argv)
{
    KIconTheme::initTheme();
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    KAddressBookApplication app(argc, &argv);
    app.setDesktopFileName(u"org.kde.kaddressbook"_s);
    KStyleManager::initStyle();
    KLocalizedString::setApplicationDomain(QByteArrayLiteral("kaddressbook"));

    AboutData about;
    app.setAboutData(about);
    KCrash::initialize();

    app.setWindowIcon(QIcon::fromTheme(u"kaddressbook"_s));
    QCommandLineParser *cmdArgs = app.cmdArgs();
    kaddressbook_options(cmdArgs);

    const QStringList args = QCoreApplication::arguments();
    cmdArgs->process(args);
    about.processCommandLine(cmdArgs);

#if KADDRESSBOOK_WITH_KUSERFEEDBACK
    if (cmdArgs->isSet(u"feedback"_s)) {
        auto userFeedBack = new KAddressBookUserFeedbackProvider(nullptr);
        QTextStream(stdout) << userFeedBack->describeDataSources() << '\n';
        delete userFeedBack;
        return 0;
    }
#endif

    if (!KAddressBookApplication::start(args)) {
        qCWarning(KADDRESSBOOK_LOG) << "kaddressbook is already running, exiting.";
        return 0;
    }
    return app.exec();
}
