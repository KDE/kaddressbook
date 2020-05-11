set(kaddressbook_importexport_csv_SRCS
    csvimportexportplugin.cpp
    csvimportexportplugininterface.cpp
    import/csvimportdialog.cpp
    import/dateparser.cpp
    import/qcsvmodel.cpp
    import/qcsvreader.cpp
    import/templateselectiondialog.cpp
    )


kcoreaddons_add_plugin(kaddressbook_importexportcsvplugin JSON kaddressbook_importexportcsvplugin.json SOURCES
    ${kaddressbook_importexport_csv_SRCS} INSTALL_NAMESPACE kaddressbook5/importexportplugin)
target_compile_definitions(kaddressbook_importexportcsvplugin PRIVATE TRANSLATION_DOMAIN=\"kaddressbook_importexportplugins\")

target_link_libraries(kaddressbook_importexportcsvplugin
        kaddressbookimportexport
        KF5::PimCommon
        KF5::I18n
        KF5::XmlGui
        KF5::Contacts
        KF5::JobWidgets
        KF5::Completion
        KF5::KIOWidgets
        KF5::AkonadiCore
    )
