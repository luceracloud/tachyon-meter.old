#include <sys/kstat.h>
#include <glog/logging.h>
#include "tm_kstat_data.h"

std::string tm_kstat_data::generate_packet(const hrtime_t snap_time, const char* host,
			    const char* module, int instance, const char* name,
			    const char* ks_name, const uint64_t value, int& len) {
  
  PBMSG::Packet* pckt = new PBMSG::Packet;
  LOG(INFO) << host << ":" << module << ":" << instance << ":" << ks_name << ":" << name << ":" << value;

  pckt->set_time(snap_time);
  pckt->set_host(host);
  pckt->set_module(module);
  pckt->set_instance(instance);
  pckt->set_statistic(ks_name);
  pckt->set_name(name);
  pckt->set_value_uint64(value);

  std::string pkt_string;
  pckt->SerializeToString(&pkt_string);
  len = pkt_string.size();
  return pkt_string;

}

std::string tm_kstat_data::generate_packet(const hrtime_t snap_time, const char* host,
			    const char* module, int instance, const char* name,
			    const uint8_t type, kstat_named_t* kstat, int& len) {

  PBMSG::Packet* pckt = new PBMSG::Packet;
  pckt->set_time(snap_time);
  pckt->set_host(host);
  pckt->set_module(module);
  pckt->set_instance(instance);
  pckt->set_statistic(kstat->name);
  pckt->set_name(name);

  switch(type)
    {
    case KSTAT_TYPE_NAMED:
      pckt->set_statistic(kstat->name);
      switch(kstat->data_type)
	{
	case KSTAT_DATA_CHAR:
	  pckt->set_value(kstat->value.c);
	    LOG(INFO) << host << ":" << module << ":" << instance << ":" << name << ":" <<  kstat->name << ":" << kstat->value.c;
	  break;
	case KSTAT_DATA_UINT64:
	  pckt->set_value_uint64(kstat->value.ui64);
	  LOG(INFO) << host << ":" << module << ":" << instance << ":" << name << ":" <<  kstat->name << ":" << kstat->value.ui64;
	  break;
	case KSTAT_DATA_INT64:
	  pckt->set_value_int64(kstat->value.i64);
	  LOG(INFO) << host << ":" << module << ":" << instance << ":" << name << ":" <<  kstat->name << ":" << kstat->value.ui64;
	  break;
	case KSTAT_DATA_UINT32:
	  pckt->set_value_uint32(kstat->value.ui32);
	  LOG(INFO) << host << ":" << module << ":" << instance << ":" << name << ":" <<  kstat->name << ":" << kstat->value.ui32;
	  break;
	case KSTAT_DATA_INT32:
	  pckt->set_value_int32(kstat->value.i32);
	  LOG(INFO) << host << ":" << module << ":" << instance << ":" << name << ":" <<  kstat->name << ":" << kstat->value.ui32;
	  break;
	}
      break;
    case KSTAT_TYPE_INTR:
      LOG(ERROR) << "unimplemented KSTAT_TYPE_INTR";
      break;
    case KSTAT_TYPE_IO:
      LOG(ERROR) << "unimplemented KSTAT_TYPE_IO";
      break;
    case KSTAT_TYPE_TIMER:
      LOG(ERROR) << "unimplemented KSTAT_TYPE_TIMER";
      break;
    default:
      LOG(ERROR) << "unknown type " << type;
      break;
    }
  std::string pkt_string;
  pckt->SerializeToString(&pkt_string);
  len = pkt_string.size();
  return pkt_string;

}
