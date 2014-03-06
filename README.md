kstat generator
=====================

To build
```bash
cd kstat
make
```

Running
`./kstat.sh` in the kstat directory
which runs `./kstat -x url stat [stat....]`

Testing is easy just use -p instead of -x url
```bash
./kstat -p 'sd:::writes' 1 1
```

Protocol
a packet is preceeded by its length (int) the packet contains, starting at time field

```
description | time | name | module | class | instance | ks_name | value |
types       | t    | s    | s      | s     | i        | s       | [isd] | d - double, i - int
length      | 8    | var  | var    | var   | 4        | var     | var   |
data        | 123  | sd   |        |       | 0        |         |       |
```

Notes
The original source is from the illuminos distro of OpenSolaris
`illumos-joyent/usr/src/uts/common/io/kstat.c`

Added -x flag that takes one parameter

around line kstat.c:923
```c
//...
	} else if (g_nflg) {
		ks_print_fn = &ks_instance_print_nsq;
		tm_curl_set_url(tachyon_url);
	} else {
//...
```

The new function ks_instance_print_nsq formats statistics and passes them via curl to 
the url passed in.  The server at the other end should be nsq.

