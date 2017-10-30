#include "cfrsa_core.h"


int main(int argc, char ** argv)
{
    /* Generate the key. */
  printf("Generating RSA key...\n");
    
    EVP_PKEY * pkey = generate_key();
    if(!pkey)
      return CFRSA_ERR;
    
    /* Generate the certificate. */
    printf("Generating x509 certificate...\n");
    
    X509 * x509 = generate_x509(pkey);
    if(!x509)
    {
        EVP_PKEY_free(pkey);
        return CFRSA_ERR;
    }
    
    /* Write the private key and certificate out to disk. */
    printf("Writing key and certificate to disk...\n");
    
    int ret = write_to_disk(pkey, x509);
    EVP_PKEY_free(pkey);
    X509_free(x509);
    
    if(ret)
    {
      printf("Success!\n");
        return CFRSA_SUCCESS;
    }
    else
        return CFRSA_ERR;
}
