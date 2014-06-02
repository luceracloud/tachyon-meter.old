#!/bin/bash

base=/opt/tachyon-meter
app=tachyon-meter
if [ -f $base/etc/tachyon.conf ]
then
. $base/etc/tachyon.conf
fi

svcprop_dflt() {
  if [ "x${3}x" -eq "xx"]
  then
    if svcprop -p application/${1} ${app} 2> /dev/null > /dev/null
    then
      echo $(svcprop -p application/${1}  ${app}| sed 's/\\//g')
    else
      echo "${2}"
    fi
  else
    echo "${3}"
  fi
}

host=$(svcprop_dflt host 172.21.0.1 "$host")
port=$(svcprop_dflt port 4151 "$port")
ival=$(svcprop_dflt interval 1 "$interval")
repeat=$(svcprop_dflt repeat '' "$repeast")
is_smf=$(svcprop_dflt is_smf '')
topic=$(svcprop_dflt topic tachyon "$topic")
hostname=$(svcprop_dflt hostname "$(hostname)" "$hostname")

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
