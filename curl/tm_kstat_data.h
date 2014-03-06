#ifndef _tm_kstat_data_h_
#define _tm_kstat_data_h_

#include <sys/kstat.h>

char* tm_kstat_data_packet(const hrtime_t snap_time, const char* host, const char* zone,
		     const char* module, int instance, const char* name,
		     const char* ksname, const uint64_t value, int* len);

char* tm_kstat_data_named_packet(const hrtime_t snap_time, const char* host, const char* zone,
		     const char* module, int instance, const char* name,
		     const uint8_t type, kstat_named_t* kstat, int* len);

#endif
