/*
 *  Some simple definitions
 *  for useful utilities.
 *
 *  CREATED:  7 AUG 2013
 *  EDITED:   7 AUG 2013
 */

#ifndef _tm_util_h_
#define _tm_util_h_

#include <iostream>

namespace UTIL {

  /* Colors */
  inline void black () {
    std::cout << "\033[30m";
  }
  inline void red () {
    std::cout << "\033[31m";
  }
  inline void green () {
    std::cout << "\033[32m";
  }
  inline void blue () {
    std::cout << "\033[34m";
  }
  inline void purple () {
    std::cout << "\033[35m";
  }
  inline void cyan () {
    std::cout << "\033[36m";
  }
  inline void yellow () {
    std::cout << "\033[33m";
  }
  inline void white () {
    std::cout << "\033[37m";
  }
  inline void clear () {
    std::cout << "\033[m";
  }

}

#endif
