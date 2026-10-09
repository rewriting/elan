typedef union {
  int y_int;
  TERM *y_term;
  TERM_LIST *y_term_list;
} YYSTYPE;
#define	IDENTIFIER	257


extern YYSTYPE yylval;
