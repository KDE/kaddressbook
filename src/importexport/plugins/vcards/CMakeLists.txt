set(kaddressbook_importexport_vcard_SRCS
    vcardimportexportplugin.cpp
    vcardimportexportplugininterface.cpp
    engine/vcardviewerdialog.cpp
    engine/vcardexportselectionwidget.cpp
    )

kcoreaddons_add_plugin(kaddressbook_importexportvcardplugin JSON kaddressbook_importexportvcardplugin.json SOURCES
    ${kaddressbook_importexport_vcard_SRCS} INSTALL_NAMESPACE kaddressbook5/importexportplugin)
target_compile_definitions(kaddressbook_importexportvcardplugin PRIVATE TRANSLATION_DOMAIN=\"kaddressbook_importexportplugins\")

target_link_libraries(kaddressbook_importexportvcardplugin
    kaddressbookimportexport
    KF5::PimCommon
    KF5::I18n
    KF5::XmlGui
    KF5::Contacts
    KF5::KaddressbookGrantlee
    KF5::JobWidgets
    KF5::AkonadiContact
    Gpgmepp
    QGpgme
    )

