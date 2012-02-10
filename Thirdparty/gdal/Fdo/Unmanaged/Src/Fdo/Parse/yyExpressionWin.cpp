/* A Bison parser, made by GNU Bison 1.875.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

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
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with fdo_expression_yy or FDO_EXPRESSION_YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define FDO_EXPRESSION_YYBISON 1

/* Skeleton name.  */
#define FDO_EXPRESSION_YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define FDO_EXPRESSION_YYPURE 0

/* Using locations.  */
#define FDO_EXPRESSION_YYLSP_NEEDED 0



/* Tokens.  */
#if 0 
# define FDO_EXPRESSION_YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum fdo_expression_yytokentype {
     FdoToken_NULL = 258,
     FdoToken_TRUE = 259,
     FdoToken_FALSE = 260,
     FdoToken_DATE = 261,
     FdoToken_TIME = 262,
     FdoToken_TIMESTAMP = 263,
     FdoToken_GEOMFROMTEXT = 264,
     FdoToken_JOINNONE = 265,
     FdoToken_JOININNER = 266,
     FdoToken_JOINRIGHTOUTER = 267,
     FdoToken_JOINLEFTOUTER = 268,
     FdoToken_JOINFULLOUTER = 269,
     FdoToken_JOINCROSS = 270,
     FdoToken_AND = 271,
     FdoToken_OR = 272,
     FdoToken_NOT = 273,
     FdoToken_LIKE = 274,
     FdoToken_IN = 275,
     FdoToken_SELECT = 276,
     FdoToken_JOIN = 277,
     FdoToken_BEYOND = 278,
     FdoToken_WITHINDISTANCE = 279,
     FdoToken_CONTAINS = 280,
     FdoToken_COVEREDBY = 281,
     FdoToken_CROSSES = 282,
     FdoToken_DISJOINT = 283,
     FdoToken_ENVELOPEINTERSECTS = 284,
     FdoToken_EQUALS = 285,
     FdoToken_INTERSECTS = 286,
     FdoToken_INSIDE = 287,
     FdoToken_OVERLAPS = 288,
     FdoToken_TOUCHES = 289,
     FdoToken_WITHIN = 290,
     FdoToken_RELATE = 291,
     FdoToken_IDENTIFIER = 292,
     FdoToken_PARAMETER = 293,
     FdoToken_STRING = 294,
     FdoToken_INTEGER = 295,
     FdoToken_INT64 = 296,
     FdoToken_INTHEX = 297,
     FdoToken_INTBIN = 298,
     FdoToken_DOUBLE = 299,
     FdoToken_DATETIME = 300,
     FdoToken_BLOB = 301,
     FdoToken_CLOB = 302,
     FdoToken_Add = 303,
     FdoToken_Subtract = 304,
     FdoToken_Multiply = 305,
     FdoToken_Divide = 306,
     FdoToken_Negate = 307,
     FdoToken_EQ = 308,
     FdoToken_NE = 309,
     FdoToken_GT = 310,
     FdoToken_GE = 311,
     FdoToken_LT = 312,
     FdoToken_LE = 313,
     FdoToken_LeftParenthesis = 314,
     FdoToken_RightParenthesis = 315,
     FdoToken_Comma = 316,
     FdoToken_Dot = 317,
     FdoToken_Colon = 318,
     FdoToken_AS = 319,
     FdoToken_BETWEEN = 320
   };
#endif
#define FdoToken_NULL 258
#define FdoToken_TRUE 259
#define FdoToken_FALSE 260
#define FdoToken_DATE 261
#define FdoToken_TIME 262
#define FdoToken_TIMESTAMP 263
#define FdoToken_GEOMFROMTEXT 264
#define FdoToken_JOINNONE 265
#define FdoToken_JOININNER 266
#define FdoToken_JOINRIGHTOUTER 267
#define FdoToken_JOINLEFTOUTER 268
#define FdoToken_JOINFULLOUTER 269
#define FdoToken_JOINCROSS 270
#define FdoToken_AND 271
#define FdoToken_OR 272
#define FdoToken_NOT 273
#define FdoToken_LIKE 274
#define FdoToken_IN 275
#define FdoToken_SELECT 276
#define FdoToken_JOIN 277
#define FdoToken_BEYOND 278
#define FdoToken_WITHINDISTANCE 279
#define FdoToken_CONTAINS 280
#define FdoToken_COVEREDBY 281
#define FdoToken_CROSSES 282
#define FdoToken_DISJOINT 283
#define FdoToken_ENVELOPEINTERSECTS 284
#define FdoToken_EQUALS 285
#define FdoToken_INTERSECTS 286
#define FdoToken_INSIDE 287
#define FdoToken_OVERLAPS 288
#define FdoToken_TOUCHES 289
#define FdoToken_WITHIN 290
#define FdoToken_RELATE 291
#define FdoToken_IDENTIFIER 292
#define FdoToken_PARAMETER 293
#define FdoToken_STRING 294
#define FdoToken_INTEGER 295
#define FdoToken_INT64 296
#define FdoToken_INTHEX 297
#define FdoToken_INTBIN 298
#define FdoToken_DOUBLE 299
#define FdoToken_DATETIME 300
#define FdoToken_BLOB 301
#define FdoToken_CLOB 302
#define FdoToken_Add 303
#define FdoToken_Subtract 304
#define FdoToken_Multiply 305
#define FdoToken_Divide 306
#define FdoToken_Negate 307
#define FdoToken_EQ 308
#define FdoToken_NE 309
#define FdoToken_GT 310
#define FdoToken_GE 311
#define FdoToken_LT 312
#define FdoToken_LE 313
#define FdoToken_LeftParenthesis 314
#define FdoToken_RightParenthesis 315
#define FdoToken_Comma 316
#define FdoToken_Dot 317
#define FdoToken_Colon 318
#define FdoToken_AS 319
#define FdoToken_BETWEEN 320




/* Copy the first part of user declarations.  */



#include "Parse.h"
#pragma warning(disable:4102)	// unreferenced labels in fdo_expression_yyExpression.cpp



/* Enabling traces.  */
#ifndef FDO_EXPRESSION_YYDEBUG
# define FDO_EXPRESSION_YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef FDO_EXPRESSION_YYERROR_VERBOSE
# undef FDO_EXPRESSION_YYERROR_VERBOSE
# define FDO_EXPRESSION_YYERROR_VERBOSE 1
#else
# define FDO_EXPRESSION_YYERROR_VERBOSE 0
#endif

#if ! defined (FDO_EXPRESSION_YYSTYPE) && ! defined (FDO_EXPRESSION_YYSTYPE_IS_DECLARED)

typedef union FDO_EXPRESSION_YYSTYPE _FDO_EXPRESSION_YYSTYPE
{
	FdoIDisposable*	m_node;		// expression or filter parse tree node
	FdoInt32		m_id;		// enumerations, keywords, ...
	FdoInt32		m_integer;	// integer values (Int8, Int16, Int32 )
	FdoInt64		m_int64;	// 64-bit integer values
	double			m_double;	// floating point values (single(float), double)
	FdoString*		m_string;	// string
	_FdoDateTime	m_datetime;	// date time
 
/* Line 191 of yacc.c.  */

# define fdo_expression_yystype FDO_EXPRESSION_YYSTYPE /* obsolescent; will be withdrawn */
# define FDO_EXPRESSION_YYSTYPE_IS_DECLARED 1
# define FDO_EXPRESSION_YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */


#if ! defined (fdo_expression_yyoverflow) || FDO_EXPRESSION_YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if FDO_EXPRESSION_YYSTACK_USE_ALLOCA
#  define FDO_EXPRESSION_YYSTACK_ALLOC alloca
# else
#  ifndef FDO_EXPRESSION_YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define FDO_EXPRESSION_YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define FDO_EXPRESSION_YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef FDO_EXPRESSION_YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define FDO_EXPRESSION_YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define FDO_EXPRESSION_YYSIZE_T size_t
#  endif
#  define FDO_EXPRESSION_YYSTACK_ALLOC malloc
#  define FDO_EXPRESSION_YYSTACK_FREE free
# endif
#endif /* ! defined (fdo_expression_yyoverflow) || FDO_EXPRESSION_YYERROR_VERBOSE */


#if (! defined (fdo_expression_yyoverflow) \
     && (! defined (__cplusplus) \
	 || (FDO_EXPRESSION_YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union fdo_expression_yyalloc
{
  short fdo_expression_yyss;
  FDO_EXPRESSION_YYSTYPE fdo_expression_yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define FDO_EXPRESSION_YYSTACK_GAP_MAXIMUM (sizeof (union fdo_expression_yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define FDO_EXPRESSION_YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (FDO_EXPRESSION_YYSTYPE))				\
      + FDO_EXPRESSION_YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef FDO_EXPRESSION_YYCOPY
#  if 1 < __GNUC__
#   define FDO_EXPRESSION_YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define FDO_EXPRESSION_YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register FDO_EXPRESSION_YYSIZE_T fdo_expression_yyi;		\
	  for (fdo_expression_yyi = 0; fdo_expression_yyi < (Count); fdo_expression_yyi++)	\
	    (To)[fdo_expression_yyi] = (From)[fdo_expression_yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables FDO_EXPRESSION_YYSIZE and FDO_EXPRESSION_YYSTACKSIZE give the old and new number of
   elements in the stack, and FDO_EXPRESSION_YYPTR gives the new location of the
   stack.  Advance FDO_EXPRESSION_YYPTR to a properly aligned location for the next
   stack.  */
# define FDO_EXPRESSION_YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	FDO_EXPRESSION_YYSIZE_T fdo_expression_yynewbytes;						\
	FDO_EXPRESSION_YYCOPY (&fdo_expression_yyptr->Stack, Stack, fdo_expression_yysize);				\
	Stack = &fdo_expression_yyptr->Stack;						\
	fdo_expression_yynewbytes = fdo_expression_yystacksize * sizeof (*Stack) + FDO_EXPRESSION_YYSTACK_GAP_MAXIMUM; \
	fdo_expression_yyptr += fdo_expression_yynewbytes / sizeof (*fdo_expression_yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char fdo_expression_yysigned_char;
#else
   typedef short fdo_expression_yysigned_char;
#endif

/* FDO_EXPRESSION_YYFINAL -- State number of the termination state. */
#define FDO_EXPRESSION_YYFINAL  48
/* FDO_EXPRESSION_YYLAST -- Last index in FDO_EXPRESSION_YYTABLE.  */
#define FDO_EXPRESSION_YYLAST   218

/* FDO_EXPRESSION_YYNTOKENS -- Number of terminals. */
#define FDO_EXPRESSION_YYNTOKENS  69
/* FDO_EXPRESSION_YYNNTS -- Number of nonterminals. */
#define FDO_EXPRESSION_YYNNTS  28
/* FDO_EXPRESSION_YYNRULES -- Number of rules. */
#define FDO_EXPRESSION_YYNRULES  76
/* FDO_EXPRESSION_YYNRULES -- Number of states. */
#define FDO_EXPRESSION_YYNSTATES  141

/* FDO_EXPRESSION_YYTRANSLATE(FDO_EXPRESSION_YYLEX) -- Bison symbol number corresponding to FDO_EXPRESSION_YYLEX.  */
#define FDO_EXPRESSION_YYUNDEFTOK  2
#define FDO_EXPRESSION_YYMAXUTOK   320

#define FDO_EXPRESSION_YYTRANSLATE(FDO_EXPRESSION_YYX) 						\
  ((unsigned int) (FDO_EXPRESSION_YYX) <= FDO_EXPRESSION_YYMAXUTOK ? fdo_expression_yytranslate[FDO_EXPRESSION_YYX] : FDO_EXPRESSION_YYUNDEFTOK)

/* FDO_EXPRESSION_YYTRANSLATE[FDO_EXPRESSION_YYLEX] -- Bison symbol number corresponding to FDO_EXPRESSION_YYLEX.  */
static const unsigned char fdo_expression_yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      66,    67,     2,     2,    68,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65
};

#if FDO_EXPRESSION_YYDEBUG
/* FDO_EXPRESSION_YYPRHS[FDO_EXPRESSION_YYN] -- Index of the first RHS symbol of rule number FDO_EXPRESSION_YYN in
   FDO_EXPRESSION_YYRHS.  */
static const unsigned short fdo_expression_yyprhs[] =
{
       0,     0,     3,     5,     7,    11,    13,    15,    17,    19,
      21,    23,    27,    31,    35,    39,    41,    43,    45,    47,
      49,    51,    53,    55,    57,    59,    61,    63,    65,    67,
      69,    71,    73,    75,    77,    79,    84,    90,    94,   100,
     104,   106,   112,   116,   122,   126,   133,   142,   149,   158,
     160,   164,   171,   180,   189,   200,   206,   210,   216,   220,
     221,   223,   227,   232,   234,   236,   238,   240,   242,   244,
     247,   250,   252,   254,   256,   258,   260
};

/* FDO_EXPRESSION_YYRHS -- A `-1'-separated list of the rules' RHS. */
static const fdo_expression_yysigned_char fdo_expression_yyrhs[] =
{
      70,     0,    -1,    71,    -1,     1,    -1,    66,    71,    67,
      -1,    95,    -1,    72,    -1,    82,    -1,    91,    -1,    92,
      -1,    88,    -1,    71,    48,    71,    -1,    71,    49,    71,
      -1,    71,    50,    71,    -1,    71,    51,    71,    -1,    74,
      -1,    81,    -1,    80,    -1,    76,    -1,    77,    -1,    78,
      -1,    79,    -1,    75,    -1,    46,    -1,    47,    -1,     3,
      -1,     4,    -1,     5,    -1,    39,    -1,    40,    -1,    41,
      -1,    42,    -1,    43,    -1,    44,    -1,    45,    -1,    91,
      66,    89,    67,    -1,    66,    91,    67,    64,    91,    -1,
      91,    64,    91,    -1,    66,    91,    67,    64,    91,    -1,
      91,    64,    91,    -1,    91,    -1,    66,    87,    67,    64,
      91,    -1,    87,    64,    91,    -1,    66,    71,    67,    64,
      91,    -1,    71,    64,    91,    -1,    22,    66,    91,    68,
      96,    67,    -1,    22,    66,    91,    68,    96,    68,    75,
      67,    -1,    22,    66,    83,    68,    96,    67,    -1,    22,
      66,    83,    68,    96,    68,    75,    67,    -1,    85,    -1,
      86,    68,    85,    -1,    21,    66,    91,    68,    84,    67,
      -1,    21,    66,    91,    68,    84,    68,    75,    67,    -1,
      21,    66,    91,    68,    84,    68,    86,    67,    -1,    21,
      66,    91,    68,    84,    68,    75,    68,    86,    67,    -1,
      66,    71,    67,    64,    91,    -1,    71,    64,    91,    -1,
      66,    87,    67,    64,    91,    -1,    87,    64,    91,    -1,
      -1,    71,    -1,    89,    68,    71,    -1,     9,    66,    39,
      67,    -1,    37,    -1,    93,    -1,    94,    -1,    90,    -1,
      73,    -1,    38,    -1,    63,    39,    -1,    52,    71,    -1,
      10,    -1,    11,    -1,    12,    -1,    13,    -1,    14,    -1,
      15,    -1
};

/* FDO_EXPRESSION_YYRLINE[FDO_EXPRESSION_YYN] -- source line where rule number FDO_EXPRESSION_YYN was defined.  */
static const unsigned short fdo_expression_yyrline[] =
{
       0,   114,   114,   115,   119,   120,   121,   122,   123,   124,
     125,   129,   130,   131,   132,   136,   139,   141,   143,   145,
     147,   149,   151,   153,   155,   157,   162,   163,   166,   169,
     172,   175,   178,   181,   184,   188,   193,   195,   200,   202,
     204,   206,   208,   210,   212,   217,   219,   221,   223,   228,
     230,   235,   237,   239,   241,   246,   247,   248,   249,   252,
     253,   254,   258,   263,   268,   269,   273,   274,   278,   280,
     285,   289,   290,   291,   292,   293,   294
};
#endif

#if FDO_EXPRESSION_YYDEBUG || FDO_EXPRESSION_YYERROR_VERBOSE
/* FDO_EXPRESSION_YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at FDO_EXPRESSION_YYNTOKENS, nonterminals. */
static const char *const fdo_expression_yytname[] =
{
  "$end", "error", "$undefined", "FdoToken_NULL", "FdoToken_TRUE", 
  "FdoToken_FALSE", "FdoToken_DATE", "FdoToken_TIME", 
  "FdoToken_TIMESTAMP", "FdoToken_GEOMFROMTEXT", "FdoToken_JOINNONE", 
  "FdoToken_JOININNER", "FdoToken_JOINRIGHTOUTER", 
  "FdoToken_JOINLEFTOUTER", "FdoToken_JOINFULLOUTER", 
  "FdoToken_JOINCROSS", "FdoToken_AND", "FdoToken_OR", "FdoToken_NOT", 
  "FdoToken_LIKE", "FdoToken_IN", "FdoToken_SELECT", "FdoToken_JOIN", 
  "FdoToken_BEYOND", "FdoToken_WITHINDISTANCE", "FdoToken_CONTAINS", 
  "FdoToken_COVEREDBY", "FdoToken_CROSSES", "FdoToken_DISJOINT", 
  "FdoToken_ENVELOPEINTERSECTS", "FdoToken_EQUALS", "FdoToken_INTERSECTS", 
  "FdoToken_INSIDE", "FdoToken_OVERLAPS", "FdoToken_TOUCHES", 
  "FdoToken_WITHIN", "FdoToken_RELATE", "FdoToken_IDENTIFIER", 
  "FdoToken_PARAMETER", "FdoToken_STRING", "FdoToken_INTEGER", 
  "FdoToken_INT64", "FdoToken_INTHEX", "FdoToken_INTBIN", 
  "FdoToken_DOUBLE", "FdoToken_DATETIME", "FdoToken_BLOB", 
  "FdoToken_CLOB", "FdoToken_Add", "FdoToken_Subtract", 
  "FdoToken_Multiply", "FdoToken_Divide", "FdoToken_Negate", 
  "FdoToken_EQ", "FdoToken_NE", "FdoToken_GT", "FdoToken_GE", 
  "FdoToken_LT", "FdoToken_LE", "FdoToken_LeftParenthesis", 
  "FdoToken_RightParenthesis", "FdoToken_Comma", "FdoToken_Dot", 
  "FdoToken_Colon", "FdoToken_AS", "FdoToken_BETWEEN", "'('", "')'", 
  "','", "$accept", "fdo", "Expression", "BinaryExpression", "DataValue", 
  "boolean", "string", "integer", "int64", "inthex", "intbin", "double", 
  "datetime", "Function", "ComputedIdentifierOnly", "ComputedIdentifier3", 
  "JoinCriteria", "JoinCriteriaCollection", "SubSelectExpression", 
  "ComputedIdentifier", "ExpressionCollection", "GeometryValue", 
  "Identifier", "ValueExpression", "LiteralValue", "Parameter", 
  "UnaryExpression", "JoinOperations", 0
};
#endif

# ifdef FDO_EXPRESSION_YYPRINT
/* FDO_EXPRESSION_YYTOKNUM[FDO_EXPRESSION_YYLEX-NUM] -- Internal token number corresponding to
   token FDO_EXPRESSION_YYLEX-NUM.  */
static const unsigned short fdo_expression_yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,    40,    41,    44
};
# endif

/* FDO_EXPRESSION_YYR1[FDO_EXPRESSION_YYN] -- Symbol number of symbol that rule FDO_EXPRESSION_YYN derives.  */
static const unsigned char fdo_expression_yyr1[] =
{
       0,    69,    70,    70,    71,    71,    71,    71,    71,    71,
      71,    72,    72,    72,    72,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    74,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    83,    84,    84,
      84,    84,    84,    84,    84,    85,    85,    85,    85,    86,
      86,    87,    87,    87,    87,    88,    88,    88,    88,    89,
      89,    89,    90,    91,    92,    92,    93,    93,    94,    94,
      95,    96,    96,    96,    96,    96,    96
};

/* FDO_EXPRESSION_YYR2[FDO_EXPRESSION_YYN] -- Number of symbols composing right hand side of rule FDO_EXPRESSION_YYN.  */
static const unsigned char fdo_expression_yyr2[] =
{
       0,     2,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     5,     3,     5,     3,
       1,     5,     3,     5,     3,     6,     8,     6,     8,     1,
       3,     6,     8,     8,    10,     5,     3,     5,     3,     0,
       1,     3,     4,     1,     1,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     1,     1
};

/* FDO_EXPRESSION_YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when FDO_EXPRESSION_YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char fdo_expression_yydefact[] =
{
       0,     3,    25,    26,    27,     0,     0,    63,    68,    28,
      29,    30,    31,    32,    33,    34,    23,    24,     0,     0,
       0,     0,     2,     6,    67,    15,    22,    18,    19,    20,
      21,    17,    16,     7,     0,    10,    66,     8,     9,    64,
      65,     5,     0,     0,    70,    69,     0,     0,     1,     0,
       0,     0,     0,     0,     0,    59,     0,     0,     4,     0,
      11,    12,    13,    14,    56,    58,    60,     0,    62,     0,
       0,     0,    35,     0,     0,     0,     0,     0,     8,    55,
      57,    61,     0,     0,     8,     0,    51,     0,     0,     0,
       4,     0,     0,    56,     0,     0,    49,     0,    58,    39,
       0,     0,     0,     0,    52,     0,    53,     0,    55,    57,
      38,     0,     0,     0,     0,    50,     0,     0,     0,     0,
      54,     0,    71,    72,    73,    74,    75,    76,     0,    37,
       0,     0,    47,     0,    45,     0,    36,     0,     0,    48,
      46
};

/* FDO_EXPRESSION_YYDEFGOTO[NTERM-NUM]. */
static const short fdo_expression_yydefgoto[] =
{
      -1,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,   112,    76,    96,    97,    34,    35,
      67,    36,    37,    38,    39,    40,    41,   128
};

/* FDO_EXPRESSION_YYPACT[STATE-NUM] -- Index in FDO_EXPRESSION_YYTABLE of the portion describing
   STATE-NUM.  */
#define FDO_EXPRESSION_YYPACT_NINF -82
static const short fdo_expression_yypact[] =
{
      60,   -82,   -82,   -82,   -82,   -58,   -41,   -82,   -82,   -82,
     -82,   -82,   -82,   -82,   -82,   -82,   -82,   -82,   106,   -16,
     106,    36,    68,   -82,   -82,   -82,   -82,   -82,   -82,   -82,
     -82,   -82,   -82,   -82,    -9,   -82,   -82,    -4,   -82,   -82,
     -82,   -82,    27,    33,    16,   -82,   -29,   -52,   -82,   106,
     106,   106,   106,    33,    33,   106,    24,    40,    32,    58,
     -47,   -47,    16,    16,   -82,   -82,    68,   -54,   -82,   152,
      33,    33,   -82,   106,   106,    90,   -34,    71,   -27,   -82,
     -82,    68,    28,   -35,   -23,    33,   -82,   -15,    33,    33,
      73,    78,    95,     6,    70,    22,   -82,    26,    46,   -82,
      33,    33,    33,   -36,   -82,   138,   -82,   138,    53,    57,
     -82,    33,    94,   -59,    61,   -82,    96,    72,    33,    72,
     -82,   100,   -82,   -82,   -82,   -82,   -82,   -82,    63,   -82,
      66,    33,   -82,   126,   -82,   126,   -82,    99,   101,   -82,
     -82
};

/* FDO_EXPRESSION_YYPGOTO[NTERM-NUM].  */
static const fdo_expression_yysigned_char fdo_expression_yypgoto[] =
{
     -82,   -82,    -2,   -82,   -82,   -82,   -81,   -82,   -82,   -82,
     -82,   -82,   -82,   -82,   -82,   -82,    64,    62,   -18,   -82,
     -82,   -82,   -43,   -82,   -82,   -82,   -82,    51
};

/* FDO_EXPRESSION_YYTABLE[FDO_EXPRESSION_YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what FDO_EXPRESSION_YYDEFACT says.
   If FDO_EXPRESSION_YYTABLE_NINF, syntax error.  */
#define FDO_EXPRESSION_YYTABLE_NINF -45
static const short fdo_expression_yytable[] =
{
      57,     7,    47,    51,    52,   118,    95,    94,    42,   119,
      64,    65,    54,    72,    73,    59,    44,    53,    46,    49,
      50,    51,    52,    45,     9,    43,    78,    79,    80,    54,
     111,    84,    91,    86,    87,    53,    48,    89,    58,    55,
     -40,   -40,    93,    55,    92,    98,    99,    60,    61,    62,
      63,    77,   137,    66,   138,    54,    83,   108,   109,   110,
     113,     1,    55,     2,     3,     4,    56,    75,   116,     5,
       7,    81,    82,   -44,   -44,   129,    49,    50,    51,    52,
      53,     6,   122,   123,   124,   125,   126,   127,   136,   104,
     105,    68,    53,   106,   107,    90,    70,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    69,     2,
       3,     4,    18,   -42,   -42,     5,    49,    50,    51,    52,
     -43,   -43,    71,    19,   -41,   -41,    20,     6,   120,   107,
     132,   133,    53,   134,   135,    88,   103,   100,    49,    50,
      51,    52,   101,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    85,     2,     3,     4,    18,   102,
      94,     5,   117,   121,   131,     9,   139,   114,   140,    19,
     130,   115,    20,     6,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
       0,     0,     0,     0,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    19,     0,     0,    74
};

static const short fdo_expression_yycheck[] =
{
      43,    37,    20,    50,    51,    64,    87,    22,    66,    68,
      53,    54,    64,    67,    68,    67,    18,    64,    20,    48,
      49,    50,    51,    39,    39,    66,    69,    70,    71,    64,
      66,    74,    67,    67,    68,    64,     0,    64,    67,    66,
      67,    68,    85,    66,    67,    88,    89,    49,    50,    51,
      52,    69,   133,    55,   135,    64,    74,   100,   101,   102,
     103,     1,    66,     3,     4,     5,    39,    69,   111,     9,
      37,    73,    74,    67,    68,   118,    48,    49,    50,    51,
      64,    21,    10,    11,    12,    13,    14,    15,   131,    67,
      68,    67,    64,    67,    68,    67,    64,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    68,     3,
       4,     5,    52,    67,    68,     9,    48,    49,    50,    51,
      67,    68,    64,    63,    67,    68,    66,    21,    67,    68,
      67,    68,    64,    67,    68,    64,    66,    64,    48,    49,
      50,    51,    64,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    64,     3,     4,     5,    52,    64,
      22,     9,    68,    67,    64,    39,    67,   105,    67,    63,
     119,   107,    66,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    66
};

/* FDO_EXPRESSION_YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char fdo_expression_yystos[] =
{
       0,     1,     3,     4,     5,     9,    21,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    52,    63,
      66,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    87,    88,    90,    91,    92,    93,
      94,    95,    66,    66,    71,    39,    71,    87,     0,    48,
      49,    50,    51,    64,    64,    66,    39,    91,    67,    67,
      71,    71,    71,    71,    91,    91,    71,    89,    67,    68,
      64,    64,    67,    68,    66,    71,    84,    87,    91,    91,
      91,    71,    71,    87,    91,    64,    67,    68,    64,    64,
      67,    67,    67,    91,    22,    75,    85,    86,    91,    91,
      64,    64,    64,    66,    67,    68,    67,    68,    91,    91,
      91,    66,    83,    91,    86,    85,    91,    68,    64,    68,
      67,    67,    10,    11,    12,    13,    14,    15,    96,    91,
      96,    64,    67,    68,    67,    68,    91,    75,    75,    67,
      67
};

#if ! defined (FDO_EXPRESSION_YYSIZE_T) && defined (__SIZE_TYPE__)
# define FDO_EXPRESSION_YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (FDO_EXPRESSION_YYSIZE_T) && defined (size_t)
# define FDO_EXPRESSION_YYSIZE_T size_t
#endif
#if ! defined (FDO_EXPRESSION_YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define FDO_EXPRESSION_YYSIZE_T size_t
# endif
#endif
#if ! defined (FDO_EXPRESSION_YYSIZE_T)
# define FDO_EXPRESSION_YYSIZE_T unsigned int
#endif

#define fdo_expression_yyerrok		(fdo_expression_yyerrstatus = 0)
#define fdo_expression_yyclearin	(pParse->fdo_expression_yychar = FDO_EXPRESSION_YYEMPTY)
#define FDO_EXPRESSION_YYEMPTY		(-2)
#define FDO_EXPRESSION_YYEOF		0

#define FDO_EXPRESSION_YYACCEPT	goto fdo_expression_yyacceptlab
#define FDO_EXPRESSION_YYABORT		goto fdo_expression_yyabortlab
#define FDO_EXPRESSION_YYERROR		goto fdo_expression_yyerrlab1

/* Like FDO_EXPRESSION_YYERROR except do call fdo_expression_yyerror.  This remains here temporarily
   to ease the transition to the new meaning of FDO_EXPRESSION_YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define FDO_EXPRESSION_YYFAIL		goto fdo_expression_yyerrlab

#define FDO_EXPRESSION_YYRECOVERING()  (!!fdo_expression_yyerrstatus)

#define FDO_EXPRESSION_YYBACKUP(Token, Value)					\
do								\
  if (pParse->fdo_expression_yychar == FDO_EXPRESSION_YYEMPTY && fdo_expression_yylen == 1)				\
    {								\
      pParse->fdo_expression_yychar = (Token);						\
      pParse->fdo_expression_yylval = (Value);						\
      fdo_expression_yytoken = FDO_EXPRESSION_YYTRANSLATE (pParse->fdo_expression_yychar);				\
      FDO_EXPRESSION_YYPOPSTACK;						\
      goto fdo_expression_yybackup;						\
    }								\
  else								\
    { 								\
      fdo_expression_yyerror(pParse,"syntax error: cannot back up");\
      FDO_EXPRESSION_YYERROR;							\
    }								\
while (0)

#define FDO_EXPRESSION_YYTERROR	1
#define FDO_EXPRESSION_YYERRCODE	256

/* FDO_EXPRESSION_YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef FDO_EXPRESSION_YYLLOC_DEFAULT
# define FDO_EXPRESSION_YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* FDO_EXPRESSION_YYLEX -- calling `fdo_expression_yylex' with the right arguments.  */

#ifdef FDO_EXPRESSION_YYLEX_PARAM
# define FDO_EXPRESSION_YYLEX fdo_expression_yylex (FDO_EXPRESSION_YYLEX_PARAM)
#else
# define FDO_EXPRESSION_YYLEX fdo_expression_yylex(pParse)
#endif

/* Enable debugging if requested.  */
#if FDO_EXPRESSION_YYDEBUG

# ifndef FDO_EXPRESSION_YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define FDO_EXPRESSION_YYFPRINTF fprintf
# endif

# define FDO_EXPRESSION_YYDPRINTF(Args)			\
do {						\
  if (pParse->fdo_expression_yydebug)					\
    FDO_EXPRESSION_YYFPRINTF Args;				\
} while (0)

# define FDO_EXPRESSION_YYDSYMPRINT(Args)			\
do {						\
  if (pParse->fdo_expression_yydebug)					\
    fdo_expression_yysymprint Args;				\
} while (0)

# define FDO_EXPRESSION_YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (pParse->fdo_expression_yydebug)							\
    {								\
      FDO_EXPRESSION_YYFPRINTF (stderr, "%s ", Title);				\
      fdo_expression_yysymprint (stderr, 					\
                  Token, Value);	\
      FDO_EXPRESSION_YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| fdo_expression_yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
fdo_expression_yy_stack_print (short *bottom, short *top)
#else
static void
fdo_expression_yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  FDO_EXPRESSION_YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    FDO_EXPRESSION_YYFPRINTF (stderr, " %d", *bottom);
  FDO_EXPRESSION_YYFPRINTF (stderr, "\n");
}

# define FDO_EXPRESSION_YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (pParse->fdo_expression_yydebug)							\
    fdo_expression_yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the FDO_EXPRESSION_YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
fdo_expression_yy_reduce_print (int fdo_expression_yyrule)
#else
static void
fdo_expression_yy_reduce_print (fdo_expression_yyrule)
    int fdo_expression_yyrule;
#endif
{
  int fdo_expression_yyi;
  unsigned int pParse->fdo_expression_yylineno = fdo_expression_yyrline[fdo_expression_yyrule];
  FDO_EXPRESSION_YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             fdo_expression_yyrule - 1, pParse->fdo_expression_yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (fdo_expression_yyi = fdo_expression_yyprhs[fdo_expression_yyrule]; 0 <= fdo_expression_yyrhs[fdo_expression_yyi]; fdo_expression_yyi++)
    FDO_EXPRESSION_YYFPRINTF (stderr, "%s ", fdo_expression_yytname [fdo_expression_yyrhs[fdo_expression_yyi]]);
  FDO_EXPRESSION_YYFPRINTF (stderr, "-> %s\n", fdo_expression_yytname [fdo_expression_yyr1[fdo_expression_yyrule]]);
}

# define FDO_EXPRESSION_YY_REDUCE_PRINT(Rule)		\
do {					\
  if (pParse->fdo_expression_yydebug)				\
    fdo_expression_yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
#else /* !FDO_EXPRESSION_YYDEBUG */
# define FDO_EXPRESSION_YYDPRINTF(Args)
# define FDO_EXPRESSION_YYDSYMPRINT(Args)
# define FDO_EXPRESSION_YYDSYMPRINTF(Title, Token, Value, Location)
# define FDO_EXPRESSION_YY_STACK_PRINT(Bottom, Top)
# define FDO_EXPRESSION_YY_REDUCE_PRINT(Rule)
#endif /* !FDO_EXPRESSION_YYDEBUG */


/* FDO_EXPRESSION_YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	FDO_EXPRESSION_YYINITDEPTH
# define FDO_EXPRESSION_YYINITDEPTH 200
#endif

/* FDO_EXPRESSION_YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < FDO_EXPRESSION_YYSTACK_BYTES (FDO_EXPRESSION_YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if FDO_EXPRESSION_YYMAXDEPTH == 0
# undef FDO_EXPRESSION_YYMAXDEPTH
#endif

#ifndef FDO_EXPRESSION_YYMAXDEPTH
# define FDO_EXPRESSION_YYMAXDEPTH 10000
#endif



#if FDO_EXPRESSION_YYERROR_VERBOSE

# ifndef fdo_expression_yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define fdo_expression_yystrlen strlen
#  else
/* Return the length of FDO_EXPRESSION_YYSTR.  */
static FDO_EXPRESSION_YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
fdo_expression_yystrlen (const char *fdo_expression_yystr)
#   else
fdo_expression_yystrlen (fdo_expression_yystr)
     const char *fdo_expression_yystr;
#   endif
{
  register const char *fdo_expression_yys = fdo_expression_yystr;

  while (*fdo_expression_yys++ != '\0')
    continue;

  return fdo_expression_yys - fdo_expression_yystr - 1;
}
#  endif
# endif

# ifndef fdo_expression_yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define fdo_expression_yystpcpy stpcpy
#  else
/* Copy FDO_EXPRESSION_YYSRC to FDO_EXPRESSION_YYDEST, returning the address of the terminating '\0' in
   FDO_EXPRESSION_YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
fdo_expression_yystpcpy (char *fdo_expression_yydest, const char *fdo_expression_yysrc)
#   else
fdo_expression_yystpcpy (fdo_expression_yydest, fdo_expression_yysrc)
     char *fdo_expression_yydest;
     const char *fdo_expression_yysrc;
#   endif
{
  register char *fdo_expression_yyd = fdo_expression_yydest;
  register const char *fdo_expression_yys = fdo_expression_yysrc;

  while ((*fdo_expression_yyd++ = *fdo_expression_yys++) != '\0')
    continue;

  return fdo_expression_yyd - 1;
}
#  endif
# endif

#endif /* !FDO_EXPRESSION_YYERROR_VERBOSE */



#if FDO_EXPRESSION_YYDEBUG
/*--------------------------------.
| Print this symbol on FDO_EXPRESSION_YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
fdo_expression_yysymprint (FILE *fdo_expression_yyoutput, int fdo_expression_yytype, FDO_EXPRESSION_YYSTYPE *fdo_expression_yyvaluep)
#else
static void
fdo_expression_yysymprint (fdo_expression_yyoutput, fdo_expression_yytype, fdo_expression_yyvaluep)
    FILE *fdo_expression_yyoutput;
    int fdo_expression_yytype;
    FDO_EXPRESSION_YYSTYPE *fdo_expression_yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) fdo_expression_yyvaluep;

  if (fdo_expression_yytype < FDO_EXPRESSION_YYNTOKENS)
    {
      FDO_EXPRESSION_YYFPRINTF (fdo_expression_yyoutput, "token %s (", fdo_expression_yytname[fdo_expression_yytype]);
# ifdef FDO_EXPRESSION_YYPRINT
      FDO_EXPRESSION_YYPRINT (fdo_expression_yyoutput, fdo_expression_yytoknum[fdo_expression_yytype], *fdo_expression_yyvaluep);
# endif
    }
  else
    FDO_EXPRESSION_YYFPRINTF (fdo_expression_yyoutput, "nterm %s (", fdo_expression_yytname[fdo_expression_yytype]);

  switch (fdo_expression_yytype)
    {
      default:
        break;
    }
  FDO_EXPRESSION_YYFPRINTF (fdo_expression_yyoutput, ")");
}

#endif /* ! FDO_EXPRESSION_YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
fdo_expression_yydestruct (int fdo_expression_yytype, FDO_EXPRESSION_YYSTYPE *fdo_expression_yyvaluep)
#else
static void
fdo_expression_yydestruct (fdo_expression_yytype, fdo_expression_yyvaluep)
    int fdo_expression_yytype;
    FDO_EXPRESSION_YYSTYPE *fdo_expression_yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) fdo_expression_yyvaluep;

  switch (fdo_expression_yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef FDO_EXPRESSION_YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int fdo_expression_yyparse (void *FDO_EXPRESSION_YYPARSE_PARAM);
# else
int fdo_expression_yyparse(FdoParse* pParse);
# endif
#else /* ! FDO_EXPRESSION_YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int fdo_expression_yyparse(FdoParse* pParse);
#else
int fdo_expression_yyparse(FdoParse* pParse);
#endif
#endif /* ! FDO_EXPRESSION_YYPARSE_PARAM */



/* The lookahead symbol.  */

/* The semantic value of the lookahead symbol.  */

/* Number of syntax errors so far.  */



/*----------.
| fdo_expression_yyparse.  |
`----------*/

#ifdef FDO_EXPRESSION_YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int fdo_expression_yyparse (void *FDO_EXPRESSION_YYPARSE_PARAM)
# else
int fdo_expression_yyparse (FDO_EXPRESSION_YYPARSE_PARAM)
  void *FDO_EXPRESSION_YYPARSE_PARAM;
# endif
#else /* ! FDO_EXPRESSION_YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
fdo_expression_yyparse(FdoParse* pParse)
#else
int
fdo_expression_yyparse(FdoParse* pParse)

#endif
#endif
{
  
  register int fdo_expression_yystate;
  register int fdo_expression_yyn;
  int fdo_expression_yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int fdo_expression_yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int fdo_expression_yytoken = 0;

  /* Three stacks and their tools:
     `fdo_expression_yyss': related to states,
     `fdo_expression_yyvs': related to semantic values,
     `fdo_expression_yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow fdo_expression_yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short *fdo_expression_yyss = pParse->fdo_expression_yyss;

  /* The semantic value stack.  */
  FDO_EXPRESSION_YYSTYPE *fdo_expression_yyvs = pParse->fdo_expression_yyvs;



#define FDO_EXPRESSION_YYPOPSTACK   (pParse->fdo_expression_yyvsp--, pParse->fdo_expression_yyssp--)

  FDO_EXPRESSION_YYSIZE_T fdo_expression_yystacksize = FDO_EXPRESSION_YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int fdo_expression_yylen;

  FDO_EXPRESSION_YYDPRINTF ((stderr, "Starting parse\n"));

  fdo_expression_yystate = 0;
  fdo_expression_yyerrstatus = 0;
  pParse->fdo_expression_yynerrs = 0;
  pParse->fdo_expression_yychar = FDO_EXPRESSION_YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  pParse->fdo_expression_yyssp = fdo_expression_yyss;
  pParse->fdo_expression_yyvsp = fdo_expression_yyvs;

  goto fdo_expression_yysetstate;

/*------------------------------------------------------------.
| fdo_expression_yynewstate -- Push a new state, which is found in fdo_expression_yystate.  |
`------------------------------------------------------------*/
 fdo_expression_yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  pParse->fdo_expression_yyssp++;

 fdo_expression_yysetstate:
  *pParse->fdo_expression_yyssp = fdo_expression_yystate;

  if (fdo_expression_yyss + fdo_expression_yystacksize - 1 <= pParse->fdo_expression_yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      FDO_EXPRESSION_YYSIZE_T fdo_expression_yysize = pParse->fdo_expression_yyssp - fdo_expression_yyss + 1;

#ifdef fdo_expression_yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	FDO_EXPRESSION_YYSTYPE *fdo_expression_yyvs1 = fdo_expression_yyvs;
	short *fdo_expression_yyss1 = fdo_expression_yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if fdo_expression_yyoverflow is a macro.  */
	fdo_expression_yyoverflow ("parser stack overflow",
		    &fdo_expression_yyss1, fdo_expression_yysize * sizeof (*pParse->fdo_expression_yyssp),
		    &fdo_expression_yyvs1, fdo_expression_yysize * sizeof (*pParse->fdo_expression_yyvsp),

		    &fdo_expression_yystacksize);

	fdo_expression_yyss = fdo_expression_yyss1;
	fdo_expression_yyvs = fdo_expression_yyvs1;
      }
#else /* no fdo_expression_yyoverflow */
# ifndef FDO_EXPRESSION_YYSTACK_RELOCATE
      goto fdo_expression_yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (FDO_EXPRESSION_YYMAXDEPTH <= fdo_expression_yystacksize)
	goto fdo_expression_yyoverflowlab;
      fdo_expression_yystacksize *= 2;
      if (FDO_EXPRESSION_YYMAXDEPTH < fdo_expression_yystacksize)
	fdo_expression_yystacksize = FDO_EXPRESSION_YYMAXDEPTH;

      {
	short *fdo_expression_yyss1 = fdo_expression_yyss;
	union fdo_expression_yyalloc *fdo_expression_yyptr =
	  (union fdo_expression_yyalloc *) FDO_EXPRESSION_YYSTACK_ALLOC (FDO_EXPRESSION_YYSTACK_BYTES (fdo_expression_yystacksize));
	if (! fdo_expression_yyptr)
	  goto fdo_expression_yyoverflowlab;
	FDO_EXPRESSION_YYSTACK_RELOCATE (fdo_expression_yyss);
	FDO_EXPRESSION_YYSTACK_RELOCATE (fdo_expression_yyvs);

#  undef FDO_EXPRESSION_YYSTACK_RELOCATE
	if (fdo_expression_yyss1 != pParse->fdo_expression_yyss)
	  FDO_EXPRESSION_YYSTACK_FREE (fdo_expression_yyss1);
      }
# endif
#endif /* no fdo_expression_yyoverflow */

      pParse->fdo_expression_yyssp = fdo_expression_yyss + fdo_expression_yysize - 1;
      pParse->fdo_expression_yyvsp = fdo_expression_yyvs + fdo_expression_yysize - 1;


      FDO_EXPRESSION_YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) fdo_expression_yystacksize));

      if (fdo_expression_yyss + fdo_expression_yystacksize - 1 <= pParse->fdo_expression_yyssp)
	FDO_EXPRESSION_YYABORT;
    }

  FDO_EXPRESSION_YYDPRINTF ((stderr, "Entering state %d\n", fdo_expression_yystate));

  goto fdo_expression_yybackup;

/*-----------.
| fdo_expression_yybackup.  |
`-----------*/
fdo_expression_yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* fdo_expression_yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  fdo_expression_yyn = fdo_expression_yypact[fdo_expression_yystate];
  if (fdo_expression_yyn == FDO_EXPRESSION_YYPACT_NINF)
    goto fdo_expression_yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* FDO_EXPRESSION_YYCHAR is either FDO_EXPRESSION_YYEMPTY or FDO_EXPRESSION_YYEOF or a valid lookahead symbol.  */
  if (pParse->fdo_expression_yychar == FDO_EXPRESSION_YYEMPTY)
    {
      FDO_EXPRESSION_YYDPRINTF ((stderr, "Reading a token: "));
      pParse->fdo_expression_yychar = FDO_EXPRESSION_YYLEX;
    }

  if (pParse->fdo_expression_yychar <= FDO_EXPRESSION_YYEOF)
    {
      pParse->fdo_expression_yychar = fdo_expression_yytoken = FDO_EXPRESSION_YYEOF;
      FDO_EXPRESSION_YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      fdo_expression_yytoken = FDO_EXPRESSION_YYTRANSLATE (pParse->fdo_expression_yychar);
      FDO_EXPRESSION_YYDSYMPRINTF ("Next token is", fdo_expression_yytoken, &pParse->fdo_expression_yylval, &fdo_expression_yylloc);
    }

  /* If the proper action on seeing token FDO_EXPRESSION_YYTOKEN is to reduce or to
     detect an error, take that action.  */
  fdo_expression_yyn += fdo_expression_yytoken;
  if (fdo_expression_yyn < 0 || FDO_EXPRESSION_YYLAST < fdo_expression_yyn || fdo_expression_yycheck[fdo_expression_yyn] != fdo_expression_yytoken)
    goto fdo_expression_yydefault;
  fdo_expression_yyn = fdo_expression_yytable[fdo_expression_yyn];
  if (fdo_expression_yyn <= 0)
    {
      if (fdo_expression_yyn == 0 || fdo_expression_yyn == FDO_EXPRESSION_YYTABLE_NINF)
	goto fdo_expression_yyerrlab;
      fdo_expression_yyn = -fdo_expression_yyn;
      goto fdo_expression_yyreduce;
    }

  if (fdo_expression_yyn == FDO_EXPRESSION_YYFINAL)
    FDO_EXPRESSION_YYACCEPT;

  /* Shift the lookahead token.  */
  FDO_EXPRESSION_YYDPRINTF ((stderr, "Shifting token %s, ", fdo_expression_yytname[fdo_expression_yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (pParse->fdo_expression_yychar != FDO_EXPRESSION_YYEOF)
    pParse->fdo_expression_yychar = FDO_EXPRESSION_YYEMPTY;

  *++pParse->fdo_expression_yyvsp = pParse->fdo_expression_yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (fdo_expression_yyerrstatus)
    fdo_expression_yyerrstatus--;

  fdo_expression_yystate = fdo_expression_yyn;
  goto fdo_expression_yynewstate;


/*-----------------------------------------------------------.
| fdo_expression_yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
fdo_expression_yydefault:
  fdo_expression_yyn = fdo_expression_yydefact[fdo_expression_yystate];
  if (fdo_expression_yyn == 0)
    goto fdo_expression_yyerrlab;
  goto fdo_expression_yyreduce;


/*-----------------------------.
| fdo_expression_yyreduce -- Do a reduction.  |
`-----------------------------*/
fdo_expression_yyreduce:
  /* fdo_expression_yyn is the number of a rule to reduce with.  */
  fdo_expression_yylen = fdo_expression_yyr2[fdo_expression_yyn];

  /* If FDO_EXPRESSION_YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets FDO_EXPRESSION_YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to FDO_EXPRESSION_YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that FDO_EXPRESSION_YYVAL may be used uninitialized.  */
  pParse->fdo_expression_yyval = pParse->fdo_expression_yyvsp[1-fdo_expression_yylen];


  FDO_EXPRESSION_YY_REDUCE_PRINT (fdo_expression_yyn);
  switch (fdo_expression_yyn)
    {
        case 2:

    {pParse->SetRoot(Node_Copy(L"Expression", pParse->fdo_expression_yyvsp[0].m_node));	FDO_EXPRESSION_YYACCEPT;}
    break;

  case 3:

    {Node_Trace(L"ERROR!");	pParse->SetRoot((FdoIDisposable*)NULL);	FDO_EXPRESSION_YYABORT;}
    break;

  case 4:

    {pParse->fdo_expression_yyval.m_node=Node_Copy(L"(Expression)", pParse->fdo_expression_yyvsp[-1].m_node);}
    break;

  case 5:

    {pParse->fdo_expression_yyval.m_node=Node_Copy(L"UnaryExpression", pParse->fdo_expression_yyvsp[0].m_node);}
    break;

  case 6:

    {pParse->fdo_expression_yyval.m_node=Node_Copy(L"BinaryExpression", pParse->fdo_expression_yyvsp[0].m_node);}
    break;

  case 7:

    {pParse->fdo_expression_yyval.m_node=Node_Copy(L"Function", pParse->fdo_expression_yyvsp[0].m_node);}
    break;

  case 8:

    {pParse->fdo_expression_yyval.m_node=Node_Copy(L"Identifier", pParse->fdo_expression_yyvsp[0].m_node);}
    break;

  case 9:

    {pParse->fdo_expression_yyval.m_node=Node_Copy(L"ValueExpression", pParse->fdo_expression_yyvsp[0].m_node);}
    break;

  case 10:

    {pParse->fdo_expression_yyval.m_node=Node_Copy(L"ComputedIdentifier", pParse->fdo_expression_yyvsp[0].m_node);}
    break;

  case 11:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"Add", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node, FdoBinaryOperations_Add, (FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node));}
    break;

  case 12:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"Subtract", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node, FdoBinaryOperations_Subtract, (FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node));}
    break;

  case 13:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"Multiply", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node, FdoBinaryOperations_Multiply, (FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node));}
    break;

  case 14:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"Divide", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node, FdoBinaryOperations_Divide, (FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node));}
    break;

  case 15:

    {pParse->fdo_expression_yyval.m_node=Node_Copy(L"boolean", pParse->fdo_expression_yyvsp[0].m_node);}
    break;

  case 16:

    {pParse->fdo_expression_yyval.m_node=Node_Copy(L"datetime", pParse->fdo_expression_yyvsp[0].m_node);}
    break;

  case 17:

    {pParse->fdo_expression_yyval.m_node=Node_Copy(L"double", pParse->fdo_expression_yyvsp[0].m_node);}
    break;

  case 18:

    {pParse->fdo_expression_yyval.m_node=Node_Copy(L"integer", pParse->fdo_expression_yyvsp[0].m_node);}
    break;

  case 19:

    {pParse->fdo_expression_yyval.m_node=Node_Copy(L"int64", pParse->fdo_expression_yyvsp[0].m_node);}
    break;

  case 20:

    {pParse->fdo_expression_yyval.m_node=Node_Copy(L"inthex", pParse->fdo_expression_yyvsp[0].m_node);}
    break;

  case 21:

    {pParse->fdo_expression_yyval.m_node=Node_Copy(L"intbin", pParse->fdo_expression_yyvsp[0].m_node);}
    break;

  case 22:

    {pParse->fdo_expression_yyval.m_node=Node_Copy(L"string", pParse->fdo_expression_yyvsp[0].m_node);}
    break;

  case 23:

    {pParse->fdo_expression_yyval.m_node=Node_Copy(L"TODO:BLOB", NULL);}
    break;

  case 24:

    {pParse->fdo_expression_yyval.m_node=Node_Copy(L"TODO:CLOB", NULL);}
    break;

  case 25:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"NULL", FdoDataValue::Create(FdoDataType_Boolean));}
    break;

  case 26:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"TRUE", FdoBooleanValue::Create(true));}
    break;

  case 27:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"FALSE", FdoBooleanValue::Create(false));}
    break;

  case 28:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"STRING", FdoStringValue::Create(pParse->fdo_expression_yyvsp[0].m_string));}
    break;

  case 29:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"INTEGER", FdoInt32Value::Create(pParse->fdo_expression_yyvsp[0].m_integer));}
    break;

  case 30:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"INT64", FdoInt64Value::Create(pParse->fdo_expression_yyvsp[0].m_int64));}
    break;

  case 31:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"INTHEX", FdoIntHexValue::Create(pParse->fdo_expression_yyvsp[0].m_int64));}
    break;

  case 32:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"INTBIN", FdoIntBinValue::Create(pParse->fdo_expression_yyvsp[0].m_int64));}
    break;

  case 33:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"DOUBLE", FdoDoubleValue::Create(pParse->fdo_expression_yyvsp[0].m_double));}
    break;

  case 34:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"DateTime", FdoDataValue::Create(pParse->fdo_expression_yyvsp[0].m_datetime));}
    break;

  case 35:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"ExpressionCollection", FdoFunction::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[-3].m_node)->GetName(), (FdoExpressionCollection*)pParse->fdo_expression_yyvsp[-1].m_node));}
    break;

  case 36:

    {pParse->fdo_expression_yyval.m_node = Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_expression_yyvsp[-3].m_node));}
    break;

  case 37:

    {pParse->fdo_expression_yyval.m_node = Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node));}
    break;

  case 38:

    {pParse->fdo_expression_yyval.m_node = Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_expression_yyvsp[-3].m_node));}
    break;

  case 39:

    {pParse->fdo_expression_yyval.m_node = Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node));}
    break;

  case 40:

    {pParse->fdo_expression_yyval.m_node = Node_Add(L"IDENTIFIER", FdoIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName()));}
    break;

  case 41:

    {pParse->fdo_expression_yyval.m_node = Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_expression_yyvsp[-3].m_node));}
    break;

  case 42:

    {pParse->fdo_expression_yyval.m_node = Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node));}
    break;

  case 43:

    {pParse->fdo_expression_yyval.m_node = Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_expression_yyvsp[-3].m_node));}
    break;

  case 44:

    {pParse->fdo_expression_yyval.m_node = Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node));}
    break;

  case 45:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"JoinCriteria", FdoJoinCriteria::Create((FdoIdentifier*)pParse->fdo_expression_yyvsp[-3].m_node, (FdoJoinType)pParse->fdo_expression_yyvsp[-1].m_id));}
    break;

  case 46:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"JoinCriteria", FdoJoinCriteria::Create((FdoIdentifier*)pParse->fdo_expression_yyvsp[-5].m_node, (FdoJoinType)pParse->fdo_expression_yyvsp[-3].m_id, FdoPtr<FdoFilter>(FdoFilter::Parse(((FdoStringValue*)pParse->fdo_expression_yyvsp[-1].m_node)->GetString()))));}
    break;

  case 47:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"JoinCriteria", FdoJoinCriteria::Create(((FdoComputedIdentifier*)pParse->fdo_expression_yyvsp[-3].m_node)->GetName(), FdoPtr<FdoIdentifier>(static_cast<FdoIdentifier*>(((FdoComputedIdentifier*)pParse->fdo_expression_yyvsp[-3].m_node)->GetExpression())), (FdoJoinType)pParse->fdo_expression_yyvsp[-1].m_id));}
    break;

  case 48:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"JoinCriteria", FdoJoinCriteria::Create(((FdoComputedIdentifier*)pParse->fdo_expression_yyvsp[-5].m_node)->GetName(), FdoPtr<FdoIdentifier>(static_cast<FdoIdentifier*>(((FdoComputedIdentifier*)pParse->fdo_expression_yyvsp[-5].m_node)->GetExpression())), (FdoJoinType)pParse->fdo_expression_yyvsp[-3].m_id, FdoPtr<FdoFilter>(FdoFilter::Parse(((FdoStringValue*)pParse->fdo_expression_yyvsp[-1].m_node)->GetString()))));}
    break;

  case 49:

    {pParse->fdo_expression_yyval.m_node=pParse->AddNodeToDelete(FdoJoinCriteriaCollection::Create()); ((FdoJoinCriteriaCollection*)pParse->fdo_expression_yyval.m_node)->Add((FdoJoinCriteria*)pParse->fdo_expression_yyvsp[0].m_node); Node_Trace(L"JoinCriteria 1");}
    break;

  case 50:

    {((FdoJoinCriteriaCollection*)pParse->fdo_expression_yyval.m_node)->Add((FdoJoinCriteria*)pParse->fdo_expression_yyvsp[0].m_node); Node_Trace(L"JoinCriteria n");}
    break;

  case 51:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"SubSelectExpression", FdoSubSelectExpression::Create((FdoIdentifier*)pParse->fdo_expression_yyvsp[-3].m_node, (FdoIdentifier*)pParse->fdo_expression_yyvsp[-1].m_node));}
    break;

  case 52:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"SubSelectExpression", FdoSubSelectExpression::Create((FdoIdentifier*)pParse->fdo_expression_yyvsp[-5].m_node, (FdoIdentifier*)pParse->fdo_expression_yyvsp[-3].m_node, FdoPtr<FdoFilter>(FdoFilter::Parse(((FdoStringValue*)pParse->fdo_expression_yyvsp[-1].m_node)->GetString()))));}
    break;

  case 53:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"SubSelectExpression", FdoSubSelectExpression::Create((FdoIdentifier*)pParse->fdo_expression_yyvsp[-5].m_node, (FdoIdentifier*)pParse->fdo_expression_yyvsp[-3].m_node, (FdoJoinCriteriaCollection*)pParse->fdo_expression_yyvsp[-1].m_node));}
    break;

  case 54:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"SubSelectExpression", FdoSubSelectExpression::Create((FdoIdentifier*)pParse->fdo_expression_yyvsp[-7].m_node, (FdoIdentifier*)pParse->fdo_expression_yyvsp[-5].m_node, FdoPtr<FdoFilter>(FdoFilter::Parse(((FdoStringValue*)pParse->fdo_expression_yyvsp[-3].m_node)->GetString())), (FdoJoinCriteriaCollection*)pParse->fdo_expression_yyvsp[-1].m_node));}
    break;

  case 55:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_expression_yyvsp[-3].m_node));}
    break;

  case 56:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node));}
    break;

  case 57:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_expression_yyvsp[-3].m_node));}
    break;

  case 58:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node));}
    break;

  case 59:

    {pParse->fdo_expression_yyval.m_node=pParse->AddNodeToDelete(FdoExpressionCollection::Create());}
    break;

  case 60:

    {pParse->fdo_expression_yyval.m_node=pParse->AddNodeToDelete(FdoExpressionCollection::Create()); ((FdoExpressionCollection*)pParse->fdo_expression_yyval.m_node)->Add((FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node); Node_Trace(L"Expression Arg 1,");}
    break;

  case 61:

    {((FdoExpressionCollection*)pParse->fdo_expression_yyval.m_node)->Add((FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node); Node_Trace(L"Expression Arg N,");}
    break;

  case 62:

    {pParse->fdo_expression_yyval.m_node=Node_Add(L"GeomFromText('FGFT')", pParse->FgftToGeometryValue(pParse->fdo_expression_yyvsp[-1].m_string));}
    break;

  case 63:

    {pParse->fdo_expression_yyval.m_node = Node_Add(L"IDENTIFIER", FdoIdentifier::Create(pParse->fdo_expression_yyvsp[0].m_string));}
    break;

  case 64:

    {pParse->fdo_expression_yyval.m_node=Node_Copy(L"LiteralValue", pParse->fdo_expression_yyvsp[0].m_node);}
    break;

  case 65:

    {pParse->fdo_expression_yyval.m_node=Node_Copy(L"Parameter", pParse->fdo_expression_yyvsp[0].m_node);}
    break;

  case 66:

    {pParse->fdo_expression_yyval.m_node=Node_Copy(L"GeometryValue", pParse->fdo_expression_yyvsp[0].m_node);}
    break;

  case 67:

    {pParse->fdo_expression_yyval.m_node=Node_Copy(L"DataValue", pParse->fdo_expression_yyvsp[0].m_node);}
    break;

  case 68:

    {pParse->fdo_expression_yyval.m_node = Node_Add(L"PARAMETER", FdoParameter::Create(pParse->fdo_expression_yyvsp[0].m_string));}
    break;

  case 69:

    {pParse->fdo_expression_yyval.m_node = Node_Add(L":Parameter", FdoParameter::Create(pParse->fdo_expression_yyvsp[0].m_string));}
    break;

  case 70:

    {pParse->fdo_expression_yyval.m_node = Node_Add(L"UnaryExpression", FdoUnaryExpression::Create(FdoUnaryOperations_Negate, (FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node));}
    break;

  case 71:

    {pParse->fdo_expression_yyval.m_id=FdoJoinType_None; Node_Trace(L"NONE");}
    break;

  case 72:

    {pParse->fdo_expression_yyval.m_id=FdoJoinType_Inner; Node_Trace(L"INNER");}
    break;

  case 73:

    {pParse->fdo_expression_yyval.m_id=FdoJoinType_RightOuter; Node_Trace(L"RIGHTOUTER");}
    break;

  case 74:

    {pParse->fdo_expression_yyval.m_id=FdoJoinType_LeftOuter; Node_Trace(L"LEFTOUTER");}
    break;

  case 75:

    {pParse->fdo_expression_yyval.m_id=FdoJoinType_FullOuter; Node_Trace(L"FULLOUTER");}
    break;

  case 76:

    {pParse->fdo_expression_yyval.m_id=FdoJoinType_Cross; Node_Trace(L"CROSS");}
    break;


    }

/* Line 991 of yacc.c.  */


  pParse->fdo_expression_yyvsp -= fdo_expression_yylen;
  pParse->fdo_expression_yyssp -= fdo_expression_yylen;


  FDO_EXPRESSION_YY_STACK_PRINT (fdo_expression_yyss, pParse->fdo_expression_yyssp);

  *++pParse->fdo_expression_yyvsp = pParse->fdo_expression_yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  fdo_expression_yyn = fdo_expression_yyr1[fdo_expression_yyn];

  fdo_expression_yystate = fdo_expression_yypgoto[fdo_expression_yyn - FDO_EXPRESSION_YYNTOKENS] + *pParse->fdo_expression_yyssp;
  if (0 <= fdo_expression_yystate && fdo_expression_yystate <= FDO_EXPRESSION_YYLAST && fdo_expression_yycheck[fdo_expression_yystate] == *pParse->fdo_expression_yyssp)
    fdo_expression_yystate = fdo_expression_yytable[fdo_expression_yystate];
  else
    fdo_expression_yystate = fdo_expression_yydefgoto[fdo_expression_yyn - FDO_EXPRESSION_YYNTOKENS];

  goto fdo_expression_yynewstate;


/*------------------------------------.
| fdo_expression_yyerrlab -- here on detecting error |
`------------------------------------*/
fdo_expression_yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!fdo_expression_yyerrstatus)
    {
      ++pParse->fdo_expression_yynerrs;
#if FDO_EXPRESSION_YYERROR_VERBOSE
      fdo_expression_yyn = fdo_expression_yypact[fdo_expression_yystate];

      if (FDO_EXPRESSION_YYPACT_NINF < fdo_expression_yyn && fdo_expression_yyn < FDO_EXPRESSION_YYLAST)
	{
	  FDO_EXPRESSION_YYSIZE_T fdo_expression_yysize = 0;
	  int fdo_expression_yytype = FDO_EXPRESSION_YYTRANSLATE (pParse->fdo_expression_yychar);
	  char *fdo_expression_yymsg;
	  int fdo_expression_yyx, fdo_expression_yycount;

	  fdo_expression_yycount = 0;
	  /* Start FDO_EXPRESSION_YYX at -FDO_EXPRESSION_YYN if negative to avoid negative indexes in
	     FDO_EXPRESSION_YYCHECK.  */
	  for (fdo_expression_yyx = fdo_expression_yyn < 0 ? -fdo_expression_yyn : 0;
	       fdo_expression_yyx < (int) (sizeof (fdo_expression_yytname) / sizeof (char *)); fdo_expression_yyx++)
	    if (fdo_expression_yycheck[fdo_expression_yyx + fdo_expression_yyn] == fdo_expression_yyx && fdo_expression_yyx != FDO_EXPRESSION_YYTERROR)
	      fdo_expression_yysize += fdo_expression_yystrlen (fdo_expression_yytname[fdo_expression_yyx]) + 15, fdo_expression_yycount++;
	  fdo_expression_yysize += fdo_expression_yystrlen ("syntax error, unexpected ") + 1;
	  fdo_expression_yysize += fdo_expression_yystrlen (fdo_expression_yytname[fdo_expression_yytype]);
	  fdo_expression_yymsg = (char *) FDO_EXPRESSION_YYSTACK_ALLOC (fdo_expression_yysize);
	  if (fdo_expression_yymsg != 0)
	    {
	      char *fdo_expression_yyp = fdo_expression_yystpcpy (fdo_expression_yymsg, "syntax error, unexpected ");
	      fdo_expression_yyp = fdo_expression_yystpcpy (fdo_expression_yyp, fdo_expression_yytname[fdo_expression_yytype]);

	      if (fdo_expression_yycount < 5)
		{
		  fdo_expression_yycount = 0;
		  for (fdo_expression_yyx = fdo_expression_yyn < 0 ? -fdo_expression_yyn : 0;
		       fdo_expression_yyx < (int) (sizeof (fdo_expression_yytname) / sizeof (char *));
		       fdo_expression_yyx++)
		    if (fdo_expression_yycheck[fdo_expression_yyx + fdo_expression_yyn] == fdo_expression_yyx && fdo_expression_yyx != FDO_EXPRESSION_YYTERROR)
		      {
			const char *fdo_expression_yyq = ! fdo_expression_yycount ? ", expecting " : " or ";
			fdo_expression_yyp = fdo_expression_yystpcpy (fdo_expression_yyp, fdo_expression_yyq);
			fdo_expression_yyp = fdo_expression_yystpcpy (fdo_expression_yyp, fdo_expression_yytname[fdo_expression_yyx]);
			fdo_expression_yycount++;
		      }
		}
	      fdo_expression_yyerror(pParse,fdo_expression_yymsg);
	      FDO_EXPRESSION_YYSTACK_FREE (fdo_expression_yymsg);
	    }
	  else
	    fdo_expression_yyerror(pParse,"syntax error; also virtual memory exhausted");
	}
      else
#endif /* FDO_EXPRESSION_YYERROR_VERBOSE */
	fdo_expression_yyerror(pParse,"syntax error");
    }



  if (fdo_expression_yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (pParse->fdo_expression_yychar == FDO_EXPRESSION_YYEOF)
        {
	  /* Pop the error token.  */
          FDO_EXPRESSION_YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (fdo_expression_yyss < pParse->fdo_expression_yyssp)
	    {
	      FDO_EXPRESSION_YYDSYMPRINTF ("Error: popping", fdo_expression_yystos[*pParse->fdo_expression_yyssp], pParse->fdo_expression_yyvsp, fdo_expression_yylsp);
	      fdo_expression_yydestruct (fdo_expression_yystos[*pParse->fdo_expression_yyssp], pParse->fdo_expression_yyvsp);
	      FDO_EXPRESSION_YYPOPSTACK;
	    }
	  FDO_EXPRESSION_YYABORT;
        }

      FDO_EXPRESSION_YYDSYMPRINTF ("Error: discarding", fdo_expression_yytoken, &pParse->fdo_expression_yylval, &fdo_expression_yylloc);
      fdo_expression_yydestruct (fdo_expression_yytoken, &pParse->fdo_expression_yylval);
      pParse->fdo_expression_yychar = FDO_EXPRESSION_YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto fdo_expression_yyerrlab2;


/*----------------------------------------------------.
| fdo_expression_yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
fdo_expression_yyerrlab1:

  /* Suppress GCC warning that fdo_expression_yyerrlab1 is unused when no action
     invokes FDO_EXPRESSION_YYERROR.  */
#if defined (__GNUC_MINOR__) && 2093 <= (__GNUC__ * 1000 + __GNUC_MINOR__)
  __attribute__ ((__unused__))
#endif


  goto fdo_expression_yyerrlab2;


/*---------------------------------------------------------------.
| fdo_expression_yyerrlab2 -- pop states until the error token can be shifted.  |
`---------------------------------------------------------------*/
fdo_expression_yyerrlab2:
  fdo_expression_yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      fdo_expression_yyn = fdo_expression_yypact[fdo_expression_yystate];
      if (fdo_expression_yyn != FDO_EXPRESSION_YYPACT_NINF)
	{
	  fdo_expression_yyn += FDO_EXPRESSION_YYTERROR;
	  if (0 <= fdo_expression_yyn && fdo_expression_yyn <= FDO_EXPRESSION_YYLAST && fdo_expression_yycheck[fdo_expression_yyn] == FDO_EXPRESSION_YYTERROR)
	    {
	      fdo_expression_yyn = fdo_expression_yytable[fdo_expression_yyn];
	      if (0 < fdo_expression_yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (pParse->fdo_expression_yyssp == fdo_expression_yyss)
	FDO_EXPRESSION_YYABORT;

      FDO_EXPRESSION_YYDSYMPRINTF ("Error: popping", fdo_expression_yystos[*pParse->fdo_expression_yyssp], pParse->fdo_expression_yyvsp, fdo_expression_yylsp);
      fdo_expression_yydestruct (fdo_expression_yystos[fdo_expression_yystate], pParse->fdo_expression_yyvsp);
      pParse->fdo_expression_yyvsp--;
      fdo_expression_yystate = *--pParse->fdo_expression_yyssp;

      FDO_EXPRESSION_YY_STACK_PRINT (fdo_expression_yyss, pParse->fdo_expression_yyssp);
    }

  if (fdo_expression_yyn == FDO_EXPRESSION_YYFINAL)
    FDO_EXPRESSION_YYACCEPT;

  FDO_EXPRESSION_YYDPRINTF ((stderr, "Shifting error token, "));

  *++pParse->fdo_expression_yyvsp = pParse->fdo_expression_yylval;


  fdo_expression_yystate = fdo_expression_yyn;
  goto fdo_expression_yynewstate;


/*-------------------------------------.
| fdo_expression_yyacceptlab -- FDO_EXPRESSION_YYACCEPT comes here.  |
`-------------------------------------*/
fdo_expression_yyacceptlab:
  fdo_expression_yyresult = 0;
  goto fdo_expression_yyreturn;

/*-----------------------------------.
| fdo_expression_yyabortlab -- FDO_EXPRESSION_YYABORT comes here.  |
`-----------------------------------*/
fdo_expression_yyabortlab:
  fdo_expression_yyresult = 1;
  goto fdo_expression_yyreturn;

#ifndef fdo_expression_yyoverflow
/*----------------------------------------------.
| fdo_expression_yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
fdo_expression_yyoverflowlab:
  fdo_expression_yyerror(pParse,"parser stack overflow");
  fdo_expression_yyresult = 2;
  /* Fall through.  */
#endif

fdo_expression_yyreturn:
#ifndef fdo_expression_yyoverflow
  if (fdo_expression_yyss != pParse->fdo_expression_yyss)
    FDO_EXPRESSION_YYSTACK_FREE (fdo_expression_yyss);
#endif
  return fdo_expression_yyresult;
}




///////////////////////////////////////////////////////////////////////

#include <stdio.h>

FdoInt32 fdo_expression_yyerror_default(char *str, FdoInt32 lineno)
{
#ifdef FDO_FDO_EXPRESSION_YYDEBUG
	fprintf(stderr, "line %d: %s\n", lineno, str);
#endif
	return 0;
}

typedef FdoInt32 (*ERRFUNC)(char*, FdoInt32);
static ERRFUNC fdo_expression_yyerror_func = fdo_expression_yyerror_default;

FdoInt32 fdo_expression_yyerror(FdoParse *pParse, char* str)
{
	return (*fdo_expression_yyerror_func)(str, pParse->fdo_expression_yylineno);
}

ERRFUNC fdo_expression_yyerror_set(ERRFUNC func)
{
	ERRFUNC old_func = fdo_expression_yyerror_func;

	if (func != NULL)
		fdo_expression_yyerror_func = func;

	return old_func;
}

static void  print_current_token()
{
//TODO: this has not been updated, but it is unused anyway
#ifdef FDO_FDO_EXPRESSION_YYDEBUG
	char  *token_name;
	
	if( pParse->fdo_expression_yychar <= FDO_EXPRESSION_YYMAXTOKEN )
	{
		switch( pParse->fdo_expression_yychar )
		{
			case IDENTIFIER:
				token_name = pParse->fdo_expression_yylval.id;
				break;

			default:
				token_name = fdo_expression_yyname[pParse->fdo_expression_yychar];
				break;
		}
	}
	else
	{
		token_name = "illegal-name";
	}

	printf( "***  current token is %d (%s)  ***\n", pParse->fdo_expression_yychar, token_name );
#endif
}

