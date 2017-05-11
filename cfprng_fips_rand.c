/*************************************************************************
*    R Khera 5/3/17 
*   ANSI C wrapper code for 
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
*   Tested wth openssl-fips-2.0.11 and openssl-1.0.1r
*   This file is compiled into a shared object that should 
*   be placed in the caller's LD_LIBRARY_PATH (see 
*  Makefile for compiler versions, fipsld, ld.so / c flags etc.
*************************************************************************/    

/* generic header for cf_nist_rand and cf_fips_rand functions */

#include "cfprng_fips_rand.h"

int cfprng_fips_rand(unsigned char* buf, int len)
{ 

  const RAND_METHOD* rm = RAND_get_rand_method();

  if(len > CFPRNG_MAX_RAND_BYTES) {
#ifdef CFPRNG_LOG_LEVEL_ERR
    fprintf(stderr, "%s: %d : length exceeds %d", __FILE__, __LINE__, CFPRNG_MAX_RAND_BYTES );
#endif
    return CFPRNG_ERR;
  }

  /* Enter FIPS mode */
  if(FIPS_mode_set(1)) { 
#ifdef CFPRNG_LOG_LEVEL_INFO
    fprintf(stderr,"FIPS mode enabled\n"); 
#endif
  } 
  else { 
#ifdef CFPRNG_LOG_LEVEL_INFO
    ERR_load_crypto_strings(); 
    ERR_print_errors_fp(stderr); 
#endif
    return CFPRNG_ERR;
  } 

  /* phase this out  since by default RAND_bytes() 
   * should seed from the LRNG entropy pools */  

  int rc = RAND_load_file("/dev/urandom", 32);
  if(rc != 32) {
#ifdef CFPRNG_LOG_LEVEL_ERR
    fprintf(stderr, "%s: %d :could not open /dev/urandom\n", __FILE__, __LINE__ );
#endif
    return CFPRNG_ERR;
  }

  if (!RAND_bytes(buf, len)) {
#ifdef CFPRNG_LOG_LEVEL_ERR    
    fprintf(stderr, "%s: %d : RAND_bytes() fail\n", __FILE__, __LINE__ );
#endif
    return CFPRNG_ERR;
  }
  else 
    return CFPRNG_SUCCESS;

}



