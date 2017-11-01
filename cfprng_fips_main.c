/************************************************************************
*    R Khera 10/31/2017
*   ANSI C wrapper code for 
*   (1)  
*   generating pseudo random  bytes 
*   Using NIST SP 800-90A algos with the OpenSSL FIPS module 
*   The goal in CF is to only use 
*   a PRNG specified in NIST SP 800-90A 
*   Enabling OpenSSL FIPS mode only allows algorithms 
*   specified in SP 800-90A and ANS X9.31
*   However, we would prefer to just use algorithms 
*   from SP800-90A over ANS X9.31
*   Among the SP800-90A specified algorithms, we 
*   must avoid "Dual_EC_DRBG". 
*   (2)
*   RSA 4K key gen and cert gen 
*   (3) 
*   AES GCM 128
*   Tested wth openssl-fips-2.0.11 and openssl-1.0.1r
*   This file is compiled into a shared object that should 
*   be placed in the caller's LD_LIBRARY_PATH 
*  Apple LLVM version 8.1.0 (clang-802.0.42)
*  Target: x86_64-apple-darwin16.5.0
* Thread model: posix
* Also tested with gcc version 4.8.2 (Ubuntu 4.8.2-19ubuntu1) 
*  Also see Makefile for more on compiler versions, 
*  fipsld, ld.so / c flags etc.
*
*************************************************************************/    



/* Test program for cf_fips_rand() 


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
    cfopenssl_log_err(__FILE__,__LINE,"cfprng_fips_rand_fail()");
    retVal=CFPRNG_ERR;
    exit(retVal);
  }

  if(cfprng_fips_tests()==CFPRNG_ERR) {

    cfopenssl_log_err(__FILE__,__LINE__"Failed simple FIPS statistical tests");
    retVal=CFPRNG_ERR;
  }

  else

    cfopenssl_log_info(__FILE__,__LINE__,"Passed simple FIPS statistical tests");
#endif

    exit(retVal);
}
