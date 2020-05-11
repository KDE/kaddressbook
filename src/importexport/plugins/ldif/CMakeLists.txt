set(kaddressbook_importexport_ldif_SRCS
    ldifimportexportplugin.cpp
    ldifimportexportplugininterface.cpp
    )


kcoreaddons_add_plugin(kaddressbook_importexportldifplugin JSON kaddressbook_importexportldifplugin.json SOURCES
    ${kaddressbook_importexport_ldif_SRCS} INSTALL_NAMESPACE kaddressbook5/importexportplugin)
target_compile_definitions(kaddressbook_importexportldifplugin PRIVATE TRANSLATION_DOMAIN=\"kaddressbook_importexportplugins\")

target_link_libraries(kaddressbook_importexportldifplugin
        kaddressbookimportexport
        KF5::PimCommon
        KF5::I18n
        KF5::XmlGui
        KF5::Contacts
        KF5::JobWidgets
        KF5::AkonadiCore
    )
