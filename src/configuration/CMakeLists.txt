
set(kcm_config_plugins_SRCS
    kaddressbook_config_plugins.cpp
    kaddressbookconfigpluginlistwidget.cpp
    )

add_library(kaddressbook_config_plugins MODULE ${kcm_config_plugins_SRCS})

target_link_libraries(kaddressbook_config_plugins
    KF5::KCMUtils
    KF5::I18n
    KF5::IconThemes
    KF5::PimCommon
    )

kcoreaddons_desktop_to_json(kaddressbook_config_plugins kaddressbook_config_plugins.desktop SERVICE_TYPES kcmodule.desktop)
install(FILES kaddressbook_config_plugins.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
install(TARGETS kaddressbook_config_plugins DESTINATION ${KDE_INSTALL_PLUGINDIR})

