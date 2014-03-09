#include "tm_curl.h"
#include "tm_kstat_data.h"
#include <sys/kstat.h>
#include <curl/curl.h>
#include <curl/easy.h>

static CURL* _curl = NULL;
//static struct curl_slist *_headers = NULL;
static void tm_curl_init(const char* url);

void tm_curl_global_init() {
	curl_global_init(CURL_GLOBAL_ALL);
}
void tm_curl_set_url(const char* url) {
  tm_curl_init(url);
  // _headers = curl_slist_append(_headers, "Content-Type: text/xml");
}

void tm_curl_done() {
 // curl_slist_free_all(_headers);
 	curl_easy_cleanup(_curl);
}

int curl_verbose(int on) {
  curl_easy_setopt(_curl, CURLOPT_VERBOSE, on);
}

static void tm_curl_init(const char* url) {
  _curl = curl_easy_init();
  if (_curl == NULL) {
    fprintf(stderr, "Error opening connection with nsq");
  } else {
    curl_easy_setopt(_curl, CURLOPT_URL, url);
  }
}

static size_t resp_cb ( char *ptr, size_t size, size_t nmemb, char *data ) {
  return size * nmemb;
}

static void tm_curl_set_opts(const char* data, int len) {
  curl_easy_setopt(_curl, CURLOPT_POST, 1);
  curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, 1);
  curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, resp_cb);
  curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, data);
  curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE, len);
}

int tm_curl_post(const char* data, int len) {
  
  int rc = 0;
  size_t sent = 0;

  tm_curl_set_opts(data, len);
  rc = curl_easy_perform(_curl);
  //curl_easy_reset(_curl);

  if (rc != CURLE_OK) {
    fprintf(stderr, "Curl failed %d", rc);
  }
  return rc;
}
