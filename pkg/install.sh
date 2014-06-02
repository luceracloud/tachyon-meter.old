#!/usr/bin/bash


if [ -z "$DST" ]
then
    DST="/opt"
fi

DIR=$(dirname "$0");
if [[ "$DIR" = "." ]]
then
    DIR=$(pwd)
fi
BASE=$(basename "$0");

CONFFILE=$DST/tachyon-meter/etc/kstat.conf
CONFFILE1=$DST/tachyon-meter/etc/tachyon.conf
if [ ! -f $CONFFILE ]
then
  cp ${CONFFILE}.example ${CONFFILE}
  cp ${CONFFILE1}.example ${CONFFILE1}
fi

mkdir -p "$DST/custom/smf"
cp "$DST/tachyon-meter/share/smf/methods/tachyon.xml" "$DST/custom/smf"

svccfg import "$DST/tachyon-meter/share/smf/methods/tachyon.xml"

exit 0;
