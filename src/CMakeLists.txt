# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
configure_file(kaddressbook-version.h.in ${CMAKE_CURRENT_BINARY_DIR}/kaddressbook-version.h @ONLY)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/printing/)

add_subdirectory(icons)
add_subdirectory(printing)
add_subdirectory(configuration)
add_subdirectory(importexport)

add_library(kaddressbookprivate)

target_sources(
    kaddressbookprivate
    PRIVATE
        printing/detailed/detailledstyle.cpp
        printing/mike/mikesstyle.cpp
        printing/printingwizard.cpp
        printing/printprogress.cpp
        printing/printstyle.cpp
        printing/ringbinder/ringbinderstyle.cpp
        printing/compact/compactstyle.cpp
        printing/grantlee/grantleeprintstyle.cpp
        printing/stylepage.cpp
        plugininterface/kaddressbookplugininterface.cpp
        aboutdata.cpp
        categoryfilterproxymodel.cpp
        categoryselectwidget.cpp
        contactinfoproxymodel.cpp
        contactsorter.cpp
        contactswitcher.cpp
        globalcontactmodel.cpp
        mainwidget.cpp
        uistatesaver.cpp
        manageshowcollectionproperties.cpp
        modelcolumnmanager.cpp
        stylecontactlistdelegate.cpp
        widgets/quicksearchwidget.cpp
        printing/detailed/detailledstyle.h
        printing/mike/mikesstyle.h
        printing/printingwizard.h
        printing/printprogress.h
        printing/printstyle.h
        printing/ringbinder/ringbinderstyle.h
        printing/compact/compactstyle.h
        printing/grantlee/grantleeprintstyle.h
        printing/stylepage.h
        plugininterface/kaddressbookplugininterface.h
        aboutdata.h
        categoryfilterproxymodel.h
        categoryselectwidget.h
        contactinfoproxymodel.h
        contactsorter.h
        contactswitcher.h
        globalcontactmodel.h
        mainwidget.h
        uistatesaver.h
        manageshowcollectionproperties.h
        modelcolumnmanager.h
        stylecontactlistdelegate.h
        widgets/quicksearchwidget.h
        contactentitymimetypefiltermodel.h
        contactentitymimetypefiltermodel.cpp
        configuredialog.h
        configuredialog.cpp
        whatsnew/whatsnewtranslations.cpp
        whatsnew/whatsnewtranslations.h
)

ki18n_wrap_ui(kaddressbookprivate
    printing/ringbinder/rbs_appearance.ui
    printing/detailed/ds_appearance.ui
    printing/compact/compactstyle.ui
)

if(TARGET KF6::UserFeedbackWidgets)
    target_sources(
        kaddressbookprivate
        PRIVATE
            userfeedback/userfeedbackmanager.cpp
            userfeedback/kaddressbookuserfeedbackprovider.cpp
            userfeedback/userfeedbackmanager.h
            userfeedback/kaddressbookuserfeedbackprovider.h
    )
endif()

if(HAVE_ACTIVITY_SUPPORT)
    target_sources(
        kaddressbookprivate
        PRIVATE
            activities/activitiesmanager.h
            activities/activitiesmanager.cpp
            activities/ldapactivities.cpp
            activities/ldapactivities.h
            activities/accountactivities.h
            activities/accountactivities.cpp
    )
    if(BUILD_TESTING)
        add_subdirectory(activities/autotests)
    endif()
endif()

ecm_qt_declare_logging_category(kaddressbookprivate HEADER kaddressbook_debug.h IDENTIFIER KADDRESSBOOK_LOG CATEGORY_NAME org.kde.pim.kaddressbook
        DESCRIPTION "kaddressbook (kaddressbook)"
        OLD_CATEGORY_NAMES log_kaddressbook
        EXPORT KADDRESSBOOK
)

ecm_qt_declare_logging_category(kaddressbookprivate HEADER kaddressbook_activities_debug.h IDENTIFIER KADDRESSBOOK_ACTIVITIES_LOG CATEGORY_NAME org.kde.pim.kaddressbook.activities
        DESCRIPTION "kaddressbook activities (kaddressbook)"
        EXPORT KADDRESSBOOK
)

qt_add_dbus_adaptor(kaddressbook_LIB_SRCS org.kde.kaddressbook.xml mainwidget.h MainWidget)
target_sources(kaddressbookprivate PRIVATE ${kaddressbook_LIB_SRCS})

kconfig_add_kcfg_files(kaddressbookprivate settings/settings.kcfgc)
if(COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(
        kaddressbookprivate
        PROPERTIES
            UNITY_BUILD
                ON
    )
endif()
generate_export_header(kaddressbookprivate BASE_NAME kaddressbook)
target_link_libraries(
    kaddressbookprivate
    PUBLIC
        KPim6::PimCommonAkonadi
    PRIVATE
        KPim6AddressbookImportExport
        KPim6::Libkdepim
        KPim6::AkonadiCore
        KF6::Contacts
        KPim6::AkonadiContactWidgets
        KF6::KCMUtils
        KPim6::GrantleeTheme
        KF6::Completion
        KF6::XmlGui
        KF6::JobWidgets
        KF6::KIOWidgets
        Qt::PrintSupport
        KF6::I18n
        KF6::ColorScheme
)
if(TARGET KF6::UserFeedbackWidgets)
    target_link_libraries(kaddressbookprivate PRIVATE KF6::UserFeedbackWidgets)
endif()

if(HAVE_ACTIVITY_SUPPORT)
    target_link_libraries(
        kaddressbookprivate
        PRIVATE
            KPim6::PimCommonActivities
            KPim6::LdapCore
    )
endif()

if(KPim6AkonadiSearch_FOUND)
    target_link_libraries(kaddressbookprivate PRIVATE KPim6::AkonadiSearchDebug)
endif()

target_include_directories(
    kaddressbookprivate
    PUBLIC
        "$<BUILD_INTERFACE:${kaddressbook_SOURCE_DIR};${kaddressbook_BINARY_DIR}>"
)

set_target_properties(
    kaddressbookprivate
    PROPERTIES
        VERSION
            ${KDEPIM_LIB_VERSION}
        SOVERSION
            ${KDEPIM_LIB_SOVERSION}
)
install(
    TARGETS
        kaddressbookprivate
        ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
    LIBRARY
        NAMELINK_SKIP
)

########### next target ###############

add_executable(kaddressbook)
target_sources(
    kaddressbook
    PRIVATE
        kaddressbook_debug.cpp
        mainwindow.cpp
        mainwindow.h
        main.cpp
        kaddressbook.qrc
)
if(TARGET KF6::IconThemes)
    target_link_libraries(kaddressbook KF6::IconThemes)
endif()

file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/icons/*-apps-kaddressbook.png")
ecm_add_app_icon(kaddressbook ICONS ${ICONS_SRCS})
if(COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(
        kaddressbook
        PROPERTIES
            UNITY_BUILD
                ON
    )
endif()
if(TARGET KF6::UserFeedbackWidgets)
    target_link_libraries(kaddressbook KF6::UserFeedbackWidgets)
endif()

target_link_libraries(
    kaddressbook
    KF6::I18n
    KPim6::KontactInterface
    kaddressbookprivate
    KF6::Crash
)

install(
    TARGETS
        kaddressbook
        ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)
install(
    PROGRAMS
        data/org.kde.kaddressbook.desktop
        data/kaddressbook-view.desktop
        data/kaddressbook-importer.desktop
    DESTINATION ${KDE_INSTALL_APPDIR}
)
install(FILES data/org.kde.kaddressbook.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

####################################"
add_library(kaddressbookpart MODULE)

target_sources(
    kaddressbookpart
    PRIVATE
        kaddressbookpart.cpp
        kaddressbookpart.h
        kaddressbook_debug.cpp
        kaddressbook.qrc
)
target_link_libraries(
    kaddressbookpart
    kaddressbookprivate
    KF6::I18n
    KF6::Parts
)
install(TARGETS kaddressbookpart DESTINATION ${KDE_INSTALL_PLUGINDIR})

install(
    FILES
        viewertemplates/contact.html
        viewertemplates/contact_embedded.html
        viewertemplates/contact_row.html
        viewertemplates/contactgroup.html
        viewertemplates/contactgroup_embedded.html
        viewertemplates/contactgroup_additionalfieldrow.html
        viewertemplates/contactgroup_memberrow.html
        viewertemplates/theme.desktop
    DESTINATION ${KDE_INSTALL_DATADIR}/kaddressbook/viewertemplates/default/
)

install(DIRECTORY printing/grantlee/themes/ DESTINATION ${KDE_INSTALL_DATADIR}/kaddressbook/printing/themes/)
