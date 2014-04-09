/* A Bison parser, made by GNU Bison 2.7.1.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.7.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         tdataparse
#define yylex           tdatalex
#define yyerror         tdataerror
#define yylval          tdatalval
#define yychar          tdatachar
#define yydebug         tdatadebug
#define yynerrs         tdatanerrs
#define yylloc          tdatalloc

/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 4 "D:/GitHub/TData/source/tdata_y.yy"

//bison生成的代码会有几个warning, 在这里屏蔽掉
#ifdef _WIN32
#pragma warning (disable: 4244)
#pragma warning (disable: 4702)
#pragma warning (disable: 4127)
#endif

//必须要包含这个玩意， 不然bison生成的文件编译不过
#include <stdio.h>
#include <string.h>

#define YYERROR_VERBOSE
#define YYLEX_PARAM self

#define GET_PARSER TLIBC_CONTAINER_OF(YYLEX_PARAM, PARSER, scanner)
#define GET_SYMBOLS GET_PARSER->symbols
#define GET_DEFINITION GET_PARSER->pn_definition


#define GET_UNION_FIELD_LIST GET_DEFINITION.definition.de_union.union_field_list
#define GET_FIELD_LIST GET_DEFINITION.definition.de_struct.field_list

/* Line 371 of yacc.c  */
#line 100 "D:/GitHub/TData/source/gen/source/tdata_y.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int tdatadebug;
#endif
/* "%code requires" blocks.  */
/* Line 387 of yacc.c  */
#line 30 "D:/GitHub/TData/source/tdata_y.yy"

#include "parse/parser.h"
#include "parse/scanner.h"

#include "parse/check.h"
#include "parse/reduce.h"
#include "symbols.h"


/* Line 387 of yacc.c  */
#line 139 "D:/GitHub/TData/source/gen/source/tdata_y.c"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     tok_import = 258,
     tok_struct = 259,
     tok_union = 260,
     tok_if = 261,
     tok_reserved_keyword = 262,
     tok_int = 263,
     tok_hex = 264,
     tok_int64 = 265,
     tok_uint64 = 266,
     tok_hex_int64 = 267,
     tok_hex_uint64 = 268,
     tok_double = 269,
     tok_string = 270,
     tok_char = 271,
     tok_identifier = 272,
     tok_const = 273,
     tok_enum = 274,
     tok_equal = 275,
     tok_unequal = 276,
     tok_count = 277,
     tok_unixcomment = 278,
     tok_typedef = 279,
     tok_t_int8 = 280,
     tok_t_int16 = 281,
     tok_t_int32 = 282,
     tok_t_int64 = 283,
     tok_t_uint8 = 284,
     tok_t_uint16 = 285,
     tok_t_uint32 = 286,
     tok_t_uint64 = 287,
     tok_t_char = 288,
     tok_t_double = 289,
     tok_t_string = 290,
     tok_t_vector = 291
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int tdataparse (void *YYPARSE_PARAM);
#else
int tdataparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int tdataparse (scanner_t *self);
#else
int tdataparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 224 "D:/GitHub/TData/source/gen/source/tdata_y.c"

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
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (! defined __GNUC__ || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5))
#  define __attribute__(Spec) /* empty */
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif


/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

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

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   121

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  71
/* YYNRULES -- Number of states.  */
#define YYNSTATES  134

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   291

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    45,     2,
      43,    44,     2,     2,    41,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    42,    38,
      46,    37,    47,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    39,     2,    40,     2,     2,     2,     2,
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
      35,    36
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     8,     9,    11,    13,    15,    17,
      19,    21,    23,    26,    31,    38,    39,    47,    50,    52,
      58,    62,    63,    72,    75,    77,    84,    88,    89,    93,
      95,    98,    99,   107,   110,   112,   119,   126,   133,   140,
     141,   143,   145,   152,   154,   156,   158,   160,   162,   164,
     166,   168,   170,   172,   174,   179,   181,   185,   186,   190,
     192,   194,   196,   198,   200,   202,   204,   206,   208,   213,
     215,   217
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      49,     0,    -1,    50,    -1,    50,    51,    -1,    -1,    52,
      -1,    54,    -1,    53,    -1,    66,    -1,    59,    -1,    55,
      -1,    77,    -1,     3,    15,    -1,    24,    73,    17,    38,
      -1,    18,    73,    17,    37,    76,    38,    -1,    -1,    19,
      17,    56,    39,    57,    40,    38,    -1,    57,    58,    -1,
      58,    -1,    17,    37,    76,    41,    78,    -1,    17,    41,
      78,    -1,    -1,     5,    17,    60,    63,    39,    61,    40,
      38,    -1,    61,    62,    -1,    62,    -1,    17,    42,    73,
      17,    38,    78,    -1,    43,    64,    44,    -1,    -1,    64,
      41,    65,    -1,    65,    -1,    73,    17,    -1,    -1,     4,
      17,    67,    39,    68,    40,    38,    -1,    68,    69,    -1,
      69,    -1,    70,    71,    17,    74,    38,    78,    -1,     6,
      43,    17,    45,    76,    44,    -1,     6,    43,    17,    20,
      76,    44,    -1,     6,    43,    17,    21,    76,    44,    -1,
      -1,    73,    -1,    72,    -1,    36,    46,    73,    41,    17,
      47,    -1,    33,    -1,    34,    -1,    25,    -1,    26,    -1,
      27,    -1,    28,    -1,    29,    -1,    30,    -1,    31,    -1,
      32,    -1,    17,    -1,    35,    46,    17,    47,    -1,    35,
      -1,    43,    75,    44,    -1,    -1,    75,    41,    17,    -1,
      17,    -1,    11,    -1,    13,    -1,    10,    -1,    12,    -1,
      14,    -1,    15,    -1,    16,    -1,    17,    -1,    22,    43,
      17,    44,    -1,    23,    -1,    23,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   122,   122,   125,   129,   133,   138,   143,   148,   152,
     156,   160,   167,   176,   187,   202,   201,   216,   228,   234,
     248,   271,   270,   284,   294,   300,   314,   319,   324,   334,
     341,   360,   359,   378,   388,   394,   428,   442,   457,   473,
     481,   485,   491,   505,   509,   513,   517,   521,   525,   529,
     533,   537,   541,   545,   553,   563,   570,   575,   580,   589,
     597,   601,   605,   609,   613,   617,   621,   625,   633,   643,
     650,   656
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "tok_import", "tok_struct", "tok_union",
  "tok_if", "tok_reserved_keyword", "tok_int", "tok_hex", "tok_int64",
  "tok_uint64", "tok_hex_int64", "tok_hex_uint64", "tok_double",
  "tok_string", "tok_char", "tok_identifier", "tok_const", "tok_enum",
  "tok_equal", "tok_unequal", "tok_count", "tok_unixcomment",
  "tok_typedef", "tok_t_int8", "tok_t_int16", "tok_t_int32", "tok_t_int64",
  "tok_t_uint8", "tok_t_uint16", "tok_t_uint32", "tok_t_uint64",
  "tok_t_char", "tok_t_double", "tok_t_string", "tok_t_vector", "'='",
  "';'", "'{'", "'}'", "','", "':'", "'('", "')'", "'&'", "'<'", "'>'",
  "$accept", "Document", "DefinitionList", "Definition", "Import",
  "Typedef", "Const", "Enum", "$@1", "EnumDefList", "EnumDef", "Union",
  "$@2", "UnionFieldList", "UnionField", "Parameters", "ParameterList",
  "Parameter", "Struct", "$@3", "FieldList", "Field", "Condition", "Type",
  "ContainerType", "SimpleType", "Arguments", "ArgumentList", "Value",
  "UnixComment", "UnixCommentOrNot", YY_NULL
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
     285,   286,   287,   288,   289,   290,   291,    61,    59,   123,
     125,    44,    58,    40,    41,    38,    60,    62
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    48,    49,    50,    50,    51,    51,    51,    51,    51,
      51,    51,    52,    53,    54,    56,    55,    57,    57,    58,
      58,    60,    59,    61,    61,    62,    63,    63,    64,    64,
      65,    67,    66,    68,    68,    69,    70,    70,    70,    70,
      71,    71,    72,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    74,    74,    75,    75,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    77,
      78,    78
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     1,     1,     1,     1,     1,
       1,     1,     2,     4,     6,     0,     7,     2,     1,     5,
       3,     0,     8,     2,     1,     6,     3,     0,     3,     1,
       2,     0,     7,     2,     1,     6,     6,     6,     6,     0,
       1,     1,     6,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     1,     3,     0,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     1,
       1,     0
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,     2,     1,     0,     0,     0,     0,     0,    69,
       0,     3,     5,     7,     6,    10,     9,     8,    11,    12,
      31,    21,    53,    45,    46,    47,    48,    49,    50,    51,
      52,    43,    44,    55,     0,    15,     0,     0,    27,     0,
       0,     0,     0,    39,     0,     0,     0,     0,     0,    13,
       0,    39,    34,     0,     0,    29,     0,     0,    54,    62,
      60,    63,    61,    64,    65,    66,    67,     0,     0,     0,
       0,    18,     0,     0,    33,     0,     0,    41,    40,     0,
      26,    30,     0,     0,    24,     0,    14,     0,    71,     0,
      17,     0,    32,     0,    57,    28,     0,     0,    23,     0,
       0,    70,    20,    16,     0,     0,     0,     0,     0,     0,
       0,    22,    68,    71,     0,     0,     0,     0,    59,     0,
      71,     0,    19,    37,    38,    36,     0,     0,    56,    35,
      71,    42,    58,    25
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     2,    11,    12,    13,    14,    15,    41,    70,
      71,    16,    38,    83,    84,    45,    54,    55,    17,    37,
      51,    52,    53,    76,    77,    56,   109,   119,    68,    18,
     102
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -98
static const yytype_int8 yypact[] =
{
     -98,     9,     8,   -98,     5,    -9,     4,    42,     7,   -98,
      42,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,
     -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,
     -98,   -98,   -98,   -24,    11,   -98,    17,     1,    -2,    25,
       6,    21,    23,    38,    42,    24,    15,    68,    28,   -98,
      44,    -5,   -98,    22,   -27,   -98,    47,    48,   -98,   -98,
     -98,   -98,   -98,   -98,   -98,   -98,   -98,    45,    53,   -31,
     -15,   -98,    49,    54,   -98,    50,    76,   -98,   -98,    42,
     -98,   -98,    52,   -10,   -98,    78,   -98,    68,    74,    60,
     -98,   -16,   -98,    42,    56,   -98,    42,    62,   -98,    57,
      61,   -98,   -98,   -98,    68,    68,    68,    63,    86,    67,
      89,   -98,   -98,    74,    64,    65,    66,    90,   -98,   -26,
      74,    73,   -98,   -98,   -98,   -98,    69,    95,   -98,   -98,
      74,   -98,   -98,   -98
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,
      43,   -98,   -98,   -98,    31,   -98,   -98,    36,   -98,   -98,
     -98,    70,   -98,   -98,   -98,    -7,   -98,   -98,   -68,   -98,
     -97
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      34,    50,    69,    36,   104,   105,    87,    82,    20,     3,
      88,     4,     5,     6,    79,   127,   122,    80,   128,   100,
      19,    21,    39,   129,    35,    89,     7,     8,    40,   106,
      97,     9,    10,   133,    42,    73,   114,   115,   116,    22,
      43,    44,    46,    47,    50,    69,    78,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    75,    22,
      48,    49,    58,    57,    81,    82,    91,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    59,    60,
      61,    62,    63,    64,    65,    66,   107,    72,    85,   110,
      67,    86,    92,    94,    96,    99,    93,   101,   103,   108,
     111,   112,   113,   118,   117,   120,   121,   126,   123,   124,
     125,   130,   132,    90,    98,    95,   131,     0,     0,     0,
       0,    74
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-98)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       7,     6,    17,    10,    20,    21,    37,    17,    17,     0,
      41,     3,     4,     5,    41,    41,   113,    44,    44,    87,
      15,    17,    46,   120,    17,    40,    18,    19,    17,    45,
      40,    23,    24,   130,    17,    40,   104,   105,   106,    17,
      39,    43,    17,    37,     6,    17,    53,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    17,
      39,    38,    47,    39,    17,    17,    17,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    10,    11,
      12,    13,    14,    15,    16,    17,    93,    43,    43,    96,
      22,    38,    38,    17,    42,    17,    46,    23,    38,    43,
      38,    44,    41,    17,    41,    38,    17,    17,    44,    44,
      44,    38,    17,    70,    83,    79,    47,    -1,    -1,    -1,
      -1,    51
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    49,    50,     0,     3,     4,     5,    18,    19,    23,
      24,    51,    52,    53,    54,    55,    59,    66,    77,    15,
      17,    17,    17,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    73,    17,    73,    67,    60,    46,
      17,    56,    17,    39,    43,    63,    17,    37,    39,    38,
       6,    68,    69,    70,    64,    65,    73,    39,    47,    10,
      11,    12,    13,    14,    15,    16,    17,    22,    76,    17,
      57,    58,    43,    40,    69,    36,    71,    72,    73,    41,
      44,    17,    17,    61,    62,    43,    38,    37,    41,    40,
      58,    17,    38,    46,    17,    65,    42,    40,    62,    17,
      76,    23,    78,    38,    20,    21,    45,    73,    43,    74,
      73,    38,    44,    41,    76,    76,    76,    41,    17,    75,
      38,    17,    78,    44,    44,    44,    17,    41,    44,    78,
      38,    47,    17,    78
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
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (&yylloc, self, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (YYID (N))                                                     \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (YYID (0))
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

__attribute__((__unused__))
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
#else
static unsigned
yy_location_print_ (yyo, yylocp)
    FILE *yyo;
    YYLTYPE const * const yylocp;
#endif
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += fprintf (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += fprintf (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += fprintf (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += fprintf (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += fprintf (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, &yylloc, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, &yylloc)
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
		  Type, Value, Location, self); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, scanner_t *self)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, self)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    scanner_t *self;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
  YYUSE (self);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, scanner_t *self)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp, self)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    scanner_t *self;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, self);
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
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, scanner_t *self)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule, self)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
    scanner_t *self;
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
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       , self);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule, self); \
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, scanner_t *self)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp, self)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
    scanner_t *self;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (self);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YYUSE (yytype);
}




/*----------.
| yyparse.  |
`----------*/

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
yyparse (scanner_t *self)
#else
int
yyparse (self)
    scanner_t *self;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;


#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
static YYSTYPE yyval_default;
# define YY_INITIAL_VALUE(Value) = Value
#endif
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc = yyloc_default;


    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
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
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
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
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
  if (yypact_value_is_default (yyn))
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
      if (yytable_value_is_error (yyn))
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
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
/* Line 1787 of yacc.c  */
#line 126 "D:/GitHub/TData/source/tdata_y.yy"
    {
		parser_on_definition(GET_PARSER, &yylloc, &GET_DEFINITION);
	}
    break;

  case 4:
/* Line 1787 of yacc.c  */
#line 129 "D:/GitHub/TData/source/tdata_y.yy"
    {
	}
    break;

  case 5:
/* Line 1787 of yacc.c  */
#line 134 "D:/GitHub/TData/source/tdata_y.yy"
    {
		GET_DEFINITION.type = E_DT_IMPORT;
		GET_DEFINITION.definition.de_import = (yyvsp[(1) - (1)].sn_import);
	}
    break;

  case 6:
/* Line 1787 of yacc.c  */
#line 139 "D:/GitHub/TData/source/tdata_y.yy"
    {
		GET_DEFINITION.type = E_DT_CONST;
		GET_DEFINITION.definition.de_const = (yyvsp[(1) - (1)].sn_const);
	}
    break;

  case 7:
/* Line 1787 of yacc.c  */
#line 144 "D:/GitHub/TData/source/tdata_y.yy"
    {
		GET_DEFINITION.type = E_DT_TYPEDEF;
		GET_DEFINITION.definition.de_typedef = (yyvsp[(1) - (1)].sn_typedef);
	}
    break;

  case 8:
/* Line 1787 of yacc.c  */
#line 149 "D:/GitHub/TData/source/tdata_y.yy"
    {
		GET_DEFINITION.type = E_DT_STRUCT;
	}
    break;

  case 9:
/* Line 1787 of yacc.c  */
#line 153 "D:/GitHub/TData/source/tdata_y.yy"
    {
		GET_DEFINITION.type = E_DT_UNION;
	}
    break;

  case 10:
/* Line 1787 of yacc.c  */
#line 157 "D:/GitHub/TData/source/tdata_y.yy"
    {
		GET_DEFINITION.type = E_DT_ENUM;
	}
    break;

  case 11:
/* Line 1787 of yacc.c  */
#line 161 "D:/GitHub/TData/source/tdata_y.yy"
    {
		GET_DEFINITION.type = E_DT_UNIX_COMMENT;
		GET_DEFINITION.definition.de_unix_comment = (yyvsp[(1) - (1)].sn_unix_comment);
	}
    break;

  case 12:
/* Line 1787 of yacc.c  */
#line 168 "D:/GitHub/TData/source/tdata_y.yy"
    {
		check_strlen_too_long(&yylloc, (yyvsp[(2) - (2)].sn_string), "", MAX_PACKAGE_NAME_LENGTH);
		

		reduce_Import(&(yyval.sn_import), (yyvsp[(2) - (2)].sn_string));
	}
    break;

  case 13:
/* Line 1787 of yacc.c  */
#line 177 "D:/GitHub/TData/source/tdata_y.yy"
    {
		check_identifier_not_defined(&GET_SYMBOLS, &yylloc, "", (yyvsp[(3) - (4)].sn_tok_identifier));
		check_string_length(&yylloc, &(yyvsp[(2) - (4)].sn_simple_type), TRUE);

		reduce_Typedef(&(yyval.sn_typedef), &(yyvsp[(2) - (4)].sn_simple_type), (yyvsp[(3) - (4)].sn_tok_identifier));

		symbols_add_Typedef(&GET_SYMBOLS, &yylloc, &(yyval.sn_typedef));
	}
    break;

  case 14:
/* Line 1787 of yacc.c  */
#line 188 "D:/GitHub/TData/source/tdata_y.yy"
    {
		check_identifier_not_defined(&GET_SYMBOLS, &yylloc, "", (yyvsp[(3) - (6)].sn_tok_identifier));

		check_string_length(&yylloc, &(yyvsp[(2) - (6)].sn_simple_type), FALSE);

		check_value_type(&GET_SYMBOLS, &yylloc, &(yyvsp[(2) - (6)].sn_simple_type), &(yyvsp[(5) - (6)].sn_value));


		reduce_Const(&(yyval.sn_const), &(yyvsp[(2) - (6)].sn_simple_type), (yyvsp[(3) - (6)].sn_tok_identifier), &(yyvsp[(5) - (6)].sn_value));		
		symbols_add_Const(&GET_SYMBOLS, &yylloc, &(yyval.sn_const));
	}
    break;

  case 15:
/* Line 1787 of yacc.c  */
#line 202 "D:/GitHub/TData/source/tdata_y.yy"
    {
		GET_SYMBOLS.enum_name = (yyvsp[(2) - (2)].sn_tok_identifier);
		GET_DEFINITION.definition.de_enum.enum_def_list_num = 0;
	}
    break;

  case 16:
/* Line 1787 of yacc.c  */
#line 207 "D:/GitHub/TData/source/tdata_y.yy"
    {
		check_identifier_not_defined(&GET_SYMBOLS, &yylloc, "", (yyvsp[(2) - (7)].sn_tok_identifier));

		reduce_Enum(&GET_DEFINITION.definition.de_enum, (yyvsp[(2) - (7)].sn_tok_identifier));

		symbols_add_Enum(&GET_SYMBOLS, &yylloc, &GET_DEFINITION.definition.de_enum);
	}
    break;

  case 17:
/* Line 1787 of yacc.c  */
#line 217 "D:/GitHub/TData/source/tdata_y.yy"
    {
		if(GET_DEFINITION.definition.de_enum.enum_def_list_num >= MAX_ENUM_DEF_LIST_NUM)
		{
			scanner_error_halt(&yylloc, E_LS_TOO_MANY_MEMBERS, MAX_ENUM_DEF_LIST_NUM);
		}
		
		GET_DEFINITION.definition.de_enum.enum_def_list[GET_DEFINITION.definition.de_enum.enum_def_list_num] = (yyvsp[(2) - (2)].sn_enum_def);
		check_enumdef_is_unique(&yylloc, &GET_DEFINITION.definition.de_enum, GET_DEFINITION.definition.de_enum.enum_def_list_num);
		++GET_DEFINITION.definition.de_enum.enum_def_list_num;
	}
    break;

  case 18:
/* Line 1787 of yacc.c  */
#line 229 "D:/GitHub/TData/source/tdata_y.yy"
    {
		GET_DEFINITION.definition.de_enum.enum_def_list[GET_DEFINITION.definition.de_enum.enum_def_list_num++] = (yyvsp[(1) - (1)].sn_enum_def);
	}
    break;

  case 19:
/* Line 1787 of yacc.c  */
#line 235 "D:/GitHub/TData/source/tdata_y.yy"
    {
		ST_SIMPLE_TYPE enum_type;
		enum_type.st = E_ST_INT32;

		check_identifier_not_defined(&GET_SYMBOLS, &yylloc, "", (yyvsp[(1) - (5)].sn_tok_identifier));
		check_identifier_not_defined(&GET_SYMBOLS, &yylloc, GET_SYMBOLS.enum_name, (yyvsp[(1) - (5)].sn_tok_identifier));
				
		check_value_type(&GET_SYMBOLS, &yylloc, &enum_type, &(yyvsp[(3) - (5)].sn_value));

		reduce_EnumDef_Value(&(yyval.sn_enum_def), (yyvsp[(1) - (5)].sn_tok_identifier), &(yyvsp[(3) - (5)].sn_value), &(yyvsp[(5) - (5)].sn_unix_comment));

		symbols_add_EnumDef(&GET_SYMBOLS, &yylloc, &(yyval.sn_enum_def));
	}
    break;

  case 20:
/* Line 1787 of yacc.c  */
#line 249 "D:/GitHub/TData/source/tdata_y.yy"
    {
		ST_SIMPLE_TYPE enum_type;
		enum_type.st = E_ST_INT32;

		check_identifier_not_defined(&GET_SYMBOLS, &yylloc, "", (yyvsp[(1) - (3)].sn_tok_identifier));
		check_identifier_not_defined(&GET_SYMBOLS, &yylloc, GET_SYMBOLS.enum_name, (yyvsp[(1) - (3)].sn_tok_identifier));
		
		if(GET_DEFINITION.definition.de_enum.enum_def_list_num == 0)
		{
			reduce_EnumDef(&(yyval.sn_enum_def), (yyvsp[(1) - (3)].sn_tok_identifier), NULL, &(yyvsp[(3) - (3)].sn_unix_comment));
		}
		else
		{
			reduce_EnumDef(&(yyval.sn_enum_def), (yyvsp[(1) - (3)].sn_tok_identifier), 
			&GET_DEFINITION.definition.de_enum.enum_def_list[GET_DEFINITION.definition.de_enum.enum_def_list_num - 1].val, &(yyvsp[(3) - (3)].sn_unix_comment));
		}

		symbols_add_EnumDef(&GET_SYMBOLS, &yylloc, &(yyval.sn_enum_def));
	}
    break;

  case 21:
/* Line 1787 of yacc.c  */
#line 271 "D:/GitHub/TData/source/tdata_y.yy"
    {
		check_identifier_not_defined(&GET_SYMBOLS, &yylloc, "", (yyvsp[(2) - (2)].sn_tok_identifier));
		GET_SYMBOLS.union_name = (yyvsp[(2) - (2)].sn_tok_identifier);
		GET_UNION_FIELD_LIST.union_field_list_num = 0;
	}
    break;

  case 22:
/* Line 1787 of yacc.c  */
#line 277 "D:/GitHub/TData/source/tdata_y.yy"
    {
		reduce_Union(&GET_DEFINITION.definition.de_union, (yyvsp[(2) - (8)].sn_tok_identifier), &(yyvsp[(4) - (8)].sn_parameters));

		symbols_add_Union(&GET_SYMBOLS, &yylloc, &GET_DEFINITION.definition.de_union);
	}
    break;

  case 23:
/* Line 1787 of yacc.c  */
#line 285 "D:/GitHub/TData/source/tdata_y.yy"
    {
		if(GET_UNION_FIELD_LIST.union_field_list_num >= MAX_UNION_FIELD_LIST_NUM)
		{
			scanner_error_halt(&yylloc, E_LS_TOO_MANY_MEMBERS, MAX_UNION_FIELD_LIST_NUM);
		}

		GET_UNION_FIELD_LIST.union_field_list[GET_UNION_FIELD_LIST.union_field_list_num++] = (yyvsp[(2) - (2)].sn_union_field);
	}
    break;

  case 24:
/* Line 1787 of yacc.c  */
#line 295 "D:/GitHub/TData/source/tdata_y.yy"
    {
		GET_UNION_FIELD_LIST.union_field_list[GET_UNION_FIELD_LIST.union_field_list_num++] = (yyvsp[(1) - (1)].sn_union_field);
	}
    break;

  case 25:
/* Line 1787 of yacc.c  */
#line 301 "D:/GitHub/TData/source/tdata_y.yy"
    {
		check_identifier_not_defined_as_value(&GET_SYMBOLS, &yylloc, "", (yyvsp[(4) - (6)].sn_tok_identifier));

		check_identifier_not_defined(&GET_SYMBOLS, &yylloc, GET_SYMBOLS.union_name, (yyvsp[(1) - (6)].sn_tok_identifier));

		check_string_length_defined(&yylloc, &(yyvsp[(3) - (6)].sn_simple_type));

		reduce_UnionField(&(yyval.sn_union_field), (yyvsp[(1) - (6)].sn_tok_identifier), &(yyvsp[(3) - (6)].sn_simple_type), (yyvsp[(4) - (6)].sn_tok_identifier), &(yyvsp[(6) - (6)].sn_unix_comment));

		symbols_add_UnionField(&GET_SYMBOLS, &yylloc, &(yyval.sn_union_field));
	}
    break;

  case 26:
/* Line 1787 of yacc.c  */
#line 315 "D:/GitHub/TData/source/tdata_y.yy"
    {
		(yyval.sn_parameters) = (yyvsp[(2) - (3)].sn_parameters);
	}
    break;

  case 27:
/* Line 1787 of yacc.c  */
#line 319 "D:/GitHub/TData/source/tdata_y.yy"
    {
		(yyval.sn_parameters).par_list_num = 0;
	}
    break;

  case 28:
/* Line 1787 of yacc.c  */
#line 325 "D:/GitHub/TData/source/tdata_y.yy"
    {
		if((yyvsp[(1) - (3)].sn_parameters).par_list_num >= MAX_PARAMETER_NUM)
		{
			scanner_error_halt(&yylloc, E_LS_TOO_MANY_PARAMETERS, MAX_PARAMETER_NUM);
		}
		(yyval.sn_parameters) = (yyvsp[(1) - (3)].sn_parameters);
		(yyval.sn_parameters).par_list[(yyval.sn_parameters).par_list_num++] = (yyvsp[(3) - (3)].sn_parameter);
	}
    break;

  case 29:
/* Line 1787 of yacc.c  */
#line 335 "D:/GitHub/TData/source/tdata_y.yy"
    {
		(yyval.sn_parameters).par_list[0] = (yyvsp[(1) - (1)].sn_parameter);
		(yyval.sn_parameters).par_list_num = 1;
	}
    break;

  case 30:
/* Line 1787 of yacc.c  */
#line 342 "D:/GitHub/TData/source/tdata_y.yy"
    {
		check_str_equal(&yylloc, (yyvsp[(2) - (2)].sn_tok_identifier), "selector");

		check_simpletype_is_enum(&GET_SYMBOLS, &yylloc, &(yyvsp[(1) - (2)].sn_simple_type));

		(yyval.sn_parameter).type = (yyvsp[(1) - (2)].sn_simple_type);
		strncpy((yyval.sn_parameter).identifier, (yyvsp[(2) - (2)].sn_tok_identifier), TLIBC_MAX_LENGTH_OF_IDENTIFIER - 1);
		(yyval.sn_parameter).identifier[TLIBC_MAX_LENGTH_OF_IDENTIFIER - 1] = 0;
	}
    break;

  case 31:
/* Line 1787 of yacc.c  */
#line 360 "D:/GitHub/TData/source/tdata_y.yy"
    {
		parser_on_struct_begin(GET_PARSER, &yylloc, (yyvsp[(2) - (2)].sn_tok_identifier));

		check_identifier_not_defined(&GET_SYMBOLS, &yylloc, "", (yyvsp[(2) - (2)].sn_tok_identifier));

		GET_SYMBOLS.struct_name = (yyvsp[(2) - (2)].sn_tok_identifier);
		GET_FIELD_LIST.field_list_num = 0;
	}
    break;

  case 32:
/* Line 1787 of yacc.c  */
#line 369 "D:/GitHub/TData/source/tdata_y.yy"
    {
		strncpy(GET_DEFINITION.definition.de_struct.name, (yyvsp[(2) - (7)].sn_tok_identifier), TLIBC_MAX_LENGTH_OF_IDENTIFIER);
		GET_DEFINITION.definition.de_struct.name[TLIBC_MAX_LENGTH_OF_IDENTIFIER - 1] = 0;

		symbols_add_Struct(&GET_SYMBOLS, &yylloc, &GET_DEFINITION.definition.de_struct);
		parser_on_struct_end(GET_PARSER, &yylloc, &GET_DEFINITION.definition.de_struct);
	}
    break;

  case 33:
/* Line 1787 of yacc.c  */
#line 379 "D:/GitHub/TData/source/tdata_y.yy"
    {
		if(GET_FIELD_LIST.field_list_num >= MAX_FIELD_LIST_NUM)
		{
			scanner_error_halt(&yylloc, E_LS_TOO_MANY_MEMBERS, MAX_FIELD_LIST_NUM);
		}

		GET_FIELD_LIST.field_list[GET_FIELD_LIST.field_list_num++] = (yyvsp[(2) - (2)].sn_field);
	}
    break;

  case 34:
/* Line 1787 of yacc.c  */
#line 389 "D:/GitHub/TData/source/tdata_y.yy"
    {
		GET_FIELD_LIST.field_list[GET_FIELD_LIST.field_list_num++] = (yyvsp[(1) - (1)].sn_field);
	}
    break;

  case 35:
/* Line 1787 of yacc.c  */
#line 395 "D:/GitHub/TData/source/tdata_y.yy"
    {
		check_identifier_not_defined(&GET_SYMBOLS, &yylloc, GET_SYMBOLS.struct_name, (yyvsp[(3) - (6)].sn_tok_identifier));

		check_identifier_not_defined_as_value(&GET_SYMBOLS, &yylloc, "", (yyvsp[(3) - (6)].sn_tok_identifier));
		

		if(((yyvsp[(2) - (6)].sn_type).type == E_SNT_CONTAINER) && ((yyvsp[(2) - (6)].sn_type).ct.ct == E_CT_VECTOR))
		{
			char name[TLIBC_MAX_LENGTH_OF_IDENTIFIER];
			check_strlen_too_long(&yylloc, (yyvsp[(3) - (6)].sn_tok_identifier), "_num", TLIBC_MAX_LENGTH_OF_IDENTIFIER);
			snprintf(name, TLIBC_MAX_LENGTH_OF_IDENTIFIER, "%s_num", (yyvsp[(3) - (6)].sn_tok_identifier));

			check_identifier_not_defined(&GET_SYMBOLS, &yylloc, GET_SYMBOLS.struct_name, name);

			check_identifier_not_defined_as_value(&GET_SYMBOLS, &yylloc, "", name);
		}
		

		if((yyvsp[(2) - (6)].sn_type).type == E_SNT_SIMPLE)
		{
			check_string_length_defined(&yylloc, &(yyvsp[(2) - (6)].sn_type).st);
		}

		check_arguments(&GET_SYMBOLS, &yylloc, &(yyvsp[(2) - (6)].sn_type), &(yyvsp[(4) - (6)].sn_arguments));

		reduce_Field(&(yyval.sn_field), &(yyvsp[(1) - (6)].sn_condition), &(yyvsp[(2) - (6)].sn_type), (yyvsp[(3) - (6)].sn_tok_identifier), &(yyvsp[(4) - (6)].sn_arguments), &(yyvsp[(6) - (6)].sn_unix_comment));		
		
		symbols_add_Field(&GET_SYMBOLS, &yylloc, &(yyval.sn_field));

		parser_on_field(GET_PARSER, &yylloc, &(yyval.sn_field));
	}
    break;

  case 36:
/* Line 1787 of yacc.c  */
#line 429 "D:/GitHub/TData/source/tdata_y.yy"
    {
		check_identifier_defined(&GET_SYMBOLS, &yylloc, GET_SYMBOLS.struct_name, (yyvsp[(3) - (6)].sn_tok_identifier));

		check_identifier_refer_to_a_field_with_integer_type(&GET_SYMBOLS, &yylloc, GET_SYMBOLS.struct_name, (yyvsp[(3) - (6)].sn_tok_identifier));

		check_integer_value(&GET_SYMBOLS, &yylloc, &(yyvsp[(5) - (6)].sn_value));

		strncpy((yyval.sn_condition).op0, (yyvsp[(3) - (6)].sn_tok_identifier), TLIBC_MAX_LENGTH_OF_IDENTIFIER);
		(yyval.sn_condition).op0[TLIBC_MAX_LENGTH_OF_IDENTIFIER - 1] = 0;

		(yyval.sn_condition).oper = E_EO_AND;
		(yyval.sn_condition).op1 = (yyvsp[(5) - (6)].sn_value);
	}
    break;

  case 37:
/* Line 1787 of yacc.c  */
#line 443 "D:/GitHub/TData/source/tdata_y.yy"
    {
		check_identifier_defined(&GET_SYMBOLS, &yylloc, GET_SYMBOLS.struct_name, (yyvsp[(3) - (6)].sn_tok_identifier));

		check_identifier_refer_to_a_field_with_integer_type(&GET_SYMBOLS, &yylloc, GET_SYMBOLS.struct_name, (yyvsp[(3) - (6)].sn_tok_identifier));

		check_integer_value(&GET_SYMBOLS, &yylloc, &(yyvsp[(5) - (6)].sn_value));


		strncpy((yyval.sn_condition).op0, (yyvsp[(3) - (6)].sn_tok_identifier), TLIBC_MAX_LENGTH_OF_IDENTIFIER);
		(yyval.sn_condition).op0[TLIBC_MAX_LENGTH_OF_IDENTIFIER - 1] = 0;

		(yyval.sn_condition).oper = E_EO_EQUAL;
		(yyval.sn_condition).op1 = (yyvsp[(5) - (6)].sn_value);
	}
    break;

  case 38:
/* Line 1787 of yacc.c  */
#line 458 "D:/GitHub/TData/source/tdata_y.yy"
    {
		check_identifier_defined(&GET_SYMBOLS, &yylloc, GET_SYMBOLS.struct_name, (yyvsp[(3) - (6)].sn_tok_identifier));

		check_identifier_refer_to_a_field_with_integer_type(&GET_SYMBOLS, &yylloc, GET_SYMBOLS.struct_name, (yyvsp[(3) - (6)].sn_tok_identifier));

		check_integer_value(&GET_SYMBOLS, &yylloc, &(yyvsp[(5) - (6)].sn_value));


		strncpy((yyval.sn_condition).op0, (yyvsp[(3) - (6)].sn_tok_identifier), TLIBC_MAX_LENGTH_OF_IDENTIFIER);
		(yyval.sn_condition).op0[TLIBC_MAX_LENGTH_OF_IDENTIFIER - 1] = 0;

		(yyval.sn_condition).oper = E_EO_UNEQUAL;
		(yyval.sn_condition).op1 = (yyvsp[(5) - (6)].sn_value);
	}
    break;

  case 39:
/* Line 1787 of yacc.c  */
#line 473 "D:/GitHub/TData/source/tdata_y.yy"
    {
		(yyval.sn_condition).oper = E_EO_NON;
	}
    break;

  case 40:
/* Line 1787 of yacc.c  */
#line 482 "D:/GitHub/TData/source/tdata_y.yy"
    {
		reduce_Type_SimpleType(&(yyval.sn_type), &(yyvsp[(1) - (1)].sn_simple_type));
	}
    break;

  case 41:
/* Line 1787 of yacc.c  */
#line 486 "D:/GitHub/TData/source/tdata_y.yy"
    {
		(yyval.sn_type) = (yyvsp[(1) - (1)].sn_type);
	}
    break;

  case 42:
/* Line 1787 of yacc.c  */
#line 492 "D:/GitHub/TData/source/tdata_y.yy"
    {
		check_identifier_defined(&GET_SYMBOLS, &yylloc, "", (yyvsp[(5) - (6)].sn_tok_identifier));

		check_identifier_is_value(&GET_SYMBOLS, &yylloc, "", (yyvsp[(5) - (6)].sn_tok_identifier));

		check_identifier_is_positive_integer(&GET_SYMBOLS, &yylloc, "", (yyvsp[(5) - (6)].sn_tok_identifier));

		check_string_length_defined(&yylloc, &(yyvsp[(3) - (6)].sn_simple_type));

		reduce_ContainerType_tok_t_vector(&(yyval.sn_type), &(yyvsp[(3) - (6)].sn_simple_type), (yyvsp[(5) - (6)].sn_tok_identifier));
	}
    break;

  case 43:
/* Line 1787 of yacc.c  */
#line 506 "D:/GitHub/TData/source/tdata_y.yy"
    {
		(yyval.sn_simple_type).st = (yyvsp[(1) - (1)].sn_st);
	}
    break;

  case 44:
/* Line 1787 of yacc.c  */
#line 510 "D:/GitHub/TData/source/tdata_y.yy"
    {
		(yyval.sn_simple_type).st = (yyvsp[(1) - (1)].sn_st);
	}
    break;

  case 45:
/* Line 1787 of yacc.c  */
#line 514 "D:/GitHub/TData/source/tdata_y.yy"
    {
		(yyval.sn_simple_type).st = (yyvsp[(1) - (1)].sn_st);
	}
    break;

  case 46:
/* Line 1787 of yacc.c  */
#line 518 "D:/GitHub/TData/source/tdata_y.yy"
    {
		(yyval.sn_simple_type).st = (yyvsp[(1) - (1)].sn_st);
	}
    break;

  case 47:
/* Line 1787 of yacc.c  */
#line 522 "D:/GitHub/TData/source/tdata_y.yy"
    {
		(yyval.sn_simple_type).st = (yyvsp[(1) - (1)].sn_st);
	}
    break;

  case 48:
/* Line 1787 of yacc.c  */
#line 526 "D:/GitHub/TData/source/tdata_y.yy"
    {
		(yyval.sn_simple_type).st = (yyvsp[(1) - (1)].sn_st);
	}
    break;

  case 49:
/* Line 1787 of yacc.c  */
#line 530 "D:/GitHub/TData/source/tdata_y.yy"
    {
		(yyval.sn_simple_type).st = (yyvsp[(1) - (1)].sn_st);
	}
    break;

  case 50:
/* Line 1787 of yacc.c  */
#line 534 "D:/GitHub/TData/source/tdata_y.yy"
    {
		(yyval.sn_simple_type).st = (yyvsp[(1) - (1)].sn_st);
	}
    break;

  case 51:
/* Line 1787 of yacc.c  */
#line 538 "D:/GitHub/TData/source/tdata_y.yy"
    {
		(yyval.sn_simple_type).st = (yyvsp[(1) - (1)].sn_st);
	}
    break;

  case 52:
/* Line 1787 of yacc.c  */
#line 542 "D:/GitHub/TData/source/tdata_y.yy"
    {
		(yyval.sn_simple_type).st = (yyvsp[(1) - (1)].sn_st);
	}
    break;

  case 53:
/* Line 1787 of yacc.c  */
#line 546 "D:/GitHub/TData/source/tdata_y.yy"
    {
		check_identifier_defined(&GET_SYMBOLS, &yylloc, "", (yyvsp[(1) - (1)].sn_tok_identifier));

		check_identifier_is_type(&GET_SYMBOLS, &yylloc, "", (yyvsp[(1) - (1)].sn_tok_identifier));

		reduce_SimpleType_tok_identifier(&(yyval.sn_simple_type), (yyvsp[(1) - (1)].sn_tok_identifier));
	}
    break;

  case 54:
/* Line 1787 of yacc.c  */
#line 554 "D:/GitHub/TData/source/tdata_y.yy"
    {
		check_identifier_defined(&GET_SYMBOLS, &yylloc, "", (yyvsp[(3) - (4)].sn_tok_identifier));

		check_identifier_is_value(&GET_SYMBOLS, &yylloc, "", (yyvsp[(3) - (4)].sn_tok_identifier));

		check_identifier_is_positive_integer(&GET_SYMBOLS, &yylloc, "", (yyvsp[(3) - (4)].sn_tok_identifier));

		reduce_SimpleType_tok_t_string(&(yyval.sn_simple_type), (yyvsp[(3) - (4)].sn_tok_identifier));
	}
    break;

  case 55:
/* Line 1787 of yacc.c  */
#line 564 "D:/GitHub/TData/source/tdata_y.yy"
    {
		reduce_SimpleType_tok_t_string(&(yyval.sn_simple_type), NULL);
	}
    break;

  case 56:
/* Line 1787 of yacc.c  */
#line 571 "D:/GitHub/TData/source/tdata_y.yy"
    {
		(yyval.sn_arguments) = (yyvsp[(2) - (3)].sn_arguments);
	}
    break;

  case 57:
/* Line 1787 of yacc.c  */
#line 575 "D:/GitHub/TData/source/tdata_y.yy"
    {
		(yyval.sn_arguments).arg_list_num = 0;
	}
    break;

  case 58:
/* Line 1787 of yacc.c  */
#line 581 "D:/GitHub/TData/source/tdata_y.yy"
    {
		if((yyvsp[(1) - (3)].sn_arguments).arg_list_num >= MAX_ARGUMENT_NUM)
		{
			scanner_error_halt(&yylloc, E_LS_TOO_MANY_ARGUMENTS, MAX_ARGUMENT_NUM);
		}

		reduce_ArgumentList_ArgumentList_tok_identifier(&(yyval.sn_arguments), &(yyvsp[(1) - (3)].sn_arguments), (yyvsp[(3) - (3)].sn_tok_identifier));
	}
    break;

  case 59:
/* Line 1787 of yacc.c  */
#line 590 "D:/GitHub/TData/source/tdata_y.yy"
    {
		check_identifier_defined(&GET_SYMBOLS, &yylloc, GET_SYMBOLS.struct_name, (yyvsp[(1) - (1)].sn_tok_identifier));

		reduce_ArgumentList_tok_identifier(&(yyval.sn_arguments), (yyvsp[(1) - (1)].sn_tok_identifier));
	}
    break;

  case 60:
/* Line 1787 of yacc.c  */
#line 598 "D:/GitHub/TData/source/tdata_y.yy"
    {
		reduce_Value_tok_uint64(&(yyval.sn_value), (yyvsp[(1) - (1)].sn_uint64));
	}
    break;

  case 61:
/* Line 1787 of yacc.c  */
#line 602 "D:/GitHub/TData/source/tdata_y.yy"
    {
		reduce_Value_tok_hex_uint64(&(yyval.sn_value), (yyvsp[(1) - (1)].sn_hex_uint64));
	}
    break;

  case 62:
/* Line 1787 of yacc.c  */
#line 606 "D:/GitHub/TData/source/tdata_y.yy"
    {
		reduce_Value_tok_int64(&(yyval.sn_value), (yyvsp[(1) - (1)].sn_int64));
	}
    break;

  case 63:
/* Line 1787 of yacc.c  */
#line 610 "D:/GitHub/TData/source/tdata_y.yy"
    {
		reduce_Value_tok_hex_int64(&(yyval.sn_value), (yyvsp[(1) - (1)].sn_hex_int64));
	}
    break;

  case 64:
/* Line 1787 of yacc.c  */
#line 614 "D:/GitHub/TData/source/tdata_y.yy"
    {
		reduce_Value_tok_double(&(yyval.sn_value), (yyvsp[(1) - (1)].pn_tok_double));
	}
    break;

  case 65:
/* Line 1787 of yacc.c  */
#line 618 "D:/GitHub/TData/source/tdata_y.yy"
    {
		reduce_Value_tok_string(&(yyval.sn_value), (yyvsp[(1) - (1)].sn_string));
	}
    break;

  case 66:
/* Line 1787 of yacc.c  */
#line 622 "D:/GitHub/TData/source/tdata_y.yy"
    {
		reduce_Value_tok_char(&(yyval.sn_value), (yyvsp[(1) - (1)].sn_char));
	}
    break;

  case 67:
/* Line 1787 of yacc.c  */
#line 626 "D:/GitHub/TData/source/tdata_y.yy"
    {
		check_identifier_defined(&GET_SYMBOLS, &yylloc, "", (yyvsp[(1) - (1)].sn_tok_identifier));

		check_identifier_is_value(&GET_SYMBOLS, &yylloc, "", (yyvsp[(1) - (1)].sn_tok_identifier));

		reduce_Value_tok_identifier(&(yyval.sn_value), (yyvsp[(1) - (1)].sn_tok_identifier));
	}
    break;

  case 68:
/* Line 1787 of yacc.c  */
#line 634 "D:/GitHub/TData/source/tdata_y.yy"
    {
		check_identifier_defined(&GET_SYMBOLS, &yylloc, "", (yyvsp[(3) - (4)].sn_tok_identifier));

		check_identifier_is_type(&GET_SYMBOLS, &yylloc, "", (yyvsp[(3) - (4)].sn_tok_identifier));

		reduce_Value_tok_count(&GET_SYMBOLS, &(yyval.sn_value), (yyvsp[(3) - (4)].sn_tok_identifier));
	}
    break;

  case 69:
/* Line 1787 of yacc.c  */
#line 644 "D:/GitHub/TData/source/tdata_y.yy"
    {
		strncpy((yyval.sn_unix_comment).text, (yyvsp[(1) - (1)].sn_tok_unixcomment), MAX_COMMENT_LENGTH);
		(yyval.sn_unix_comment).text[MAX_COMMENT_LENGTH - 1] = 0;
	}
    break;

  case 70:
/* Line 1787 of yacc.c  */
#line 651 "D:/GitHub/TData/source/tdata_y.yy"
    {
		strncpy((yyval.sn_unix_comment).text, (yyvsp[(1) - (1)].sn_tok_unixcomment), MAX_COMMENT_LENGTH);
		(yyval.sn_unix_comment).text[MAX_COMMENT_LENGTH - 1] = 0;
	}
    break;

  case 71:
/* Line 1787 of yacc.c  */
#line 656 "D:/GitHub/TData/source/tdata_y.yy"
    {
		(yyval.sn_unix_comment).text[0] = 0;
	}
    break;


/* Line 1787 of yacc.c  */
#line 2414 "D:/GitHub/TData/source/gen/source/tdata_y.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

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
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, self, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (&yylloc, self, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

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
		      yytoken, &yylval, &yylloc, self);
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

  yyerror_range[1] = yylsp[1-yylen];
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
      if (!yypact_value_is_default (yyn))
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp, self);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

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

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, self, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, self);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp, self);
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


/* Line 2050 of yacc.c  */
#line 660 "D:/GitHub/TData/source/tdata_y.yy"

