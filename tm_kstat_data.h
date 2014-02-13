#ifndef _tm_kstat_data_h_
#define _tm_kstat_data_h_

#include <sys/kstat.h>
#include "pckt.pb.h"

class tm_kstat_data {
 public:

  static std::string generate_packet(const hrtime_t snap_time, const char* host,
				     const char* module, int instance, const char* name,
				     const char* ksname, const uint64_t value, int& len);

  static std::string generate_packet(const hrtime_t snap_time, const char* host,
				     const char* module, int instance, const char* name,
				     const uint8_t type, kstat_named_t* kstat, int& len);

};

#endif
