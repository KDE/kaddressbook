# FIXME: Fold all translations in kaddressbok domain
remove_definitions(-DTRANSLATION_DOMAIN=\"kaddressbook\")

include_directories(${CMAKE_BINARY_DIR}/src)

set(kaddressbook_importexport_LIB_SRCS
    importexportengine.cpp
    kaddressbookimportexportplugin.cpp
    kaddressbookimportexportplugininterface.cpp
    kaddressbookimportexportpluginmanager.cpp
    kaddressbookimportexportcontactlist.cpp
    kaddressbookcontactselectionwidget.cpp
    kaddressbookcontactselectiondialog.cpp
    kaddressbookexportselectionwidget.cpp
    kaddressbookimportexportcontactfields.cpp
    )

ecm_qt_declare_logging_category(kaddressbook_importexport_LIB_SRCS HEADER libkaddressbookexportimport_debug.h IDENTIFIER LIBKADDRESSBOOKIMPORTEXPORT_LOG CATEGORY_NAME org.kde.pim.kaddressbookimportexport
        DESCRIPTION "kaddressbook (kaddressbookimportexport)"
        EXPORT KADDRESSBOOK
    )


add_library(kaddressbookimportexport STATIC ${kaddressbook_importexport_LIB_SRCS})
target_compile_definitions(kaddressbookimportexport PRIVATE TRANSLATION_DOMAIN=\"libkaddressbookexportimport\")

target_link_libraries(kaddressbookimportexport
    KF5::Contacts
    KF5::PimCommon
    KF5::I18n
    KF5::AkonadiWidgets
    )


add_subdirectory(plugins)
