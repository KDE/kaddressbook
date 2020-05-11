set(kaddressbook_importexport_ldap_SRCS
    ldapimportexportplugin.cpp
    ldapimportexportplugininterface.cpp
    )


kcoreaddons_add_plugin(kaddressbook_importexportldapplugin JSON kaddressbook_importexportldapplugin.json SOURCES
    ${kaddressbook_importexport_ldap_SRCS} INSTALL_NAMESPACE kaddressbook5/importexportplugin)
target_compile_definitions(kaddressbook_importexportldapplugin PRIVATE TRANSLATION_DOMAIN=\"kaddressbook_importexportplugins\")

target_link_libraries(kaddressbook_importexportldapplugin
        kaddressbookimportexport
        KF5::PimCommon
        KF5::AkonadiCore
        KF5::I18n
        KF5::XmlGui
        KF5::Contacts
        KF5::Libkdepim
    )
