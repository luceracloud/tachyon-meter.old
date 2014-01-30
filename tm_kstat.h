/*
 *  kstat.hpp
 *
 *   Includes a nice library of functions
 *   that allow easy interface with libkstat
 *   from a C++ program.
 *
 *   CREATED:   5 AUG 2013
 *   EDITED:    8 AUG 2013
 */

#ifndef _tm_kstat_h_
#define _tm_kstat_h_

#include <kstat.h>
#include <string>
#include <vector>

namespace KSTAT {

  /*
   * Useful function to translate from the
   * ambiguous kstat data form to a uint64_t
   * no matter what (or return 0).
   */
  uint64_t translate_to_ui64 (kstat_named_t *knp);

  /*
   * Returns a single statistic value
   * from kstat.
   */
  int retrieve_kstat (kstat_ctl_t *kc,
		      const std::string& module,
                      const std::string& name,
		      const std::string& statistic,
		      int instance,
                      uint64_t& value);
  /*
   * Allows the return of multiple values
   * in a vector. Works with "named" kstat
   * type.
   */
  int retrieve_multiple_kstat (kstat_ctl_t *kc,
			       const std::string& module,
                               const std::string& statistic,
			       std::vector<uint64_t>& values,
                               std::vector<std::string>& names,
			       std::vector<std::string>& zones);

}
#endif
