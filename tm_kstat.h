// from #: kstat -h

// Usage:
// kstat [ -Cjlpq ] [ -T d|u ] [ -c class ]
//      [ -m module ] [ -i instance ] [ -n name ] [ -s statistic ]

// ------  second line above is modeled in this class ------
// instance is called zone for better clarity

// config is done by passing in lines of form
// module:instance:name:statistic which conforms to the -p output of kstat

// so kstat -p -m link -s obytes64
// is equivalent to the following config line
// link::obytes64:

#ifndef _tm_kstat_h_
#define _tm_kstat_h_

#include <kstat.h>
#include <string>
#include <vector>
#include <ostream>
#include <iostream>
#include <string.h>
#include <glog/logging.h>
#include "pckt.pb.h"
#include "tm_curl.h"

class tm_curl;
class tm_kstat {
 private:
  static tm_curl* _curl;
  static kstat_ctl_t* _kc;        // Pointer to kstat chain
  
  static std::string _hostname;
  std::string _module;
  std::string _zone;
  std::string _name;
  std::string _statistic;
  
  static int _errno;
  
  static char* _c(const std::string& s) { return (char*)s.c_str(); }

  static kstat_t* lookup_m(const tm_kstat& k) {
    kstat_t* t = kstat_lookup (_kc, _c(k._module), -1, NULL);
    return t;
  }
  static kstat_t* lookup_n(const tm_kstat& k) {
    kstat_t* t = kstat_lookup (_kc, NULL, -1, _c(k._name));
    return t;
  }
  static kstat_t* lookup_mn(const tm_kstat& k) {
    kstat_t* t = kstat_lookup (_kc, _c(k._module), -1, _c(k._name));
    return t;
  }
  static kstat_t* lookup_mns(const tm_kstat& k) {
    kstat_t* t = kstat_lookup (_kc, _c(k._module), -1, _c(k._name));
    return t;
  }
  static kstat_t* lookup_mnz(const tm_kstat& k) {
    kstat_t* t = kstat_lookup (_kc, _c(k._module), atoi(_c(k._zone)), _c(k._name));
    return t;
  }
  tm_kstat io(const std::string& k, long v);

 public:

  tm_kstat(const char* nsq_url, const std::string& config) {
    if (!_kc) {
      init(nsq_url);
    }
    parse(config, ':');
  }

 tm_kstat(const tm_kstat& k) : _module(k._module), _zone(k._zone), _name(k._name), _statistic(k._statistic) {
  }

  tm_kstat& operator=(const tm_kstat& k) {
    _module = k._module;
    _zone = k._zone;
    _name = k._name;
    _statistic = k._statistic;
    return *this;
  }

  void parse(const std::string& config, char delim);
  kstat_t* lookup();

  void send_value();
  void send_value_io(kstat_t *kp, const kstat_io_t* io);
  void send_io(kstat_t* kp, const char* name, const uint64_t value);

  std::string valueField(long l);
  std::string valueField(const kstat_named_t& f);


  static void init(const char* nsq_url);
  static void update();
  static void perror(int e) {
    if (e == 2) {
      LOG(ERROR) << "errno " << 2 << " maybe requesting invalid disk?";
    } else {
      LOG(ERROR) << "errno " << e;
    }
    LOG(ERROR) << strerror(e);
  }
  
  friend std::ostream& operator<< (std::ostream& os, const tm_kstat& k);
};

inline std::ostream& operator<< (std::ostream& os, const tm_kstat& k) {
  os << k._zone << " " << k._module << " " << k._name << " " << k._statistic;
  return os;
}

#endif
