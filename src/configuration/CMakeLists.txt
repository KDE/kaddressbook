
set(kcm_config_plugins_SRCS
    kaddressbook_config_plugins.cpp
    kaddressbookconfigpluginlistwidget.cpp
    )

ecm_qt_declare_logging_category(kcm_config_plugins_SRCS HEADER kaddressbook_configure_debug.h IDENTIFIER KADDRESSBOOK_CONFIGURE_LOG CATEGORY_NAME org.kde.pim.kaddressbook_configure
        DESCRIPTION "kaddressbook (configure)"
        EXPORT KADDRESSBOOK
    )



add_library(kaddressbook_config_plugins MODULE ${kcm_config_plugins_SRCS})
if (COMPILE_WITH_CMAKE_SUPPORT)
    set_target_properties(kaddressbook_config_plugins PROPERTIES UNITY_BUILD ON)
endif()
target_link_libraries(kaddressbook_config_plugins
    KF5::KCMUtils
    KF5::I18n
    KF5::PimCommon
    kaddressbookprivate
    KF5::XmlGui
    KPimAddressbookImportExport
    )

kcoreaddons_desktop_to_json(kaddressbook_config_plugins kaddressbook_config_plugins.desktop SERVICE_TYPES kcmodule.desktop)
install(FILES kaddressbook_config_plugins.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
install(TARGETS kaddressbook_config_plugins DESTINATION ${KDE_INSTALL_PLUGINDIR})

##################################################
if (TARGET KUserFeedbackWidgets)
    set(kcm_config_userfeedback_SRCS
        kaddressbook_config_userfeedback.cpp
        )

    add_library(kaddressbook_config_userfeedback MODULE ${kcm_config_userfeedback_SRCS})

    target_link_libraries(kaddressbook_config_userfeedback
        KF5::KCMUtils
        KF5::I18n
        KF5::PimCommon
        kaddressbookprivate
        KPimAddressbookImportExport
        KF5::XmlGui
        KUserFeedbackWidgets
        )

    kcoreaddons_desktop_to_json(kaddressbook_config_userfeedback kaddressbook_config_userfeedback.desktop SERVICE_TYPES kcmodule.desktop)
    install(FILES kaddressbook_config_userfeedback.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
    install(TARGETS kaddressbook_config_userfeedback DESTINATION ${KDE_INSTALL_PLUGINDIR})

endif()
