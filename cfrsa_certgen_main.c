#include "cfprng_fips_rand.h"
#include "cfrsa_core.h"


int main(int argc, char ** argv)
{
    /* Generate the key. */
  printf("Generating RSA key...\n");
    
    EVP_PKEY * pkey = generate_key();
    if(!pkey)
      return 1;
    
    /* Generate the certificate. */
    cfopenssl_log_info(__FILE__,__LINE__,"Generating x509 certificate...");
    X509 * x509 = generate_x509(pkey);
    if(!x509)
    {
        EVP_PKEY_free(pkey);
        return 1;
    }
    
    /* Write the private key and certificate out to disk. */
    cfopenssl_log_info(__FILE__,__LINE__,"Writing key and certificate to disk...");    
    
    int ret = write_to_disk(pkey, x509);

    char* cert_str = X509_to_PEM(x509);

    printf("CERT IS \n %s\n",cert_str);
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
