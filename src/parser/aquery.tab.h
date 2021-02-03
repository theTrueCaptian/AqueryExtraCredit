
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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

/* Line 1676 of yacc.c  */
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



/* Line 1676 of yacc.c  */
#line 193 "parser/aquery.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


