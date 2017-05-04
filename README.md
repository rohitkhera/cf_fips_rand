# cf_fips_rand
Shared Objects wrapping OpenSSL NIST/ FIPS approved pseudo random number generators

ANSI C wrapper code for 
generating pseudo random  bytes (1)  Using NIST SP 800-90A algos, or (2) 
Using NIST SP 800-90A algos with the OpenSSL FIPS module. 
The intent is to address the goal of using  
correctly seeded NIST SP 800-90A specified PRNGs for generation of IVs, 
symmetric keys, nonces, salts and machine passwords. The SP 800-90A PRNGs
cannot directly generate asymmetric keys. 
Enabling OpenSSL FIPS mode only allows algorithms 
specified in SP 800-90A and ANS X9.31
However, we would prefer to just use algorithms 
from SP800-90A over ANS X9.31
Among the SP800-90A specified algorithms, we 
must avoid "Dual_EC_DRBG". 
Tested wth openssl-fips-2.0.11 and openssl-1.0.1r
This file is compiled into a shared object that should 
be placed in the caller's LD_LIBRARY_PATH (see 
Makefile for compiler versions, fipsld, ld.so / c flags etc.)
Tested with gcc version 
gcc -v 
Apple LLVM version 8.1.0 (clang-802.0.42)
Target: x86_64-apple-darwin16.5.0
Thread model: posix

Owing to limitations in the underlying OpenSSL libraries, this code is not thread safe

Instructions:
1) Change compiler and relavant path / include variables in the Makefile 
to point to your OpenSSL install

2) If you have compiled and installed the  OpenSSL FIPS module installed, 
  type make in the toplevel dir 

3) If you just have OpenSSL installed, but have not compiled the 
   FIPS module, type make nist_target
   
4) this builds two shared libs encapsulating OPenSSL RAND methods, 
   one in FIPS mode and the other without FIPS mode, as well as two test programs

5) Depending on your build, you can run the test programs as ./cf_nist_rand and / or 
  ./cf_fips_rand 
