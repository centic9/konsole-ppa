#! /bin/sh
rm -f schemas.cpp
#(cd ../desktop && $PREPARETIPS > ../src/tips.cpp)
for i in ../data/color-schemes/*.colorscheme; do
grep "^Description=" $i | sed -e 's#^Description=\(.*\)$#i18n(\"\1\")#' >> schemas.cpp
done
for i in ../data/keyboard-layouts/*.keytab; do
grep "^keyboard" $i | sed -e 's#^keyboard \"\(.*\)\"$#i18n(\"\1\")#' >> schemas.cpp
done
$EXTRACTRC `find . -name \*.ui`          >>  rc.cpp
$EXTRACTRC `find . -name \*.kcfg` >> rc.cpp
$EXTRACTRC `find ../desktop -name \*.rc` >> rc.cpp
$XGETTEXT `find . -name \*.cpp -o -name \*.h | grep -v '/tests/'` -o $podir/konsole.pot
rm -f schemas.cpp
#rm -f tips.cpp
rm -f rc.cpp
