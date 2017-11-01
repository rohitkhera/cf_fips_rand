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

#include "cfprng_fips_rand.h"

/* 
   args: 
   1) unsigned char* - unsigned char buffer to hold the seed
   2) int - seed length (this should be equal to the length 
      of the buffer
      Currently not implemented
*/

int cfprng_explicit_seed(unsigned char* buf, int len) {

  return CFPRNG_ERR;

}


void cfopenssl_log_info(char* filename, int linenum, char *msg) {
#ifdef CFOPENSSL_LOG_LEVEL_INFO
  fprintf(stderr,"%s : %d: %s\n", filename, linenum, msg);    
#endif
}

void cfopenssl_log_err(char* filename, int linenum, char *msg) {
#ifdef CFOPENSSL_LOG_LEVEL_ERR
  fprintf(stderr,"%s : %d: %s\n", filename, linenum, msg);    
#endif
}


void cfprng_pretty_print(unsigned char* buf, int len) {


  int i = 0;
  cfopenssl_log_info( __FILE__, __LINE__, "");
  for(i = 0 ; i < len ; i++) {

    fprintf(stderr,"%.02X ", buf[i]);
  }

}


