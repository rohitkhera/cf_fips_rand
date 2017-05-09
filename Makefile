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
# 1) make - builds shlibs, cfprng_fips_rand & cfprng_nist_rand 
#           programs
#
# 2) make cfprng_fips_static - builds cfprng_fips_rand_static
#   statically linked to cfprng_fips_rand functionality
#
# 3) make cfprng_nist_pic - builds cfprng_nist_rand
#    and associated shlib
#
###################################################

###################################################
# cc and ld flags
###################################################

CC=gcc

###################################################
#DEBUG_FLAGS=-DDEBUG_FINGERPRINT_PREMAIN
###################################################

ARCH_FLAGS=-m32

#CFLAGS=$(ARCH_FLAGS) $(INCLUDES) $(DEBUG_FLAGS)  -Wall 

CFLAGS=$(ARCH_FLAGS) $(INCLUDES) $(DEBUG_FLAGS) 

#LD_VERBOSE=-v

LD_FLAGS=$(ARCH_FLAGS) $(DEBUG_FLAGS) $(LD_VERBOSE)

OPENSSLDIR=/usr/local/ssl/

LIBS=-L/usr/local/ssl/lib/ -L. -lcrypto -ldl 

SO_FLAGS=-shared

RELOC_FLAGS=-fpic

INCLUDES=-I$(OPENSSLDIR)/include

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

HEADERS=cfprng_fips_rand.h


##############################################
# shlib targets, exec targets, toplevel targets
##############################################

FIPS_SO=lib$(FIPS_SO_PREFIX).so

NIST_SO=lib$(NIST_SO_PREFIX).so

FIPS_PROG=$(FIPS_SO_PREFIX)

FIPS_PROG_NO_PIC=cfprng_fips_rand_static

NIST_PROG=$(NIST_SO_PREFIX)

FIPS_TARGETS=$(FIPS_SO) $(FIPS_PROG)

NIST_TARGETS=$(NIST_SO) $(NIST_PROG)

TARGETS=$(FIPS_TARGETS) $(NIST_TARGETS)

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
	$(CC) -c $(CFLAGS) $(FIPS_TEST_PREFIX).c


$(FIPS_PROG_NO_PIC): $(FIPS_PROG_NO_PIC)_OBJS
	FIPSLD_CC=$(CC) $(FIPS_LD) $(LIBS) $(OBJS_NO_PIC) $(FIPS_MAIN_PREFIX).o -o $(FIPS_PROG_NO_PIC) 


$(FIPS_SO):  $(FIPS_SO_PREFIX).o
	$(CC) $(SO_FLAGS) $(ARCH_FLAGS) -o $(FIPS_SO)  $(FIPS_SO_PREFIX).o  $(LIBS)


$(FIPS_PROG): $(OBJS) $(FIPS_MAIN_PREFIX).o
	FIPSLD_CC=$(CC) $(FIPS_LD) $(LIBS) $(OBJS) $(FIPS_MAIN_PREFIX).o -o $(FIPS_PROG) -l$(FIPS_SO_PREFIX) 


$(NIST_SO_PREFIX).o:
	$(CC) -c $(CFLAGS) $(RELOC_FLAGS)  $(NIST_SO_PREFIX).c	


$(NIST_SO):  $(NIST_SO_PREFIX).o
	$(CC) $(SO_FLAGS) $(ARCH_FLAGS) -o $(NIST_SO)  $(NIST_SO_PREFIX).o  $(LIBS)


$(NIST_PROG): $(OBJS) $(NIST_MAIN_PREFIX).o 
	$(CC) $(LD_FLAGS) $(LIBS) $(OBJS) $(NIST_MAIN_PREFIX).o -o $(NIST_PROG) -l$(NIST_SO_PREFIX) 


$(NIST_PROG)_pic: $(NIST_TARGETS)


tests1:
	./cfprng_fips_rand
	./cfprng_nist_rand


tests2:
	./cfprng_fips_rand_static

tests3:
	./cfprng_nist_rand


clean:
	@rm -rf *.o $(TARGETS) $(FIPS_PROG_NO_PIC)


##############################################
# suffixes
##############################################


.c.o:
	$(CC) -c $(CFLAGS) $(RELOC_FLAGS) $<
