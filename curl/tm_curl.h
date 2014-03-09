#ifndef _tm_curl_h_
#define _tm_curl_h_

#include <sys/kstat.h>

void tm_curl_global_init();
void tm_curl_set_url(const char* url);
void tm_curl_done();
int curl_verbose(int on);

int tm_curl_post(const char* data, int len);

#endif
