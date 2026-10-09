typedef union
{
        int integer;
	char *string;
        struct term *term;
        listTerm *listTerm;
} YYSTYPE;
#define	c_end	257
#define	c_VAR	258
#define	c_INT	259
#define	c_IDENT	260
#define	c_STRING	261
#define	c_FSYM	262
#define	c_integer	263
#define	c_string	264
#define	c_minus	265
#define	c_lbrace	266
#define	c_rbrace	267
#define	c_comma	268
#define	c_dot	269
#define	c_nil	270
#define	c_idf	271
#define	c_underscore	272
#define	c_laccol	273
#define	c_raccol	274


extern YYSTYPE yylval;
