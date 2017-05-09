

/* Test program for cf_fips_rand() */

#include "cfprng_fips_rand.h"



int main(int argc, char *argv[]) 
{ 
  char *opt = NULL; 
  int verbose = 0; 
  int fipsmode = 1; 
  
  int i; 
  /* Process command line arguments */ 
  i = 0; 
  while(++i < argc) { 
    opt = argv[i]; 
    if (!strcmp(opt,"­v")) verbose = 1; 
    else if (!strcmp(opt,"­c")) fipsmode = 0; 
    else if (strcmp(opt,"-")) { 
      printf("Usage: %s \n", argv[0]); 
      puts("Options (defunct):"); 
      puts("\t­c\tUse non­FIPS mode"); 
      puts("\t­v\tVerbose output"); 
      exit(1); 
    } 
    else break; 
  }
  
  cfprng_nist_rand();
  if(cfprng_fips_tests())
    fprintf(stderr, "%s : ln %d : Failed simple FIPS statistical tests\n", __FILE__, __LINE__);
  else
    fprintf(stderr, "%s : ln %d : Passed simple FIPS statistical tests\n", __FILE__, __LINE__);


  exit(0);
}
