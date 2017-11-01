
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
#include "cfrsa_core.h"


int main(int argc, char ** argv)
{
    /* Generate the key. */
   cfopenssl_log_info(__FILE__,__LINE__,"Generating RSA 4K key...");

    
    EVP_PKEY * pkey = cfrsa_generate_key();
    if(!pkey)
      return 1;
    
    /* Generate the certificate. */
    cfopenssl_log_info(__FILE__,__LINE__,"Generating x509 certificate...");
    X509 * x509 = cfrsa_generate_x509(pkey);
    if(!x509)
    {
        EVP_PKEY_free(pkey);
        return 1;
    }
    
    /* Write the private key and certificate out to disk. */
    cfopenssl_log_info(__FILE__,__LINE__,"Writing key and certificate to disk...");    
    
    int ret = cfrsa_write_to_disk(pkey, x509);


    char* pembuf = (char*) malloc(CFRSA_PEMBUF_SZ);

    //char* cert_str = cfrsa_X509_to_PEM2(x509);
    int numbytes = cfrsa_X509_to_PEM(x509,pembuf);
    
    cfopenssl_log_info(__FILE__,__LINE__,pembuf);
    EVP_PKEY_free(pkey);
    X509_free(x509);
    
    if(ret)
    {
          cfopenssl_log_info(__FILE__,__LINE__,"Success");    
      return 0;
    }
    else
      return 1;
}
