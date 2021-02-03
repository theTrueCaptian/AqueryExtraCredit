
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "parser/aquery.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "symtable.h"  /* manages the symbol table, to enable meta data lookup */	
#include "../ast/ast.h"       /* builds ast for parser */
#include "../ast/ast_print.h" /* provides dot printing of ast */
#include "../optimizer/optimizer.h" /* optimizing for query plans */
#include "../codegen/codegen.h" /* generates q code */
	
#define YYDEBUG 1
#define ERRORCOLOR  "\x1B[32m"
#define SAFE_ORDER_DEP(x) ((x) != NULL && (x)->order_dep);

extern int yyleng;
extern FILE *yyin;
extern int yylex();
extern int yyparse();


//for debugging purposes, defined in flex
extern int line_num; 
extern int col_num;

//For reporting query issues
int query_line_num;
int query_col_num;

//Symbol table
Symtable *env;	/* global symbol table: stack of hash tables */
Symentry *entry; /* place holder for entry pointers to perform modifications ad-hoc */

//AST
TopLevelNode* ast; /* holds any asts created during compilation */
LogicalQueryNode *curr_order;

//Code generation
FILE *DEST_FILE;
int GEN_CODE = 0;


void yyerror(const char *);

//Command line options
int optim_level = 0; //level of optimization in ast
int silence_warnings = 0;



/* Line 189 of yacc.c  */
#line 125 "parser/aquery.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     WITH = 258,
     UC_AS = 259,
     SELECT = 260,
     FROM = 261,
     ASSUMING = 262,
     ASC = 263,
     DESC = 264,
     WHERE = 265,
     GROUP = 266,
     BY = 267,
     HAVING = 268,
     LC_AS = 269,
     ROWID = 270,
     ODD = 271,
     EVEN = 272,
     EVERY = 273,
     FLATTEN = 274,
     CONCATENATE = 275,
     JOIN = 276,
     INNER = 277,
     OUTER = 278,
     FULL = 279,
     ON = 280,
     USING = 281,
     CREATE = 282,
     TABLE = 283,
     VIEW = 284,
     UPDATE = 285,
     SET = 286,
     INSERT = 287,
     INTO = 288,
     VALUES = 289,
     DELETE = 290,
     EXEC = 291,
     ARRAYS = 292,
     SHOW = 293,
     LOAD = 294,
     DATA = 295,
     INFILE = 296,
     FIELDS = 297,
     TERMINATED = 298,
     OUTFILE = 299,
     AND = 300,
     OR = 301,
     IS = 302,
     NOT = 303,
     BETWEEN = 304,
     IN = 305,
     LIKE = 306,
     NULL_KEYWORD = 307,
     OVERLAPS = 308,
     CASE = 309,
     END = 310,
     WHEN = 311,
     THEN = 312,
     ELSE = 313,
     TYPE_INT = 314,
     TYPE_FLOAT = 315,
     TYPE_STRING = 316,
     TYPE_DATE = 317,
     TYPE_TIMESTAMP = 318,
     TYPE_BOOLEAN = 319,
     TYPE_HEX = 320,
     FUNCTION = 321,
     LOCAL_ASSIGN = 322,
     ABS = 323,
     AVG = 324,
     COUNT = 325,
     DISTINCT = 326,
     DROP = 327,
     FILL = 328,
     FIRST = 329,
     LAST = 330,
     MAX = 331,
     MIN = 332,
     MOD = 333,
     NEXT = 334,
     PREV = 335,
     PRD = 336,
     REV = 337,
     SUM = 338,
     SQRT = 339,
     STDDEV = 340,
     AVGS = 341,
     DELTAS = 342,
     MAXS = 343,
     MINS = 344,
     PRDS = 345,
     SUMS = 346,
     RATIOS = 347,
     MAKENULL = 348,
     FLOAT = 349,
     INT = 350,
     TRUE = 351,
     FALSE = 352,
     DATE = 353,
     TIMESTAMP = 354,
     HEX = 355,
     ID = 356,
     STRING = 357,
     EXP_OP = 358,
     TIMES_OP = 359,
     DIV_OP = 360,
     PLUS_OP = 361,
     MINUS_OP = 362,
     LE_OP = 363,
     GE_OP = 364,
     L_OP = 365,
     G_OP = 366,
     EQ_OP = 367,
     NEQ_OP = 368,
     AND_OP = 369,
     OR_OP = 370,
     VERBATIM_Q_CODE = 371
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 191 "parser/aquery.y"

  int   intval;
  float floatval;
  char* str;
  struct ExprNode *exprnode;
  struct UDFDefNode *udfdef;
  struct IDListNode *idlist;
  struct LocalVarDefNode *localvardef;
  struct UDFBodyNode *udfbody;
  struct FullQueryNode *fullquery; 
  struct LocalQueryNode *localquery;
  struct LogicalQueryNode *plan;
  struct OrderNode *order;
  struct NamedExprNode *namedexpr;
  struct InsertNode *insert;
  struct CreateNode *create;
  struct CreateSourceNode *createsrc;
  struct SchemaNode *schema;
  struct LoadNode *load;
  struct DumpNode *dump;
  struct TopLevelNode *top;



/* Line 214 of yacc.c  */
#line 302 "parser/aquery.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 314 "parser/aquery.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  30
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   800

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  126
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  102
/* YYNRULES -- Number of rules.  */
#define YYNRULES  237
/* YYNRULES -- Number of states.  */
#define YYNSTATES  432

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   371

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     117,   118,     2,     2,   119,     2,   120,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   123,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   124,     2,   125,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   121,     2,   122,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    11,    14,    17,    20,    23,
      26,    29,    32,    33,    35,    36,    40,    42,    46,    50,
      51,    54,    55,    62,    66,    67,    70,    74,    75,    77,
      80,    86,    90,    94,    96,   100,   101,   102,   106,   109,
     110,   113,   116,   119,   121,   125,   129,   130,   133,   134,
     136,   138,   139,   145,   150,   154,   155,   159,   161,   164,
     167,   171,   172,   174,   178,   180,   183,   185,   189,   191,
     193,   195,   197,   199,   201,   203,   205,   211,   218,   222,
     227,   231,   233,   237,   242,   247,   251,   255,   260,   262,
     264,   268,   274,   276,   282,   288,   295,   302,   305,   310,
     313,   315,   320,   325,   328,   332,   334,   338,   341,   345,
     346,   351,   356,   359,   363,   366,   369,   373,   374,   376,
     378,   380,   382,   384,   386,   388,   396,   399,   403,   404,
     408,   415,   419,   420,   422,   427,   433,   440,   445,   457,
     466,   467,   477,   480,   481,   485,   486,   489,   490,   494,
     495,   497,   499,   501,   505,   507,   509,   511,   513,   515,
     517,   519,   521,   525,   527,   533,   535,   536,   539,   542,
     543,   548,   551,   553,   555,   557,   561,   563,   565,   570,
     575,   579,   584,   588,   590,   592,   595,   597,   599,   601,
     603,   605,   607,   609,   611,   613,   615,   617,   619,   621,
     623,   625,   627,   629,   631,   633,   635,   637,   639,   641,
     643,   645,   647,   649,   652,   654,   658,   660,   664,   668,
     670,   674,   678,   680,   684,   688,   692,   696,   698,   702,
     706,   708,   712,   714,   718,   720,   723,   727
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     127,     0,    -1,   128,    -1,   129,   128,    -1,   181,   128,
      -1,   191,   128,    -1,   187,   128,    -1,   194,   128,    -1,
     195,   128,    -1,   196,   128,    -1,   197,   128,    -1,   116,
     128,    -1,    -1,   130,    -1,    -1,   131,   133,   132,    -1,
     139,    -1,    36,    37,   139,    -1,     3,   135,   134,    -1,
      -1,   135,   134,    -1,    -1,   101,   136,     4,   117,   139,
     118,    -1,   117,   137,   118,    -1,    -1,   101,   138,    -1,
     119,   101,   138,    -1,    -1,   140,    -1,    38,   140,    -1,
     141,   144,   146,   151,   152,    -1,     5,   142,   143,    -1,
     225,    14,   101,    -1,   225,    -1,   119,   142,   143,    -1,
      -1,    -1,     6,   145,   179,    -1,     7,   147,    -1,    -1,
     148,   150,    -1,     8,   149,    -1,     9,   149,    -1,   101,
      -1,   101,   120,   101,    -1,   119,   148,   150,    -1,    -1,
      10,   158,    -1,    -1,   153,    -1,   154,    -1,    -1,    11,
      12,   156,   155,   157,    -1,    11,    12,   156,   155,    -1,
     119,   156,   155,    -1,    -1,   225,    14,   101,    -1,   225,
      -1,    13,   158,    -1,   160,   159,    -1,    45,   160,   159,
      -1,    -1,   161,    -1,   160,    46,   161,    -1,   162,    -1,
      48,   162,    -1,   163,    -1,   117,   160,   118,    -1,   225,
      -1,   164,    -1,   172,    -1,   165,    -1,   166,    -1,   169,
      -1,   168,    -1,   170,    -1,   225,    49,   225,    45,   225,
      -1,   225,    48,    49,   225,    45,   225,    -1,   225,    50,
     167,    -1,   225,    48,    50,   167,    -1,   117,   226,   118,
      -1,   225,    -1,   225,    51,   225,    -1,   225,    48,    51,
     225,    -1,   225,    47,    48,    52,    -1,   225,    47,    52,
      -1,   225,    47,   171,    -1,   225,    47,    48,   171,    -1,
      96,    -1,    97,    -1,   173,    53,   173,    -1,   117,   225,
     119,   225,   118,    -1,   177,    -1,   177,    22,    21,   174,
     175,    -1,   177,    22,    21,   174,   176,    -1,   177,    24,
      23,    21,   174,   175,    -1,   177,    24,    23,    21,   174,
     176,    -1,    25,   158,    -1,    26,   117,   137,   118,    -1,
      26,   101,    -1,   178,    -1,    19,   117,   178,   118,    -1,
      20,   117,   137,   118,    -1,   101,   101,    -1,   101,     4,
     101,    -1,   101,    -1,   117,   174,   118,    -1,   174,   180,
      -1,   119,   174,   180,    -1,    -1,    27,    28,   101,   182,
      -1,    27,    29,   101,   182,    -1,     4,   130,    -1,   117,
     183,   118,    -1,   184,   185,    -1,   101,   186,    -1,   119,
     184,   185,    -1,    -1,    59,    -1,    60,    -1,    61,    -1,
      62,    -1,    63,    -1,    64,    -1,    65,    -1,    30,   101,
      31,   188,   146,   151,   152,    -1,   190,   189,    -1,   119,
     190,   189,    -1,    -1,   101,   112,   225,    -1,    32,    33,
     101,   146,   192,   193,    -1,   117,   137,   118,    -1,    -1,
     130,    -1,    34,   117,   226,   118,    -1,    35,     6,   101,
     146,   151,    -1,    35,     6,   101,   146,   151,   153,    -1,
      35,   137,     6,   101,    -1,    39,    40,    41,   102,    33,
      28,   101,    42,    43,    12,   102,    -1,   130,    33,    44,
     102,    42,    43,    12,   102,    -1,    -1,    66,   101,   198,
     117,   199,   118,   121,   201,   122,    -1,   101,   200,    -1,
      -1,   119,   101,   200,    -1,    -1,   203,   202,    -1,    -1,
     123,   203,   202,    -1,    -1,   225,    -1,   204,    -1,   130,
      -1,   101,    67,   225,    -1,    95,    -1,    94,    -1,    98,
      -1,    99,    -1,   102,    -1,   100,    -1,   171,    -1,    15,
      -1,   101,   120,   101,    -1,   104,    -1,    54,   208,   209,
     212,    55,    -1,   225,    -1,    -1,   211,   210,    -1,   211,
     210,    -1,    -1,    56,   160,    57,   225,    -1,    58,   225,
      -1,   205,    -1,   206,    -1,   101,    -1,   117,   225,   118,
      -1,   207,    -1,   213,    -1,   213,   124,   215,   125,    -1,
     216,   117,   226,   118,    -1,   216,   117,   118,    -1,   101,
     117,   226,   118,    -1,   101,   117,   118,    -1,    16,    -1,
      17,    -1,    18,    95,    -1,    68,    -1,    69,    -1,    86,
      -1,    70,    -1,    87,    -1,    71,    -1,    72,    -1,    73,
      -1,    74,    -1,    75,    -1,    76,    -1,    88,    -1,    77,
      -1,    89,    -1,    78,    -1,    79,    -1,    80,    -1,    81,
      -1,    90,    -1,    82,    -1,    83,    -1,    91,    -1,    84,
      -1,    85,    -1,    93,    -1,    92,    -1,   214,    -1,   107,
     214,    -1,   217,    -1,   217,   103,   218,    -1,   218,    -1,
     219,   104,   218,    -1,   219,   105,   218,    -1,   219,    -1,
     220,   106,   219,    -1,   220,   107,   219,    -1,   220,    -1,
     221,   110,   220,    -1,   221,   111,   220,    -1,   221,   108,
     220,    -1,   221,   109,   220,    -1,   221,    -1,   222,   112,
     221,    -1,   222,   113,   221,    -1,   222,    -1,   223,   114,
     222,    -1,   223,    -1,   224,   115,   223,    -1,   224,    -1,
     225,   227,    -1,   119,   225,   227,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   230,   230,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   249,   252,   252,   257,   258,   262,   263,
     266,   267,   270,   275,   276,   279,   282,   283,   289,   290,
     294,   297,   300,   301,   304,   305,   308,   308,   311,   312,
     315,   318,   319,   322,   323,   326,   327,   330,   331,   334,
     335,   336,   342,   345,   348,   349,   354,   355,   358,   373,
     376,   377,   380,   381,   384,   385,   388,   389,   393,   394,
     395,   399,   400,   401,   402,   403,   409,   410,   413,   414,
     417,   418,   421,   422,   425,   426,   429,   430,   433,   434,
     437,   440,   447,   448,   449,   450,   451,   455,   458,   459,
     462,   463,   464,   467,   468,   469,   470,   475,   478,   479,
     483,   484,   487,   488,   491,   494,   497,   498,   501,   502,
     503,   504,   505,   506,   507,   512,   518,   521,   522,   525,
     528,   531,   532,   535,   536,   539,   540,   541,   545,   548,
     553,   553,   568,   569,   572,   573,   577,   578,   581,   582,
     585,   586,   587,   590,   598,   599,   600,   601,   602,   603,
     604,   607,   608,   609,   613,   616,   617,   620,   623,   624,
     627,   630,   633,   634,   635,   636,   637,   640,   641,   642,
     643,   644,   645,   649,   650,   651,   654,   655,   656,   657,
     658,   659,   660,   661,   662,   663,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,   677,
     678,   679,   682,   683,   686,   687,   690,   691,   692,   696,
     697,   698,   701,   702,   703,   704,   705,   708,   709,   710,
     713,   714,   717,   718,   721,   725,   728,   729
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "WITH", "UC_AS", "SELECT", "FROM",
  "ASSUMING", "ASC", "DESC", "WHERE", "GROUP", "BY", "HAVING", "LC_AS",
  "ROWID", "ODD", "EVEN", "EVERY", "FLATTEN", "CONCATENATE", "JOIN",
  "INNER", "OUTER", "FULL", "ON", "USING", "CREATE", "TABLE", "VIEW",
  "UPDATE", "SET", "INSERT", "INTO", "VALUES", "DELETE", "EXEC", "ARRAYS",
  "SHOW", "LOAD", "DATA", "INFILE", "FIELDS", "TERMINATED", "OUTFILE",
  "AND", "OR", "IS", "NOT", "BETWEEN", "IN", "LIKE", "NULL_KEYWORD",
  "OVERLAPS", "CASE", "END", "WHEN", "THEN", "ELSE", "TYPE_INT",
  "TYPE_FLOAT", "TYPE_STRING", "TYPE_DATE", "TYPE_TIMESTAMP",
  "TYPE_BOOLEAN", "TYPE_HEX", "FUNCTION", "LOCAL_ASSIGN", "ABS", "AVG",
  "COUNT", "DISTINCT", "DROP", "FILL", "FIRST", "LAST", "MAX", "MIN",
  "MOD", "NEXT", "PREV", "PRD", "REV", "SUM", "SQRT", "STDDEV", "AVGS",
  "DELTAS", "MAXS", "MINS", "PRDS", "SUMS", "RATIOS", "MAKENULL", "FLOAT",
  "INT", "TRUE", "FALSE", "DATE", "TIMESTAMP", "HEX", "ID", "STRING",
  "EXP_OP", "TIMES_OP", "DIV_OP", "PLUS_OP", "MINUS_OP", "LE_OP", "GE_OP",
  "L_OP", "G_OP", "EQ_OP", "NEQ_OP", "AND_OP", "OR_OP", "VERBATIM_Q_CODE",
  "'('", "')'", "','", "'.'", "'{'", "'}'", "';'", "'['", "']'", "$accept",
  "program", "top_level", "global_query", "full_query", "$@1",
  "query_or_exec", "local_queries", "local_queries_tail", "local_query",
  "col_aliases", "comma_identifier_list", "comma_identifier_list_tail",
  "base_query_ops", "base_query", "select_clause", "select_elem",
  "select_clause_tail", "from_clause", "$@2", "order_clause",
  "order_specs", "order_spec", "column_name", "order_specs_tail",
  "where_clause", "groupby_clause", "groupby_with_having",
  "groupby_ex_having", "groupby_tail", "groupby_elem", "having_clause",
  "and_search_condition", "and_search_condition_tail", "search_condition",
  "boolean_factor", "boolean_primary", "predicate", "postfix_predicate",
  "between_predicate", "in_predicate", "in_pred_spec", "like_predicate",
  "null_predicate", "is_predicate", "truth_value", "overlaps_predicate",
  "range_value_expression", "joined_table", "on_clause", "using_clause",
  "table_expression", "table_expression_main", "table_expressions",
  "table_expressions_tail", "create_table_or_view", "create_spec",
  "schema", "schema_element", "schema_tail", "type_name",
  "update_statement", "set_clauses", "set_clauses_tail", "set_clause",
  "insert_statement", "insert_modifier", "insert_source",
  "delete_statement", "load_statement", "dump_statement",
  "user_function_definition", "$@3", "def_arg_list", "def_arg_list_tail",
  "function_body", "function_body_tail", "function_body_elem",
  "function_local_var_def", "constant", "table_constant",
  "case_expression", "case_clause", "when_clauses", "when_clauses_tail",
  "when_clause", "else_clause", "main_expression", "call", "indexing",
  "built_in_fun", "unary_neg", "exp_expression", "mult_expression",
  "add_expression", "rel_expression", "eq_expression", "and_expression",
  "or_expression", "value_expression", "comma_value_expression_list",
  "comma_value_expression_list_tail", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,    40,    41,    44,
      46,   123,   125,    59,    91,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   126,   127,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   129,   131,   130,   132,   132,   133,   133,
     134,   134,   135,   136,   136,   137,   138,   138,   139,   139,
     140,   141,   142,   142,   143,   143,   145,   144,   146,   146,
     147,   148,   148,   149,   149,   150,   150,   151,   151,   152,
     152,   152,   153,   154,   155,   155,   156,   156,   157,   158,
     159,   159,   160,   160,   161,   161,   162,   162,   163,   163,
     163,   164,   164,   164,   164,   164,   165,   165,   166,   166,
     167,   167,   168,   168,   169,   169,   170,   170,   171,   171,
     172,   173,   174,   174,   174,   174,   174,   175,   176,   176,
     177,   177,   177,   178,   178,   178,   178,   179,   180,   180,
     181,   181,   182,   182,   183,   184,   185,   185,   186,   186,
     186,   186,   186,   186,   186,   187,   188,   189,   189,   190,
     191,   192,   192,   193,   193,   194,   194,   194,   195,   196,
     198,   197,   199,   199,   200,   200,   201,   201,   202,   202,
     203,   203,   203,   204,   205,   205,   205,   205,   205,   205,
     205,   206,   206,   206,   207,   208,   208,   209,   210,   210,
     211,   212,   213,   213,   213,   213,   213,   214,   214,   214,
     214,   214,   214,   215,   215,   215,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   217,   217,   218,   218,   219,   219,   219,   220,
     220,   220,   221,   221,   221,   221,   221,   222,   222,   222,
     223,   223,   224,   224,   225,   226,   227,   227
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     0,     1,     0,     3,     1,     3,     3,     0,
       2,     0,     6,     3,     0,     2,     3,     0,     1,     2,
       5,     3,     3,     1,     3,     0,     0,     3,     2,     0,
       2,     2,     2,     1,     3,     3,     0,     2,     0,     1,
       1,     0,     5,     4,     3,     0,     3,     1,     2,     2,
       3,     0,     1,     3,     1,     2,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     5,     6,     3,     4,
       3,     1,     3,     4,     4,     3,     3,     4,     1,     1,
       3,     5,     1,     5,     5,     6,     6,     2,     4,     2,
       1,     4,     4,     2,     3,     1,     3,     2,     3,     0,
       4,     4,     2,     3,     2,     2,     3,     0,     1,     1,
       1,     1,     1,     1,     1,     7,     2,     3,     0,     3,
       6,     3,     0,     1,     4,     5,     6,     4,    11,     8,
       0,     9,     2,     0,     3,     0,     2,     0,     3,     0,
       1,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     5,     1,     0,     2,     2,     0,
       4,     2,     1,     1,     1,     3,     1,     1,     4,     4,
       3,     4,     3,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     3,     1,     3,     3,     1,
       3,     3,     1,     3,     3,     3,     3,     1,     3,     3,
       1,     3,     1,     3,     1,     2,     3,     0
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      14,     0,     0,     0,     0,     0,     0,    14,     0,     2,
      14,    13,    19,    14,    14,    14,    14,    14,    14,    14,
       0,     0,     0,     0,     0,    27,     0,     0,   140,    11,
       1,     3,     0,     0,     0,     4,     6,     5,     7,     8,
       9,    10,     0,     0,     0,    39,    39,     0,    25,     0,
       0,     0,     0,    24,    21,     0,     0,     0,    15,    16,
      28,     0,    14,     0,   110,   111,     0,    39,   128,     0,
     132,    48,    27,   137,     0,   143,     0,     0,     0,    18,
      21,   161,   166,   186,   187,   189,   191,   192,   193,   194,
     195,   196,   198,   200,   201,   202,   203,   205,   206,   208,
     209,   188,   190,   197,   199,   204,   207,   211,   210,   155,
     154,    88,    89,   156,   157,   159,   174,   158,   163,     0,
       0,    35,   160,   172,   173,   176,   177,   212,     0,   214,
     216,   219,   222,   227,   230,   232,   234,    33,     0,    29,
      36,    39,   112,     0,     0,   117,     0,    48,     0,   126,
       0,     0,    38,    46,     0,    14,     0,   135,    26,     0,
     145,     0,     0,     0,     0,    20,     0,   165,     0,     0,
     213,     0,     0,    31,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      17,     0,    48,   118,   119,   120,   121,   122,   123,   124,
     115,   113,     0,   114,   129,    51,   128,    43,    41,    42,
       0,    40,     0,     0,   133,   130,     0,     0,    47,    61,
      62,    64,    66,    69,    71,    72,    74,    73,    75,    70,
       0,    68,     0,   136,     0,     0,   142,     0,     0,    23,
       0,     0,     0,   169,   182,   237,     0,   162,   175,    35,
     183,   184,     0,     0,   180,     0,   215,   217,   218,   220,
     221,   225,   226,   223,   224,   228,   229,   231,   233,    32,
       0,     0,   105,     0,   109,    92,   100,    37,    51,   117,
       0,   125,    49,    50,   127,     0,    46,   131,     0,    65,
       0,    68,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,   145,    14,     0,     0,     0,     0,     0,
     167,   169,     0,   235,   181,    34,   185,   178,   179,     0,
       0,     0,   103,     0,     0,   107,     0,     0,    30,   116,
       0,    44,    45,     0,    67,     0,    61,    63,     0,    90,
       0,    85,    86,     0,     0,     0,     0,     0,    78,    81,
      82,    55,    57,     0,   144,   174,   152,     0,   149,   151,
     150,   139,    22,     0,   171,   164,   168,   237,     0,     0,
     104,   106,   109,     0,     0,    55,   134,     0,    60,     0,
      84,    87,     0,    79,    83,     0,     0,     0,     0,     0,
       0,     0,     0,   141,    14,   146,   170,   236,   101,   102,
     108,     0,     0,    53,    91,     0,    76,    80,    55,     0,
      52,    56,     0,   153,   149,     0,     0,    93,    94,     0,
      77,    54,    58,   138,   148,    97,    99,     0,    95,    96,
       0,    98
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     8,     9,    10,    11,    12,    58,    34,    79,    80,
      78,    26,    48,    59,    60,    61,   121,   173,   141,   191,
      70,   152,   153,   208,   211,   157,   281,   282,   283,   389,
     351,   410,   218,   294,   219,   220,   221,   222,   223,   224,
     225,   348,   226,   227,   228,   122,   229,   230,   274,   417,
     418,   275,   276,   277,   325,    13,    64,   144,   145,   203,
     200,    14,    67,   149,    68,    15,   155,   215,    16,    17,
      18,    19,    51,   161,   236,   357,   395,   358,   359,   123,
     124,   125,   166,   242,   310,   311,   309,   126,   127,   253,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   231,
     246,   313
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -373
static const yytype_int16 yypact[] =
{
       3,    34,   -60,    -5,     9,    10,   -49,     3,    54,  -373,
       3,    24,    56,     3,     3,     3,     3,     3,     3,     3,
     -19,     4,    44,    29,    50,    39,   153,   120,  -373,  -373,
    -373,  -373,   121,    63,    20,  -373,  -373,  -373,  -373,  -373,
    -373,  -373,     1,     1,    65,   156,   156,    66,  -373,    67,
      68,    52,    69,    55,    63,   477,   172,   206,  -373,  -373,
    -373,   208,  -373,   111,  -373,  -373,   103,   156,    97,    58,
     100,   209,    39,  -373,   185,   123,   178,   124,   217,  -373,
      63,  -373,   477,  -373,  -373,  -373,  -373,  -373,  -373,  -373,
    -373,  -373,  -373,  -373,  -373,  -373,  -373,  -373,  -373,  -373,
    -373,  -373,  -373,  -373,  -373,  -373,  -373,  -373,  -373,  -373,
    -373,  -373,  -373,  -373,  -373,  -373,   -71,  -373,  -373,   683,
     477,   107,  -373,  -373,  -373,  -373,    98,  -373,   110,   126,
    -373,    -4,    -3,    14,    -1,   116,   117,   220,    26,  -373,
    -373,   156,  -373,    33,   113,   118,   477,   209,    65,  -373,
     134,   134,  -373,   122,   124,   202,   426,   227,  -373,   211,
     131,   133,   197,   136,   125,  -373,   196,  -373,   287,   154,
    -373,   138,   477,  -373,    62,   338,   477,   477,   477,   477,
     477,   477,   477,   477,   477,   477,   477,   477,   477,   157,
    -373,   -11,   209,  -373,  -373,  -373,  -373,  -373,  -373,  -373,
    -373,  -373,   111,  -373,  -373,   248,    97,   140,  -373,  -373,
      58,  -373,   143,   145,  -373,  -373,   529,   426,  -373,    70,
    -373,  -373,  -373,  -373,  -373,  -373,  -373,  -373,  -373,  -373,
     210,    92,   252,  -373,   164,   165,  -373,   146,   256,  -373,
      26,   426,   212,   196,  -373,   150,   155,  -373,  -373,   107,
    -373,  -373,   176,   147,  -373,   158,  -373,  -373,  -373,    -4,
      -4,    -3,    -3,    -3,    -3,    14,    14,    -1,   116,  -373,
     160,   161,     6,   -11,   162,    23,  -373,  -373,   248,   118,
     262,  -373,  -373,  -373,  -373,   181,   122,  -373,   477,  -373,
     -35,   -30,   426,   426,  -373,   167,   -26,    35,   477,   580,
     477,   477,   237,   131,   106,   183,   168,    -6,   477,   232,
    -373,   196,   477,  -373,  -373,  -373,  -373,  -373,  -373,   -69,
     124,   190,  -373,   175,   -11,  -373,   273,   272,  -373,  -373,
     477,  -373,  -373,   179,  -373,   477,    70,  -373,   477,  -373,
     -36,  -373,  -373,   477,   580,   477,   251,   477,  -373,  -373,
    -373,   180,   284,   257,  -373,   -44,  -373,   182,   184,  -373,
    -373,  -373,  -373,   477,  -373,  -373,  -373,   150,   187,   188,
    -373,  -373,   162,   -11,   280,   180,  -373,   191,  -373,   192,
    -373,  -373,   258,  -373,  -373,   477,    36,   194,   477,   297,
     213,   301,   477,  -373,   632,  -373,  -373,  -373,  -373,  -373,
    -373,   102,   -11,   297,  -373,   477,  -373,  -373,   180,   426,
    -373,  -373,   214,  -373,   184,   426,   -62,  -373,  -373,   102,
    -373,  -373,  -373,  -373,  -373,  -373,  -373,   124,  -373,  -373,
     199,  -373
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -373,  -373,   119,  -373,   -56,  -373,  -373,  -373,   235,   285,
    -373,   -73,   247,  -131,   263,  -373,   149,    73,  -373,  -373,
     -33,  -373,   114,   174,    37,  -118,    48,   170,  -373,  -331,
    -316,  -373,  -372,    -8,  -205,    38,   127,  -373,  -373,  -373,
    -373,   -15,  -373,  -373,  -373,  -272,  -373,    40,  -271,   -87,
     -85,  -373,    17,  -373,   -32,  -373,   299,  -373,   142,    72,
    -373,  -373,  -373,   139,   198,  -373,  -373,  -373,  -373,  -373,
    -373,  -373,  -373,  -373,    45,  -373,   -67,   -45,  -373,  -373,
    -373,  -373,  -373,  -373,    41,   224,  -373,  -373,   274,  -373,
    -373,  -373,   -28,   -27,   -37,   -29,   215,   207,  -373,   -55,
    -174,    30
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -148
static const yytype_int16 yytable[] =
{
     137,   255,   323,   -12,   163,    62,   142,   190,   270,   271,
     321,   293,   290,    71,   375,    24,   380,   296,   297,   298,
     299,   300,   340,   392,   342,    55,   341,   167,    23,   205,
       1,    55,   272,     2,   147,     3,   307,   422,     4,   426,
     293,    22,     5,   425,   403,   326,   168,   327,   273,   169,
      27,   363,    28,   372,    30,   427,    56,    32,    57,    33,
     111,   112,    20,    21,    57,   171,   150,   151,   381,     6,
     111,   112,   408,   168,   278,    44,   169,   421,   250,   251,
     252,   212,    42,   334,   343,   344,   345,   336,   248,   335,
     272,   204,   193,   194,   195,   196,   197,   198,   199,   214,
     177,   178,   401,   179,   180,    43,   273,   322,   192,   306,
      25,   185,   186,   245,   333,   292,   293,   137,    63,     7,
     245,    81,   181,   182,   183,   184,    29,   415,   416,    31,
      45,   419,    35,    36,    37,    38,    39,    40,    41,   296,
     297,   298,   299,   300,   261,   262,   263,   264,   256,   257,
     258,    46,   259,   260,   248,   312,   265,   266,    47,    49,
      82,    50,   291,    69,    53,    52,    66,    72,    73,    75,
      74,    76,    77,   387,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   355,   117,   138,
     118,    55,   143,   119,   140,   146,   148,   154,   159,   156,
     162,   164,   174,   120,   160,    25,   172,   175,  -147,   176,
     187,   201,   188,   245,   189,   207,   213,   202,   232,   234,
     238,   210,   240,   346,   349,   350,   352,   369,   356,   360,
     235,   237,   241,   364,   239,   247,   248,   367,   269,   280,
     285,   287,   288,   295,   301,   302,   303,   304,   305,   312,
     308,   316,   317,   314,   330,   352,   318,   319,   320,   353,
     377,   324,   331,   379,   338,   361,   362,   365,   382,   349,
     384,   370,   386,   371,   373,   374,   385,   376,   390,   388,
     391,   402,    81,   405,   393,   398,   399,   394,   396,   404,
     409,   335,   407,   412,   411,   165,   423,   431,    54,   158,
     139,   249,   315,   332,   286,   209,   328,   233,   378,   383,
     406,   337,   428,   352,   429,   339,   368,   413,   356,   360,
     400,    82,    65,   289,   279,   284,   206,   424,   354,   414,
     420,   329,   366,    81,   430,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     243,   118,    82,   170,   119,   268,     0,   397,     0,     0,
       0,     0,   267,     0,   120,   244,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    81,   118,     0,     0,   119,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   120,   254,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   216,     0,     0,     0,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    81,     0,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,     0,
     118,    82,     0,   119,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   217,    81,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
       0,   118,     0,    82,   119,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   120,    81,     0,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,     0,   118,    82,     0,   119,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   217,    81,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,     0,   118,     0,    82,   119,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,    81,     0,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   355,   117,     0,   118,    82,     0,   119,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   120,
       0,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,     0,   118,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     120
};

static const yytype_int16 yycheck[] =
{
      55,   175,   273,     0,    77,     4,    62,   138,    19,    20,
       4,    46,   217,    46,   330,     6,    52,    47,    48,    49,
      50,    51,    48,    67,   296,     5,    52,    82,    33,   147,
      27,     5,   101,    30,    67,    32,   241,   409,    35,   101,
      46,   101,    39,   415,   375,    22,   117,    24,   117,   120,
      40,    57,   101,   324,     0,   117,    36,    33,    38,     3,
      96,    97,    28,    29,    38,   120,     8,     9,   340,    66,
      96,    97,   388,   117,   192,    31,   120,   408,    16,    17,
      18,   154,   101,   118,    49,    50,    51,   292,   118,   119,
     101,   146,    59,    60,    61,    62,    63,    64,    65,   155,
     104,   105,   373,   106,   107,   101,   117,   101,   141,   240,
     101,   112,   113,   168,   288,    45,    46,   172,   117,   116,
     175,    15,   108,   109,   110,   111,     7,    25,    26,    10,
     101,   402,    13,    14,    15,    16,    17,    18,    19,    47,
      48,    49,    50,    51,   181,   182,   183,   184,   176,   177,
     178,   101,   179,   180,   118,   119,   185,   186,   119,     6,
      54,    41,   217,     7,   101,    44,   101,   101,   101,   117,
     102,   102,   117,   347,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,    37,
     104,     5,   101,   107,     6,   112,   119,   117,    33,    10,
      42,     4,   124,   117,   101,   101,   119,   117,   122,   103,
     114,   118,   115,   288,    14,   101,    34,   119,    11,    28,
      43,   119,   117,   298,   299,   300,   301,   320,   304,   304,
     119,   118,    56,   308,   118,   101,   118,   312,   101,    11,
     120,   118,   117,    53,    12,   101,   101,   121,    12,   119,
      58,    95,   125,   118,    12,   330,   118,   117,   117,    42,
     335,   119,   101,   338,   117,   102,   118,    55,   343,   344,
     345,   101,   347,   118,    21,    23,    45,   118,    14,   119,
      43,    21,    15,    45,   122,   118,   118,   123,   363,   118,
      13,   119,   118,    12,   101,    80,   102,   118,    33,    72,
      57,   172,   249,   286,   210,   151,   278,   157,   336,   344,
     385,   293,   419,   388,   419,   295,   319,   392,   394,   394,
     372,    54,    43,   216,   202,   206,   148,   414,   303,   394,
     405,   279,   311,    15,   427,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     166,   104,    54,   119,   107,   188,    -1,   367,    -1,    -1,
      -1,    -1,   187,    -1,   117,   118,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,    15,   104,    -1,    -1,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    15,    -1,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,    -1,
     104,    54,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    15,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
      -1,   104,    -1,    54,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    15,    -1,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,    -1,   104,    54,    -1,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,    15,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,    -1,   104,    -1,    54,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    15,    -1,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,    -1,   104,    54,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    27,    30,    32,    35,    39,    66,   116,   127,   128,
     129,   130,   131,   181,   187,   191,   194,   195,   196,   197,
      28,    29,   101,    33,     6,   101,   137,    40,   101,   128,
       0,   128,    33,     3,   133,   128,   128,   128,   128,   128,
     128,   128,   101,   101,    31,   101,   101,   119,   138,     6,
      41,   198,    44,   101,   135,     5,    36,    38,   132,   139,
     140,   141,     4,   117,   182,   182,   101,   188,   190,     7,
     146,   146,   101,   101,   102,   117,   102,   117,   136,   134,
     135,    15,    54,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   104,   107,
     117,   142,   171,   205,   206,   207,   213,   214,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,    37,   140,
       6,   144,   130,   101,   183,   184,   112,   146,   119,   189,
       8,     9,   147,   148,   117,   192,    10,   151,   138,    33,
     101,   199,    42,   137,     4,   134,   208,   225,   117,   120,
     214,   225,   119,   143,   124,   117,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,    14,
     139,   145,   146,    59,    60,    61,    62,    63,    64,    65,
     186,   118,   119,   185,   225,   151,   190,   101,   149,   149,
     119,   150,   137,    34,   130,   193,    48,   117,   158,   160,
     161,   162,   163,   164,   165,   166,   168,   169,   170,   172,
     173,   225,    11,   153,    28,   119,   200,   118,    43,   118,
     117,    56,   209,   211,   118,   225,   226,   101,   118,   142,
      16,    17,    18,   215,   118,   226,   218,   218,   218,   219,
     219,   220,   220,   220,   220,   221,   221,   222,   223,   101,
      19,    20,   101,   117,   174,   177,   178,   179,   151,   184,
      11,   152,   153,   154,   189,   120,   148,   118,   117,   162,
     160,   225,    45,    46,   159,    53,    47,    48,    49,    50,
      51,    12,   101,   101,   121,    12,   139,   160,    58,   212,
     210,   211,   119,   227,   118,   143,    95,   125,   118,   117,
     117,     4,   101,   174,   119,   180,    22,    24,   152,   185,
      12,   101,   150,   226,   118,   119,   160,   161,   117,   173,
      48,    52,   171,    49,    50,    51,   225,   117,   167,   225,
     225,   156,   225,    42,   200,   101,   130,   201,   203,   204,
     225,   102,   118,    57,   225,    55,   210,   225,   178,   137,
     101,   118,   174,    21,    23,   156,   118,   225,   159,   225,
      52,   171,   225,   167,   225,    45,   225,   226,   119,   155,
      14,    43,    67,   122,   123,   202,   225,   227,   118,   118,
     180,   174,    21,   155,   118,    45,   225,   118,   156,    13,
     157,   101,    12,   225,   203,    25,    26,   175,   176,   174,
     225,   155,   158,   102,   202,   158,   101,   117,   175,   176,
     137,   118
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 230 "parser/aquery.y"
    { (yyval.top) = (yyvsp[(1) - (1)].top); ast = (yyval.top); ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 232 "parser/aquery.y"
    { (yyval.top) = make_Top_GlobalQuery((yyvsp[(1) - (2)].fullquery), (yyvsp[(2) - (2)].top)); ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 233 "parser/aquery.y"
    { (yyval.top) = make_Top_Create((yyvsp[(1) - (2)].create), (yyvsp[(2) - (2)].top)); ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 234 "parser/aquery.y"
    { (yyval.top) = make_Top_Insert((yyvsp[(1) - (2)].insert), (yyvsp[(2) - (2)].top)); ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 235 "parser/aquery.y"
    { (yyval.top) = make_Top_UpdateDelete((yyvsp[(1) - (2)].plan), (yyvsp[(2) - (2)].top)); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 236 "parser/aquery.y"
    { (yyval.top) = make_Top_UpdateDelete((yyvsp[(1) - (2)].plan), (yyvsp[(2) - (2)].top)); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 237 "parser/aquery.y"
    { (yyval.top) = make_Top_Load((yyvsp[(1) - (2)].load), (yyvsp[(2) - (2)].top)); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 238 "parser/aquery.y"
    { (yyval.top) = make_Top_Dump((yyvsp[(1) - (2)].dump), (yyvsp[(2) - (2)].top)); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 239 "parser/aquery.y"
    { (yyval.top) = make_Top_UDF((yyvsp[(1) - (2)].udfdef), (yyvsp[(2) - (2)].top)); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 240 "parser/aquery.y"
    { (yyval.top) = make_Top_VerbatimQ((yyvsp[(1) - (2)].str), (yyvsp[(2) - (2)].top)); ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 241 "parser/aquery.y"
    { (yyval.top) = NULL; ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 249 "parser/aquery.y"
    { (yyval.fullquery) = (yyvsp[(1) - (1)].fullquery); ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 252 "parser/aquery.y"
    { env = push_env(env); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 254 "parser/aquery.y"
    { env = pop_env(env); (yyval.fullquery) = make_FullQueryNode((yyvsp[(2) - (3)].localquery), (yyvsp[(3) - (3)].plan)); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 257 "parser/aquery.y"
    {(yyval.plan) = (yyvsp[(1) - (1)].plan); ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 258 "parser/aquery.y"
    {(yyval.plan) = make_execArrays((yyvsp[(3) - (3)].plan)); ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 262 "parser/aquery.y"
    { (yyvsp[(2) - (3)].localquery)->next_sibling = (yyvsp[(3) - (3)].localquery); (yyval.localquery) = (yyvsp[(2) - (3)].localquery); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 263 "parser/aquery.y"
    { (yyval.localquery) = NULL; ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 266 "parser/aquery.y"
    { (yyvsp[(1) - (2)].localquery)->next_sibling = (yyvsp[(2) - (2)].localquery); (yyval.localquery) = (yyvsp[(1) - (2)].localquery); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 267 "parser/aquery.y"
    { (yyval.localquery) = NULL; ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 272 "parser/aquery.y"
    { put_sym(env, (yyvsp[(1) - (6)].str), TABLE_TYPE, 0, 0); (yyval.localquery) = make_LocalQueryNode((yyvsp[(1) - (6)].str), (yyvsp[(2) - (6)].idlist), (yyvsp[(5) - (6)].plan)); add_order(env, (yyvsp[(1) - (6)].str), curr_order); ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 275 "parser/aquery.y"
    { (yyval.idlist) = (yyvsp[(2) - (3)].idlist); ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 276 "parser/aquery.y"
    { (yyval.idlist) = NULL; ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 279 "parser/aquery.y"
    { (yyval.idlist) = make_IDListNode((yyvsp[(1) - (2)].str), (yyvsp[(2) - (2)].idlist)); ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 282 "parser/aquery.y"
    { (yyval.idlist) = make_IDListNode((yyvsp[(2) - (3)].str), (yyvsp[(3) - (3)].idlist));;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 283 "parser/aquery.y"
    { (yyval.idlist) = NULL; ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 289 "parser/aquery.y"
    { (yyval.plan) = (yyvsp[(1) - (1)].plan); ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 290 "parser/aquery.y"
    { (yyval.plan) = make_showOp((yyvsp[(2) - (2)].plan)); ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 294 "parser/aquery.y"
    { (yyval.plan) = assemble_plan((yyvsp[(1) - (5)].plan), (yyvsp[(2) - (5)].plan), (yyvsp[(3) - (5)].plan), (yyvsp[(4) - (5)].plan), (yyvsp[(5) - (5)].plan)); curr_order = (yyvsp[(3) - (5)].plan); if((yyvsp[(5) - (5)].plan) != NULL) { annotate_groupedNamedExpr((yyvsp[(1) - (5)].plan)->params.namedexprs); } ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 297 "parser/aquery.y"
    { (yyvsp[(2) - (3)].namedexpr)->next_sibling = (yyvsp[(3) - (3)].namedexpr); (yyval.plan) = make_project(PROJECT_SELECT, NULL, (yyvsp[(2) - (3)].namedexpr)); ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 300 "parser/aquery.y"
    { (yyval.namedexpr) = make_NamedExprNode((yyvsp[(3) - (3)].str), (yyvsp[(1) - (3)].exprnode)); ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 301 "parser/aquery.y"
    { (yyval.namedexpr) = make_NamedExprNode(NULL, (yyvsp[(1) - (1)].exprnode)); ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 304 "parser/aquery.y"
    { (yyvsp[(2) - (3)].namedexpr)->next_sibling = (yyvsp[(3) - (3)].namedexpr); (yyval.namedexpr) = (yyvsp[(2) - (3)].namedexpr); ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 305 "parser/aquery.y"
    { (yyval.namedexpr) = NULL; ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 308 "parser/aquery.y"
    {query_line_num = line_num; query_col_num = col_num; ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 308 "parser/aquery.y"
    { (yyval.plan) = (yyvsp[(3) - (3)].plan); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 311 "parser/aquery.y"
    { (yyval.plan) = make_sort(NULL, (yyvsp[(2) - (2)].order));;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 312 "parser/aquery.y"
    { (yyval.plan) = NULL; ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 315 "parser/aquery.y"
    { (yyvsp[(1) - (2)].order)->next = (yyvsp[(2) - (2)].order); (yyval.order) = (yyvsp[(1) - (2)].order); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 318 "parser/aquery.y"
    { (yyval.order) = make_OrderNode(ASC_SORT, (yyvsp[(2) - (2)].exprnode)); ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 319 "parser/aquery.y"
    { (yyval.order) = make_OrderNode(DESC_SORT, (yyvsp[(2) - (2)].exprnode)); ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 322 "parser/aquery.y"
    { (yyval.exprnode) = make_id(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 323 "parser/aquery.y"
    { (yyval.exprnode) = make_colDotAccessNode(make_id(env, (yyvsp[(1) - (3)].str)), make_id(env, (yyvsp[(3) - (3)].str))); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 326 "parser/aquery.y"
    { (yyvsp[(2) - (3)].order)->next = (yyvsp[(3) - (3)].order); (yyval.order) = (yyvsp[(2) - (3)].order); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 327 "parser/aquery.y"
    { (yyval.order) = NULL; ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 330 "parser/aquery.y"
    { (yyval.plan) = make_filterWhere(NULL, (yyvsp[(2) - (2)].exprnode)); ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 331 "parser/aquery.y"
    { (yyval.plan) = NULL; ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 334 "parser/aquery.y"
    { (yyval.plan) = (yyvsp[(1) - (1)].plan);   ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 335 "parser/aquery.y"
    { (yyval.plan) = (yyvsp[(1) - (1)].plan);   ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 336 "parser/aquery.y"
    { (yyval.plan) = NULL; ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 342 "parser/aquery.y"
    { (yyvsp[(3) - (5)].namedexpr)->next_sibling = (yyvsp[(4) - (5)].namedexpr); (yyval.plan) = pushdown_logical((yyvsp[(5) - (5)].plan), make_groupby(NULL, (yyvsp[(3) - (5)].namedexpr))); ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 345 "parser/aquery.y"
    { (yyvsp[(3) - (4)].namedexpr)->next_sibling = (yyvsp[(4) - (4)].namedexpr); (yyval.plan) = make_groupby(NULL, (yyvsp[(3) - (4)].namedexpr)); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 348 "parser/aquery.y"
    { (yyvsp[(2) - (3)].namedexpr)->next_sibling = (yyvsp[(3) - (3)].namedexpr); (yyval.namedexpr) = (yyvsp[(2) - (3)].namedexpr); ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 349 "parser/aquery.y"
    { (yyval.namedexpr) = NULL; ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 354 "parser/aquery.y"
    { (yyval.namedexpr) = make_NamedExprNode((yyvsp[(3) - (3)].str), (yyvsp[(1) - (3)].exprnode)); ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 355 "parser/aquery.y"
    { (yyval.namedexpr) = make_NamedExprNode(NULL, (yyvsp[(1) - (1)].exprnode)); ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 358 "parser/aquery.y"
    { annotate_groupedExpr((yyvsp[(2) - (2)].exprnode)); (yyval.plan) = make_filterHaving(NULL, (yyvsp[(2) - (2)].exprnode)); ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 373 "parser/aquery.y"
    { (yyvsp[(1) - (2)].exprnode)->next_sibling = (yyvsp[(2) - (2)].exprnode); (yyval.exprnode) = (yyvsp[(1) - (2)].exprnode); ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 376 "parser/aquery.y"
    { (yyvsp[(2) - (3)].exprnode)->next_sibling = (yyvsp[(3) - (3)].exprnode); (yyval.exprnode) = (yyvsp[(2) - (3)].exprnode); ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 377 "parser/aquery.y"
    { (yyval.exprnode) = NULL;                      ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 380 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode); ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 381 "parser/aquery.y"
    { (yyval.exprnode) = make_logicOpNode(WHERE_OR_EXPR, (yyvsp[(1) - (3)].exprnode), (yyvsp[(3) - (3)].exprnode)); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 384 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 385 "parser/aquery.y"
    { (yyval.exprnode) = make_callNode(make_predNode((yyvsp[(1) - (2)].str)), (yyvsp[(2) - (2)].exprnode)); ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 388 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 389 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(2) - (3)].exprnode); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 393 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode); ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 394 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode); ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 395 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode); ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 399 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode); ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 400 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode); ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 401 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode); ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 402 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode); ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 403 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode); ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 409 "parser/aquery.y"
    { (yyvsp[(3) - (5)].exprnode)->next_sibling = (yyvsp[(5) - (5)].exprnode); (yyvsp[(1) - (5)].exprnode)->next_sibling = make_exprListNode((yyvsp[(3) - (5)].exprnode)); (yyval.exprnode) = make_callNode(make_predNode((yyvsp[(2) - (5)].str)), make_exprListNode((yyvsp[(1) - (5)].exprnode))); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 410 "parser/aquery.y"
    { (yyvsp[(4) - (6)].exprnode)->next_sibling = (yyvsp[(6) - (6)].exprnode); (yyvsp[(1) - (6)].exprnode)->next_sibling = make_exprListNode((yyvsp[(4) - (6)].exprnode)); (yyval.exprnode) = make_callNode(make_predNode((yyvsp[(2) - (6)].str)), make_exprListNode(make_callNode(make_predNode((yyvsp[(3) - (6)].str)), make_exprListNode((yyvsp[(1) - (6)].exprnode))))); ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 413 "parser/aquery.y"
    { (yyvsp[(1) - (3)].exprnode)->next_sibling = (yyvsp[(3) - (3)].exprnode); (yyval.exprnode) = make_callNode(make_predNode((yyvsp[(2) - (3)].str)), make_exprListNode((yyvsp[(1) - (3)].exprnode))); ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 414 "parser/aquery.y"
    { (yyvsp[(1) - (4)].exprnode)->next_sibling = (yyvsp[(4) - (4)].exprnode); (yyval.exprnode) = make_callNode(make_predNode((yyvsp[(2) - (4)].str)), make_exprListNode(make_callNode(make_predNode((yyvsp[(3) - (4)].str)), make_exprListNode((yyvsp[(1) - (4)].exprnode))))); ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 417 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(2) - (3)].exprnode); ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 418 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode); ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 421 "parser/aquery.y"
    { (yyvsp[(1) - (3)].exprnode)->next_sibling = (yyvsp[(3) - (3)].exprnode);  (yyval.exprnode) = make_callNode(make_predNode((yyvsp[(2) - (3)].str)), make_exprListNode((yyvsp[(1) - (3)].exprnode))); ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 422 "parser/aquery.y"
    { (yyvsp[(1) - (4)].exprnode)->next_sibling = (yyvsp[(4) - (4)].exprnode);  (yyval.exprnode) = make_callNode(make_predNode((yyvsp[(2) - (4)].str)), make_exprListNode(make_callNode(make_predNode((yyvsp[(3) - (4)].str)), make_exprListNode((yyvsp[(1) - (4)].exprnode))))); ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 425 "parser/aquery.y"
    { (yyval.exprnode) = make_callNode(make_predNode((yyvsp[(3) - (4)].str)), make_exprListNode(make_callNode(make_predNode((yyvsp[(4) - (4)].str)), make_exprListNode((yyvsp[(1) - (4)].exprnode))))); ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 426 "parser/aquery.y"
    { (yyval.exprnode) = make_callNode(make_predNode((yyvsp[(3) - (3)].str)), make_exprListNode((yyvsp[(1) - (3)].exprnode))); ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 429 "parser/aquery.y"
    { (yyvsp[(1) - (3)].exprnode)->next_sibling = (yyvsp[(3) - (3)].exprnode);  (yyval.exprnode) = make_callNode(make_predNode((yyvsp[(2) - (3)].str)), make_exprListNode((yyvsp[(1) - (3)].exprnode))); ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 430 "parser/aquery.y"
    { (yyvsp[(1) - (4)].exprnode)->next_sibling = (yyvsp[(4) - (4)].exprnode);  (yyval.exprnode) = make_callNode(make_predNode((yyvsp[(3) - (4)].str)),  make_exprListNode(make_callNode(make_predNode((yyvsp[(2) - (4)].str)), make_exprListNode((yyvsp[(1) - (4)].exprnode))))); ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 433 "parser/aquery.y"
    { (yyval.exprnode) = make_bool(1); ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 434 "parser/aquery.y"
    { (yyval.exprnode) = make_bool(0); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 437 "parser/aquery.y"
    { (yyvsp[(1) - (3)].exprnode)->next_sibling = (yyvsp[(3) - (3)].exprnode); (yyval.exprnode) = make_callNode(make_predNode((yyvsp[(2) - (3)].str)), make_exprListNode((yyvsp[(1) - (3)].exprnode))); ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 440 "parser/aquery.y"
    { (yyvsp[(2) - (5)].exprnode)->next_sibling = (yyvsp[(4) - (5)].exprnode); (yyval.exprnode) = make_exprListNode((yyvsp[(2) - (5)].exprnode)); ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 447 "parser/aquery.y"
    { (yyval.plan) = (yyvsp[(1) - (1)].plan);                                                ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 448 "parser/aquery.y"
    { (yyval.plan) = make_joinOn(INNER_JOIN_ON, (yyvsp[(1) - (5)].plan), (yyvsp[(4) - (5)].plan), (yyvsp[(5) - (5)].exprnode));            ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 449 "parser/aquery.y"
    { (yyval.plan) = make_joinUsing(INNER_JOIN_USING, (yyvsp[(1) - (5)].plan), (yyvsp[(4) - (5)].plan), (yyvsp[(5) - (5)].idlist));      ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 450 "parser/aquery.y"
    { (yyval.plan) = make_joinOn(FULL_OUTER_JOIN_ON, (yyvsp[(1) - (6)].plan), (yyvsp[(5) - (6)].plan), (yyvsp[(6) - (6)].exprnode));       ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 451 "parser/aquery.y"
    { (yyval.plan) = make_joinUsing(FULL_OUTER_JOIN_USING, (yyvsp[(1) - (6)].plan), (yyvsp[(5) - (6)].plan), (yyvsp[(6) - (6)].idlist)); ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 455 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(2) - (2)].exprnode); ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 458 "parser/aquery.y"
    { (yyval.idlist) = (yyvsp[(3) - (4)].idlist); ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 459 "parser/aquery.y"
    { (yyval.idlist) = make_IDListNode((yyvsp[(2) - (2)].str), NULL); ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 462 "parser/aquery.y"
    { (yyval.plan) = (yyvsp[(1) - (1)].plan);               ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 463 "parser/aquery.y"
    { (yyval.plan) = make_flatten((yyvsp[(3) - (4)].plan)); ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 464 "parser/aquery.y"
    { (yyval.plan) = make_concatenate((yyvsp[(3) - (4)].idlist)); ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 467 "parser/aquery.y"
    { (yyval.plan) = make_alias(make_table((yyvsp[(1) - (2)].str)), (yyvsp[(2) - (2)].str)); ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 468 "parser/aquery.y"
    { (yyval.plan) = make_alias(make_table((yyvsp[(1) - (3)].str)), (yyvsp[(3) - (3)].str)); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 469 "parser/aquery.y"
    { (yyval.plan) = make_table((yyvsp[(1) - (1)].str));                 ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 470 "parser/aquery.y"
    { (yyval.plan) = (yyvsp[(2) - (3)].plan);                             ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 475 "parser/aquery.y"
    { if((yyvsp[(2) - (2)].plan) == NULL){ (yyval.plan) = (yyvsp[(1) - (2)].plan); } else { (yyval.plan) = make_cross((yyvsp[(1) - (2)].plan), (yyvsp[(2) - (2)].plan)); } ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 478 "parser/aquery.y"
    { if((yyvsp[(3) - (3)].plan) == NULL){ (yyval.plan) = (yyvsp[(2) - (3)].plan); } else { (yyval.plan) = make_cross((yyvsp[(2) - (3)].plan), (yyvsp[(3) - (3)].plan)); } ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 479 "parser/aquery.y"
    { (yyval.plan) = NULL;                                                   ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 483 "parser/aquery.y"
    { put_sym(env, (yyvsp[(3) - (4)].str), TABLE_TYPE, 0, 0); (yyval.create) = make_createNode(CREATE_TABLE, (yyvsp[(3) - (4)].str), (yyvsp[(4) - (4)].createsrc)); ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 484 "parser/aquery.y"
    { put_sym(env, (yyvsp[(3) - (4)].str), VIEW_TYPE, 0, 0);  (yyval.create) = make_createNode(CREATE_VIEW, (yyvsp[(3) - (4)].str), (yyvsp[(4) - (4)].createsrc));  ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 487 "parser/aquery.y"
    { (yyval.createsrc) = make_querySource((yyvsp[(2) - (2)].fullquery));  ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 488 "parser/aquery.y"
    { (yyval.createsrc) = make_schemaSource((yyvsp[(2) - (3)].schema)); ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 491 "parser/aquery.y"
    { (yyvsp[(1) - (2)].schema)->next_sibling = (yyvsp[(2) - (2)].schema); (yyval.schema) = (yyvsp[(1) - (2)].schema); ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 494 "parser/aquery.y"
    { (yyval.schema) = make_schemaNode((yyvsp[(1) - (2)].str), (yyvsp[(2) - (2)].str)); ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 497 "parser/aquery.y"
    { (yyvsp[(2) - (3)].schema)->next_sibling = (yyvsp[(3) - (3)].schema); (yyval.schema) = (yyvsp[(2) - (3)].schema); ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 498 "parser/aquery.y"
    { (yyval.schema) = NULL;                      ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 501 "parser/aquery.y"
    {(yyval.str) = (yyvsp[(1) - (1)].str); ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 502 "parser/aquery.y"
    {(yyval.str) = (yyvsp[(1) - (1)].str); ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 503 "parser/aquery.y"
    {(yyval.str) = (yyvsp[(1) - (1)].str); ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 504 "parser/aquery.y"
    {(yyval.str) = (yyvsp[(1) - (1)].str); ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 505 "parser/aquery.y"
    {(yyval.str) = (yyvsp[(1) - (1)].str); ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 506 "parser/aquery.y"
    {(yyval.str) = (yyvsp[(1) - (1)].str); ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 507 "parser/aquery.y"
    {(yyval.str) = (yyvsp[(1) - (1)].str); ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 512 "parser/aquery.y"
    {
																									if((yyvsp[(7) - (7)].plan) != NULL){ annotate_groupedNamedExpr((yyvsp[(4) - (7)].namedexpr)); }
                                                  (yyval.plan)  = assemble_flat(make_project(PROJECT_UPDATE, NULL, (yyvsp[(4) - (7)].namedexpr)), make_table((yyvsp[(2) - (7)].str)), (yyvsp[(5) - (7)].plan), (yyvsp[(6) - (7)].plan), (yyvsp[(7) - (7)].plan));
                                                  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 518 "parser/aquery.y"
    { (yyvsp[(1) - (2)].namedexpr)->next_sibling = (yyvsp[(2) - (2)].namedexpr); (yyval.namedexpr) = (yyvsp[(1) - (2)].namedexpr); ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 521 "parser/aquery.y"
    { (yyvsp[(2) - (3)].namedexpr)->next_sibling = (yyvsp[(3) - (3)].namedexpr); (yyval.namedexpr) = (yyvsp[(2) - (3)].namedexpr); ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 522 "parser/aquery.y"
    { (yyval.namedexpr) = NULL; ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 525 "parser/aquery.y"
    { (yyval.namedexpr) = make_NamedExprNode((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].exprnode)); ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 528 "parser/aquery.y"
    { (yyval.insert) = make_insert(assemble_base(NULL, make_table((yyvsp[(3) - (6)].str)), (yyvsp[(4) - (6)].plan), NULL, NULL), (yyvsp[(5) - (6)].idlist), (yyvsp[(6) - (6)].fullquery)); ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 531 "parser/aquery.y"
    {  (yyval.idlist) = (yyvsp[(2) - (3)].idlist); ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 532 "parser/aquery.y"
    {  (yyval.idlist) = NULL; ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 535 "parser/aquery.y"
    { (yyval.fullquery) = (yyvsp[(1) - (1)].fullquery); ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 536 "parser/aquery.y"
    { (yyval.fullquery) = make_FullQueryNode(NULL, make_values((yyvsp[(3) - (4)].exprnode))); ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 539 "parser/aquery.y"
    { (yyval.plan) = assemble_flat(make_delete(NULL, NULL), make_table((yyvsp[(3) - (5)].str)), (yyvsp[(4) - (5)].plan), (yyvsp[(5) - (5)].plan), NULL); ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 540 "parser/aquery.y"
    { (yyval.plan) = assemble_flat(make_delete(NULL, NULL), make_table((yyvsp[(3) - (6)].str)), (yyvsp[(4) - (6)].plan), (yyvsp[(5) - (6)].plan), (yyvsp[(6) - (6)].plan));   ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 541 "parser/aquery.y"
    { (yyval.plan) = assemble_flat(make_delete(NULL, (yyvsp[(2) - (4)].idlist)), make_table((yyvsp[(4) - (4)].str)), NULL, NULL, NULL);   ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 545 "parser/aquery.y"
    { (yyval.load) = make_loadNode((yyvsp[(4) - (11)].str), (yyvsp[(11) - (11)].str), (yyvsp[(7) - (11)].str)); ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 548 "parser/aquery.y"
    { (yyval.dump) = make_dumpNode((yyvsp[(1) - (8)].fullquery), (yyvsp[(8) - (8)].str), (yyvsp[(4) - (8)].str)); ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 553 "parser/aquery.y"
    { put_sym(env, (yyvsp[(2) - (2)].str), FUNCTION_TYPE, 0, 0); env = push_env(env);  ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 559 "parser/aquery.y"
    { (yyval.udfdef) =  make_UDFDefNode((yyvsp[(2) - (9)].str), (yyvsp[(5) - (9)].idlist), (yyvsp[(8) - (9)].udfbody)); 
                                                entry = lookup_sym(env, (yyvsp[(2) - (9)].str)); 
                                                entry->order_dep = (yyval.udfdef)->order_dep; 
                                                entry->uses_agg = (yyval.udfdef)->uses_agg; 
                                                entry->is_odx = (yyval.udfdef)->is_odx;
                                                env = pop_env(env);
                                                ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 568 "parser/aquery.y"
    { put_sym(env, (yyvsp[(1) - (2)].str), UNKNOWN_TYPE, 0, 0); (yyval.idlist) = make_IDListNode((yyvsp[(1) - (2)].str), (yyvsp[(2) - (2)].idlist)); ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 569 "parser/aquery.y"
    { (yyval.idlist) = NULL; ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 572 "parser/aquery.y"
    { put_sym(env, (yyvsp[(2) - (3)].str), UNKNOWN_TYPE, 0, 0); (yyval.idlist) = make_IDListNode((yyvsp[(2) - (3)].str), (yyvsp[(3) - (3)].idlist)); ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 573 "parser/aquery.y"
    {(yyval.idlist) = NULL; ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 577 "parser/aquery.y"
    { (yyvsp[(1) - (2)].udfbody)->next_sibling = (yyvsp[(2) - (2)].udfbody); (yyval.udfbody) = (yyvsp[(1) - (2)].udfbody);;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 578 "parser/aquery.y"
    { (yyval.udfbody) = NULL; ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 581 "parser/aquery.y"
    { (yyvsp[(2) - (3)].udfbody)->next_sibling = (yyvsp[(3) - (3)].udfbody); (yyval.udfbody) = (yyvsp[(2) - (3)].udfbody); ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 582 "parser/aquery.y"
    { (yyval.udfbody)= NULL; ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 585 "parser/aquery.y"
    {(yyval.udfbody) = make_UDFExpr((yyvsp[(1) - (1)].exprnode));   ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 586 "parser/aquery.y"
    {(yyval.udfbody) = make_UDFVardef((yyvsp[(1) - (1)].namedexpr)); ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 587 "parser/aquery.y"
    {(yyval.udfbody) = make_UDFQuery((yyvsp[(1) - (1)].fullquery));  ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 590 "parser/aquery.y"
    { put_sym(env, (yyvsp[(1) - (3)].str), UNKNOWN_TYPE, 0, 0);
                                                            entry = lookup_sym(env, (yyvsp[(1) - (3)].str)); 
                                                            entry->is_odx = (yyvsp[(3) - (3)].exprnode)->is_odx;
                                                            (yyval.namedexpr) = make_NamedExprNode((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].exprnode));  
                                                             ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 598 "parser/aquery.y"
    { (yyval.exprnode) = make_int((yyvsp[(1) - (1)].intval));    ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 599 "parser/aquery.y"
    { (yyval.exprnode) = make_float((yyvsp[(1) - (1)].floatval));  ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 600 "parser/aquery.y"
    { (yyval.exprnode) = make_date((yyvsp[(1) - (1)].str));   ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 601 "parser/aquery.y"
    { (yyval.exprnode) = make_timestamp((yyvsp[(1) - (1)].str));   ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 602 "parser/aquery.y"
    { (yyval.exprnode) = make_string((yyvsp[(1) - (1)].str)); ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 603 "parser/aquery.y"
    { (yyval.exprnode) = make_hex((yyvsp[(1) - (1)].str));    ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 604 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode);              ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 607 "parser/aquery.y"
    { (yyval.exprnode) = make_rowId();                                              ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 608 "parser/aquery.y"
    { (yyval.exprnode) = make_colDotAccessNode(make_id(env, (yyvsp[(1) - (3)].str)), make_id(env, (yyvsp[(3) - (3)].str))); ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 609 "parser/aquery.y"
    { (yyval.exprnode) = make_allColsNode();                                        ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 613 "parser/aquery.y"
    { (yyval.exprnode) = make_caseNode((yyvsp[(2) - (5)].exprnode), (yyvsp[(3) - (5)].exprnode), (yyvsp[(4) - (5)].exprnode)); ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 616 "parser/aquery.y"
    { (yyval.exprnode) = make_caseClauseNode((yyvsp[(1) - (1)].exprnode));   ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 617 "parser/aquery.y"
    { (yyval.exprnode) = make_caseClauseNode(NULL); ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 620 "parser/aquery.y"
    { (yyval.exprnode) = make_whenClausesNode((yyvsp[(1) - (2)].exprnode), (yyvsp[(2) - (2)].exprnode)); ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 623 "parser/aquery.y"
    { (yyvsp[(1) - (2)].exprnode)->next_sibling = (yyvsp[(2) - (2)].exprnode); (yyval.exprnode) = (yyvsp[(1) - (2)].exprnode); ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 624 "parser/aquery.y"
    { (yyval.exprnode) = NULL; ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 627 "parser/aquery.y"
    { (yyval.exprnode) = make_caseWhenNode((yyvsp[(2) - (4)].exprnode), (yyvsp[(4) - (4)].exprnode)); ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 630 "parser/aquery.y"
    { (yyval.exprnode) = make_elseClauseNode((yyvsp[(2) - (2)].exprnode)); ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 633 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode); ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 634 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode); ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 635 "parser/aquery.y"
    { (yyval.exprnode) = make_id(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 636 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(2) - (3)].exprnode); ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 637 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode); ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 640 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode);                                         ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 641 "parser/aquery.y"
    { (yyval.exprnode) = make_indexNode((yyvsp[(1) - (4)].exprnode), (yyvsp[(3) - (4)].exprnode));                     ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 642 "parser/aquery.y"
    { (yyval.exprnode) = make_callNode((yyvsp[(1) - (4)].exprnode), (yyvsp[(3) - (4)].exprnode));                      ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 643 "parser/aquery.y"
    { (yyval.exprnode) = make_callNode((yyvsp[(1) - (3)].exprnode), NULL);                    ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 644 "parser/aquery.y"
    { (yyval.exprnode) = make_callNode(make_udfNode(env, (yyvsp[(1) - (4)].str)), (yyvsp[(3) - (4)].exprnode));   ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 645 "parser/aquery.y"
    { (yyval.exprnode) = make_callNode(make_udfNode(env, (yyvsp[(1) - (3)].str)), NULL); ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 649 "parser/aquery.y"
    { (yyval.exprnode) = make_oddix();      ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 650 "parser/aquery.y"
    { (yyval.exprnode) = make_evenix();     ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 651 "parser/aquery.y"
    { (yyval.exprnode) = make_everynix((yyvsp[(2) - (2)].intval)); ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 654 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 655 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 656 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 657 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 658 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 659 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 660 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 661 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 662 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 663 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 664 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 665 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 666 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 667 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 668 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 669 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 670 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 671 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 672 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 673 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 674 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 675 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 676 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 677 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 678 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 679 "parser/aquery.y"
    { (yyval.exprnode) = make_builtInFunNode(env, (yyvsp[(1) - (1)].str)); ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 682 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode); ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 683 "parser/aquery.y"
    { (yyval.exprnode) = make_neg((yyvsp[(2) - (2)].exprnode)); ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 686 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode);                               ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 687 "parser/aquery.y"
    { (yyval.exprnode) = make_arithNode(POW_EXPR, (yyvsp[(1) - (3)].exprnode), (yyvsp[(3) - (3)].exprnode)); ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 690 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode);                                ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 691 "parser/aquery.y"
    { (yyval.exprnode) = make_arithNode(MULT_EXPR, (yyvsp[(1) - (3)].exprnode), (yyvsp[(3) - (3)].exprnode)); ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 692 "parser/aquery.y"
    { (yyval.exprnode) = make_arithNode(DIV_EXPR, (yyvsp[(1) - (3)].exprnode), (yyvsp[(3) - (3)].exprnode));  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 696 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode);                                 ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 697 "parser/aquery.y"
    { (yyval.exprnode) = make_arithNode(PLUS_EXPR, (yyvsp[(1) - (3)].exprnode), (yyvsp[(3) - (3)].exprnode));  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 698 "parser/aquery.y"
    { (yyval.exprnode) = make_arithNode(MINUS_EXPR, (yyvsp[(1) - (3)].exprnode), (yyvsp[(3) - (3)].exprnode)); ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 701 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode);                             ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 702 "parser/aquery.y"
    { (yyval.exprnode) = make_compNode(LT_EXPR, (yyvsp[(1) - (3)].exprnode), (yyvsp[(3) - (3)].exprnode)); ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 703 "parser/aquery.y"
    { (yyval.exprnode) = make_compNode(GT_EXPR, (yyvsp[(1) - (3)].exprnode), (yyvsp[(3) - (3)].exprnode)); ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 704 "parser/aquery.y"
    { (yyval.exprnode) = make_compNode(LE_EXPR, (yyvsp[(1) - (3)].exprnode), (yyvsp[(3) - (3)].exprnode)); ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 705 "parser/aquery.y"
    { (yyval.exprnode) = make_compNode(GE_EXPR, (yyvsp[(1) - (3)].exprnode), (yyvsp[(3) - (3)].exprnode)); ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 708 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode);                              ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 709 "parser/aquery.y"
    { (yyval.exprnode) = make_compNode(EQ_EXPR, (yyvsp[(1) - (3)].exprnode), (yyvsp[(3) - (3)].exprnode));  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 710 "parser/aquery.y"
    { (yyval.exprnode) = make_compNode(NEQ_EXPR, (yyvsp[(1) - (3)].exprnode), (yyvsp[(3) - (3)].exprnode)); ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 713 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode);                                  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 714 "parser/aquery.y"
    { (yyval.exprnode) = make_logicOpNode(LAND_EXPR, (yyvsp[(1) - (3)].exprnode), (yyvsp[(3) - (3)].exprnode)); ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 717 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode);                                 ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 718 "parser/aquery.y"
    { (yyval.exprnode) = make_logicOpNode(LOR_EXPR, (yyvsp[(1) - (3)].exprnode), (yyvsp[(3) - (3)].exprnode)); ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 721 "parser/aquery.y"
    { (yyval.exprnode) = (yyvsp[(1) - (1)].exprnode); ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 725 "parser/aquery.y"
    { (yyvsp[(1) - (2)].exprnode)->next_sibling = (yyvsp[(2) - (2)].exprnode); (yyval.exprnode) = make_exprListNode((yyvsp[(1) - (2)].exprnode)); ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 728 "parser/aquery.y"
    { (yyvsp[(2) - (3)].exprnode)->next_sibling = (yyvsp[(3) - (3)].exprnode); (yyval.exprnode) = (yyvsp[(2) - (3)].exprnode); ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 729 "parser/aquery.y"
    { (yyval.exprnode) = NULL; ;}
    break;



/* Line 1455 of yacc.c  */
#line 3696 "parser/aquery.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 735 "parser/aquery.y"


void yyerror(const char *s) 
{
	//TODO: rewrite with more robust fgets wrapper
	//rewind file and find error
	rewind(yyin);
	int lines = line_num, cols = col_num;
	int max_size = 500;
	
	char *line = malloc(sizeof(char) * (max_size - 1));
	char *marker = malloc(sizeof(char) * (col_num + 2));
	memset(marker, ' ', col_num + 1);
	marker[col_num - yyleng] = '^';
	marker[col_num + 1] = '\0';
	
	while(lines-- > 0) 
	{ //find line with error
		fgets(line, max_size, yyin);
	}
	
	printf("%s at line %d, column %d\n%s" ERRORCOLOR "%s\n", s, line_num, col_num - yyleng, line, marker);
	fclose(yyin);
	free(line); free(marker);
	exit(1);
}

void help()
{
	printf("Usage: ./a2q [-p|-c][-o output_q_file][-a <#>] aquery_file\n");
	printf("-p  print dot file AST to stdout\n");
	printf("-a  optimization level [0-1]\n");
    printf("-s  silence warnings\n");
    printf("-c  generate code\n");
    printf("-o  code output file\n");
    printf("-c/-p are mutually exclusive\n");
}



int main(int argc, char *argv[]) {
	yydebug = 0;
	
	/* Aquery compiler flags */
	int print_ast_flag = 0;
	optim_level = 0;
	int max_optim_level = 1;
	
	/* getopt values */
	int op;
	
    /* where to save the code generated */
    DEST_FILE = stdout;
    
	
	while((op = getopt(argc, argv, "cspha:o:")) != -1)
	{
		switch(op)
		{
            case 's':
                silence_warnings = 1;
                break;
			case 'p':
				print_ast_flag = 1;
				break;
            case 'c':
                GEN_CODE = 1;
                break;
			case 'h':
				help();
				exit(0);
				break;
			case 'a':
				if(!isdigit(optarg[0]))
				{
					printf("-%c requires numeric arg\n", optopt);
					help();
					exit(1);
				}
				
				optim_level = atoi(optarg);
				
				if(optim_level > max_optim_level)
				{
					printf("Defaulting to highest optimizing level:%d\n", max_optim_level);
					optim_level = max_optim_level;
				}
				break;
            case 'o':
                DEST_FILE = fopen(optarg, "w");
                if(DEST_FILE == NULL)
                {
            		printf("Unable to open %s for writing\n", optarg);
            		exit(1);
                }
                break;
			case '?':
				if(optopt == 'a')
				{
					printf("Option -%c requires an option\n", optopt);
					exit(1);
				}
				break;
			default:
 				exit(1);
		}
	}
	
    //can not generate code and print out ast...creates mess in stdout and we manipulate tree in code generation...
    if(print_ast_flag && GEN_CODE)
    {
        help();
        exit(1);
    }
    
	FILE *to_parse;
    
	if(1 > argc - optind) 
	{ //Did we get a file to analyze?
		//help();
		//exit(1);
        to_parse = stdin;
	}
    else
    {
        to_parse = fopen(argv[optind], "r");
    }
	
	if(to_parse == NULL)
	{
		printf("Unable to open %s for reading\n", argv[optind]);
		exit(1);
	}
	else
	{
		yyin = to_parse;
	}
	
	env = init_symtable(); //create the global environment
	
	do 
	{
		yyparse();
	} 
	while(!feof(yyin));
	
	if(print_ast_flag)
	{
		print_ast(ast);
	}
    else if(GEN_CODE)
    {
        cg_AQUERY2Q(ast); 
        fclose(DEST_FILE);
    }
    else
    {
        printf("Input file fits Aquery grammar, call with -c[code] or -p[ast viz]\n");
    }		
		
    return 0;
}


