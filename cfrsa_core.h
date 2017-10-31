/************************************************************************* 
*   R Khera 5/3/17 
*  ANSI C wrapper code for 
*  for openSSL RSA functions 
*  including key gen, current supported 
*  FIPS methods are 
*  GenKey9.31, SigGen9.31, SigGenPKCS1.5,
*  SigGenPSS, SigVer9.31, SigVerPKCS1.5,
*  SigVerPSS (1024/1536/2048/3072/4096 with all
*  SHA sizes)
*  Tested wth openssl-1.0.1
*  This file is compiled into a shared object that should 
*  be placed in the caller's LD_LIBRARY_PATH (see 
*  Makefile for compiler versions, fipsld, ld.so / c flags etc.)
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

  /*
static char fips_label[] = "@(#)FIPS approved RAND"; 
  */

  
/* 
   Generates a RSA 4K key
   args: None
   RetVal : allocated EVP_PKEY * or NULL
   
*/

  EVP_PKEY * generate_key();

/* 
   Generates a self-signed x509 certificate. 
   args: EVP_PKEY *
   RetVal : allocated X509 * or NULL

*/

  X509 * generate_x509(EVP_PKEY * pkey);


/* 
   
   args: EVP_PKEY *, X509 *
   RetVal : CFRSA_ERR or CFRSA_SUCCESS

*/
    
  int write_to_disk(EVP_PKEY * pkey, X509 * x509);
  
/* 
   
   args:  X509 *
   RetVal : char* 

*/

  char *X509_to_PEM(X509 *cert);
  
#ifdef  __cplusplus
}
#endif

#endif                          /* !HEADER_CF_RSA_CORE_H */



