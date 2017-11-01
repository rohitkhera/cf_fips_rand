/***********************************************************************
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

/* deprecated */
char* cfrsa_X509_to_PEM2(X509 *cert) {

    BIO *bio = NULL;
    char *pem = NULL;

    if (NULL == cert) {
        return NULL;
    }

    bio = BIO_new(BIO_s_mem());
    if (NULL == bio) {
        return NULL;
    }

    if (0 == PEM_write_bio_X509(bio, cert)) {
        BIO_free(bio);
        return NULL;
    }

    pem = (char *) malloc(bio->num_write + 1);
    if (NULL == pem) {
        BIO_free(bio);
        return NULL;    
    }

    memset(pem, 0, bio->num_write + 1);
    BIO_read(bio, pem, bio->num_write);
    BIO_free(bio);
    return pem;
}

int cfrsa_X509_to_PEM(X509 *cert, char* pembuf) {

    BIO *bio = NULL;
    char *pem = NULL;

    if (NULL == cert) {
      return -1;
    }

    bio = BIO_new(BIO_s_mem());
    if (NULL == bio) {
      return -1;
    }

    if (0 == PEM_write_bio_X509(bio, cert)) {
        BIO_free(bio);
	return -1;
    }


    memset(pem, 0, bio->num_write + 1);
    BIO_read(bio, pem, bio->num_write);
    BIO_free(bio);
    return bio->num_write;
}


/* Gen 4k-bit RSA key. */
EVP_PKEY * cfrsa_generate_key()
{

  EVP_PKEY * pkey = EVP_PKEY_new();
  if(!pkey)
    {
      cfopenssl_log_err(__FILE__,__LINE__,"Unable to allocate EVP key struct");
      return NULL;
    }
    

  RSA * rsa = RSA_generate_key(4096, RSA_F4, NULL, NULL);
  if(!EVP_PKEY_assign_RSA(pkey, rsa))
    {
      cfopenssl_log_err(__FILE__,__LINE__,"Unable to 4096 bit RSA key");      
      EVP_PKEY_free(pkey);
      return NULL;
    }
  
  return pkey;
}

/* Genself-signed cert (i.e. sign it with previously generated private key */
X509 * cfrsa_generate_x509(EVP_PKEY * pkey)
{
  
  X509 * x509 = X509_new();
  if(!x509)
    {
      cfopenssl_log_err(__FILE__,__LINE__,"Unable to allocate x509 struct");
      return NULL;
    }
    
  /* Set the serial number. */
  ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);
    
  /* set validity to 1 year or whatever */
  X509_gmtime_adj(X509_get_notBefore(x509), 0);
  X509_gmtime_adj(X509_get_notAfter(x509), 31536000L);
  
  /* Set the public key */
  X509_set_pubkey(x509, pkey);
  
  /* here just copy the subject name to the issuer name. */
  X509_NAME * name = X509_get_subject_name(x509);
  
  /* Set the country code and common name. */
  X509_NAME_add_entry_by_txt(name, "C",  MBSTRING_ASC, (unsigned char *)"CA",        -1, -1, 0);
  X509_NAME_add_entry_by_txt(name, "O",  MBSTRING_ASC, (unsigned char *)"MyCompany", -1, -1, 0);
  X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char *)"localhost", -1, -1, 0);
  
  /* Now set the issuer name. */
  X509_set_issuer_name(x509, name);
  
  /* Actually sign the certificate with our key. */
  if(!X509_sign(x509, pkey, EVP_sha256()))
    {
#ifdef CFOPENSSL_LOG_LEVEL_ERR
      fprintf(stderr, "%s: %d : Signing error\n", __FILE__, __LINE__);
#endif      
      X509_free(x509);
      return NULL;
    }
  
  return x509;
}

int cfrsa_write_to_disk(EVP_PKEY * privkey, X509 * x509)
{
  /* Create the PEM file for writing priv key */
    FILE * privkey_file = fopen("key.pem", "wb");
    if(!privkey_file)
      {
	cfopenssl_log_err(__FILE__,__LINE__,"Can't open private key file for write");	
	return CFRSA_ERR;
	
      }
    
    /* Write priv key. */
    int retVal = PEM_write_PrivateKey(privkey_file, privkey, NULL, NULL, 0, NULL, NULL);
    fclose(privkey_file);
    
    if(!retVal)
      {
	cfopenssl_log_err(__FILE__,__LINE__,"Can't write private key to file");		
        return CFRSA_ERR;
      }
    
    /* Create the PEM file for writing the certificate to disk. */
    FILE * x509_file = fopen("cert.pem", "wb");
    if(!x509_file)
      {
	cfopenssl_log_err(__FILE__,__LINE__,"Can't open cery key file for write");			
        return CFRSA_ERR;
      }
    
    /* Write cert to file */
    retVal = PEM_write_X509(x509_file, x509);
    fclose(x509_file);
    
    if(!retVal)
      {
	cfopenssl_log_err(__FILE__,__LINE__,"Can't write cert to disk");	
        return CFRSA_ERR;	
      }
    
    return CFRSA_SUCCESS;
}

/* deprecated for testing only */
char* cfrsa_certgen2()
{
    /* Generate the key. */
   cfopenssl_log_info(__FILE__,__LINE__,"Generating RSA 4K key...");

    
    EVP_PKEY * pkey = cfrsa_generate_key();
    if(!pkey)
      return CFRSA_ERR;
    
    /* Generate the certificate. */
    cfopenssl_log_info(__FILE__,__LINE__,"Generating x509 certificate...");
    X509 * x509 = cfrsa_generate_x509(pkey);
    if(!x509)
    {
        EVP_PKEY_free(pkey);
        return CFRSA_ERR;
    }
    
    /* Write the private key and certificate out to disk. */
    cfopenssl_log_info(__FILE__,__LINE__,"Writing key and certificate to disk...");    
    
    int ret = cfrsa_write_to_disk(pkey, x509);

    char* cert_str = cfrsa_X509_to_PEM2(x509);

    cfopenssl_log_info(__FILE__,__LINE__,cert_str);

    EVP_PKEY_free(pkey);
    X509_free(x509);
    
    if(ret)
    {
          cfopenssl_log_info(__FILE__,__LINE__,"Success");    
	  return NULL;
    }
    else
      return cert_str;
}


int cfrsa_certgen(char* pembuf)
{
    /* Generate the key. */
   cfopenssl_log_info(__FILE__,__LINE__,"Generating RSA 4K key...");

    
    EVP_PKEY * pkey = cfrsa_generate_key();
    if(!pkey)
      return CFRSA_ERR;
    
    /* Generate the certificate. */
    cfopenssl_log_info(__FILE__,__LINE__,"Generating x509 certificate...");
    X509 * x509 = cfrsa_generate_x509(pkey);
    if(!x509)
    {
        EVP_PKEY_free(pkey);
        return CFRSA_ERR;
    }
    
    /* Write the private key and certificate out to disk. */
    cfopenssl_log_info(__FILE__,__LINE__,"Writing key and certificate to disk...");    
    
    int ret = cfrsa_write_to_disk(pkey, x509);

    int numbytes = cfrsa_X509_to_PEM(x509,pembuf);

    cfopenssl_log_info(__FILE__,__LINE__,pembuf);

    EVP_PKEY_free(pkey);
    X509_free(x509);
    
    if(ret)
    {
          cfopenssl_log_info(__FILE__,__LINE__,"Success");    
	  return -1;
    }
    else
      return numbytes;
}
