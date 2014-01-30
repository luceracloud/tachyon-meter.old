/*
 *  dtrace.hpp
 *
 *   Includes a nice library of functions
 *   that allow easy interface with libdtrace
 *   from a C++ program.
 *
 *   CREATED:   8 AUG 2013
 *   EDITED:    8 AUG 2013
 */

#ifndef _tm_dtrace_h_
#define _tm_dtrace_h_

#define EX32(addr) *((uint32_t *)addr)

#include <dtrace.h>

extern bool VERBOSE;

namespace DTRACE {

  // Debugging function, prints
  // something from the libdtrace
  // aggreate in the form it was
  // originally written.
  int formatted_print (const dtrace_recdesc_t *rec, caddr_t addr);

  /* Initialize dtrace program (passed by handle and string) */
  int init (dtrace_hdl_t **this_dtp, std::string this_prog);

  /* For dtrace "command line" */
  int setopts (dtrace_hdl_t **this_g_dtp);

  /*
   * Given a bucket id, return the max
   * value. The min of the value is
   * simply (max/2)+1.
   */
  inline int64_t max_quantize_range (int bckt_id) {
    if (bckt_id < DTRACE_QUANTIZE_ZEROBUCKET) return -1;
    else if (bckt_id == DTRACE_QUANTIZE_ZEROBUCKET) return 0;
    else return DTRACE_QUANTIZE_BUCKETVAL (bckt_id);
  }

  /*
   * Return values from DTrace aggregates
   */
  int aggwalk (const dtrace_aggdata_t *agg, void *arg);

}
#endif
