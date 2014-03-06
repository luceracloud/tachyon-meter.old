#!/bin/bash

parms=`cat kstat.conf | grep -v \#`
# run $parms every 1 second 10 times
#./kstat -x $parms 1 10
# same thing but run forever
#./kstat -x $parms 1

./kstat -x http://102.168.253.128:4151/put?topic=tachyon $parms 1 1

