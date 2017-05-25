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
Also tested with gcc version 4.8.2 (Ubuntu 4.8.2-19ubuntu1) 

Owing to limitations in the underlying OpenSSL libraries, this code is not thread safe

The following toplevel make targets are defined

a) make - builds shlibs, cfprng_fips_rand & cfprng_nist_rand 
           programs, java jni wrapper classes and jni .so

b) make cfprng_fips_rand_static - builds cfprng_fips_rand_static
  statically linked to cfprng_fips_rand functionality

c) make cfprng_nist_rand_pic - builds cfprng_nist_rand
    and associated shlib, java jni wrapper classes and jni .so

Instructions:

1) Change compiler (cc, javac) and relavant path / include variables in the Makefile 
   to point to your OpenSSL install, java install and jni header files

2) If you have the OpenSSL FIPS module compiled use target (a) above 
   to build the shlibs, programs, java classes and jni shlib

3) If you just wish to compile the statically linked fips program
   use target (b) above (requires that you compile / install OpenSSL FIPS module)

4) If you just have OpenSSL installed, but have not compiled the 
   FIPS module, use build target (c) above
   

5) The following run targets are defined (requires setting LD_LIBRARY_PATH and 
   java.library.path)


run1:
	./cfprng_fips_rand
	./cfprng_nist_rand

run2:
	./cfprng_fips_rand_static

run3:
	./cfprng_nist_rand


run4:
	java -Djava.library.path=$(JAVA_LIB_PATH) CfprngRand

java version info 
Java(TM) SE Runtime Environment (build 1.8.0_101-b13)
Java HotSpot(TM) 64-Bit Server VM (build 25.101-b13, mixed mode)

OpenSSL install:
The following are instructions to install and build openssl and the openssl fips module. The libraries and fips cannister objects are intalled in the default location /usr/local/ssl.
1) Download openssl-fips-2.0.16;
 ./config;
 make;
 sudo make install
1) Download openssl-1.0.2l
./config fips;
 make depend;
 make;
 sudo make install