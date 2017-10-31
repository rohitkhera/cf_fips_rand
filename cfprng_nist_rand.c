/*******************************************************
*   R Khera 5/3/17 
*   ANSI C wrapper code for 
*   generating pseudo random  bytes in 
*   OpenSSL with NIST algos.  The goal in CF is to use 
*   a PRNG specified in NIST SP 800-90A(r1). 
*   There is an option to enable 
*   OpenSSL FIPS mode and this only allows algorithms 
*   specified in SP 800-90A and ANS X9.31
*   However, we would prefer to just use algorithms 
*   from SP800-90A over ANS X9.31
*   Among the SP800-90A specified algorithms, we 
*   must avoid "Dual_EC_DRBG". 
*   Tested wth openssl-1.0.1r
*   This unit is compiled into a shared object that should 
*   be placed in the caller's LD_LIBRARY_PATH (see 
*   Makefile for compiler versions and and ld / c flags etc
********************************************************/   
  
 /* generic header for cf_nist_rand and cf_fips_rand functions */

#include "cfprng_fips_rand.h"

int cfprng_nist_rand(unsigned char* buf, int len)
{ 

  if(len > CFPRNG_MAX_RAND_BYTES) {
    cfopenssl_log_err(__FILE__,__LINE__,"length exceeds CFPRNG_MAX_RAND_BYTES");
    return CFPRNG_ERR;
  }

  /* phase this out  since by default RAND_bytes() 
   * should seed from the LRNG entropy pools */

  int rc = RAND_load_file("/dev/urandom", 32);
  if(rc != 32) {
    cfopenssl_log_err(__FILE__,__LINE__,"could not open /dev/urandom");    
    return CFPRNG_ERR;
  }

#ifdef CFOPENSSL_LOG_LEVEL_INFO
    cfprng_pretty_print(buf,len);
#endif

  if (!RAND_bytes(buf, len)) {
    cfopenssl_log_err(__FILE__,__LINE__,"RAND_bytes() fail");        
    return CFPRNG_ERR;
  }
  else {
#ifdef CFOPENSSL_LOG_LEVEL_INFO
    cfprng_pretty_print(buf,len);
#endif
    return CFPRNG_SUCCESS;
  }

}



