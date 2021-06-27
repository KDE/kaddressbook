include_directories(${kaddressbook_SOURCE_DIR})
add_library(kontact_kaddressbookplugin MODULE)

target_sources(kontact_kaddressbookplugin PRIVATE
    kaddressbook_plugin.cpp
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
    KF5::I18n
    KF5::KontactInterface
)
kcoreaddons_desktop_to_json(kontact_kaddressbookplugin kaddressbookplugin.desktop SERVICE_TYPES kcmodule.desktop)

install(TARGETS kontact_kaddressbookplugin DESTINATION ${KDE_INSTALL_PLUGINDIR}/kontact5)
install(FILES kaddressbookplugin.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR}/kontact)
install(FILES kaddressbook.setdlg DESTINATION ${KDE_INSTALL_DATADIR}/kontact/ksettingsdialog)
