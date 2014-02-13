#include <vector>
#include <fstream>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <glog/logging.h>
#include "tm_kstat.h"

bool beginswith(const std::string s, char c) {
  bool rc = false;
  if (s[0] == c)
    rc = true;
  return rc;
}

int main(int argc, char* argv[]) {
  std::string line;
  std::vector<tm_kstat> vec;

  google::InitGoogleLogging(argv[0]);

  if (argc != 2) {
    LOG(ERROR) << "Takes one arg url for nsq, example http://127.0.0.1:4151/";
    exit(1);
  }

  std::ifstream is("kstat.conf", std::ifstream::in);
  if (is.fail()) {
    LOG(ERROR) << "istream failed to open kstat.conf";
  }
  try {
    tm_kstat::init(argv[1]);
    while (std::getline(is, line)) {
      LOG(INFO) << line;
      if (!beginswith(line, '#')) {
	tm_kstat k(argv[2], line);
	vec.push_back(k);
      }
      line.clear();
    }
  } catch (int e) {
    tm_kstat::perror(e);
  }

  while (1) {
    tm_kstat::update();
    for (auto v : vec) {
      try {
	v.send_value();
      } catch (int e) {
	tm_kstat::perror(e);
      }
    }
    usleep(1000000);
  }

  return 0;
}
