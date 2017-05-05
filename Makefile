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
###################################################

CC=gcc

#DEBUG_FLAGS=-DDEBUG_FINGERPRINT_PREMAIN

ARCH_FLAGS=-m32

CFLAGS=$(ARCH_FLAGS) $(INCLUDES) $(DEBUG_FLAGS)  -Wall 

#CFLAGS=$(ARCH_FLAGS) $(INCLUDES) $(DEBUG_FLAGS) 

#LD_VERBOSE=-v

LD_FLAGS=$(ARCH_FLAGS) $(DEBUG_FLAGS) $(LD_VERBOSE)

OPENSSLDIR=/usr/local/ssl/

LIBS=-L/usr/local/ssl/lib/ -L. -lcrypto -ldl 

SO_FLAGS=-shared

RELOC_FLAGS=-fpic

INCLUDES=-I$(OPENSSLDIR)/include

FIPS_SO_PREFIX=cfprng_fips_rand

FIPS_SO=lib$(FIPS_SO_PREFIX).so

NIST_SO_PREFIX=cfprng_nist_rand

NIST_SO=lib$(NIST_SO_PREFIX).so

OBJS=cfprng_utils.o

NIST_MAIN=cfprng_nist_main.o

FIPS_MAIN=cfprng_fips_main.o

SRCS1=cfprng_fips_rand.c \
	cfprng_nist_rand.c \
	cfprng_utils.c

SRCS2=cfprng_nist_main.c cfprng_fips_main.c

HEADERS=cfprng_fips_rand.h

FIPS_PROG=$(FIPS_SO_PREFIX)

NIST_PROG=$(NIST_SO_PREFIX)

FIPS_TARGETS=$(FIPS_SO) $(FIPS_PROG)

NIST_TARGETS=$(NIST_SO) $(NIST_PROG)

TARGETS=$(FIPS_TARGETS) $(NIST_TARGETS)

FIPS_LD=$(OPENSSLDIR)/fips-2.0/bin/fipsld $(LD_FLAGS)



all: $(TARGETS)

$(FIPS_SO_PREFIX).o: 
	$(CC) -c $(CFLAGS) $(RELOC_FLAGS)  $(FIPS_SO_PREFIX).c

$(FIPS_SO):  $(FIPS_SO_PREFIX).o
	$(CC) $(SO_FLAGS) $(ARCH_FLAGS) -o $(FIPS_SO)  $(FIPS_SO_PREFIX).o  $(LIBS)

$(FIPS_PROG): $(OBJS) $(FIPS_MAIN) $(NIST_SO)
	FIPSLD_CC=$(CC) $(FIPS_LD) $(LIBS) $(OBJS) $(FIPS_MAIN) -o $(FIPS_PROG) -l$(FIPS_SO_PREFIX) -l$(NIST_SO_PREFIX)


$(NIST_SO_PREFIX).o:
	$(CC) -c $(CFLAGS) $(RELOC_FLAGS)  $(NIST_SO_PREFIX).c	


$(NIST_SO):  $(NIST_SO_PREFIX).o
	$(CC) $(SO_FLAGS) $(ARCH_FLAGS) -o $(NIST_SO)  $(NIST_SO_PREFIX).o  $(LIBS)

$(NIST_PROG): $(OBJS) $(NIST_MAIN) $(FIPS_SO)
	$(CC) $(LD_FLAGS) $(LIBS) $(OBJS) $(NIST_MAIN) -o $(NIST_PROG) -l$(NIST_SO_PREFIX) -l$(FIPS_SO_PREFIX)


nist_target: $(NIST_SO_PREFIX).o $(NIST_SO) $(NIST_PROG)

clean:
	@rm -rf *.o $(TARGETS)

.c.o:
	$(CC) -c $(CFLAGS) $(RELOC_FLAGS) $<
