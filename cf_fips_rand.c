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

#include "cf_fips_rand.h"


void cf_fips_rand()
{ 


  const RAND_METHOD* rm = RAND_get_rand_method();

  unsigned char key[16], iv[16];


  /* Enter FIPS mode by default */ 
  if(FIPS_mode_set(1)) { 
    fputs("FIPS mode enabled\n",stderr); 
  } 
  else { 
    ERR_load_crypto_strings(); 
    ERR_print_errors_fp(stderr); 
    exit(1); 
  } 
  

  /*
  int rc = RAND_load_file("/dev/urandom", 32);
  if(rc != 32) {
    printf("could not open /dev/random\n");
  }
  else {
    printf("Opened /dev/urandom\n");
  }
 */

  if (!RAND_bytes(key, sizeof key)) {
    fputs("RAND bytes failed\n",stderr);
  }
  if (!RAND_bytes(iv, sizeof iv)) {
    fputs("RAND bytes failed\n",stderr);

  }

}



