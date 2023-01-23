# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
include_directories(${kaddressbook_SOURCE_DIR})
configure_file(kaddressbookplugin.json.cmake ${CMAKE_CURRENT_BINARY_DIR}/kaddressbookplugin.json )
add_library(kontact_kaddressbookplugin MODULE)

target_sources(kontact_kaddressbookplugin PRIVATE
    kaddressbook_plugin.cpp
    kaddressbook_plugin.h
)
ecm_qt_declare_logging_category(kontact_kaddressbookplugin HEADER kaddressbookplugin_debug.h IDENTIFIER KADDRESSBOOKPLUGIN_LOG CATEGORY_NAME org.kde.pim.kaddressbook_plugin
        DESCRIPTION "kaddressbook (kaddressbook kontact plugins)"
        OLD_CATEGORY_NAMES log_kaddressbook_plugin
        EXPORT KADDRESSBOOK
    )
set(kontact_kaddressbook_interface_SRCS)
qt_add_dbus_interfaces(kontact_kaddressbook_interface_SRCS ../src/org.kde.kaddressbook.xml)
target_sources(kontact_kaddressbookplugin PRIVATE ${kontact_kaddressbook_interface_SRCS})


target_link_libraries(kontact_kaddressbookplugin
    KF${KF_MAJOR_VERSION}::I18n
    KF5::KontactInterface
)

install(TARGETS kontact_kaddressbookplugin DESTINATION ${KDE_INSTALL_PLUGINDIR}/pim${QT_MAJOR_VERSION}/kontact)
