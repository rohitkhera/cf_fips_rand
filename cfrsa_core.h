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

#ifndef HEADER_CF_RSA_CORE_H
# define HEADER_CF_RSA_CORE_H  

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdio.h> 
#include <string.h>
#include <assert.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/pem.h>
#include <openssl/x509.h>

  /* ERROR CODES */

#undef CFRSA_SUCCESS
#define CFRSA_SUCCESS 0x01U

#undef  CFRSA_ERR
#define CFRSA_ERR 0x00U

#undef  CFRSA_PEMBUF_SZ 
#define CFRSA_PEMBUF_SZ 10000

  
  /*
static char fips_label[] = "@(#)FIPS approved RAND"; 
  */

  
/* 
   Generates a RSA 4K key
   args: None
   RetVal : allocated EVP_PKEY * or NULL
   
*/

  EVP_PKEY * cfrsa_generate_key();

/* 
   Generates a self-signed x509 certificate. 
   args: EVP_PKEY *
   RetVal : allocated X509 * or NULL

*/

  X509 * cfrsa_generate_x509(EVP_PKEY * pkey);


/* 
   
   args: EVP_PKEY *, X509 *
   RetVal : CFRSA_ERR or CFRSA_SUCCESS

*/
    
  int cfrsa_write_to_disk(EVP_PKEY * pkey, X509 * x509);
  
/* 
   deprecated: for testing only 
   args:  X509 *
   RetVal : char* (x509 PEM string )

*/

  
  char *cfrsa_X509_to_PEM2(X509 *cert);

/* 
   args:  X509 *, char* pembuffer
   RetVal : numbytes written to pembuffer

*/


  int cfrsa_X509_to_PEM(X509 *cert, char* pembuf);

  
/* 
   Deprecated, for testing only
   args:  none
   RetVal : 

*/

  char* cfrsa_certgen2();

  
/* 

   args:  char buffer for x509 PEM of size CFRSA_PEMBUF_SZ
          char buffer for priv key PEM of size CFRSA_PEMBUF_SZ
   RetVal : number of characters written to keybuf
            pass in an int array that returns num bytes wrrite
            to both keybuf and pembuf

*/

  int cfrsa_cert_key_gen(char *pembuf, char* keybuf);


/* 

   args:  EVP_PKEY* private key pointer
          char* - pem write buffer
   RetVal : number of characters written

*/
  

  int cfrsa_privkey_to_PEM(EVP_PKEY* key, char* keybuf);

#ifdef  __cplusplus
}
#endif

#endif                          /* !HEADER_CF_RSA_CORE_H */



