#
# Makefile for compiling Perl under OS/2
#
# Needs emx 0.8f (gcc 2.3.3) or newer
#

# Requires:
# - GNU gdbm library, if missing, remove HAS_GDBM from config.h and -lgdbm
# - GNU ufc library, if missing, remove HAS_CRYPT from config.h and -lufc
#   (ufc = ultra fast crypt, part of GNU glibc 1.04 or above)

# Don't worry about the strange kind of linking, but
# otherwise it wouldn't work (probably bug in ld).

SHELL   = cmd.exe

CC	= gcc -O -funsigned-char
CFLAGS	= -DDEBUGGING
LDFLAGS	= -s
LIBS 	= -lgdbm -lufc
O	= .o

YACC	= bison
YFLAGS	= -d

DEF 	= os2/perl.def
OBJ	= array$O cmd$O cons$O consarg$O doarg$O doio$O dolist$O \
	dump$O eval$O form$O hash$O perl$O perly$O regcomp$O \
	regexec$O stab$O str$O toke$O util$O os2$O suffix$O

.c$O:
	$(CC) -c $(CFLAGS) $<

all: perl.exe perlglob.exe a2p.exe

perl.exe: $(OBJ)
	$(CC) -o perl $(DEF) $(OBJ) $(LIBS)
	emxbind -b -sq perl
	rm -f perl

perlglob.exe: os2/glob.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(DEF) os2/glob.c

a2p.exe:
	cd x2p && $(MAKE)

os2$O: os2/os2.c
	$(CC) -c $(CFLAGS) -I. os2/os2.c

suffix$O: os2/suffix.c
	$(CC) -c $(CFLAGS) -I. os2/suffix.c

$(OBJ): config.h
perl$O str$O cons$O toke$O: perly.h

config.h: os2/config.h
	copy "os2\config.h" "config.h"

perly.c perly.h: perly.y
	$(YACC) $(YFLAGS) -o $*.c $*.y

clean:
	-rm -f perly.c perly.h config.h *$O >nul
