#!/bin/bash

app=tachyon_meter
svcprop_dflt() {
	if svcprop -p application/${1} ${app} 2> /dev/null > /dev/null
  then
    echo $(svcprop -p application/${1}  ${app}| sed 's/\\//g')
  else
    echo "${2}"
  fi
}

host=$(svcprop_dflt host 172.21.0.1)
port=$(svcprop_dflt port 4151)
ival=$(svcprop_dflt interval 1)
repeat=$(svcprop_dflt repeat '')
topic=$(svcprop_dflt topic tachyon)
hostname=$(svcprop_dflt hostname "$(hostname)")

url="http://${host}:${port}/put?topic=${topic}"
cmd="./kstat -x '${url}' -H '${hostname}'"

while read stat
do
  if ! echo "$stat" | grep '^#' > /dev/null
  then
    cmd="${cmd} '${stat}'"
  fi
done < kstat.conf

if [ ! -z ${ival} ]
then
  cmd="${cmd} ${ival}"
  if [ ! -z ${repeat} ]
  then
    cmd="${cmd} ${repeat}"
  fi
fi


# run $parms every 1 second 10 times
#./kstat -x $parms 1 10
# same thing but run forever
#./kstat -x $parms 1

echo "${cmd}"
eval "${cmd}"
