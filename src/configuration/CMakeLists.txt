add_library(kaddressbook_config_plugins MODULE)

target_sources(kaddressbook_config_plugins PRIVATE
    kaddressbook_config_plugins.cpp
    kaddressbookconfigpluginlistwidget.cpp
    kaddressbook_config_plugins.h
    kaddressbookconfigpluginlistwidget.h
    )

ecm_qt_declare_logging_category(kaddressbook_config_plugins HEADER kaddressbook_configure_debug.h IDENTIFIER KADDRESSBOOK_CONFIGURE_LOG CATEGORY_NAME org.kde.pim.kaddressbook_configure
        DESCRIPTION "kaddressbook (configure)"
        EXPORT KADDRESSBOOK
    )



if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(kaddressbook_config_plugins PROPERTIES UNITY_BUILD ON)
endif()
target_link_libraries(kaddressbook_config_plugins
    KF${KF_MAJOR_VERSION}::KCMUtils
    KF${KF_MAJOR_VERSION}::I18n
    KF5::PimCommon
    kaddressbookprivate
    KF${KF_MAJOR_VERSION}::XmlGui
    KPimAddressbookImportExport
    )

install(TARGETS kaddressbook_config_plugins DESTINATION ${KDE_INSTALL_PLUGINDIR}/pim${QT_MAJOR_VERSION}/kcms/kaddressbook)

##################################################
if (TARGET KUserFeedbackWidgets)
    add_library(kaddressbook_config_userfeedback MODULE)

    target_sources(kaddressbook_config_userfeedback PRIVATE
        kaddressbook_config_userfeedback.cpp
        kaddressbook_config_userfeedback.h
        )


    target_link_libraries(kaddressbook_config_userfeedback
        KF${KF_MAJOR_VERSION}::KCMUtils
        KF${KF_MAJOR_VERSION}::I18n
        KF5::PimCommon
        kaddressbookprivate
        KPimAddressbookImportExport
        KF${KF_MAJOR_VERSION}::XmlGui
        KUserFeedbackWidgets
        )

    install(TARGETS kaddressbook_config_userfeedback DESTINATION ${KDE_INSTALL_PLUGINDIR}/pim${QT_MAJOR_VERSION}/kcms/kaddressbook)

endif()
