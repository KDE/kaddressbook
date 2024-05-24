# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
include_directories(${kaddressbook_SOURCE_DIR})
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
    KF6::I18n
    KPim6::KontactInterface
    Qt6::DBus
)

install(TARGETS kontact_kaddressbookplugin DESTINATION ${KDE_INSTALL_PLUGINDIR}/pim6/kontact)
