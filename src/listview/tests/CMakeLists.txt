set(newdesigntest_SRCS
    main.cpp
    mainwindow.cpp
    addressbookmainwidget.cpp
    addressbookquickwidget.cpp
    ../../globalcontactmodel.cpp
    )

qt5_add_resources(newdesigntest_SRCS listview.qrc)

add_executable(newdesigntest ${newdesigntest_SRCS})

target_link_libraries(newdesigntest
    Qt5::Widgets
    KF5::Contacts
    KF5::I18n
    KF5::IconThemes
    KF5::WidgetsAddons
    Qt5::QuickWidgets
    KF5::AkonadiCore
    KF5::AkonadiWidgets
    KF5::AkonadiContact
    KF5::KaddressbookGrantlee
    )

