/* Test program for cf_fips_rand() */

#include "cfprng_fips_rand.h"

int main(int argc, char *argv[]) 
{ 
  char *opt = NULL; 
  int verbose = 0; 
  int fipsmode = 1; 
  int retVal=CFPRNG_SUCCESS;
  unsigned char buf[CFPRNG_MAX_RAND_BYTES];
  int i; 

  /* Process command line arguments */ 
  i = 0; 
  while(++i < argc) { 
    opt = argv[i]; 
    if (!strcmp(opt,"­v")) verbose = 1; 
    else if (!strcmp(opt,"­c")) fipsmode = 0; 
    else if (strcmp(opt,"-")) { 
      printf("Usage: %s \n", argv[0]); 
      puts("Options (defunct):"); 
      puts("\t­c\tUse non­FIPS mode"); 
      puts("\t­v\tVerbose output"); 
      exit(1); 
    } 
    else break; 
  }
  
  if(cfprng_fips_rand(buf, CFPRNG_MAX_RAND_BYTES)==CFPRNG_ERR) {
#ifdef CFOPENSSL_LOG_LEVEL_ERR    
    fprintf(stderr, "%s : ln %d : cfprng_fips_rand_fail()\n", __FILE__, __LINE__);
#endif
    retVal=CFPRNG_ERR;
    exit(retVal);
  }

  if(cfprng_fips_tests()==CFPRNG_ERR) {
#ifdef CFOPENSSL_LOG_LEVEL_ERR    
    fprintf(stderr, "%s : ln %d : Failed simple FIPS statistical tests\n", __FILE__, __LINE__);
    retVal=CFPRNG_ERR;
  }
#endif
  else
#ifdef CFOPENSSL_LOG_LEVEL_INFO
    fprintf(stderr, "%s : ln %d : Passed simple FIPS statistical tests\n", __FILE__, __LINE__);
#endif

    exit(retVal);
}
