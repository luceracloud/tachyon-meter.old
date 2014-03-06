set args -x http://192.168.253.128:4151/put?topic=tachyon unix::system_pages:physmem unix::system_pages:pp_kernel unix::system_pages:freemem unix::system_pages:nalloc_calls
break main
