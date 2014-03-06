#include <memory.h>
#include <malloc.h>
#include <string.h>
#include <sys/kstat.h>
#include "tm_kstat_data.h"

#define MAXBUF 1024

static int add_string(char* dest, const void* s, char type, int len) {
  if (s && len) {
    memcpy(dest, &len, sizeof(int));
    *(dest+sizeof(int)) = type;
    memcpy(dest+sizeof(int)+1, s, len);
    return sizeof(int)+1+len;
  } else {
    memcpy(dest, 0, sizeof(int));
    return sizeof(int);
  }
}

static char* packet_standard_fields(const hrtime_t snap_time, const char* host, const char* zone,
			    const char* module, int instance, const char* name, const char* statistic,
			    int* len) {
  char* buf = malloc(512);
  int pos = *len;
  pos += add_string(buf+pos, &snap_time, 't', sizeof(hrtime_t));
  pos += add_string(buf+pos, host,       's', strlen(host));
  pos += add_string(buf+pos, zone,       's', strlen(host));
  pos += add_string(buf+pos, module,     's', strlen(module));
  pos += add_string(buf+pos, &instance,  'i', sizeof(int));
  pos += add_string(buf+pos, name,       's', strlen(name));
  pos += add_string(buf+pos, statistic,  's', strlen(statistic));
  *len = pos;
  return buf;
}

char* tm_kstat_data_packet(const hrtime_t snap_time, const char* host, const char* zone,
			   const char* module, int instance, const char* name,
			   const char* ks_name, const uint64_t value, int* len) {

  char* buf = packet_standard_fields(snap_time, host, zone, module,instance, name, ks_name, len);
  len += add_string(buf+*len, &value, 'L', sizeof(uint64_t));
  return buf;
}

char* tm_kstat_data_named_packet(const hrtime_t snap_time, const char* host, const char* zone,
			    const char* module, int instance, const char* name,
			    const uint8_t type, kstat_named_t* kstat, int* len) {

  char* buf = packet_standard_fields(snap_time, host, zone, module, instance, name, kstat->name, len);

  switch(type)
    {
    case KSTAT_TYPE_NAMED:
      switch(kstat->data_type)
	{
	case KSTAT_DATA_CHAR:
	  *len += add_string(buf+*len, kstat->value.c, 'c', 1);
	  break;
	case KSTAT_DATA_UINT64:
	  *len += add_string(buf+*len, &(kstat->value.ui64), 'L', sizeof(uint64_t));
	  break;
	case KSTAT_DATA_INT64:
	  *len += add_string(buf+*len, &(kstat->value.i64), 'l', sizeof(int64_t));
	  break;
	case KSTAT_DATA_UINT32:
	  *len += add_string(buf+*len, &(kstat->value.ui32), 'I', sizeof(uint32_t));
	  break;
	case KSTAT_DATA_INT32:
	  *len += add_string(buf+*len, &(kstat->value.i32), 'i', sizeof(int32_t));
	  break;
	}
      break;
    case KSTAT_TYPE_INTR:
      //  LOG(ERROR) << "unimplemented KSTAT_TYPE_INTR";
      break;
    case KSTAT_TYPE_IO:
      // LOG(ERROR) << "unimplemented KSTAT_TYPE_IO";
      break;
    case KSTAT_TYPE_TIMER:
      // LOG(ERROR) << "unimplemented KSTAT_TYPE_TIMER";
      break;
    default:
      // LOG(ERROR) << "unknown type " << type;
      break;
    }
  return buf;

}
