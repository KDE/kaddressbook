configure_file(kaddressbook-version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/kaddressbook-version.h @ONLY)



include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/printing/
    )


add_subdirectory(icons)
add_subdirectory(printing)
add_subdirectory(kconf_update)
add_subdirectory(configuration)
add_subdirectory(importexport)

add_library(kaddressbookprivate)


target_sources(kaddressbookprivate PRIVATE
    printing/detailled/detailledstyle.cpp
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
    kaddressbookmigrateapplication.cpp

    )

ki18n_wrap_ui(kaddressbookprivate
    printing/ringbinder/rbs_appearance.ui
    printing/detailled/ds_appearance.ui
    printing/compact/compactstyle.ui
    )


if (TARGET KUserFeedbackWidgets)
    target_sources(kaddressbookprivate PRIVATE
        userfeedback/userfeedbackmanager.cpp
        userfeedback/kaddressbookuserfeedbackprovider.cpp
        )
endif()


ecm_qt_declare_logging_category(kaddressbookprivate HEADER kaddressbook_debug.h IDENTIFIER KADDRESSBOOK_LOG CATEGORY_NAME org.kde.pim.kaddressbook
        DESCRIPTION "kaddressbook (kaddressbook)"
        OLD_CATEGORY_NAMES log_kaddressbook
        EXPORT KADDRESSBOOK
    )


qt_add_dbus_adaptor( kaddressbook_LIB_SRCS org.kde.kaddressbook.xml mainwidget.h MainWidget)
target_sources(kaddressbookprivate PRIVATE ${kaddressbook_LIB_SRCS})

kconfig_add_kcfg_files(kaddressbookprivate settings/settings.kcfgc)
if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(kaddressbookprivate PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(kaddressbookprivate BASE_NAME kaddressbook)
target_link_libraries(kaddressbookprivate
    PUBLIC
    KF5::PimCommonAkonadi
    PRIVATE
    KPimAddressbookImportExport
    KF5::Libkdepim
    KF5::AkonadiCore
    KF5::Contacts
    KF5::AkonadiContact
    KF5::KCMUtils
    KF5::GrantleeTheme
    KF5::Completion
    KF5::XmlGui
    KF5::JobWidgets
    KF5::KIOWidgets
    Qt::PrintSupport
    KF5::AkonadiSearchDebug
    KF5::I18n
    KF5::Prison
    )
if (TARGET KUserFeedbackWidgets)
    target_link_libraries(kaddressbookprivate PRIVATE  KUserFeedbackWidgets)
endif()

if(QGpgme_FOUND)
    target_link_libraries(kaddressbookprivate PRIVATE QGpgme)
endif()

target_include_directories(kaddressbookprivate PUBLIC "$<BUILD_INTERFACE:${kaddressbook_SOURCE_DIR};${kaddressbook_BINARY_DIR}>")

set_target_properties(kaddressbookprivate PROPERTIES VERSION ${KDEPIM_LIB_VERSION} SOVERSION ${KDEPIM_LIB_SOVERSION})
install(TARGETS kaddressbookprivate ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)

########### next target ###############

add_executable(kaddressbook)
target_sources(kaddressbook PRIVATE
    kaddressbook_debug.cpp
    main.cpp
    mainwindow.cpp
    kaddressbook.qrc
    )

file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/icons/*-apps-kaddressbook.png")
ecm_add_app_icon(kaddressbook ICONS ${ICONS_SRCS})
if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(kaddressbook PROPERTIES UNITY_BUILD ON)
endif()
if (TARGET KUserFeedbackWidgets)
    target_link_libraries(kaddressbook  KUserFeedbackWidgets)
endif()

target_link_libraries(kaddressbook
    KF5::I18n
    KF5::KontactInterface
    kaddressbookprivate
    KF5::Crash
    )

install(TARGETS kaddressbook ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS
    data/org.kde.kaddressbook.desktop
    data/kaddressbook-view.desktop
    data/kaddressbook-importer.desktop
    DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES data/org.kde.kaddressbook.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

####################################"
add_library(kaddressbookpart MODULE)

target_sources(kaddressbookpart PRIVATE kaddressbookpart.cpp kaddressbook_debug.cpp kaddressbook.qrc)
target_link_libraries(kaddressbookpart  kaddressbookprivate KF5::I18n KF5::Parts)
install(TARGETS kaddressbookpart DESTINATION ${KDE_INSTALL_PLUGINDIR})

install(FILES
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

install(DIRECTORY printing/grantlee/themes/ DESTINATION ${KDE_INSTALL_DATADIR}/kaddressbook/printing/themes/ )

