###################################################
# R Khera 5/3/17
# Compile cfprng_nist_rand.c and cfprng_fips_rand.c into .so's
# -m32 CFLAGS needed since openssl libs 
# were compied in 32 bit mode and without 
# this flag ld.so looks for x86_64 symbols .
# Tested wth openssl-fips-2.0.11 and openssl-1.0.1r
# Comment out the ARCH_FLAGS var for linking with 
# x86-64 libs
# compiler info
# bash$ gcc -v 
# Apple LLVM version 8.1.0 (clang-802.0.42)
# Target: x86_64-apple-darwin16.5.0
# Thread model: posix
#
# If you do not have an openssl FIPS module compiled
# then comment out the Variable FIPS_TARGETS
# since you will not be building this target
#
# Toplevel commands are as follows
#
# a) make - builds shlibs, cfprng_fips_rand & cfprng_nist_rand 
#           programs, java jni wrapper classes and jni .so
#
# b) make cfprng_fips_rand_static - builds cfprng_fips_rand_static
#   statically linked to cfprng_fips_rand functionality
#
# c) make cfprng_nist_rand_pic - builds cfprng_nist_rand
#    and associated shlib, java jni wrapper classes and jni .so
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
# target (a) above (requires that you compile / install OpenSSL FIPS module)
#
# 4) If you just have OpenSSL installed, but have not compiled 
# the FIPS module, use build target (c) above
#
###################################################

###################################################
# cc , ld flags and java defs
###################################################

CC=gcc

JAVAC=javac

JAVAH=javah

JAVA_LIB_PATH="/Users/rkhera/Downloads/cf_fips_rand"

###################################################
#DEBUG_FLAGS=-DDEBUG_FINGERPRINT_PREMAIN
###################################################

JNI_MD_ARCH=darwin

JNI_INCLUDES=-I/Library/Java/JavaVirtualMachines/jdk1.8.0_73.jdk/Contents/Home/include/

JNI_INCLUDES_MD=$(JNI_INCLUDES)/$(JNI_MD_ARCH)

LOG_LEVEL_FLAGS=-DCFPRNG_LOG_LEVEL_ERR -DCFPRNG_LOG_LEVEL_INFO

ARCH_FLAGS=-m32

CFLAGS=$(ARCH_FLAGS) $(INCLUDES) $(JNI_INCLUDES) $(JNI_INCLUDES_MD) $(DEBUG_FLAGS)  $(LOG_LEVEL_FLAGS) -Wall 

#CFLAGS=$(ARCH_FLAGS) $(INCLUDES) $(DEBUG_FLAGS) $(LOG_LEVEL_FLAGS)

#LD_VERBOSE=-v

LD_FLAGS=$(ARCH_FLAGS) $(DEBUG_FLAGS) $(LD_VERBOSE)

OPENSSLDIR=/usr/local/ssl

LIBS=-L$(OPENSSLDIR)/lib/ -L. -lcrypto -ldl 

SO_FLAGS=-shared

RELOC_FLAGS=-fpic

INCLUDES=-I$(OPENSSLDIR)/include

LD_LIBRARY_PATH=$(OPENSSLDIR)/lib/:.

############################################
# srcs, hdrs, obs and shared objs
############################################


FIPS_SO_PREFIX=cfprng_fips_rand

NIST_SO_PREFIX=cfprng_nist_rand

NIST_MAIN_PREFIX=cfprng_nist_main

FIPS_MAIN_PREFIX=cfprng_fips_main

UTILS_PREFIX=cfprng_utils

FIPS_TESTS_PREFIX=cfprng_fips_tests

SRCS1=cfprng_fips_rand.c \
	cfprng_nist_rand.c \
	$(UTILS_PREFIX).c \
	$(FIPS_TESTS_PREFIX).c

SRCS2=$(NIST_MAIN_PREFIX).c $(FIPS_MAIN_PREFIX).c

SRCS_UTILS=cfprng_utils.c

OBJS=$(FIPS_SO_PREFIX).o $(UTILS_PREFIX).o $(FIPS_TESTS_PREFIX).o

NIST_OBJS=$(UTILS_PREFIX).o $(FIPS_TESTS_PREFIX).o

JAVA_CLASSES=CfprngRand.class

HEADERS=cfprng_fips_rand.h


##############################################
# shlib targets, exec targets, toplevel targets
##############################################

FIPS_SO=lib$(FIPS_SO_PREFIX).so

NIST_SO=lib$(NIST_SO_PREFIX).so

CFPRNG_JNI_SO=libcfprng_rand_jni.so

FIPS_PROG=$(FIPS_SO_PREFIX)

FIPS_PROG_NO_PIC=cfprng_fips_rand_static

NIST_PROG=$(NIST_SO_PREFIX)

FIPS_TARGETS=$(FIPS_SO) $(FIPS_PROG)

NIST_TARGETS=$(NIST_SO) $(NIST_PROG)

TARGETS=$(FIPS_TARGETS) $(NIST_TARGETS) $(JNI_TARGETS)

JNI_TARGETS=$(CFPRNG_JNI_SO)

#############################################
# fips commands
##############################################


FIPS_LD=$(OPENSSLDIR)/fips-2.0/bin/fipsld $(LD_FLAGS)



all: $(TARGETS)


$(FIPS_SO_PREFIX).o: 
	$(CC) -c $(CFLAGS) $(RELOC_FLAGS)  $(FIPS_SO_PREFIX).c


$(FIPS_PROG_NO_PIC)_OBJS: 
	make clean 
	$(CC) -c $(CFLAGS) $(FIPS_SO_PREFIX).c	
	$(CC) -c $(CFLAGS) $(UTILS_PREFIX).c
	$(CC) -c $(CFLAGS) $(FIPS_MAIN_PREFIX).c
	$(CC) -c $(CFLAGS) $(FIPS_TESTS_PREFIX).c


$(FIPS_PROG_NO_PIC): $(FIPS_PROG_NO_PIC)_OBJS
	FIPSLD_CC=$(CC) $(FIPS_LD) $(OBJS) $(FIPS_MAIN_PREFIX).o -o $(FIPS_PROG_NO_PIC)  $(LIBS)


$(FIPS_SO):  $(FIPS_SO_PREFIX).o
	$(CC) $(SO_FLAGS) $(ARCH_FLAGS) -o $(FIPS_SO)  $(FIPS_SO_PREFIX).o  $(LIBS)


$(FIPS_PROG): $(OBJS) $(FIPS_MAIN_PREFIX).o
	FIPSLD_CC=$(CC) $(FIPS_LD) $(OBJS) $(FIPS_MAIN_PREFIX).o -o $(FIPS_PROG) -l$(FIPS_SO_PREFIX) $(LIBS)


$(NIST_SO_PREFIX).o:
	$(CC) -c $(CFLAGS) $(RELOC_FLAGS)  $(NIST_SO_PREFIX).c	


$(NIST_SO):  $(NIST_SO_PREFIX).o
	$(CC) $(SO_FLAGS) $(ARCH_FLAGS) -o $(NIST_SO)  $(NIST_SO_PREFIX).o  $(LIBS)


$(NIST_PROG): $(NIST_OBJS) $(NIST_MAIN_PREFIX).o 
	$(CC) $(LD_FLAGS) $(NIST_OBJS) $(NIST_MAIN_PREFIX).o -o $(NIST_PROG) -l$(NIST_SO_PREFIX) $(LIBS)


$(NIST_PROG)_pic: $(NIST_TARGETS) $(CFPRNG_JNI_SO)


$(CFPRNG_JNI_SO): javah  cfprng_rand_java_wrapper.o
	$(CC) $(SO_FLAGS) $(ARCH_FLAGS) -o $(CFPRNG_JNI_SO)  cfprng_rand_java_wrapper.o \
	$(LIBS) $(NIST_SO) 


javah: $(JAVA_CLASSES)
	javah -jni CfprngRand

run1:
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH);./cfprng_fips_rand
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH);./cfprng_nist_rand


run2:
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH);./cfprng_fips_rand_static

run3:
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH);./cfprng_nist_rand


run4:
	java -Djava.library.path=$(JAVA_LIB_PATH) CfprngRand

clean:
	@rm -rf *.o $(TARGETS) $(FIPS_PROG_NO_PIC) *.class


##############################################
# suffixes
##############################################

.SUFFIXES: .java .class

.c.o:
	$(CC) -c $(CFLAGS) $(RELOC_FLAGS) $<

.java.class:
	$(JAVAC) $<