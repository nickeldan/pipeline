/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parser.y" /* yacc.c:339  */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "parserUtil.h"
#include "marker.h"
#include "nameTable.h"
#include "plObject.h"

void yyerror(const char *message, ...);
int yylex(void);

static astNodePtr resolveAttributes(astNodePtr object, astNodePtr attributes);

astNodePtr programTree;

#line 84 "parser.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.tab.h".  */
#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    SOURCE = 258,
    PIPE = 259,
    SINK = 260,
    LOCAL = 261,
    STRUCT = 262,
    WHILE = 263,
    IF = 264,
    EIF = 265,
    ELSE = 266,
    PROD = 267,
    DROP = 268,
    END = 269,
    NOT = 270,
    CONT = 271,
    BREAK = 272,
    VERIFY = 273,
    ABORT = 274,
    IS = 275,
    AS = 276,
    IMPORT = 277,
    EXPORT = 278,
    MAIN = 279,
    ARROW = 280,
    LSHIFT = 281,
    RSHIFT = 282,
    AND = 283,
    OR = 284,
    NAME = 285,
    LITERAL = 286,
    TYPE = 287,
    COMPARISON = 288,
    OPERATOR_ASSIGNMENT = 289,
    CONTEXT = 290
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 22 "parser.y" /* yacc.c:355  */

	astNodePtr node;
	plObject *object;
	int marker;

#line 166 "parser.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 183 "parser.tab.c" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
# define YYCOPY_NEEDED 1
#endif


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

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

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
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  42
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   636

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  55
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  137
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  279

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   290

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    41,    44,     2,
      50,    51,    39,    37,    36,    38,    46,    40,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    53,    49,
       2,    52,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    45,     2,    54,    42,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    47,    43,    48,     2,     2,     2,     2,
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
      35
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    63,    63,    66,    67,    70,    71,    72,    73,    74,
      75,    76,    77,    80,    83,    86,    89,    92,    93,    96,
      97,   100,   103,   104,   107,   110,   113,   116,   119,   122,
     123,   126,   127,   130,   131,   134,   135,   138,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   188,   189,   192,   193,   196,   197,   200,   201,   204,
     207,   208,   209,   212,   213,   216,   217,   220,   221,   224,
     225,   226,   227,   230,   233,   234,   237,   238,   241,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   259,   262,   263,   266,   267,   270,   271,   274,
     275,   278,   279,   280,   281,   282,   283,   284
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "SOURCE", "PIPE", "SINK", "LOCAL",
  "STRUCT", "WHILE", "IF", "EIF", "ELSE", "PROD", "DROP", "END", "NOT",
  "CONT", "BREAK", "VERIFY", "ABORT", "IS", "AS", "IMPORT", "EXPORT",
  "MAIN", "ARROW", "LSHIFT", "RSHIFT", "AND", "OR", "NAME", "LITERAL",
  "TYPE", "COMPARISON", "OPERATOR_ASSIGNMENT", "CONTEXT", "','", "'+'",
  "'-'", "'*'", "'/'", "'%'", "'^'", "'|'", "'&'", "'['", "'.'", "'{'",
  "'}'", "';'", "'('", "')'", "'='", "':'", "']'", "$accept", "file",
  "file_section", "global_content", "main_definition", "import_statement",
  "export_statement", "struct_definition", "struct_field", "type",
  "moduled_name", "attribute_trail", "source_definition",
  "pipe_definition", "sink_definition", "local_definition",
  "global_var_definition", "optional_name", "possible_arg_list",
  "arg_list", "arg_element", "single_argument", "compilation_expression",
  "expression", "compilation_array_literal", "compilation_array_element",
  "array_literal", "array_element", "function_call", "call_list",
  "call_arg", "statement_list", "actual_statement_list",
  "general_statement", "if_section", "eif_section", "else_section",
  "while_section", "statement", "arrow_statement", "arrow_sender",
  "arrow_sender_item", "arrow_sender_list", "arrow_receiver",
  "arrow_receiver_item", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,    44,    43,    45,    42,
      47,    37,    94,   124,    38,    91,    46,   123,   125,    59,
      40,    41,    61,    58,    93
};
# endif

#define YYPACT_NINF -73

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-73)))

#define YYTABLE_NINF -128

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-128)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     274,     9,     9,     9,    15,   128,    43,    52,    40,   -73,
     -73,   128,   128,    89,   274,   -73,   -73,   -73,   -73,   -73,
      51,   -73,   -73,   -73,   -73,   451,   -73,   -73,    54,    72,
      74,    79,   560,    80,    83,   257,    82,    82,   511,    84,
      97,   364,   -73,   -73,    53,   104,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   128,    82,
     106,   106,   106,   106,   -73,   -73,    55,    55,    55,   -73,
     -73,    55,   -73,   -73,    55,    55,   116,   -73,   -73,    55,
     -73,    29,    -7,   114,   -73,   -73,   471,   -73,    92,   257,
     -73,   -73,   -73,    99,   -73,   125,   -73,   111,   -73,   128,
     -73,   103,    29,   491,    26,   -73,   102,    -8,   112,   560,
     560,   580,   580,   193,   193,   308,    -9,    -9,    -9,   320,
     107,   110,   126,   -73,   113,   119,   121,    27,   -73,   384,
     406,   491,   531,   491,   491,    48,    82,    82,   491,   109,
     139,   -73,   344,   129,   141,   148,    48,    55,    55,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    82,   -73,   -73,   -73,    61,   -73,    82,   511,
      82,    55,    53,   -73,   -73,   -73,    48,   162,   106,   128,
     176,   117,   106,   -73,   257,   257,   -73,   -73,   -73,   -73,
      55,   182,   -73,   280,   -73,    -3,   158,   531,   531,   551,
     491,   587,   587,   500,   500,   591,    -4,    -4,    -4,   300,
     154,   -73,   157,   -73,   -73,   -73,   186,   -73,   491,   -73,
      82,   -73,    48,   -73,   511,    48,   257,   -73,   167,   170,
      82,   491,    48,    82,   491,   173,   -73,   106,    82,   191,
      82,    23,    82,    61,   179,   181,   192,   -73,   -73,   -73,
     -73,    82,   178,   -73,   209,   -73,   257,   257,   -73,    18,
     195,   -73,   198,   215,    55,   217,   -73,   257,   -73,   -73,
     428,   257,   219,   257,   220,   -73,   232,   -73,   -73
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    29,    29,    29,     0,     0,     0,     0,     0,    22,
      22,    81,     0,     0,     2,     3,     5,     6,     7,     8,
      38,     9,    10,    11,    12,     0,    22,    30,     0,     0,
       0,     0,    46,     0,     0,    95,    21,    39,    83,     0,
      82,     0,     1,     4,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
      31,     0,     0,     0,    14,    15,     0,     0,     0,   110,
     111,     0,   112,   113,     0,     0,    22,    22,    22,    85,
      99,     0,    57,   119,   120,   121,   125,    22,     0,    96,
      97,   101,   102,     0,   118,     0,   123,     0,    22,     0,
      22,    22,     0,    93,     0,    91,     0,    55,    56,    44,
      45,    47,    48,    49,    50,    51,    52,    53,    54,     0,
       0,     0,    32,    33,    35,     0,     0,     0,    17,     0,
       0,   109,    66,   115,   114,     0,    58,    59,    87,     0,
      86,   126,   125,     0,   123,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    13,    98,   100,     0,    23,    41,    84,
      42,     0,     0,    89,    28,    22,     0,     0,     0,     0,
       0,     0,     0,    16,    95,    95,    19,   116,    20,    22,
       0,    22,    80,     0,    78,    76,    77,    64,    65,    67,
     117,    68,    69,    70,    71,    72,    73,    74,    75,     0,
       0,    22,   131,    22,    22,    22,   122,   129,    94,    92,
      40,    37,     0,    34,    36,     0,    95,    18,     0,     0,
      61,    88,     0,    62,   125,   128,    22,     0,   133,     0,
     134,   135,   137,     0,     0,     0,     0,   108,   104,    79,
     124,    60,     0,   132,     0,   130,    95,    95,    26,   106,
       0,   136,     0,     0,     0,     0,   103,    95,    24,    25,
       0,    95,     0,    95,     0,    27,     0,   107,   105
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -73,   -73,   -73,   268,   -73,   -73,   -73,   -73,   -73,   -51,
       0,    -6,    21,     1,     2,   -73,   -73,    94,   -73,   -73,
     108,   -55,   201,   -41,   -73,   -73,   -73,   -73,   142,   -73,
     118,   -40,   -73,   196,   -73,   -73,   -73,   -73,   -73,   -71,
     -73,   -72,   -73,   -73,    41
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    13,    14,    15,    16,    17,    18,    19,   127,   187,
      82,    36,    83,    84,    85,   215,    24,    28,   121,   122,
     123,   124,    25,    86,    39,    40,   139,   140,    87,   104,
     105,    88,    89,    90,    91,   259,   266,    92,    93,    94,
      95,    96,   145,   216,   217
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      20,    22,    23,   103,    37,    20,   125,   126,   128,   144,
     143,    20,    20,   146,    20,    22,    23,    46,    47,    47,
      59,    21,   147,   148,   148,   129,   130,   131,   264,   265,
     132,   143,     1,   133,   134,    21,    58,    58,   138,    27,
     142,   161,   161,    44,    71,    31,    20,    20,    20,    20,
      20,    20,    20,    20,    20,    20,    20,    20,    20,     9,
      77,   142,   172,   182,    78,     2,     3,   210,    71,    97,
      71,   136,   137,    33,    79,   183,   254,   173,     9,    81,
     186,   162,    34,   101,    77,     9,    77,    35,    78,    42,
      78,     9,   168,   211,   170,   194,    29,    30,    79,    20,
      79,    44,   141,   102,    60,   102,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   235,    61,   141,    62,   221,    63,   227,    97,    64,
     218,   103,    65,    99,   106,   188,   120,   135,    98,  -126,
     163,   167,    26,     5,   228,   229,   188,    26,   165,   231,
     166,   174,   234,    26,    26,   171,    26,    58,     9,    10,
     176,   177,   178,   189,   226,   179,   212,   213,   214,   220,
     180,   244,   181,    11,   245,   190,   188,  -127,    12,    20,
     192,   249,   252,   230,   193,   233,   246,   222,    26,    26,
      26,    26,    26,    26,    26,    26,    26,    26,    26,    26,
      26,   225,   232,   161,   237,   238,    32,   240,   241,   242,
     239,   243,    38,    41,   141,   247,   262,   263,   248,    46,
      47,   253,   188,   270,   250,   188,   256,   272,   257,   260,
     251,   274,   188,   276,    54,    55,    56,    57,    58,   261,
     258,    26,   267,   212,   213,   214,   268,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
       1,     2,     3,   269,   271,    66,    67,   275,   277,    68,
      69,    70,    71,    72,    73,    74,    75,     1,     2,     3,
     278,     4,    43,     1,   255,   164,   223,    76,    77,     5,
     219,     0,    78,     0,     0,    71,     6,     7,     8,     0,
     169,     0,    79,     0,     9,    10,    80,    81,     0,     0,
       9,    77,     0,     0,     0,    78,     0,     0,     0,    11,
       0,    26,     0,     0,    12,    79,   147,   148,   149,   150,
     102,     0,     0,   151,    46,    47,     0,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    46,    47,    48,    49,
      55,    56,    57,    58,   236,     0,     0,    50,    51,    52,
      53,    54,    55,    56,    57,    58,     0,     0,     0,     0,
     147,   148,   149,   150,   175,     0,     0,   151,     0,     0,
     224,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      46,    47,    48,    49,     0,   191,     0,     0,     0,     0,
       0,    50,    51,    52,    53,    54,    55,    56,    57,    58,
     147,   148,   149,   150,     0,   100,     0,   151,     0,     0,
       0,   153,   154,   155,   156,   157,   158,   159,   160,   161,
       0,   184,   147,   148,   149,   150,     0,     0,     0,   151,
       0,     0,     0,   153,   154,   155,   156,   157,   158,   159,
     160,   161,     0,   185,   147,   148,   149,   150,     0,     0,
       0,   151,     0,     0,     0,   153,   154,   155,   156,   157,
     158,   159,   160,   161,     0,   273,    45,    46,    47,    48,
      49,     0,     0,     0,     0,     0,     0,     0,    50,    51,
      52,    53,    54,    55,    56,    57,    58,   147,   148,   149,
     150,     0,     0,     0,   151,   152,     0,     0,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   147,   148,   149,
     150,     0,     0,     0,   151,     0,   147,   148,   153,   154,
     155,   156,   157,   158,   159,   160,   161,    46,    47,    48,
      49,   157,   158,   159,   160,   161,     0,     0,    50,    51,
      52,    53,    54,    55,    56,    57,    58,   147,   148,     0,
       0,     0,     0,     0,   151,     0,     0,     0,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   147,   148,     0,
       0,     0,     0,     0,  -128,     0,    46,    47,   153,   154,
     155,   156,   157,   158,   159,   160,   161,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    46,    47,     0,     0,
       0,     0,     0,   147,   148,     0,     0,   147,   148,    52,
      53,    54,    55,    56,    57,    58,   155,   156,   157,   158,
     159,   160,   161,   158,   159,   160,   161
};

static const yytype_int16 yycheck[] =
{
       0,     0,     0,    44,    10,     5,    61,    62,    63,    81,
      81,    11,    12,    20,    14,    14,    14,    26,    27,    27,
      26,     0,    26,    27,    27,    66,    67,    68,    10,    11,
      71,   102,     3,    74,    75,    14,    45,    45,    79,    30,
      81,    45,    45,    50,    15,    30,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    30,
      31,   102,    36,    36,    35,     4,     5,     6,    15,    46,
      15,    77,    78,    30,    45,    48,    53,    51,    30,    50,
      32,    87,    30,    30,    31,    30,    31,    47,    35,     0,
      35,    30,    98,    32,   100,   146,     2,     3,    45,    99,
      45,    50,    81,    50,    50,    50,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   193,    50,   102,    50,   176,    47,   182,    46,    49,
     171,   172,    49,    36,    30,   135,    30,    21,    54,    25,
      48,    30,     0,    15,   184,   185,   146,     5,    49,   190,
      25,    49,   193,    11,    12,    52,    14,    45,    30,    31,
      53,    51,    36,    54,    47,    52,   166,   166,   166,   175,
      51,   222,    51,    45,   225,    36,   176,    36,    50,   179,
      51,   232,   237,   189,    36,   191,   226,    25,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    25,    20,    45,    50,   211,     5,   213,   214,   215,
      53,    25,    11,    12,   193,    48,   256,   257,    48,    26,
      27,    30,   222,   264,    51,   225,    47,   267,    47,    51,
     236,   271,   232,   273,    41,    42,    43,    44,    45,    30,
      48,    99,    47,   243,   243,   243,    48,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
       3,     4,     5,    48,    47,     8,     9,    48,    48,    12,
      13,    14,    15,    16,    17,    18,    19,     3,     4,     5,
      48,     7,    14,     3,   243,    89,   178,    30,    31,    15,
     172,    -1,    35,    -1,    -1,    15,    22,    23,    24,    -1,
      99,    -1,    45,    -1,    30,    31,    49,    50,    -1,    -1,
      30,    31,    -1,    -1,    -1,    35,    -1,    -1,    -1,    45,
      -1,   179,    -1,    -1,    50,    45,    26,    27,    28,    29,
      50,    -1,    -1,    33,    26,    27,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    26,    27,    28,    29,
      42,    43,    44,    45,    54,    -1,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    -1,    -1,    -1,    -1,
      26,    27,    28,    29,    54,    -1,    -1,    33,    -1,    -1,
     179,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      26,    27,    28,    29,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      26,    27,    28,    29,    -1,    51,    -1,    33,    -1,    -1,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    47,    26,    27,    28,    29,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    47,    26,    27,    28,    29,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    25,    26,    27,    28,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    26,    27,    28,
      29,    -1,    -1,    -1,    33,    34,    -1,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    26,    27,    28,
      29,    -1,    -1,    -1,    33,    -1,    26,    27,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    26,    27,    28,
      29,    41,    42,    43,    44,    45,    -1,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    26,    27,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    26,    27,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    26,    27,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    26,    27,    -1,    -1,
      -1,    -1,    -1,    26,    27,    -1,    -1,    26,    27,    39,
      40,    41,    42,    43,    44,    45,    39,    40,    41,    42,
      43,    44,    45,    42,    43,    44,    45
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     7,    15,    22,    23,    24,    30,
      31,    45,    50,    56,    57,    58,    59,    60,    61,    62,
      65,    67,    68,    69,    71,    77,    83,    30,    72,    72,
      72,    30,    77,    30,    30,    47,    66,    66,    77,    79,
      80,    77,     0,    58,    50,    25,    26,    27,    28,    29,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    66,
      50,    50,    50,    47,    49,    49,     8,     9,    12,    13,
      14,    15,    16,    17,    18,    19,    30,    31,    35,    45,
      49,    50,    65,    67,    68,    69,    78,    83,    86,    87,
      88,    89,    92,    93,    94,    95,    96,    46,    54,    36,
      51,    30,    50,    78,    84,    85,    30,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      30,    73,    74,    75,    76,    76,    76,    63,    76,    78,
      78,    78,    78,    78,    78,    21,    66,    66,    78,    81,
      82,    67,    78,    94,    96,    97,    20,    26,    27,    28,
      29,    33,    34,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    66,    48,    88,    49,    25,    30,    66,    77,
      66,    52,    36,    51,    49,    54,    53,    51,    36,    52,
      51,    51,    36,    48,    47,    47,    32,    64,    65,    54,
      36,    51,    51,    36,    64,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
       6,    32,    65,    68,    69,    70,    98,    99,    78,    85,
      66,    64,    25,    75,    77,    25,    47,    76,    86,    86,
      66,    78,    20,    66,    78,    96,    54,    50,    66,    53,
      66,    66,    66,    25,    64,    64,    86,    48,    48,    64,
      51,    66,    76,    30,    53,    99,    47,    47,    48,    90,
      51,    30,    86,    86,    10,    11,    91,    47,    48,    48,
      78,    47,    86,    47,    86,    48,    86,    48,    48
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    55,    56,    57,    57,    58,    58,    58,    58,    58,
      58,    58,    58,    59,    60,    61,    62,    63,    63,    64,
      64,    65,    66,    66,    67,    68,    69,    70,    71,    72,
      72,    73,    73,    74,    74,    75,    75,    76,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    79,    79,    80,    80,    81,    81,    82,    82,    83,
      84,    84,    84,    85,    85,    86,    86,    87,    87,    88,
      88,    88,    88,    89,    90,    90,    91,    91,    92,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    94,    95,    95,    96,    96,    97,    97,    98,
      98,    99,    99,    99,    99,    99,    99,    99
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     3,     3,     5,     1,     3,     1,
       1,     2,     0,     3,    10,    10,     8,     7,     4,     0,
       1,     0,     1,     1,     3,     1,     3,     3,     1,     2,
       5,     4,     4,     2,     3,     3,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     2,     2,
       5,     4,     4,     2,     3,     3,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     5,
       3,     0,     1,     1,     3,     0,     1,     1,     3,     4,
       0,     1,     3,     1,     3,     0,     1,     1,     2,     1,
       2,     1,     1,     7,     0,     6,     0,     4,     5,     2,
       1,     1,     1,     1,     2,     2,     3,     3,     1,     1,
       1,     1,     3,     1,     5,     1,     1,     1,     3,     1,
       3,     1,     3,     2,     2,     2,     4,     2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      YY_LAC_DISCARD ("YYBACKUP");                              \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

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
#ifndef YYINITDEPTH
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

/* Given a state stack such that *YYBOTTOM is its bottom, such that
   *YYTOP is either its top or is YYTOP_EMPTY to indicate an empty
   stack, and such that *YYCAPACITY is the maximum number of elements it
   can hold without a reallocation, make sure there is enough room to
   store YYADD more elements.  If not, allocate a new stack using
   YYSTACK_ALLOC, copy the existing elements, and adjust *YYBOTTOM,
   *YYTOP, and *YYCAPACITY to reflect the new capacity and memory
   location.  If *YYBOTTOM != YYBOTTOM_NO_FREE, then free the old stack
   using YYSTACK_FREE.  Return 0 if successful or if no reallocation is
   required.  Return 1 if memory is exhausted.  */
static int
yy_lac_stack_realloc (YYSIZE_T *yycapacity, YYSIZE_T yyadd,
#if YYDEBUG
                      char const *yydebug_prefix,
                      char const *yydebug_suffix,
#endif
                      yytype_int16 **yybottom,
                      yytype_int16 *yybottom_no_free,
                      yytype_int16 **yytop, yytype_int16 *yytop_empty)
{
  YYSIZE_T yysize_old =
    *yytop == yytop_empty ? 0 : *yytop - *yybottom + 1;
  YYSIZE_T yysize_new = yysize_old + yyadd;
  if (*yycapacity < yysize_new)
    {
      YYSIZE_T yyalloc = 2 * yysize_new;
      yytype_int16 *yybottom_new;
      /* Use YYMAXDEPTH for maximum stack size given that the stack
         should never need to grow larger than the main state stack
         needs to grow without LAC.  */
      if (YYMAXDEPTH < yysize_new)
        {
          YYDPRINTF ((stderr, "%smax size exceeded%s", yydebug_prefix,
                      yydebug_suffix));
          return 1;
        }
      if (YYMAXDEPTH < yyalloc)
        yyalloc = YYMAXDEPTH;
      yybottom_new =
        (yytype_int16*) YYSTACK_ALLOC (yyalloc * sizeof *yybottom_new);
      if (!yybottom_new)
        {
          YYDPRINTF ((stderr, "%srealloc failed%s", yydebug_prefix,
                      yydebug_suffix));
          return 1;
        }
      if (*yytop != yytop_empty)
        {
          YYCOPY (yybottom_new, *yybottom, yysize_old);
          *yytop = yybottom_new + (yysize_old - 1);
        }
      if (*yybottom != yybottom_no_free)
        YYSTACK_FREE (*yybottom);
      *yybottom = yybottom_new;
      *yycapacity = yyalloc;
    }
  return 0;
}

/* Establish the initial context for the current lookahead if no initial
   context is currently established.

   We define a context as a snapshot of the parser stacks.  We define
   the initial context for a lookahead as the context in which the
   parser initially examines that lookahead in order to select a
   syntactic action.  Thus, if the lookahead eventually proves
   syntactically unacceptable (possibly in a later context reached via a
   series of reductions), the initial context can be used to determine
   the exact set of tokens that would be syntactically acceptable in the
   lookahead's place.  Moreover, it is the context after which any
   further semantic actions would be erroneous because they would be
   determined by a syntactically unacceptable token.

   YY_LAC_ESTABLISH should be invoked when a reduction is about to be
   performed in an inconsistent state (which, for the purposes of LAC,
   includes consistent states that don't know they're consistent because
   their default reductions have been disabled).  Iff there is a
   lookahead token, it should also be invoked before reporting a syntax
   error.  This latter case is for the sake of the debugging output.

   For parse.lac=full, the implementation of YY_LAC_ESTABLISH is as
   follows.  If no initial context is currently established for the
   current lookahead, then check if that lookahead can eventually be
   shifted if syntactic actions continue from the current context.
   Report a syntax error if it cannot.  */
#define YY_LAC_ESTABLISH                                         \
do {                                                             \
  if (!yy_lac_established)                                       \
    {                                                            \
      YYDPRINTF ((stderr,                                        \
                  "LAC: initial context established for %s\n",   \
                  yytname[yytoken]));                            \
      yy_lac_established = 1;                                    \
      {                                                          \
        int yy_lac_status =                                      \
          yy_lac (yyesa, &yyes, &yyes_capacity, yyssp, yytoken); \
        if (yy_lac_status == 2)                                  \
          goto yyexhaustedlab;                                   \
        if (yy_lac_status == 1)                                  \
          goto yyerrlab;                                         \
      }                                                          \
    }                                                            \
} while (0)

/* Discard any previous initial lookahead context because of Event,
   which may be a lookahead change or an invalidation of the currently
   established initial context for the current lookahead.

   The most common example of a lookahead change is a shift.  An example
   of both cases is syntax error recovery.  That is, a syntax error
   occurs when the lookahead is syntactically erroneous for the
   currently established initial context, so error recovery manipulates
   the parser stacks to try to find a new initial context in which the
   current lookahead is syntactically acceptable.  If it fails to find
   such a context, it discards the lookahead.  */
#if YYDEBUG
# define YY_LAC_DISCARD(Event)                                           \
do {                                                                     \
  if (yy_lac_established)                                                \
    {                                                                    \
      if (yydebug)                                                       \
        YYFPRINTF (stderr, "LAC: initial context discarded due to "      \
                   Event "\n");                                          \
      yy_lac_established = 0;                                            \
    }                                                                    \
} while (0)
#else
# define YY_LAC_DISCARD(Event) yy_lac_established = 0
#endif

/* Given the stack whose top is *YYSSP, return 0 iff YYTOKEN can
   eventually (after perhaps some reductions) be shifted, return 1 if
   not, or return 2 if memory is exhausted.  As preconditions and
   postconditions: *YYES_CAPACITY is the allocated size of the array to
   which *YYES points, and either *YYES = YYESA or *YYES points to an
   array allocated with YYSTACK_ALLOC.  yy_lac may overwrite the
   contents of either array, alter *YYES and *YYES_CAPACITY, and free
   any old *YYES other than YYESA.  */
static int
yy_lac (yytype_int16 *yyesa, yytype_int16 **yyes,
        YYSIZE_T *yyes_capacity, yytype_int16 *yyssp, int yytoken)
{
  yytype_int16 *yyes_prev = yyssp;
  yytype_int16 *yyesp = yyes_prev;
  YYDPRINTF ((stderr, "LAC: checking lookahead %s:", yytname[yytoken]));
  if (yytoken == YYUNDEFTOK)
    {
      YYDPRINTF ((stderr, " Always Err\n"));
      return 1;
    }
  while (1)
    {
      int yyrule = yypact[*yyesp];
      if (yypact_value_is_default (yyrule)
          || (yyrule += yytoken) < 0 || YYLAST < yyrule
          || yycheck[yyrule] != yytoken)
        {
          yyrule = yydefact[*yyesp];
          if (yyrule == 0)
            {
              YYDPRINTF ((stderr, " Err\n"));
              return 1;
            }
        }
      else
        {
          yyrule = yytable[yyrule];
          if (yytable_value_is_error (yyrule))
            {
              YYDPRINTF ((stderr, " Err\n"));
              return 1;
            }
          if (0 < yyrule)
            {
              YYDPRINTF ((stderr, " S%d\n", yyrule));
              return 0;
            }
          yyrule = -yyrule;
        }
      {
        YYSIZE_T yylen = yyr2[yyrule];
        YYDPRINTF ((stderr, " R%d", yyrule - 1));
        if (yyesp != yyes_prev)
          {
            YYSIZE_T yysize = yyesp - *yyes + 1;
            if (yylen < yysize)
              {
                yyesp -= yylen;
                yylen = 0;
              }
            else
              {
                yylen -= yysize;
                yyesp = yyes_prev;
              }
          }
        if (yylen)
          yyesp = yyes_prev -= yylen;
      }
      {
        int yystate;
        {
          int yylhs = yyr1[yyrule] - YYNTOKENS;
          yystate = yypgoto[yylhs] + *yyesp;
          if (yystate < 0 || YYLAST < yystate
              || yycheck[yystate] != *yyesp)
            yystate = yydefgoto[yylhs];
          else
            yystate = yytable[yystate];
        }
        if (yyesp == yyes_prev)
          {
            yyesp = *yyes;
            *yyesp = yystate;
          }
        else
          {
            if (yy_lac_stack_realloc (yyes_capacity, 1,
#if YYDEBUG
                                      " (", ")",
#endif
                                      yyes, yyesa, &yyesp, yyes_prev))
              {
                YYDPRINTF ((stderr, "\n"));
                return 2;
              }
            *++yyesp = yystate;
          }
        YYDPRINTF ((stderr, " G%d", yystate));
      }
    }
}


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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
   YYSSP.  In order to see if a particular token T is a
   valid looakhead, invoke yy_lac (YYESA, YYES, YYES_CAPACITY, YYSSP, T).

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store or if
   yy_lac returned 2.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyesa, yytype_int16 **yyes,
                YYSIZE_T *yyes_capacity, yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
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
       In the first two cases, it might appear that the current syntax
       error should have been detected in the previous state when yy_lac
       was invoked.  However, at that time, there might have been a
       different syntax error that discarded a different initial context
       during error recovery, leaving behind the current lookahead.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      YYDPRINTF ((stderr, "Constructing syntax error message\n"));
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          int yyx;

          for (yyx = 0; yyx < YYNTOKENS; ++yyx)
            if (yyx != YYTERROR && yyx != YYUNDEFTOK)
              {
                {
                  int yy_lac_status = yy_lac (yyesa, yyes, yyes_capacity,
                                              yyssp, yyx);
                  if (yy_lac_status == 2)
                    return 2;
                  if (yy_lac_status == 1)
                    continue;
                }
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
# if YYDEBUG
      else if (yydebug)
        YYFPRINTF (stderr, "No expected tokens.\n");
# endif
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

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

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

    YYSIZE_T yystacksize;

    yytype_int16 yyesa[20];
    yytype_int16 *yyes;
    YYSIZE_T yyes_capacity;

  int yy_lac_established = 0;
  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
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

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  yyes = yyesa;
  yyes_capacity = sizeof yyesa / sizeof *yyes;
  if (YYMAXDEPTH < yyes_capacity)
    yyes_capacity = YYMAXDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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
    {
      YY_LAC_ESTABLISH;
      goto yydefault;
    }
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      YY_LAC_ESTABLISH;
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
  YY_LAC_DISCARD ("shift");

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  {
    int yychar_backup = yychar;
    switch (yyn)
      {
          case 2:
#line 63 "parser.y" /* yacc.c:1646  */
    {programTree=(yyvsp[0].node);}
#line 1762 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 66 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1768 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 67 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = NODE(';',(yyvsp[-1].node),(yyvsp[0].node));}
#line 1774 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 70 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1780 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 71 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1786 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 72 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1792 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 73 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1798 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 74 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1804 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 75 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1810 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 76 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1816 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 77 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1822 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 80 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(MAIN,(yyvsp[-1].node));}
#line 1828 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 83 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(IMPORT,(yyvsp[-1].node));}
#line 1834 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 86 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(EXPORT,(yyvsp[-1].node));}
#line 1840 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 89 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(STRUCT,(yyvsp[-3].node),(yyvsp[-1].node));}
#line 1846 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 92 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1852 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 93 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(',',(yyvsp[-2].node),(yyvsp[0].node));}
#line 1858 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 96 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(TYPE); (yyval.node)->marker=(yyvsp[0].marker);}
#line 1864 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 97 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1870 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 100 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=resolveAttributes((yyvsp[-1].node),(yyvsp[0].node));}
#line 1876 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 103 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NULL;}
#line 1882 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 104 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[-2].node)? NODE('.',(yyvsp[-2].node),(yyvsp[0].node)) : (yyvsp[0].node);}
#line 1888 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 107 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(SOURCE,(yyvsp[-8].node),(yyvsp[-6].node),(yyvsp[-3].node),(yyvsp[-1].node));}
#line 1894 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 110 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(PIPE,(yyvsp[-8].node),(yyvsp[-6].node),(yyvsp[-3].node),(yyvsp[-1].node));}
#line 1900 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 113 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(SINK,(yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-1].node));}
#line 1906 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 116 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(LOCAL,(yyvsp[-4].node),(yyvsp[-1].node));}
#line 1912 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 119 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(ARROW,(yyvsp[-3].node),(yyvsp[-1].node));}
#line 1918 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 122 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NULL;}
#line 1924 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 123 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1930 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 126 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NULL;}
#line 1936 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 127 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1942 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 130 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1948 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 131 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(',',(yyvsp[-2].node),(yyvsp[0].node));}
#line 1954 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 134 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1960 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 135 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('=',(yyvsp[-2].node),(yyvsp[0].node));}
#line 1966 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 138 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(':',(yyvsp[-2].node),(yyvsp[0].node));}
#line 1972 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 141 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1978 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 142 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=resolveAttributes(NODE(LITERAL,(yyvsp[-1].object)),(yyvsp[0].node));}
#line 1984 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 143 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=resolveAttributes(NODE('[',(yyvsp[-4].node),(yyvsp[-2].node)),(yyvsp[0].node));}
#line 1990 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 144 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=resolveAttributes(NODE('L',(yyvsp[-2].node)),(yyvsp[0].node));}
#line 1996 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 145 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=resolveAttributes((yyvsp[-2].node),(yyvsp[0].node));}
#line 2002 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 146 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=resolveAttributes((yyvsp[-1].node),(yyvsp[0].node));}
#line 2008 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 147 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(AND,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2014 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 148 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(OR,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2020 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 149 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(NOT,(yyvsp[0].node));}
#line 2026 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 150 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('+',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2032 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 151 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('-',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2038 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 152 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('*',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2044 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 153 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('/',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2050 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 154 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('%',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2056 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 155 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('^',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2062 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 156 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('|',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2068 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 157 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('&',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2074 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 158 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(LSHIFT,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2080 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 159 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(RSHIFT,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2086 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 162 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2092 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 163 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=resolveAttributes(NODE(LITERAL,(yyvsp[-1].object)),(yyvsp[0].node));}
#line 2098 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 164 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(CONTEXT); (yyval.node)->marker=(yyvsp[-1].marker); if ( (yyvsp[0].node) ) {(yyval.node)=resolveAttributes((yyval.node),(yyvsp[0].node));}}
#line 2104 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 165 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=resolveAttributes(NODE('[',(yyvsp[-4].node),(yyvsp[-2].node)),(yyvsp[0].node));}
#line 2110 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 166 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=resolveAttributes(NODE('L',(yyvsp[-2].node)),(yyvsp[0].node));}
#line 2116 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 167 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=resolveAttributes((yyvsp[-2].node),(yyvsp[0].node));}
#line 2122 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 168 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=resolveAttributes((yyvsp[-1].node),(yyvsp[0].node));}
#line 2128 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 169 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(AND,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2134 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 170 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(OR,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2140 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 171 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(NOT,(yyvsp[0].node));}
#line 2146 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 172 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(COMPARISON,(yyvsp[-2].node),(yyvsp[0].node)); (yyval.node)->marker=(yyvsp[-1].marker);}
#line 2152 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 173 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('+',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2158 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 174 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('-',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2164 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 175 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('*',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2170 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 176 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('/',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2176 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 177 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('%',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2182 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 178 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('^',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2188 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 179 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('|',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2194 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 180 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('&',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2200 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 181 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(LSHIFT,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2206 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 182 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(RSHIFT,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2212 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 183 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(IS,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2218 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 184 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(IS,(yyvsp[-3].node),(yyvsp[0].node));}
#line 2224 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 185 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[-1].node);}
#line 2230 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 188 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NULL;}
#line 2236 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 189 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2242 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 192 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2248 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 193 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(',',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2254 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 196 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NULL;}
#line 2260 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 197 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2266 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 200 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2272 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 201 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(',',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2278 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 204 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('C',(yyvsp[-3].node),(yyvsp[-1].node));}
#line 2284 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 207 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NULL;}
#line 2290 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 208 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2296 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 209 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(',',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2302 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 212 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2308 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 213 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('=',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2314 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 216 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NULL;}
#line 2320 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 217 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2326 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 220 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2332 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 221 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[-1].node)? NODE(';',(yyvsp[-1].node),(yyvsp[0].node)) : (yyvsp[0].node);}
#line 2338 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 224 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NULL;}
#line 2344 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 225 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[-1].node);}
#line 2350 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 226 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2356 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 227 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2362 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 230 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(IF,(yyvsp[-5].node),(yyvsp[-3].node),(yyvsp[-1].node),(yyvsp[0].node));}
#line 2368 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 233 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NULL;}
#line 2374 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 234 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(EIF,(yyvsp[-5].node),(yyvsp[-3].node),(yyvsp[-1].node));}
#line 2380 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 237 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NULL;}
#line 2386 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 238 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[-1].node);}
#line 2392 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 241 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(WHILE,(yyvsp[-3].node),(yyvsp[-1].node));}
#line 2398 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 244 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(PROD,(yyvsp[0].node));}
#line 2404 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 245 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(DROP);}
#line 2410 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 246 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(END);}
#line 2416 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 247 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(CONT);}
#line 2422 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 248 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(BREAK);}
#line 2428 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 249 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(ABORT,(yyvsp[0].node));}
#line 2434 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 250 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(VERIFY,(yyvsp[0].node));}
#line 2440 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 251 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(AS,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2446 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 252 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(OPERATOR_ASSIGNMENT,(yyvsp[-2].node),(yyvsp[0].node)); (yyval.node)->marker=(yyvsp[-1].marker);}
#line 2452 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 253 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2458 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 254 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2464 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 255 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2470 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 256 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2476 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 259 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(ARROW,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2482 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 262 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2488 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 263 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(',',(yyvsp[-3].node),(yyvsp[-1].node));}
#line 2494 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 266 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2500 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 267 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2506 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 270 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2512 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 271 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(',',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2518 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 274 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2524 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 275 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(ARROW,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2530 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 278 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2536 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 279 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(':',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2542 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 280 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(TYPE); (yyval.node)->marker=(yyvsp[-1].marker); (yyval.node)=resolveAttributes((yyval.node),(yyvsp[0].node));}
#line 2548 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 281 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=resolveAttributes((yyvsp[-1].node),(yyvsp[0].node));}
#line 2554 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 282 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=resolveAttributes((yyvsp[-1].node),(yyvsp[0].node));}
#line 2560 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 283 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(':',resolveAttributes((yyvsp[-3].node),(yyvsp[-2].node)),(yyvsp[0].node));}
#line 2566 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 284 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=resolveAttributes((yyvsp[-1].node),(yyvsp[0].node));}
#line 2572 "parser.tab.c" /* yacc.c:1646  */
    break;


#line 2576 "parser.tab.c" /* yacc.c:1646  */
        default: break;
      }
    if (yychar_backup != yychar)
      YY_LAC_DISCARD ("yychar change");
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

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyesa, &yyes, &yyes_capacity, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        if (yychar != YYEMPTY)
          YY_LAC_ESTABLISH;
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
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
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

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

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


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  /* If the stack popping above didn't lose the initial context for the
     current lookahead token, the shift below will for sure.  */
  YY_LAC_DISCARD ("error recovery");

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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

#if 1
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
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
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
  if (yyes != yyesa)
    YYSTACK_FREE (yyes);
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 287 "parser.y" /* yacc.c:1906  */


void yyerror(const char *format, ...) {
	va_list args;

	fprintf(stderr,"Syntax error on line %i: ", yylineno);
	va_start(args,format);
	vfprintf(stderr,format,args);
	va_end(args);
	fprintf(stderr,"\n");
}

static astNodePtr resolveAttributes(astNodePtr object, astNodePtr attributes) {
	return attributes? NODE('.',object,attributes) : object;
}
