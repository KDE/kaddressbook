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



if(COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(kaddressbook_config_plugins PROPERTIES UNITY_BUILD ON)
endif()
target_link_libraries(kaddressbook_config_plugins
    KF6::KCMUtils
    KF6::I18n
    KPim6::PimCommon
    kaddressbookprivate
    KF6::XmlGui
    KPim6AddressbookImportExport
   )

install(TARGETS kaddressbook_config_plugins DESTINATION ${KDE_INSTALL_PLUGINDIR}/pim6/kcms/kaddressbook)

##################################################
if(TARGET KF6::UserFeedbackWidgets)
    add_library(kaddressbook_config_userfeedback MODULE)

    target_sources(kaddressbook_config_userfeedback PRIVATE
        kaddressbook_config_userfeedback.cpp
        kaddressbook_config_userfeedback.h
       )


    target_link_libraries(kaddressbook_config_userfeedback
        KF6::KCMUtils
        KF6::I18n
        KPim6::PimCommon
        kaddressbookprivate
        KPim6AddressbookImportExport
        KF6::XmlGui
        KF6::UserFeedbackWidgets
       )

    install(TARGETS kaddressbook_config_userfeedback DESTINATION ${KDE_INSTALL_PLUGINDIR}/pim6/kcms/kaddressbook)

endif()
