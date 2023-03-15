add_library(KPim${KF_MAJOR_VERSION}AddressbookImportExport)
add_library(KPim${KF_MAJOR_VERSION}::AddressbookImportExport ALIAS KPim${KF_MAJOR_VERSION}AddressbookImportExport)

ecm_setup_version(PROJECT VARIABLE_PREFIX KADDRESSBOOKIMPORTEXPORT
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kaddressbookimportexport_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}AddressbookImportExportConfigVersion.cmake"
    SOVERSION 5
)

target_sources(KPim${KF_MAJOR_VERSION}AddressbookImportExport PRIVATE
    contactfields.cpp
    contactlist.cpp
    contactselectiondialog.cpp
    contactselectionwidget.cpp
    exportselectionwidget.cpp
    importexportengine.cpp
    plugin.cpp
    plugininterface.cpp
    pluginmanager.cpp
    contactfields.h
    contactlist.h
    contactselectiondialog.h
    contactselectionwidget.h
    exportselectionwidget.h
    importexportengine.h
    plugin.h
    plugininterface.h
    pluginmanager.h
)

ecm_qt_declare_logging_category(KPim${KF_MAJOR_VERSION}AddressbookImportExport
    HEADER libkaddressbookexportimport_debug.h
    IDENTIFIER LIBKADDRESSBOOKIMPORTEXPORT_LOG
    CATEGORY_NAME org.kde.pim.kaddressbookimportexport
    DESCRIPTION "kaddressbook (kaddressbookimportexport)"
    EXPORT KADDRESSBOOK
)


if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPim${KF_MAJOR_VERSION}AddressbookImportExport PROPERTIES UNITY_BUILD ON)
endif()

generate_export_header(KPim${KF_MAJOR_VERSION}AddressbookImportExport BASE_NAME kaddressbook_importexport)

target_include_directories(KPim${KF_MAJOR_VERSION}AddressbookImportExport
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION};${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KAddressBookImportExport/;${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/kaddressbookimportexport>"
)

target_link_libraries(KPim${KF_MAJOR_VERSION}AddressbookImportExport
    PRIVATE
    KF${KF_MAJOR_VERSION}::Contacts
    KPim${KF_MAJOR_VERSION}::PimCommon
    KF${KF_MAJOR_VERSION}::I18n
    KPim${KF_MAJOR_VERSION}::AkonadiWidgets
)

set_target_properties(KPim${KF_MAJOR_VERSION}AddressbookImportExport PROPERTIES
    VERSION ${KDEPIM_LIB_VERSION}
    SOVERSION ${KDEPIM_LIB_SOVERSION}
    EXPORT_NAME AddressbookImportExport
)

install(TARGETS
    KPim${KF_MAJOR_VERSION}AddressbookImportExport
    EXPORT KPim${KF_MAJOR_VERSION}AddressbookImportExportTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
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
    LIB_NAME KPim${KF_MAJOR_VERSION}AddressbookImportExport
    DEPS ""
    FILENAME_VAR
    PRI_FILENAME
    INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KAddressbookImportExport
)

######## CMake Config Files ##########

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KPim${KF_MAJOR_VERSION}AddressbookImportExport")
set(IMPORTEXPORT_KF5_COMPAT FALSE)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KPimAddressbookImportExportConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}AddressbookImportExportConfig.cmake"
    INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}AddressbookImportExportConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}AddressbookImportExportConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT KPim${KF_MAJOR_VERSION}AddressbookImportExportTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE KPim${KF_MAJOR_VERSION}AddressbookImportExportTargets.cmake
    NAMESPACE KPim${KF_MAJOR_VERSION}::
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kaddressbookimportexport_version.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION} COMPONENT Devel
)

install(FILES
    ${KaddressbookImportExport_CamelCase_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/KAddressBookImportExport
    COMPONENT Devel
)

install(FILES
    ${KaddressbookImportExport_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/kaddressbook_importexport_export.h
    contactfields.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/kaddressbookimportexport
    COMPONENT Devel
)

install(FILES
    ${PRI_FILENAME}
    DESTINATION ${ECM_MKSPECS_INSTALL_DIR}
)

if (QT_MAJOR_VERSION STREQUAL "5")
##
# TODO: Backwards compatibility. Remove in next major version
##
set(CMAKECONFIG_INSTALL_DIR_KF5 "${KDE_INSTALL_CMAKEPACKAGEDIR}/KPimAddressbookImportExport")
set(IMPORTEXPORT_KF5_COMPAT TRUE)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KPimAddressbookImportExportConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KPimAddressbookImportExportConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR_KF5}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KPimAddressbookImportExportConfig.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR_KF5}"
    COMPONENT Devel
)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KPim${KF_MAJOR_VERSION}AddressbookImportExportConfigVersion.cmake"
    RENAME "KPimAddressbookImportExportConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR_KF5}"
    COMPONENT Devel
)
install(EXPORT KPim${KF_MAJOR_VERSION}AddressbookImportExportTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR_KF5}"
    FILE KPimAddressbookImportExportTargets.cmake
    NAMESPACE KPim::
)
endif()

