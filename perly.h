typedef union {
    int	ival;
    char *cval;
    ARG *arg;
    CMD *cmdval;
    struct compcmd compval;
    STAB *stabval;
    FCMD *formval;
} YYSTYPE;
#define	WORD	258
#define	LABEL	259
#define	APPEND	260
#define	OPEN	261
#define	SSELECT	262
#define	LOOPEX	263
#define	DOTDOT	264
#define	USING	265
#define	FORMAT	266
#define	DO	267
#define	SHIFT	268
#define	PUSH	269
#define	POP	270
#define	LVALFUN	271
#define	WHILE	272
#define	UNTIL	273
#define	IF	274
#define	UNLESS	275
#define	ELSE	276
#define	ELSIF	277
#define	CONTINUE	278
#define	SPLIT	279
#define	FLIST	280
#define	FOR	281
#define	FILOP	282
#define	FILOP2	283
#define	FILOP3	284
#define	FILOP4	285
#define	FILOP22	286
#define	FILOP25	287
#define	FUNC0	288
#define	FUNC1	289
#define	FUNC2	290
#define	FUNC2x	291
#define	FUNC3	292
#define	FUNC4	293
#define	FUNC5	294
#define	HSHFUN	295
#define	HSHFUN3	296
#define	FLIST2	297
#define	SUB	298
#define	FILETEST	299
#define	LOCAL	300
#define	DELETE	301
#define	RELOP	302
#define	EQOP	303
#define	MULOP	304
#define	ADDOP	305
#define	PACKAGE	306
#define	AMPER	307
#define	FORMLIST	308
#define	REG	309
#define	ARYLEN	310
#define	ARY	311
#define	HSH	312
#define	STAR	313
#define	SUBST	314
#define	PATTERN	315
#define	RSTRING	316
#define	TRANS	317
#define	LISTOP	318
#define	OROR	319
#define	ANDAND	320
#define	UNIOP	321
#define	LS	322
#define	RS	323
#define	MATCH	324
#define	NMATCH	325
#define	UMINUS	326
#define	POW	327
#define	INC	328
#define	DEC	329


extern YYSTYPE yylval;
