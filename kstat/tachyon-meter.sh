#!/bin/bash

base=/opt/tachyon-meter
app=tachyon-meter

## loads config file
if [ -e ${base}/etc/tachyon-config ]
then
    . ${base}/etc/tachyon-config
else
    echo "Missing config file: ${base}/etc/tachyon-config"
    exit 1
fi

url="http://${host}:${port}/put?topic=${topic}"
cmd="${base}/bin/tachyon-meter -x '${url}' -H '${hostname}'"

while read stat
do
  if ! echo "$stat" | grep '^#' > /dev/null
  then
    cmd="${cmd} '${stat}'"
  fi
done < ${base}/etc/kstat.conf

if [ ! -z ${ival} ]
then
  cmd="${cmd} ${ival}"
  if [ ! -z ${repeat} ]
  then
    cmd="${cmd} ${repeat}"
  fi
fi

## debug
if [ -z ${DEBUG_MEM} ]
then
  if [ "${is_smf}" == "yes" ]
  then
    echo "${cmd} &"
    eval LD_LIBRARY_PATH=${base}/lib ${cmd} &
  else
    echo "${cmd}"
    eval LD_LIBRARY_PATH=${base}/lib ${cmd}
  fi
else
  eval UMEM_DEBUG=default UMEM_LOGGING=transaction LD_PRELOAD=libumem.so.1 LD_LIBRARY_PATH=${base}/lib ${cmd}
fi
