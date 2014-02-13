#include "tm_curl.h"
#include "tm_kstat.h"
#include "tm_kstat_data.h"
#include <sys/kstat.h>


void tm_curl::init() {
  _curl = curl_easy_init();
  if (_curl == NULL) {
    LOG(ERROR) << "Error opening connection with nsq";
    LOG(INFO) << "  will keep trying";
  } else {
    _good = true;
    //    curl_easy_setopt(_curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(_curl, CURLOPT_URL, _url.c_str());
    LOG(INFO) << "Set post URL=" << _url;
  }
}

static size_t resp_cb ( char *ptr, size_t size, size_t nmemb, char *data ) {
  return size * nmemb;
}

void tm_curl::set_opts(const char* data, int len) {
  curl_easy_setopt(_curl, CURLOPT_POST, 1);
  curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, 1);
  curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, resp_cb);
  curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, data);
  curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE, len);
}

int tm_curl::post(const hrtime_t snap_time, const char* host,
		  const char* module, int instance, const char* name,
		  const char* ks_name, const uint64_t value) {
  
  int rc = 0;
  size_t sent = 0;
  int len;
  std::string data = tm_kstat_data::generate_packet
    (snap_time, host, module, instance, name, ks_name, value, len);

  set_opts(data.c_str(), len);
  curl_easy_perform(_curl);

  if (rc != CURLE_OK) {
    LOG(ERROR) << "Data not sent " << rc;
  }
  return rc;
}

int tm_curl::post(const hrtime_t snap_time, const char* host,
		  const char* module, int instance, const char* name,
		  const uint8_t type, kstat_named_t* kstat) {
  int rc = 0;
  size_t sent = 0;
  int len;
  std::string data = tm_kstat_data::generate_packet
    (snap_time, host, module, instance, name, type, kstat, len);

  set_opts(data.c_str(), len);
  curl_easy_perform(_curl);

  if (rc != CURLE_OK) {
    LOG(ERROR) << "Data not sent " << rc;
  }
  return rc;
}
