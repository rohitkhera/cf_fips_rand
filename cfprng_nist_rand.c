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


void cfprng_nist_rand()
{ 


  const RAND_METHOD* rm = RAND_get_rand_method();

  unsigned char key[16], iv[16];



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



