/*******************************************************
*   R Khera 5/3/17 
*   ANSI C wrapper code for 
*   rsa self signed certs in OpenSSL
* 
*   Tested wth openssl-1.0.1
*   This unit is compiled into a shared object that should 
*   be placed in the caller's LD_LIBRARY_PATH (see 
*   Makefile for compiler versions and and ld / c flags etc
********************************************************/   
#include "cfrsa_core.h"

/* Generates a 2048-bit RSA key. */
EVP_PKEY * generate_key()
{
    /* Allocate memory for the EVP_PKEY structure. */
    EVP_PKEY * pkey = EVP_PKEY_new();
    if(!pkey)
    {
      printf("Unable to create EVP_PKEY structure\n");
        return NULL;
    }
    
    /* Generate the RSA key and assign it to pkey. */
    RSA * rsa = RSA_generate_key(2048, RSA_F4, NULL, NULL);
    if(!EVP_PKEY_assign_RSA(pkey, rsa))
    {
      printf("Unable to generate 2048-bit RSA key.\n");
        EVP_PKEY_free(pkey);
        return NULL;
    }
    
    /* The key has been generated, return it. */
    return pkey;
}

/* Generates a self-signed x509 certificate. */
X509 * generate_x509(EVP_PKEY * pkey)
{
    /* Allocate memory for the X509 structure. */
    X509 * x509 = X509_new();
    if(!x509)
    {
      printf("Unable to create X509 structure.\n");
        return NULL;
    }
    
    /* Set the serial number. */
    ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);
    
    /* This certificate is valid from now until exactly one year from now. */
    X509_gmtime_adj(X509_get_notBefore(x509), 0);
    X509_gmtime_adj(X509_get_notAfter(x509), 31536000L);
    
    /* Set the public key for our certificate. */
    X509_set_pubkey(x509, pkey);
    
    /* We want to copy the subject name to the issuer name. */
    X509_NAME * name = X509_get_subject_name(x509);
    
    /* Set the country code and common name. */
    X509_NAME_add_entry_by_txt(name, "C",  MBSTRING_ASC, (unsigned char *)"CA",        -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "O",  MBSTRING_ASC, (unsigned char *)"MyCompany", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char *)"localhost", -1, -1, 0);
    
    /* Now set the issuer name. */
    X509_set_issuer_name(x509, name);
    
    /* Actually sign the certificate with our key. */
    if(!X509_sign(x509, pkey, EVP_sha1()))
    {
      printf("Error signing certificate.\n");
        X509_free(x509);
        return NULL;
    }
    
    return x509;
}

int write_to_disk(EVP_PKEY * pkey, X509 * x509)
{
    /* Open the PEM file for writing the key to disk. */
    FILE * pkey_file = fopen("key.pem", "wb");
    if(!pkey_file)
    {
      printf("Unable to open \"key.pem\" for writing.\n");
        return 0;
    }
    
    /* Write the key to disk. */
    int ret = PEM_write_PrivateKey(pkey_file, pkey, NULL, NULL, 0, NULL, NULL);
    fclose(pkey_file);
    
    if(!ret)
    {
      printf("Unable to write private key to disk.\n");
        return 0;
    }
    
    /* Open the PEM file for writing the certificate to disk. */
    FILE * x509_file = fopen("cert.pem", "wb");
    if(!x509_file)
    {
      printf("Unable to open \"cert.pem\" for writing.\n");
        return 0;
    }
    
    /* Write the certificate to disk. */
    ret = PEM_write_X509(x509_file, x509);
    fclose(x509_file);
    
    if(!ret)
    {
      printf("Unable to write certificate to disk.\n");
        return 0;
    }
    
    return 1;
}

