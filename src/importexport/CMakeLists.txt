add_library(KPimAddressbookImportExport)
add_library(KPim::AddressbookImportExport ALIAS KPimAddressbookImportExport)

ecm_setup_version(PROJECT VARIABLE_PREFIX KADDRESSBOOKIMPORTEXPORT
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kaddressbookimportexport_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KPimAddressbookImportExportConfigVersion.cmake"
    SOVERSION 5
)

target_sources(KPimAddressbookImportExport PRIVATE
    contactfields.cpp
    contactlist.cpp
    contactselectiondialog.cpp
    contactselectionwidget.cpp
    exportselectionwidget.cpp
    importexportengine.cpp
    plugin.cpp
    plugininterface.cpp
    pluginmanager.cpp
)

ecm_qt_declare_logging_category(KPimAddressbookImportExport
    HEADER libkaddressbookexportimport_debug.h
    IDENTIFIER LIBKADDRESSBOOKIMPORTEXPORT_LOG
    CATEGORY_NAME org.kde.pim.kaddressbookimportexport
    DESCRIPTION "kaddressbook (kaddressbookimportexport)"
    EXPORT KADDRESSBOOK
)


if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPimAddressbookImportExport PROPERTIES UNITY_BUILD ON)
endif()

generate_export_header(KPimAddressbookImportExport BASE_NAME kaddressbook_importexport)

target_include_directories(KPimAddressbookImportExport
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim;${KDE_INSTALL_INCLUDEDIR}/KPim/KAddressBookImportExport/;${KDE_INSTALL_INCLUDEDIR}/KPim/kaddressbookimportexport>"
)
target_include_directories(KPimAddressbookImportExport
    PUBLIC "$<BUILD_INTERFACE:${Grantlee_INCLUDE_DIRS}>"
)

target_link_libraries(KPimAddressbookImportExport
    PRIVATE
    KF5::Contacts
    KF5::PimCommon
    KF5::I18n
    KF5::AkonadiWidgets
)

set_target_properties(KPimAddressbookImportExport PROPERTIES
    VERSION ${KDEPIM_LIB_VERSION}
    SOVERSION ${KDEPIM_LIB_SOVERSION}
    EXPORT_NAME AddressbookImportExport
)

install(TARGETS
    KPimAddressbookImportExport
    EXPORT KPimAddressbookImportExportTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS}
)

ecm_generate_headers(KaddressbookImportExport_CamelCase_HEADERS
    HEADER_NAMES
    PluginInterface
    Plugin
    PluginManager
    ContactList
    ContactFields
    ContactSelectionDialog
    ContactSelectionWidget
    ImportExportEngine
    ExportSelectionWidget
    REQUIRED_HEADERS KaddressbookImportExport_HEADERS
    PREFIX KAddressBookImportExport
)

ecm_generate_pri_file(BASE_NAME KAddressbookImportExport
    LIB_NAME KPimAddressbookImportExport
    DEPS ""
    FILENAME_VAR
    PRI_FILENAME
    INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR}/KPim/KAddressbookImportExport
)

######## CMake Config Files ##########

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KPimAddressbookImportExport")

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KPimAddressbookImportExportConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KPimAddressbookImportExportConfig.cmake"
    INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KPimAddressbookImportExportConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KPimAddressbookImportExportConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT KPimAddressbookImportExportTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE KPimAddressbookImportExportTargets.cmake
    NAMESPACE KPim::
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kaddressbookimportexport_version.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim COMPONENT Devel
)

install(FILES
    ${KaddressbookImportExport_CamelCase_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim/KAddressBookImportExport
    COMPONENT Devel
)

install(FILES
    ${KaddressbookImportExport_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/kaddressbook_importexport_export.h
    contactfields.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim/kaddressbookimportexport
    COMPONENT Devel
)

install(FILES
    ${PRI_FILENAME}
    DESTINATION ${ECM_MKSPECS_INSTALL_DIR}
)

