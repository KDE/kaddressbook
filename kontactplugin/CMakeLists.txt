include_directories(${kaddressbook_SOURCE_DIR})

set(kontact_kaddressbookplugin_PART_SRCS
    kaddressbook_plugin.cpp
)
ecm_qt_declare_logging_category(kontact_kaddressbookplugin_PART_SRCS HEADER kaddressbookplugin_debug.h IDENTIFIER KADDRESSBOOKPLUGIN_LOG CATEGORY_NAME org.kde.pim.kaddressbook_plugin
        DESCRIPTION "kaddressbook (kaddressbook kontact plugins)"
        OLD_CATEGORY_NAMES log_kaddressbook_plugin
        EXPORT KADDRESSBOOK
    )

qt5_add_dbus_interfaces(kontact_kaddressbookplugin_PART_SRCS ../src/org.kde.kaddressbook.xml)


add_library(kontact_kaddressbookplugin MODULE ${kontact_kaddressbookplugin_PART_SRCS})
target_link_libraries(kontact_kaddressbookplugin
    KF5::KontactInterface
)
kcoreaddons_desktop_to_json(kontact_kaddressbookplugin kaddressbookplugin.desktop)

install(TARGETS kontact_kaddressbookplugin DESTINATION ${KDE_INSTALL_PLUGINDIR}/kontact5)
install(FILES kaddressbookplugin.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR}/kontact)
install(FILES kaddressbook.setdlg DESTINATION ${KDE_INSTALL_DATADIR}/kontact/ksettingsdialog)
