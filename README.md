# cfprng_fips_rand
Shared Objects wrapping OpenSSL NIST/ FIPS approved pseudo random number generators

ANSI C wrapper code for 
generating pseudo random  bytes (1)  Using NIST SP 800-90A algos, or (2) 
Using NIST SP 800-90A algos in OpenSSL FIPS mode. 
The intent is to address the goal of using  
correctly seeded NIST SP 800-90A specified PRNGs for generation of IVs, 
symmetric keys, nonces, salts and machine passwords. The SP 800-90A PRNGs
cannot directly generate asymmetric keys. Seeding of generators is 
done explicitly, by extracting 256 bits of noise from the Linux entropy 
pools. Entropy of the seed (measured in bits) is therefore
256 bits based on the Linux entropy pool estimation 'heuristic' (refer to 
https://ringcipher.com/2013/05/04/uncertainty-randomness-virtualization/ 
for an explanation of this). Given this fact, the libraries 
instantiate the NIST generators 
with sufficient strength for generation of 256 bit symmetric keys 
(Look at sections 8.2 and 8.4 of the following for an explanation of 
of some of this) - 
http://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-90Ar1.pdf

These libraries currently do not expose any methods to reseed the generators. 

Enabling OpenSSL FIPS mode only allows algorithms 
specified in SP 800-90A and ANS X9.31
However, we would prefer to just use algorithms 
from SP800-90A over ANS X9.31
Among the SP800-90A specified algorithms, we 
must avoid "Dual_EC_DRBG". 
Tested wth openssl-fips-2.0.11 and openssl-1.0.1r
Along with test programs, this build compiles shared 
libs that should be placed in the caller's LD_LIBRARY_PATH 
(see Makefile for more info on compiler versions, 
fipsld, ld.so / c flags etc.)

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
   
4) This builds two shared libs encapsulating OpenSSL RAND methods, 
   one in FIPS mode and the other without FIPS mode, as well as two test programs

5) Depending on inclusion or lack of a FIPS build, you can run the test programs (i) ./cfprng_nist_rand and / or 
  (ii) ./cfprng_fips_rand 
