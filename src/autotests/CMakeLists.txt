add_executable( contactfieldstest contactfieldstest.cpp ../contactfields.cpp )
add_test( NAME contactfieldstest COMMAND contactfieldstest )
ecm_mark_as_test(kaddrebook-contactfieldstest)
target_link_libraries(contactfieldstest  KF5::Contacts Qt::Test Qt::Gui KF5::I18n)
