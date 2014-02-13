#ifndef _tm_curl_h_
#define _tm_curl_h_

// wrapper for curl easy.  not technically necessary but it gives the code a 
// consistant feel.

#include <curl/curl.h>
#include <curl/easy.h>
#include <sys/kstat.h>
#include <string>
#include "tm_kstat.h"

class tm_kstat;
class tm_curl {

 private:
  CURL* _curl;
  std::string _url;
  bool _good;
  struct curl_slist *_headers=NULL;

 public:
  tm_curl(const char* url): _url(url) {
    // just open the connection.
    init();
    _headers = curl_slist_append(_headers, "Content-Type: text/xml");

  }
  
  ~tm_curl() {
    curl_slist_free_all(_headers);
    curl_easy_cleanup(_curl);
  }

  void init();
  bool good() { return _good; }

  void set_opts(const char* data, int len);
  int post(const hrtime_t snap_time, const char* host,
	   const char* module, int instance, const char* name,
	   const char* ks_name, const uint64_t value);
  int post(const hrtime_t snap_time, const char* host,
	   const char* module, int instance, const char* name,
	   const uint8_t type, kstat_named_t* kstat);

  int curl_verbose(bool on) {
    curl_easy_setopt(_curl, CURLOPT_VERBOSE, on);
  }

  int status() {
    // check if the http is still open
    return 0;
  }

};
#endif
