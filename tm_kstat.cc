#include <sstream>
#include <unistd.h>
#include <netdb.h>
#include "tm_kstat.h"
#include "pckt.pb.h"
#include "tm_curl.h"

int tm_kstat::_errno = 0;
tm_curl* tm_kstat::_curl = 0;
kstat_ctl_t* tm_kstat::_kc = 0;
std::string tm_kstat::_hostname;

void tm_kstat::init(const char* nsq_url) {

  char hostname[NI_MAXHOST];
  gethostname(hostname, NI_MAXHOST);
  _hostname = hostname;
  
  _kc = kstat_open();

  if (_kc != 0 && (_errno != 0 && _errno != 12)) {  // 12 is nomem and is expected behaviour
    _errno = errno;
    throw _errno;
  }

  _curl = new tm_curl(nsq_url);
}

void tm_kstat::parse(const std::string& config, char delim) {

  std::vector<std::string> el;
  int mark = 0, point;

  while ((point = config.find(delim, mark)) != std::string::npos) {
    el.push_back(config.substr(mark, point - mark));
    mark = point+1;
  }
  el.push_back(config.substr(mark, config.size() - mark));

  if (el.size() != 4) {
    std::cerr << "misconfigured " << config << " should have four elements" << std::endl;
  } else {
    _module = el[0];
    _zone = el[1];
    _name = el[2];
    _statistic = el[3];
  }
}

kstat_t* tm_kstat::lookup() {
  kstat_t* ksp = 0;
  errno = 0;
  int stat = 0;
  if (!_module.empty())
    stat |= 1;
  if (!_name.empty())
    stat |= 2;
  if (!_statistic.empty())
    stat |= 4;
  if (!_zone.empty())
    stat |= 8;

  if (stat == 1 || stat == 5)
    ksp = lookup_m(*this);
  else if (stat == 2)
    ksp = lookup_n(*this);
  else if (stat == 3)
    ksp = lookup_mn(*this);
  else if (stat == 7)
    ksp = lookup_mns(*this);
  else if (stat == 11)
    ksp = lookup_mnz(*this);
  else
    std::cout << "Can't handle combo of inputs: "
	      << _module << ":" << _zone << ":" << _name << ":" << _statistic
	      << std::endl;
  
  if (ksp == NULL) {
    _errno = errno;
    throw _errno;
  }
  return ksp;
}

void tm_kstat::update() {
  kid_t kc = kstat_chain_update(_kc);
  if (kc == -1) {
    _errno = errno;
    throw _errno;
  }
}

/* Fetch statistic from kernel, send */
void tm_kstat::send_value() {
  kstat_t *kp;

  while (!_curl->good()) {
    _curl->init();
    usleep(5000000);
  }
  if ((kp = lookup()) != NULL) {
    if (kstat_read(_kc, kp, NULL) >= 0) {

      kstat_named_t *kstatFields = KSTAT_NAMED_PTR(kp);
      if (kp->ks_type ==  KSTAT_TYPE_NAMED) {
	  for (int i=0; i<kp->ks_ndata; i++) {
	    if (kstatFields[i].name == _statistic) {
	      if (_curl->good()) {
		_curl->post(kp->ks_snaptime, _hostname.c_str(),
			    kp->ks_module, kp->ks_instance, kp->ks_name,
			    kp->ks_type,  &kstatFields[i]);
	      }
	    }
	  }
      } else if (kp->ks_type == KSTAT_TYPE_IO) {
	do {
	  for (int i=0; i<kp->ks_ndata; i++) {
	    send_value_io(kp, KSTAT_IO_PTR(kp));
	  }
	} while ((kp->ks_next->ks_type == KSTAT_TYPE_IO &&
		  (kp = kp->ks_next)));
      } else {
	std::cout << "Unsupported ks_type = " << kp->ks_type << std::endl;
      }
    }
  }
}

void tm_kstat::send_io(kstat_t* kp, const char* name, const uint64_t value) {
  if (_curl->good()) {
    _curl->post(kp->ks_snaptime, _hostname.c_str(),
		kp->ks_module, kp->ks_instance, name, kp->ks_name, value);
  }
}

void tm_kstat::send_value_io(kstat_t *kp, const kstat_io_t* io) {

  send_io(kp, "nread", io->nread);
  send_io(kp, "nwritten", io->nwritten);
  send_io(kp, "reads", io->reads);
  send_io(kp, "writes", io->writes);
  send_io(kp, "wtime", io->wtime);
  send_io(kp, "wlentime", io->wlentime);
  send_io(kp, "wlastupdate", io->wlastupdate);
  send_io(kp, "rtime", io->rtime);
  send_io(kp, "rlentime", io->rlentime);
  send_io(kp, "rlastupdate", io->rlastupdate);
  send_io(kp, "wcnt", io->wcnt);
  send_io(kp, "rcnt", io->rcnt);

}

std::string tm_kstat::valueField(long l) {
  std::ostringstream value;
  value << l;
  return value.str();
}

std::string tm_kstat::valueField(const kstat_named_t& f) {
  std::ostringstream value;
  switch (f.data_type)
    {
    case KSTAT_DATA_CHAR:
      value << f.value.c;
      break;
    case KSTAT_DATA_INT32:
      value << f.value.i32;
      break;
    case KSTAT_DATA_UINT32:
      value << f.value.ui32;
      break;
    case KSTAT_DATA_INT64:
      value << f.value.i64;
      break;
    case KSTAT_DATA_UINT64:
      value << f.value.ui64;
      break;
    default:
      value << -1;
    }
  return value.str();
}
