#! /bin/sh
$EXTRACTRC `find . -name \*.ui` `find . -name \*.rc` >> rc.cpp || exit 11
$EXTRACT_GRANTLEE_TEMPLATE_STRINGS `find . -name \*.html` >> rc.cpp
$XGETTEXT `find -name \*.cpp -o -name \*.h` -o $podir/kaddressbook.pot
rm -f rc.cpp
