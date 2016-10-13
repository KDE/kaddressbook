set(kaddressbook_akonadi_SRCS akonadicontactgrouprepository.cpp akonadicontactrepository.cpp)


add_library(kaddressbookdomainakonadi ${kaddressbook_akonadi_SRCS})
generate_export_header(kaddressbookdomainakonadi BASE_NAME kaddressbookdomainakonadi)

target_link_libraries(kaddressbookdomainakonadi
    PUBLIC
    KF5::Contacts
    PRIVATE
    Qt5::Core
    kaddressbookdomain
    )


set_target_properties(kaddressbookdomainakonadi
    PROPERTIES VERSION ${KDEPIM_LIB_VERSION} SOVERSION ${KDEPIM_LIB_SOVERSION}
    )

# Don't install it yet
#install(TARGETS kaddressbookdomain ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)

#add_subdirectory(autotests)
