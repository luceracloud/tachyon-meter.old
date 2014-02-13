#include <stdio.h>
#include <kstat.h>
#include <fcntl.h>
#include <stdlib.h>
// shamelessly pilfered from http://people.cs.clemson.edu/~wayne/cpsc824/samples/kstat_demo.c

/* Fetch numerical statistic from kernel */
long getKStatNumber(kstat_ctl_t *kernelDesc, char *moduleName, 
		    char *recordName, char *fieldName) {
  kstat_t *kstatRecordPtr;
  kstat_named_t *kstatFields;
  long value;
  int i;
       
  if ((kstatRecordPtr = kstat_lookup(kernelDesc, moduleName, -1, recordName)) ==
      NULL) {
    return(-1);
  }

  if (kstat_read(kernelDesc, kstatRecordPtr, NULL) < 0)
    return(-1);

  kstatFields = KSTAT_NAMED_PTR(kstatRecordPtr);

  for (i=0; i<kstatRecordPtr->ks_ndata; i++) {
    if (strcmp(kstatFields[i].name, fieldName) == 0) {
      switch(kstatFields[i].data_type) {
      case KSTAT_DATA_INT32:
	value = kstatFields[i].value.i32;
	break;
      case KSTAT_DATA_UINT32:
	value = kstatFields[i].value.ui32;
	break;
      case KSTAT_DATA_INT64:
	value = kstatFields[i].value.i64;
	break;
      case KSTAT_DATA_UINT64:
	value = kstatFields[i].value.ui64;
	break;
      default:
	value = -1;
      }
      return(value);
    }
  }
  return(-1);
}

/* Fetch string statistic from kernel */
char *getKStatString(kstat_ctl_t *kernelDesc, char *moduleName, 
		     char *recordName, char *fieldName) {
  kstat_t *kstatRecordPtr;
  kstat_named_t *kstatFields;
  char *value;
  int i;
       
  if ((kstatRecordPtr = kstat_lookup(kernelDesc, moduleName, -1, recordName)) ==
      NULL) {
    return(NULL);
  }

  if (kstat_read(kernelDesc, kstatRecordPtr, NULL) < 0)
    return(NULL);

  kstatFields = KSTAT_NAMED_PTR(kstatRecordPtr);

  for (i=0; i<kstatRecordPtr->ks_ndata; i++) {
    if (strcmp(kstatFields[i].name, fieldName) == 0) {
      switch(kstatFields[i].data_type) {
      case KSTAT_DATA_CHAR:
	value = kstatFields[i].value.c;
	break;
      default:
	value = NULL;
      }
      return(value);
    }
  }
  return(NULL);
}


/* Test main program */
main() {
  kstat_ctl_t *kernelDesc;
  long value;
  int loops;

  /* Open the kernel statistics device */
  if ((kernelDesc = kstat_open()) == NULL) {
    perror("kstat_open");
    exit (1);
  }

  /* Fetch CPU info */
  printf("CPU Type=%s\n", 
	 getKStatString(kernelDesc, "cpu_info", "cpu_info0", "cpu_type"));
  
  value = getKStatNumber(kernelDesc, "cpu_info", "cpu_info0", "clock_MHz");
  printf("CPU Speed=%ld\n", value);

  value = getKStatNumber(kernelDesc, "unix", "system_misc", "ncpus");
  printf("Number of cpus=%ld\n", value);

  /* Loop for 10 times, display load averages and memory */
  for (loops=0; loops < 10; loops++) {
    /* Fetch the current load averages */
    value = getKStatNumber(kernelDesc, "unix", "system_misc", "avenrun_1min");
    printf("1 minute load average=%ld (%5.2f)\n", value, value/256.0);
  
    value = getKStatNumber(kernelDesc, "unix", "system_misc", "avenrun_5min");
    printf("5 minute load average=%ld (%5.2f)\n", value, value/256.0);
  
    value = getKStatNumber(kernelDesc, "unix", "system_misc", "avenrun_15min");
    printf("15 minute load average=%ld (%5.2f)\n", value, value/256.0);
  
    /* Fetch information on physical memory usage */
    value = getKStatNumber(kernelDesc, "unix", "system_pages", "physmem");
    printf("Total physical memory: pages=%ld (bytes=%ld)\n", 
      value, value*8192);
  
    value = getKStatNumber(kernelDesc, "unix", "system_pages", "freemem");
    printf("Free physical memory: pages=%ld (bytes=%ld)\n", value, value*8192);
    
    sleep(2);
  }
}
