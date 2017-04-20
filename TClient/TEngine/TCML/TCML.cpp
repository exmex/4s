
/*  A Bison parser, made from TCML.y
 by  GNU Bison version 1.25
  */

#define YYBISON 1  /* Identify Bison output.  */

#define alloca

#define yyparse TCMLparse
#define yylex TCMLlex
#define yyerror TCMLerror
#define yylval TCMLlval
#define yychar TCMLchar
#define yydebug TCMLdebug
#define yynerrs TCMLnerrs
#define	STRING	258
#define	INT_NUM	259
#define	BRACKET_OPEN	260
#define	BRACKET_CLOSE	261
#define	SLASH	262
#define	IDENTIFIER	263
#define	SET	264
#define	TCML	265
#define	HEAD	266
#define	FRAME	267
#define	COMPONENT	268
#define	BUTTON	269
#define	SCROLL	270
#define	EDIT	271
#define	LIST	272
#define	COMBO	273
#define	TAB	274
#define	TABCTRL	275
#define	GAUGE	276
#define	IMAGELIST	277
#define	METER	278
#define	DEFINE	279
#define	FONT	280
#define	TIPFONT	281
#define	COLOR	282
#define	ESCAPEMENT	283
#define	ORIENTATION	284
#define	WEIGHT	285
#define	ITALIC	286
#define	UNDERLINE	287
#define	STRIKEOUT	288
#define	CHARSET	289
#define	OUTPRECISION	290
#define	CLIPPRECISION	291
#define	QUALITY	292
#define	PITCHANDFAMILY	293
#define	FACENAME	294
#define	FACECLR	295
#define	ID	296
#define	LCLICK	297
#define	RCLICK	298
#define	DBLCLICK	299
#define	LINEUP	300
#define	LINEDOWN	301
#define	VSCROLL	302
#define	HSCROLL	303
#define	SELCHANGE	304
#define	TABACTIVE	305
#define	ENTER	306
#define	ESC	307
#define	TEXT_DEF	308
#define	SOUND	309
#define	HMARGINE	310
#define	VMARGINE	311
#define	POSX	312
#define	POSY	313
#define	WIDTH	314
#define	HEIGHT	315
#define	DEFAULT	316
#define	DISABLE	317
#define	ENABLE	318
#define	ALIGN	319
#define	BLT	320
#define	DOWN	321
#define	UP	322
#define	HOVER	323
#define	UPLEFT	324
#define	DOWNRIGHT	325
#define	BAR	326
#define	CARET	327
#define	ITEM	328
#define	VERTICAL	329
#define	DROP	330
#define	DOWNLIST	331
#define	BTNTAB	332
#define	SUPER	333
#define	SUB	334
#define	SUPERLEVEL	335
#define	TOOLTIP	336
#define	PASSWORD	337
#define	STATEBUTTON	338

#line 2 "TCML.y"

#include "cinterf.h"
#include <stdio.h>

#include <string.h>

extern int TCMLerror(char* s);
extern int TCMLlex();

extern int tcml_attr;
extern unsigned char tcml_type;
extern char tcml_identifier[MAX_TCML_SYMBOL];

#ifdef _DEBUG
	#define TCML_TRACE				printf
#else
	#define TCML_TRACE				//
#endif

#line 27 "TCML.y"
typedef union 
{
	char string_value[MAX_TCML_SYMBOL];
	int integer_value;
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		154
#define	YYFLAG		32768
#define	YYNTBASE	84

#define YYTRANSLATE(x) ((unsigned)(x) <= 338 ? yytranslate[x] : 114)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
    46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
    56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
    66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
    76,    77,    78,    79,    80,    81,    82,    83
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     4,     6,     9,    11,    13,    15,    17,    19,
    21,    23,    25,    27,    29,    31,    33,    35,    37,    39,
    41,    45,    47,    50,    54,    61,    63,    65,    68,    72,
    77,    81,    83,    85,    87,    89,    91,    93,    95,    97,
    99,   101,   103,   105,   107,   109,   111,   113,   115,   117,
   119,   121,   123,   125,   127,   129,   131,   133,   135,   137,
   139,   141,   143,   145,   147,   149,   151,   153,   155,   157,
   159,   161,   163,   165,   167,   169,   171,   173,   177,   179,
   182,   184,   186,   188,   190,   192,   194,   196,   198,   200,
   202,   205,   209,   212,   216,   219,   223,   226,   228,   231,
   234,   238,   241,   246,   250,   253,   257,   262,   264,   267
};

static const short yyrhs[] = {     3,
     0,     4,     0,     8,     0,     5,    25,     0,    41,     0,
    60,     0,    59,     0,    39,     0,    28,     0,    29,     0,
    30,     0,    31,     0,    32,     0,    33,     0,    34,     0,
    35,     0,    36,     0,    37,     0,    38,     0,    40,     0,
    87,     9,    84,     0,    88,     0,    89,    88,     0,    86,
    89,     6,     0,     5,    24,    85,     9,    84,     6,     0,
    90,     0,    91,     0,    92,    91,     0,     5,    11,     6,
     0,     5,     7,    11,     6,     0,    93,    92,    94,     0,
    41,     0,    42,     0,    43,     0,    44,     0,    45,     0,
    46,     0,    47,     0,    48,     0,    49,     0,    50,     0,
    51,     0,    52,     0,    53,     0,    54,     0,    55,     0,
    56,     0,    57,     0,    58,     0,    59,     0,    60,     0,
    61,     0,    62,     0,    63,     0,    64,     0,    65,     0,
    66,     0,    67,     0,    68,     0,    69,     0,    70,     0,
    71,     0,    72,     0,    73,     0,    74,     0,    75,     0,
    76,     0,    77,     0,    25,     0,    26,     0,    27,     0,
    78,     0,    79,     0,    80,     0,    81,     0,    82,     0,
    83,     0,    96,     9,    84,     0,    97,     0,    98,    97,
     0,    13,     0,    14,     0,    15,     0,    16,     0,    17,
     0,    18,     0,    20,     0,    21,     0,    23,     0,    22,
     0,     5,    99,     0,   100,    98,     6,     0,   100,     6,
     0,     5,     7,    99,     0,   102,     6,     0,   101,   105,
   103,     0,   101,   103,     0,   104,     0,   105,   104,     0,
     5,    12,     0,   106,    98,     6,     0,   106,     6,     0,
     5,     7,    12,     6,     0,   107,   105,   108,     0,   107,
   108,     0,     5,    10,     6,     0,     5,     7,    10,     6,
     0,   109,     0,   112,   109,     0,   110,    95,   112,   111,
     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    59,    63,    68,    77,    82,    86,    90,    94,    98,   102,
   106,   110,   114,   118,   122,   126,   130,   134,   138,   142,
   147,   155,   158,   162,   167,   172,   177,   181,   186,   191,
   196,   201,   206,   210,   214,   218,   222,   226,   230,   234,
   238,   242,   246,   250,   255,   260,   265,   270,   275,   280,
   285,   290,   295,   300,   305,   310,   315,   320,   325,   330,
   335,   340,   345,   350,   355,   361,   366,   371,   376,   381,
   386,   391,   396,   401,   406,   411,   416,   422,   439,   443,
   448,   453,   458,   463,   468,   473,   478,   483,   488,   493,
   499,   509,   513,   518,   528,   533,   537,   542,   546,   551,
   561,   565,   570,   576,   580,   585,   590,   595,   599,   604
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","STRING",
"INT_NUM","BRACKET_OPEN","BRACKET_CLOSE","SLASH","IDENTIFIER","SET","TCML","HEAD",
"FRAME","COMPONENT","BUTTON","SCROLL","EDIT","LIST","COMBO","TAB","TABCTRL",
"GAUGE","IMAGELIST","METER","DEFINE","FONT","TIPFONT","COLOR","ESCAPEMENT","ORIENTATION",
"WEIGHT","ITALIC","UNDERLINE","STRIKEOUT","CHARSET","OUTPRECISION","CLIPPRECISION",
"QUALITY","PITCHANDFAMILY","FACENAME","FACECLR","ID","LCLICK","RCLICK","DBLCLICK",
"LINEUP","LINEDOWN","VSCROLL","HSCROLL","SELCHANGE","TABACTIVE","ENTER","ESC",
"TEXT_DEF","SOUND","HMARGINE","VMARGINE","POSX","POSY","WIDTH","HEIGHT","DEFAULT",
"DISABLE","ENABLE","ALIGN","BLT","DOWN","UP","HOVER","UPLEFT","DOWNRIGHT","BAR",
"CARET","ITEM","VERTICAL","DROP","DOWNLIST","BTNTAB","SUPER","SUB","SUPERLEVEL",
"TOOLTIP","PASSWORD","STATEBUTTON","value","identifier","font_def_start","font_attr",
"set_font_attr","set_font_attr_list","font_def","definition","definition_list",
"start_header","end_header","header","attr","set_attr","set_attr_list","component_type",
"start_component_sign","start_component","end_component_sign","end_component",
"component_def","component_def_list","start_frame_sign","start_frame","end_frame",
"frame_def","start_tcml","end_tcml","frame_def_list","root", NULL
};
#endif

static const short yyr1[] = {     0,
    84,    84,    85,    86,    87,    87,    87,    87,    87,    87,
    87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
    88,    89,    89,    90,    91,    91,    92,    92,    93,    94,
    95,    96,    96,    96,    96,    96,    96,    96,    96,    96,
    96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
    96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
    96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
    96,    96,    96,    96,    96,    96,    96,    97,    98,    98,
    99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
   100,   101,   101,   102,   103,   104,   104,   105,   105,   106,
   107,   107,   108,   109,   109,   110,   111,   112,   112,   113
};

static const short yyr2[] = {     0,
     1,     1,     1,     2,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     3,     1,     2,     3,     6,     1,     1,     2,     3,     4,
     3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     3,     1,     2,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     2,     3,     2,     3,     2,     3,     2,     1,     2,     2,
     3,     2,     4,     3,     2,     3,     4,     1,     2,     4
};

static const short yydefact[] = {     0,
     0,     0,     0,     0,     0,     0,   106,     0,     0,     0,
    26,    27,     0,     0,     0,     0,   108,     0,    29,     0,
     4,     9,    10,    11,    12,    13,    14,    15,    16,    17,
    18,    19,     8,    20,     5,     7,     6,     0,    22,     0,
     0,    28,    31,   100,   102,    69,    70,    71,    32,    33,
    34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
    44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
    54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
    64,    65,    66,    67,    68,    72,    73,    74,    75,    76,
    77,     0,    79,     0,     0,     0,     0,    98,     0,   105,
     0,   109,   110,     3,     0,     0,    24,    23,     0,     0,
   101,    80,     0,    81,    82,    83,    84,    85,    86,    87,
    88,    90,    89,    91,    93,     0,     0,     0,    97,     0,
    99,   104,     0,     0,     1,     2,    21,     0,    78,     0,
    92,     0,    95,    96,     0,     0,    30,   103,    94,   107,
    25,     0,     0,     0
};

static const short yydefgoto[] = {   137,
   105,    10,    38,    39,    40,    11,    12,    13,     5,    43,
     6,    92,    93,    94,   124,    96,    97,   128,   129,    98,
    99,    15,    16,   100,    17,     2,   103,    18,   152
};

static const short yypact[] = {    13,
    15,    21,    22,    16,    24,    25,-32768,    28,    -9,   -27,
-32768,-32768,    26,    70,    -6,    85,-32768,    90,-32768,    88,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,    93,-32768,   306,
    76,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,    94,-32768,    72,   151,   150,   100,-32768,    85,-32768,
    10,-32768,-32768,-32768,    97,    20,-32768,-32768,    96,    20,
-32768,-32768,    98,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,   228,   229,   102,-32768,   100,
-32768,-32768,    99,    20,-32768,-32768,-32768,   153,-32768,   154,
-32768,    71,-32768,-32768,   155,   156,-32768,-32768,-32768,-32768,
-32768,   157,   163,-32768
};

static const short yypgoto[] = {   -30,
-32768,-32768,-32768,   130,-32768,-32768,   165,-32768,-32768,-32768,
-32768,-32768,   -15,    83,    38,-32768,-32768,-32768,    51,   -18,
    86,-32768,-32768,    87,   164,-32768,-32768,-32768,-32768
};


#define	YYLAST		366


static const short yytable[] = {    45,
    22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
    32,    33,    34,    35,    20,    21,   133,     1,    46,    47,
    48,    44,   135,   136,     3,     4,     8,     7,     9,    14,
    41,    36,    37,    19,    49,    50,    51,    52,    53,    54,
    55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
    65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
    75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
    85,    86,    87,    88,    89,    90,    91,   111,   112,   139,
   131,    44,   109,   114,   115,   116,   117,   118,   119,    95,
   120,   121,   122,   123,   101,   104,    46,    47,    48,    20,
    21,   106,   110,   146,   127,   134,   138,   143,   145,   140,
   112,   131,    49,    50,    51,    52,    53,    54,    55,    56,
    57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
    67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
    77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
    87,    88,    89,    90,    91,   125,   153,   113,   147,   148,
   150,   151,   154,   114,   115,   116,   117,   118,   119,   108,
   120,   121,   122,   123,    46,    47,    48,    42,   126,   149,
   144,   102,   130,     0,     0,   132,     0,     0,     0,     0,
    49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
    59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
    69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
    79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
    89,    90,    91,   141,     0,   142,     0,     0,     0,     0,
     0,   114,   115,   116,   117,   118,   119,     0,   120,   121,
   122,   123,    46,    47,    48,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    49,    50,
    51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
    61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
    71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
    81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
    91,   107,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    22,    23,    24,    25,    26,    27,    28,
    29,    30,    31,    32,    33,    34,    35,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    36,    37
};

static const short yycheck[] = {     6,
    28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
    38,    39,    40,    41,    24,    25,     7,     5,    25,    26,
    27,    12,     3,     4,    10,     5,    11,     6,     5,     5,
     5,    59,    60,     6,    41,    42,    43,    44,    45,    46,
    47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
    57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
    67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
    77,    78,    79,    80,    81,    82,    83,     6,    94,   110,
    99,    12,     7,    13,    14,    15,    16,    17,    18,     5,
    20,    21,    22,    23,     5,     8,    25,    26,    27,    24,
    25,     9,     9,   134,     5,     9,    11,     6,    10,    12,
   126,   130,    41,    42,    43,    44,    45,    46,    47,    48,
    49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
    59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
    69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
    79,    80,    81,    82,    83,     6,     0,     7,     6,     6,
     6,     6,     0,    13,    14,    15,    16,    17,    18,    40,
    20,    21,    22,    23,    25,    26,    27,    13,    96,   142,
   130,    18,    97,    -1,    -1,    99,    -1,    -1,    -1,    -1,
    41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
    51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
    61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
    71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
    81,    82,    83,     6,    -1,     7,    -1,    -1,    -1,    -1,
    -1,    13,    14,    15,    16,    17,    18,    -1,    20,    21,
    22,    23,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    42,
    43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
    53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
    63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
    73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
    83,     6,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    28,    29,    30,    31,    32,    33,    34,
    35,    36,    37,    38,    39,    40,    41,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    59,    60
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif


/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 60 "TCML.y"
{
						TCML_TRACE("value			->STRING\n");
					;
    break;}
case 2:
#line 64 "TCML.y"
{
						TCML_TRACE("value			->INT_NUM\n");
					;
    break;}
case 3:
#line 69 "TCML.y"
{
						TCML_TRACE("identifier		->IDENTIFIER\n");
						strcpy(tcml_identifier, TCMLlval.string_value);
					
						TCML_TRACE(":identifier %s\n", tcml_identifier);
					;
    break;}
case 4:
#line 78 "TCML.y"
{
						add_tcml_logfont();
					;
    break;}
case 5:
#line 83 "TCML.y"
{
						tcml_attr = ID;	
					;
    break;}
case 6:
#line 87 "TCML.y"
{
						tcml_attr = HEIGHT;	
					;
    break;}
case 7:
#line 91 "TCML.y"
{
						tcml_attr = WIDTH;	
					;
    break;}
case 8:
#line 95 "TCML.y"
{
						tcml_attr = FACENAME;	
					;
    break;}
case 9:
#line 99 "TCML.y"
{
						tcml_attr = ESCAPEMENT;	
					;
    break;}
case 10:
#line 103 "TCML.y"
{
						tcml_attr = ORIENTATION;	
					;
    break;}
case 11:
#line 107 "TCML.y"
{
						tcml_attr = WEIGHT;	
					;
    break;}
case 12:
#line 111 "TCML.y"
{
						tcml_attr = ITALIC;	
					;
    break;}
case 13:
#line 115 "TCML.y"
{
						tcml_attr = UNDERLINE;	
					;
    break;}
case 14:
#line 119 "TCML.y"
{
						tcml_attr = STRIKEOUT;	
					;
    break;}
case 15:
#line 123 "TCML.y"
{
						tcml_attr = CHARSET;	
					;
    break;}
case 16:
#line 127 "TCML.y"
{
						tcml_attr = OUTPRECISION;	
					;
    break;}
case 17:
#line 131 "TCML.y"
{
						tcml_attr = CLIPPRECISION;	
					;
    break;}
case 18:
#line 135 "TCML.y"
{
						tcml_attr = QUALITY;	
					;
    break;}
case 19:
#line 139 "TCML.y"
{
						tcml_attr = PITCHANDFAMILY;	
					;
    break;}
case 20:
#line 143 "TCML.y"
{
						tcml_attr = FACECLR;
					;
    break;}
case 21:
#line 148 "TCML.y"
{
						if(0!=set_font_attr(tcml_attr))
						{
							return -5;
						}
					;
    break;}
case 22:
#line 156 "TCML.y"
{
					;
    break;}
case 23:
#line 159 "TCML.y"
{
					;
    break;}
case 24:
#line 163 "TCML.y"
{
						
					;
    break;}
case 25:
#line 168 "TCML.y"
{
						TCML_TRACE("definition		->BRACKET_OPEN DEFINE IDENTIFIER SET value BRACKET_CLOSE\n");
						define(tcml_identifier, TCMLlval.integer_value);
					;
    break;}
case 26:
#line 173 "TCML.y"
{

					;
    break;}
case 27:
#line 178 "TCML.y"
{
						TCML_TRACE("definition_list		->definition\n");
					;
    break;}
case 28:
#line 182 "TCML.y"
{
						TCML_TRACE("defintion_list		->definition_list definition\n");
					;
    break;}
case 29:
#line 187 "TCML.y"
{
						TCML_TRACE("start_header		->BRACKET_OPEN HEAD BRACKET_CLOSE\n");
					;
    break;}
case 30:
#line 192 "TCML.y"
{
						TCML_TRACE("end_header		->BRACKET_OPEN SLASH HEAD BRACKET_CLOSE\n");
					;
    break;}
case 31:
#line 197 "TCML.y"
{
						TCML_TRACE("header			->start_header definition_list end_header\n");
					;
    break;}
case 32:
#line 202 "TCML.y"
{
						TCML_TRACE("attr			->ID\n");
						tcml_attr = ID;
					;
    break;}
case 33:
#line 207 "TCML.y"
{
						tcml_attr = LCLICK;
					;
    break;}
case 34:
#line 211 "TCML.y"
{
						tcml_attr = RCLICK;
					;
    break;}
case 35:
#line 215 "TCML.y"
{
						tcml_attr = DBLCLICK;
					;
    break;}
case 36:
#line 219 "TCML.y"
{
						tcml_attr = LINEUP;
					;
    break;}
case 37:
#line 223 "TCML.y"
{
						tcml_attr = LINEDOWN;
					;
    break;}
case 38:
#line 227 "TCML.y"
{
						tcml_attr = VSCROLL;
					;
    break;}
case 39:
#line 231 "TCML.y"
{
						tcml_attr = HSCROLL;
					;
    break;}
case 40:
#line 235 "TCML.y"
{
						tcml_attr = SELCHANGE;
					;
    break;}
case 41:
#line 239 "TCML.y"
{
						tcml_attr = TABACTIVE;
					;
    break;}
case 42:
#line 243 "TCML.y"
{
						tcml_attr = ENTER;
					;
    break;}
case 43:
#line 247 "TCML.y"
{
						tcml_attr = ESC;
					;
    break;}
case 44:
#line 251 "TCML.y"
{
						TCML_TRACE("attr			->TEXT_DEF\n");
						tcml_attr = TEXT_DEF;
					;
    break;}
case 45:
#line 256 "TCML.y"
{
						TCML_TRACE("attr			->SOUND\n");
						tcml_attr = SOUND;
					;
    break;}
case 46:
#line 261 "TCML.y"
{
						TCML_TRACE("attr			->HMARGINE\n");
						tcml_attr = HMARGINE;
					;
    break;}
case 47:
#line 266 "TCML.y"
{
						TCML_TRACE("attr			->VMARGINE\n");
						tcml_attr = VMARGINE;
					;
    break;}
case 48:
#line 271 "TCML.y"
{
						TCML_TRACE("attr			->POSX\n");
						tcml_attr = POSX;
					;
    break;}
case 49:
#line 276 "TCML.y"
{
						TCML_TRACE("attr			->POSY\n");
						tcml_attr = POSY;
					;
    break;}
case 50:
#line 281 "TCML.y"
{
						TCML_TRACE("attr			->WIDTH\n");
						tcml_attr = WIDTH;
					;
    break;}
case 51:
#line 286 "TCML.y"
{
						TCML_TRACE("attr			->HEIGHT\n");
						tcml_attr = HEIGHT;
					;
    break;}
case 52:
#line 291 "TCML.y"
{
						TCML_TRACE("attr			->DEFAULT\n");
						tcml_attr = DEFAULT;
					;
    break;}
case 53:
#line 296 "TCML.y"
{
						TCML_TRACE("attr			->DISABLE\n");
						tcml_attr = DISABLE;
					;
    break;}
case 54:
#line 301 "TCML.y"
{
						TCML_TRACE("attr			->ENABLE\n");
						tcml_attr = ENABLE;
					;
    break;}
case 55:
#line 306 "TCML.y"
{
						TCML_TRACE("attr			->ALIGN\n");
						tcml_attr = ALIGN;
					;
    break;}
case 56:
#line 311 "TCML.y"
{
						TCML_TRACE("attr			->BLT\n");
						tcml_attr = BLT;
					;
    break;}
case 57:
#line 316 "TCML.y"
{
						TCML_TRACE("attr			->DOWN\n");
						tcml_attr = DOWN;
					;
    break;}
case 58:
#line 321 "TCML.y"
{
						TCML_TRACE("attr			->UP\n");
						tcml_attr = UP;
					;
    break;}
case 59:
#line 326 "TCML.y"
{
						TCML_TRACE("attr			->HOVER\n");
						tcml_attr = HOVER;
					;
    break;}
case 60:
#line 331 "TCML.y"
{
						TCML_TRACE("attr			->UPLEFT\n");
						tcml_attr = UPLEFT;
					;
    break;}
case 61:
#line 336 "TCML.y"
{
						TCML_TRACE("attr			->DOWNRIGHT\n");
						tcml_attr = DOWNRIGHT;
					;
    break;}
case 62:
#line 341 "TCML.y"
{
						TCML_TRACE("attr			->BAR\n");
						tcml_attr = BAR;
					;
    break;}
case 63:
#line 346 "TCML.y"
{
						TCML_TRACE("attr			->CARET\n");
						tcml_attr = CARET;
					;
    break;}
case 64:
#line 351 "TCML.y"
{
						TCML_TRACE("attr			->ITEM\n");
						tcml_attr = ITEM;
					;
    break;}
case 65:
#line 356 "TCML.y"
{
						TCML_TRACE("attr			->VERTICAL\n");
						tcml_attr = VERTICAL;
					;
    break;}
case 66:
#line 362 "TCML.y"
{
						TCML_TRACE("attr			->DROP\n");
						tcml_attr = DROP;
					;
    break;}
case 67:
#line 367 "TCML.y"
{
						TCML_TRACE("attr			->DOWNLIST\n");
						tcml_attr = DOWNLIST;
					;
    break;}
case 68:
#line 372 "TCML.y"
{
						TCML_TRACE("attr			->BTNTAB\n");
						tcml_attr = BTNTAB;
					;
    break;}
case 69:
#line 377 "TCML.y"
{
						TCML_TRACE("attr			->FONT\n");
						tcml_attr = FONT;
					;
    break;}
case 70:
#line 382 "TCML.y"
{
						TCML_TRACE("attr			->TIPFONT\n");
						tcml_attr = TIPFONT;
					;
    break;}
case 71:
#line 387 "TCML.y"
{
						TCML_TRACE("attr			->COLOR\n");
						tcml_attr = COLOR;
					;
    break;}
case 72:
#line 392 "TCML.y"
{
						TCML_TRACE("attr			->SUPER\n");
						tcml_attr = SUPER;
					;
    break;}
case 73:
#line 397 "TCML.y"
{
						TCML_TRACE("attr			->SUB\n");
						tcml_attr = SUB;
					;
    break;}
case 74:
#line 402 "TCML.y"
{
						TCML_TRACE("attr			->SUPERLEVEL\n");
						tcml_attr = SUPERLEVEL;
					;
    break;}
case 75:
#line 407 "TCML.y"
{
						TCML_TRACE("attr			->TOOLTIP\n");
						tcml_attr = TOOLTIP;
					;
    break;}
case 76:
#line 412 "TCML.y"
{
						TCML_TRACE("attr			->PASSWORD\n");
						tcml_attr = PASSWORD;
					;
    break;}
case 77:
#line 417 "TCML.y"
{
						TCML_TRACE("attr			->STATEBUTTON\n");
						tcml_attr = STATEBUTTON;
					;
    break;}
case 78:
#line 423 "TCML.y"
{
						TCML_TRACE("set_attr		->attr SET value\n");
						{
							int ret;
							ret = set_attr(tcml_attr);
							switch(ret)
							{
							case -1:TCML_TRACE("Unknown attribute name\n");break;
							case -2:TCML_TRACE("Unknown attribute value\n");break;
							}

							if(0!=ret)
								return ret;
						}
					;
    break;}
case 79:
#line 440 "TCML.y"
{
						TCML_TRACE("set_attr_list		->set_attr\n");
					;
    break;}
case 80:
#line 444 "TCML.y"
{
						TCML_TRACE("set_attr_list		->set_attr_list set_attr\n");
					;
    break;}
case 81:
#line 449 "TCML.y"
{
						TCML_TRACE("component_type		->COMPONENT\n");
						tcml_type = TCML_TYPE_COMPONENT;
					;
    break;}
case 82:
#line 454 "TCML.y"
{
						TCML_TRACE("component_type		->BUTTON\n");
						tcml_type = TCML_TYPE_BUTTON;
					;
    break;}
case 83:
#line 459 "TCML.y"
{
						TCML_TRACE("component_type		->SCROLL\n");
						tcml_type = TCML_TYPE_SCROLL;
					;
    break;}
case 84:
#line 464 "TCML.y"
{
						TCML_TRACE("component_type		->EDIT\n");
						tcml_type = TCML_TYPE_EDIT;
					;
    break;}
case 85:
#line 469 "TCML.y"
{
						TCML_TRACE("component_type		->LIST\n");
						tcml_type = TCML_TYPE_LIST;
					;
    break;}
case 86:
#line 474 "TCML.y"
{
						TCML_TRACE("component_type		->COMBO\n");
						tcml_type = TCML_TYPE_COMBO;
					;
    break;}
case 87:
#line 479 "TCML.y"
{
						TCML_TRACE("component_type		->TABCTRL\n");
						tcml_type = TCML_TYPE_TABCTRL;
					;
    break;}
case 88:
#line 484 "TCML.y"
{
						TCML_TRACE("component_type		->GAUGE\n");
						tcml_type = TCML_TYPE_GAUGE;
					;
    break;}
case 89:
#line 489 "TCML.y"
{
						TCML_TRACE("component_type		->METER\n");
						tcml_type = TCML_TYPE_METER;
					;
    break;}
case 90:
#line 494 "TCML.y"
{
						TCML_TRACE("component_type		->IMAGELIST\n");
						tcml_type = TCML_TYPE_IMAGELIST;
					;
    break;}
case 91:
#line 500 "TCML.y"
{
						TCML_TRACE("start_component_sign	->BRACKET_OPEN component_type\n");
						if(0!=add_component(tcml_type))
						{
							TCML_TRACE("compile error 002\n");
							return -1;
						}
					;
    break;}
case 92:
#line 510 "TCML.y"
{
						TCML_TRACE("start_component		->start_component_sign set_attr_list BRACKET_CLOSE\n");
					;
    break;}
case 93:
#line 514 "TCML.y"
{
						TCML_TRACE("start_component		->start_component_sign BRACKET_CLOSE\n"); 
					;
    break;}
case 94:
#line 519 "TCML.y"
{
						TCML_TRACE("end_component_sign	->BRACKET_OPEN SLASH component_type\n");
						if(0!=close_current_component(tcml_type))
						{
							TCML_TRACE("compile error 001\n");
							return -1;							
						}
					;
    break;}
case 95:
#line 529 "TCML.y"
{
						TCML_TRACE("end_component		->end_component_sign BRACKET_CLOSE\n");
					;
    break;}
case 96:
#line 534 "TCML.y"
{
						TCML_TRACE("component_def		->start_component component_def_list end_component\n");
					;
    break;}
case 97:
#line 538 "TCML.y"
{
						TCML_TRACE("component_def		->start_component end_component\n");
					;
    break;}
case 98:
#line 543 "TCML.y"
{
						TCML_TRACE("component_def_list	->component_def\n");
					;
    break;}
case 99:
#line 547 "TCML.y"
{
						TCML_TRACE("component_def_list	->component_def_list component_def\n");
					;
    break;}
case 100:
#line 552 "TCML.y"
{
						TCML_TRACE("start_frame_sign		->BRACKET_OPEN FRAME\n");
						if(0!=add_frame())
						{
							TCML_TRACE("compile error 004\n");
							return -1;
						}
					;
    break;}
case 101:
#line 562 "TCML.y"
{
						TCML_TRACE("start_frame_sign	->BRACKET_OPEN FRAME set_attr_list BRACKET_CLOSE\n");	
					;
    break;}
case 102:
#line 566 "TCML.y"
{
						TCML_TRACE("start_frame_sign	->BRACKET_OPEN FRAME BRACKET_CLOSE\n");
					;
    break;}
case 103:
#line 571 "TCML.y"
{
						TCML_TRACE("end_frame		->BRACKET_OPEN SLASH FRAME BRACKET_CLOSE\n");

					;
    break;}
case 104:
#line 577 "TCML.y"
{
						TCML_TRACE("frame_def		->start_frame component_def_list end_frame\n");
					;
    break;}
case 105:
#line 581 "TCML.y"
{
						TCML_TRACE("frame_def		->start_frame end_frame\n");
					;
    break;}
case 106:
#line 586 "TCML.y"
{
						TCML_TRACE("start_tcml		->BRACKET_OPEN TCML BRACKET_CLOSE\n");
					;
    break;}
case 107:
#line 591 "TCML.y"
{
						TCML_TRACE("end_tcml		->BRACKET_OPEN SLASH TCML BRACKET_CLOSE\n");
					;
    break;}
case 108:
#line 596 "TCML.y"
{
						TCML_TRACE("frame_def_list		->frame_def\n");
					;
    break;}
case 109:
#line 600 "TCML.y"
{
						TCML_TRACE("frame_def_list		->frame_def_list frame_def\n");
					;
    break;}
case 110:
#line 605 "TCML.y"
{
						TCML_TRACE("root			->start_tcml header frame_def_list end_tcml\n");
						return 0;
					;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 610 "TCML.y"


/* programs */

/*
 * ???
 */
int
TCMLwrap()
{
  return 1;
}

/*
 * Report an error situation discovered in a production
 *
 * This does not do anything since we report all error situations through
 * idl_global->err() operations
 */
int
TCMLerror(char *s)
{
    fprintf(stderr, "Syntax error: %s\n", s);
	return 0;
}
