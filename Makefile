
###################################################
# R Khera Oct 31 2017
# Compile cfprng_nist_rand.c and cfprng_fips_rand.c into .so's
# also added new targets for RSA cert gen and AES GCM which will
# also be compiled into .so.
# FIPS module related targets are deprecated
#
# -m32 CFLAGS needed since openssl libs 
# were compied in 32 bit mode and without 
# this flag ld.so looks for x86_64 symbols .
# Tested wth openssl-fips-2.0.11 and openssl-1.0.1
# Comment out the ARCH_FLAGS var for linking with 
# x86-64 libs
# compiler info
# bash$ gcc -v 
# Apple LLVM version 8.1.0 (clang-802.0.42)
# Target: x86_64-apple-darwin16.5.0
# Thread model: posix
# Also tested with gcc version 4.8.2 (Ubuntu 4.8.2-19ubuntu1) 
#
# If you do not have an openssl FIPS module compiled
# then comment out the Variable FIPS_TARGETS
# since you will not be building this target
#
# Toplevel commands are as follows
#
# a) ***deprecated**
#    make - **Only use this target if you have compiled the OpenSSL
#    FIPS module**.
#    Builds shlibs, cfprng_fips_rand & cfprng_nist_rand 
#    programs, java jni wrapper classes and jni .so
#
# b) **deprecated**
#    make cfprng_fips_rand_static - **Only use if you hae compiled
#    the OpenSSL FIPS module**.
#    Builds cfprng_fips_rand_static
#    statically linked to cfprng_fips_rand functionality
#
# c) make set1
#    cfprng_nist_rand_pic - builds cfprng_nist_rand
#    and associated .so, java jni wrapper classes and jni .so
#    (Does not leverate the openSSL FIPS module)
#
# d) make set2
#    added support for rsa cert gen. and associated .so,
#    java jni wrapper classes and jni .so
#    (Does not leverate the openSSL FIPS module)
#
# Instructions:
#
# 1) Change compiler (cc, javac) and relavant path / include variables 
# in the Makefile to point to your OpenSSL install, java install and jni header files
#
# 2) If you have the OpenSSL FIPS module compiled use target (a) above to 
# build the shlibs, programs, java classes and jni shlib
#
# 3) If you just wish to compile the statically linked fips program use 
# target (b) above (requires that you compile / install OpenSSL FIPS module)
#
# 4) If you just have OpenSSL installed, but have not compiled 
# the FIPS module, use build target (c) above
#
# java version info 
# Java(TM) SE Runtime Environment (build 1.8.0_101-b13)
# Java HotSpot(TM) 64-Bit Server VM (build 25.101-b13, mixed mode)
#
# OpenSSL install:
# The following are instructions to install and build openssl and the openssl fips module. The libraries and fips cannister objects are intalled in the default location /usr/local/ssl.
# 1) Download openssl-fips-2.0.16
# ./config
# make
# sudo make install
# 1) Download openssl-1.0.2
# ./config fips
# make depend
# make
# sudo make install
###################################################

###################################################
# cc , ld flags and java defs
###################################################

CC=gcc

#JAVA_PATH=/Library/Java/JavaVirtualMachines/jdk1.8.0_111.jdk/Contents/Home/bin/

JAVA_PATH=/usr/lib/jvm/java-8-oracle/bin/

JAVAC=$(JAVA_PATH)javac

JAVAH=$(JAVA_PATH)javah

#JAVA_LIB_PATH="/home/user/workspace/cfprng_fips_rand"

JAVA_LIB_PATH="$(LD_LIBRARY_PATH)"

#DEBUG_FLAGS=-DDEBUG_FINGERPRINT_PREMAIN

#JNI_MD_ARCH=darwin

JNI_MD_ARCH=linux

JNI_INCLUDES=-I/usr/lib/jvm/java-8-oracle/include/

#JNI_INCLUDES=-I/Library/Java/JavaVirtualMachines/jdk1.8.0_73.jdk/Contents/Home/include/

#JNI_INCLUDES=-I/Library/Java/JavaVirtualMachines/jdk1.8.0_111.jdk/Contents/Home/include/

JNI_INCLUDES_MD=$(JNI_INCLUDES)/$(JNI_MD_ARCH)

LOG_LEVEL_FLAGS=-DCFOPENSSL_LOG_LEVEL_ERR -DCFOPENSSL_LOG_LEVEL_INFO

#ARCH_FLAGS=-m32

NO_UNUSED_VAR_CFLAGS=-Wno-unused-variable

#CFLAGS=$(ARCH_FLAGS) $(INCLUDES) $(JNI_INCLUDES) $(JNI_INCLUDES_MD) $(DEBUG_FLAGS)  $(LOG_LEVEL_FLAGS) -Wall $(NO_UNUSED_VAR_CFLAGS)

CFLAGS=$(ARCH_FLAGS) $(INCLUDES) $(JNI_INCLUDES) $(JNI_INCLUDES_MD) $(DEBUG_FLAGS)  $(LOG_LEVEL_FLAGS) 

#LD_VERBOSE=-v

LD_FLAGS=$(ARCH_FLAGS) $(DEBUG_FLAGS) $(LD_VERBOSE)

OPENSSLDIR=/usr/local/ssl

LIBS=-L$(OPENSSLDIR)/lib/ -L. -lcrypto -ldl 

SO_FLAGS=-shared

# should be the same on gcc and LLVM
RELOC_FLAGS=-fpic

INCLUDES=-I$(OPENSSLDIR)/include

#LD_LIBRARY_PATH=$(OPENSSLDIR)/lib/:.

# on ubuntu, the following var should point to the dir that contains libcrypto.so.1.1
LD_LIBRARY_PATH=/usr/local/lib:.

#SO_EXT=dylib

SO_EXT=so

############################################
# srcs, hdrs, obs and shared objs
############################################


FIPS_SO_PREFIX=cfprng_fips_rand

NIST_SO_PREFIX=cfprng_nist_rand

NIST_MAIN_PREFIX=cfprng_nist_main

COMMON_SO_PREFIX=cfprng_rand_common

FIPS_MAIN_PREFIX=cfprng_fips_main

UTILS_PREFIX=cfprng_utils

FIPS_TESTS_PREFIX=cfprng_fips_tests

CFRSA_CERTGEN_PREFIX=cfrsa_certgen

CFRSA_CERTGEN_MAIN_PREFIX=$(CFRSA_CERTGEN_PREFIX)_main

SRCS1=cfprng_fips_rand.c \
	cfprng_nist_rand.c \
	$(UTILS_PREFIX).c \
	$(FIPS_TESTS_PREFIX).c

SRCS2=$(NIST_MAIN_PREFIX).c $(FIPS_MAIN_PREFIX).c

SRCS_RSA=$(CFRSA_CERTGEN_PREFIX).c $(CFRSA_CERTGEN_MAIN_PREFIX).c 

SRCS_UTILS=cfprng_utils.c

OBJS=$(FIPS_SO_PREFIX).o $(UTILS_PREFIX).o $(FIPS_TESTS_PREFIX).o

CFRSA_OBJS=$(CFRSA_CERTGEN_PREFIX).o

COMMON_OBJS=$(UTILS_PREFIX).o $(FIPS_TESTS_PREFIX).o

JAVA_CLASSES1=CfprngRand.class

JAVA_CLASSES2=CfRsaCertGen.class

HEADERS=cfprng_fips_rand.h


##############################################
# shlib targets, exec targets, toplevel targets
##############################################

FIPS_SO=lib$(FIPS_SO_PREFIX).$(SO_EXT)

NIST_SO=lib$(NIST_SO_PREFIX).$(SO_EXT)

COMMON_SO=lib$(COMMON_SO_PREFIX).$(SO_EXT)

CFPRNG_JNI_SO=libcfprng_rand_jni.$(SO_EXT)

CFRSA_CERTGEN_SO=lib$(CFRSA_CERTGEN_PREFIX).$(SO_EXT)

CFRSA_CERTGEN_JNI_SO=libcfrsa_certgen_jni.$(SO_EXT)

FIPS_PROG=$(FIPS_SO_PREFIX)

FIPS_PROG_NO_PIC=$(FIPS_PROG)_static

NIST_PROG=$(NIST_SO_PREFIX)

CFRSA_CERTGEN_PROG=$(CFRSA_CERTGEN_PREFIX)

CFRSA_CERTGEN_PROG=$(CFRSA_CERTGEN_PREFIX)

FIPS_TARGETS=$(FIPS_SO) $(FIPS_PROG)

NIST_TARGETS=$(NIST_SO) $(NIST_PROG)

#deprecated since we're no longer as concerned with FIPS module
# use TARGETS2 def instead
TARGETS=$(FIPS_TARGETS) $(NIST_TARGETS) $(COMMON_SO) $(JNI_TARGETS)

TARGETS2 = set1 set2

JNI_TARGETS=$(CFPRNG_JNI_SO)

#############################################
# fips commands
##############################################


FIPS_LD=$(OPENSSLDIR)/fips-2.0/bin/fipsld $(LD_FLAGS)



all:
	make clean;\
	make $(TARGETS2)

$(FIPS_SO_PREFIX).o: 
	$(CC) -c $(CFLAGS) $(RELOC_FLAGS)  $(FIPS_SO_PREFIX).c

# do not use default .c.o suffix rulex for FIPS statically linked exec
# since default suffixes use the RELOC (-fpic) flag

$(FIPS_PROG_NO_PIC)_OBJS: 
	make clean 
	$(CC) -c $(CFLAGS) $(FIPS_SO_PREFIX).c	
	$(CC) -c $(CFLAGS) $(UTILS_PREFIX).c
	$(CC) -c $(CFLAGS) $(FIPS_MAIN_PREFIX).c
	$(CC) -c $(CFLAGS) $(FIPS_TESTS_PREFIX).c


$(FIPS_PROG_NO_PIC): $(FIPS_PROG_NO_PIC)_OBJS
	FIPSLD_CC=$(CC) $(FIPS_LD) $(OBJS) $(FIPS_MAIN_PREFIX).o -o $(FIPS_PROG_NO_PIC)  $(LIBS)


$(COMMON_SO): $(COMMON_OBJS)
	$(CC) $(SO_FLAGS) $(ARCH_FLAGS) -o $(COMMON_SO)  $(COMMON_OBJS)  $(LIBS)	


$(FIPS_SO):  $(FIPS_SO_PREFIX).o
	$(CC) $(SO_FLAGS) $(ARCH_FLAGS) -o $(FIPS_SO)  $(FIPS_SO_PREFIX).o  $(LIBS)


$(CFRSA_CERTGEN_SO): $(COMMON_SO) $(CFRSA_OBJS)
	$(CC) $(SO_FLAGS) $(ARCH_FLAGS) -o $(CFRSA_CERTGEN_SO)  $(CFRSA_OBJS)  -l$(COMMON_SO_PREFIX) $(LIBS)


$(CFRSA_CERTGEN_PROG)_pic: $(CFRSA_CERTGEN_SO) $(CFRSA_CERTGEN_MAIN_PREFIX).o $(CFRSA_CERTGEN_JNI_SO)
	$(CC) $(LD_FLAGS) $(CFRSA_CERTGEN_MAIN_PREFIX).o -o $(CFRSA_CERTGEN_PROG) -l$(CFRSA_CERTGEN_PREFIX) -l$(COMMON_SO_PREFIX) $(LIBS)


$(FIPS_PROG): $(COMMON_SO) $(FIPS_MAIN_PREFIX).o
	FIPSLD_CC=$(CC) $(FIPS_LD) $(OBJS) $(FIPS_MAIN_PREFIX).o -o $(FIPS_PROG) -l$(FIPS_SO_PREFIX) \
	-l$(COMMON_SO_PREFIX) $(LIBS)


$(NIST_SO_PREFIX).o:
	$(CC) -c $(CFLAGS) $(RELOC_FLAGS)  $(NIST_SO_PREFIX).c	


$(NIST_SO):  $(NIST_SO_PREFIX).o
	$(CC) $(SO_FLAGS) $(ARCH_FLAGS) -o $(NIST_SO)  -l$(COMMON_SO_PREFIX) $(NIST_SO_PREFIX).o  $(LIBS)


$(NIST_PROG): $(COMMON_SO) $(NIST_MAIN_PREFIX).o 
	$(CC) $(LD_FLAGS) $(NIST_OBJS) $(NIST_MAIN_PREFIX).o -o $(NIST_PROG) -l$(NIST_SO_PREFIX) -l$(COMMON_SO_PREFIX) $(LIBS)




$(NIST_PROG)_pic: $(COMMON_SO) $(NIST_TARGETS) $(CFPRNG_JNI_SO)


# cfprng_nist_rand_pic
set1: $(NIST_PROG)_pic

set2: $(CFRSA_CERTGEN_SO) $(CFRSA_CERTGEN_PROG)_pic

$(CFPRNG_JNI_SO): javah_cfprng  cfprng_rand_java_wrapper.o
	$(CC) $(SO_FLAGS) $(ARCH_FLAGS) -o $(CFPRNG_JNI_SO)  cfprng_rand_java_wrapper.o \
	$(LIBS) -l$(COMMON_SO_PREFIX) -l$(NIST_SO_PREFIX)


javah_cfprng: $(JAVA_CLASSES1)
	$(JAVAH) -jni CfprngRand


$(CFRSA_CERTGEN_JNI_SO): javah_cfrsa cfrsa_certgen_java_wrapper.o
	$(CC) $(SO_FLAGS) $(ARCH_FLAGS) -o $(CFRSA_CERTGEN_JNI_SO)  cfrsa_certgen_java_wrapper.o \
	$(LIBS) -l$(COMMON_SO_PREFIX) -l$(CFRSA_CERTGEN_PREFIX)


javah_cfrsa: $(JAVA_CLASSES2)
	$(JAVAH) -jni CfRsaCertGen




run1:
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH); ./cfprng_fips_rand
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH); ./cfprng_nist_rand


run2:
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH);\
	./cfprng_fips_rand_static

run3:
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH);\
	./cfprng_nist_rand

run4:
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH);\
	 $(JAVA_PATH)/java -Djava.library.path=$(JAVA_LIB_PATH) CfprngRand

run5:
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH);\
	./cfrsa_certgen

run6:
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH);\
	 $(JAVA_PATH)/java -Djava.library.path=$(JAVA_LIB_PATH) CfRsaCertGen; \
	openssl verify -verbose -CAfile cert.pem  cert.pem

clean:
	@rm -rf *.o $(TARGETS) $(FIPS_PROG_NO_PIC) $(NIST_PROG) $(NIST_PROG)_pic $(CFRSA_CERTGEN_PROG) *.class *.$(SO_EXT) *.pem test_intermediate_CA/intermediate.*



##############################################
# suffixes
##############################################

.SUFFIXES: .java .class

.c.o:
	$(CC) -c $(CFLAGS) $(RELOC_FLAGS) $<

.java.class:
	$(JAVAC) $<
