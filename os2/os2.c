/* $RCSfile: os2.c,v $$Revision: 4.0.1.2 $$Date: 92/06/08 14:32:30 $
 *
 *    (C) Copyright 1989, 1990 Diomidis Spinellis.
 *
 *    You may distribute under the terms of either the GNU General Public
 *    License or the Artistic License, as specified in the README file.
 *
 * $Log:	os2.c,v $
 * Revision 4.0.1.2  92/06/08  14:32:30  lwall
 * patch20: new OS/2 support
 *
 * Revision 4.0.1.1  91/06/07  11:23:06  lwall
 * patch4: new copyright notice
 *
 * Revision 4.0  91/03/20  01:36:21  lwall
 * 4.0 baseline.
 *
 * Revision 3.0.1.2  90/11/10  01:42:38  lwall
 * patch38: more msdos/os2 upgrades
 *
 * Revision 3.0.1.1  90/10/15  17:49:55  lwall
 * patch29: Initial revision
 *
 * Revision 3.0.1.1  90/03/27  16:10:41  lwall
 * patch16: MSDOS support
 *
 * Revision 1.1  90/03/18  20:32:01  dds
 * Initial revision
 *
 */

#define INCL_DOS
#define INCL_NOPM
#include <os2.h>

#ifdef __32BIT__
#define DosCwait DosWaitChild
#endif

/*
 * Various Unix compatibility functions for OS/2
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <process.h>

#include "EXTERN.h"
#include "perl.h"

char os2_scriptdir[CCHMAXPATH] = "c:/lib/perl";

/* dummies */

int userinit()
{ return -1; }

int syscall()
{ return -1; }

/* priorities */

int setpriority(int class, int pid, int val)
{
  int flag = 0;

  if ( _osmode == DOS_MODE )
    return 0;

  if ( pid < 0 )
  {
    flag++;
    pid = -pid;
  }

  return DosSetPrty(flag ? PRTYS_PROCESSTREE : PRTYS_PROCESS, class, val, pid);
}

int getpriority(int which /* ignored */, int pid)
{
  int val;
#ifdef __32BIT__
  PTIB ptib;
  PPIB ppib;
#endif

  if ( _osmode == DOS_MODE )
    return 0;

#ifdef __32BIT__
  if ( DosGetInfoBlocks(&ptib, &ppib) )
    return -1;
  else
    return ptib -> tib_ptib2 -> tib2_ulpri;
#else
  if ( DosGetPrty(PRTYS_PROCESS, &val, pid) )
    return -1;
  else
    return val;
#endif
}


/* wait for specific pid */

int wait4pid(int pid, int *status, int flags)
{
  RESULTCODES res;
  int endpid, rc;
  if ( _osmode == DOS_MODE )
    return -1;
  if ( DosCwait(DCWA_PROCESS, flags ? DCWW_NOWAIT : DCWW_WAIT,
                &res, &endpid, pid) )
    return -1;
  *status = res.codeResult;
  return endpid;
}

/* Just pretend that everyone is a superuser */

int setuid()
{ return 0; }

int setgid()
{ return 0; }

int getuid(void)
{ return 0; }

int geteuid(void)
{ return 0; }

int getgid(void)
{ return 0; }

int getegid(void)
{ return 0; }

/*
 * The following code is based on the do_exec and do_aexec functions
 * in file doio.c
 */
 
int
do_aspawn(really,arglast)
STR *really;
int *arglast;
{
    register STR **st = stack->ary_array;
    register int sp = arglast[1];
    register int items = arglast[2] - sp;
    register char **a;
    char **argv;
    char *tmps;
    int status;

    if (items) {
	New(1101,argv, items+1, char*);
	a = argv;
	for (st += ++sp; items > 0; items--,st++) {
	    if (*st)
		*a++ = str_get(*st);
	    else
		*a++ = "";
	}
	*a = Nullch;
	if (really && *(tmps = str_get(really)))
	    status = spawnvp(P_WAIT,tmps,argv);
	else
	    status = spawnvp(P_WAIT,argv[0],argv);
	Safefree(argv);
    }
    return status;
}

int
do_spawn(cmd)
char *cmd;
{
    register char **a;
    register char *s, *t;
    char **argv;
    char flags[10];
    int status;
    char *shell, *cmd2;

    /* save an extra exec if possible */
    if ((shell = getenv("COMSPEC")) == 0)
	shell = "cmd.exe";

    /* see if there are shell metacharacters in it */
    if (strchr(cmd, '>') || strchr(cmd, '<') || strchr(cmd, '|')
        || strchr(cmd, '&') || strchr(cmd, '^'))
	    return spawnlp(P_WAIT,shell,shell,"/C",cmd,(char*)0);

    New(1102,argv, strlen(cmd) / 2 + 2, char*);
    New(1103,cmd2, strlen(cmd) + 2, char);
    s = cmd;
    t = cmd2;
    a = argv;

    while (*s) {
	while (*s && isspace(*s)) s++;
	if (*s)
	    *(a++) = t;
	while (*s && !isspace(*s))
	  if (*s == '\\') {
	    s++;
	    *t++ = *s++;
	  }
	  else if (*s == '"') {
	    s++;
	    while (*s && *s != '"') *t++ = *s++;
	    s++;
	  }
	  else
	    *t++ = *s++;
	*t++ = '\0';
    }

    *a = Nullch;

    if (argv[0])
	if ((status = spawnvp(P_WAIT,argv[0],argv)) == -1)
	    status = spawnlp(P_WAIT,shell,shell,"/C",cmd,(char*)0);

    Safefree(cmd2);
    Safefree(argv);

    return status;
}

usage(char *myname)
{
  printf("\nUsage: %s [-acdnpPsSuvw] [-0[octal]] [-l[octal]] [-Dnumber]"
	 "\n            [-i[extension]] [-Idirectory] [-x[directory]]"
         "\n            [-e \"command\"] [filename] [arguments]\n", myname);

  printf("\n  -a  autosplit mode with -n or -p"
         "\n  -c  syntaxcheck only"
         "\n  -d  run scripts under debugger"
         "\n  -n  assume 'while (<>) { ...script... }' loop arround your script"
         "\n  -p  assume loop like -n but print line also like sed"
         "\n  -P  run script through C preprocessor befor compilation"
         "\n  -s  enable some switch parsing for switches after script name"
         "\n  -S  look for the script using PATH environment variable"
         "\n  -u  dump core after compiling the script");
  printf("\n  -v  print version number and patchlevel of perl"
         "\n  -w  turn warnings on for compilation of your script\n"
         "\n  -0[octal]       specify record separator (0, if no argument)"
         "\n  -l[octal]       automatic line-ending processing ($/, if no argument)"
         "\n  -Dnumber        set debugging flags (argument is a bit mask)"
         "\n  -i[extension]   edit <> files in place (make backup if extension supplied)"
         "\n  -Idirectory     specify include directory in conjunction with -P"
         "\n  -e command      one line of script, multiple -e options are allowed"
         "\n                  [filename] can be ommitted, when -e is used"
         "\n  -x[directory]   strip off text before #!perl line and perhaps cd to directory\n");
}

void os2init(int *argc, char ***argv)
{
  char *ptr;

  if ( (ptr = getenv("PERLDIR")) != NULL )
    strcpy(os2_scriptdir, ptr);
  
  _response(argc, argv);
  _wildcard(argc, argv);
}
