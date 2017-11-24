set(kaddressbook_domain_SRCS contactgroup.cpp contact.cpp contactrepository.cpp contactgrouprepository.cpp)


add_library(kaddressbookdomain ${kaddressbook_domain_SRCS})
generate_export_header(kaddressbookdomain BASE_NAME kaddressbookdomain)

target_link_libraries(kaddressbookdomain
    PUBLIC
    KF5::Contacts
    PRIVATE
    Qt5::Core
    )


set_target_properties(kaddressbookdomain
    PROPERTIES VERSION ${KDEPIM_LIB_VERSION} SOVERSION ${KDEPIM_LIB_SOVERSION}
    )

install(TARGETS kaddressbookdomain ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)
if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()
