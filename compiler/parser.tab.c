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
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parser.y" /* yacc.c:339  */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "ast.h"
#include "marker.h"
#include "nameTable.h"
#include "plObject.h"
#include "parserWrapper.h"

int yylex(YYSTYPE *yylvalp, YYLTYPE *yyllocp, yyscan_t scanner);
void yyerror(YYLTYPE *yyllocp, yyscan_t scanner, const char *format, ...);

static astNodePtr resolveAttributes(const YYLTYPE *yyllocp, astNodePtr object, astNodePtr attributes);
#define RESOLVE_ATTRIBUTES(object,attributes) resolveAttributes(&yylloc,object,attributes)

#define NODE(...) createNode(&yylloc, __VA_ARGS__)


#line 87 "parser.tab.c" /* yacc.c:339  */

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
#line 29 "parser.y" /* yacc.c:355  */

    astNodePtr node;
    plObject *object;
    int marker;

#line 169 "parser.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int yyparse (astNodePtr *programTree, yyscan_t scanner);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 199 "parser.tab.c" /* yacc.c:358  */

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
#define YYLAST   637

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  138
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  280

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
       2,    45,     2,    54,    42,    55,     2,     2,     2,     2,
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
       0,    70,    70,    73,    74,    77,    78,    79,    80,    81,
      82,    83,    84,    87,    90,    93,    96,    99,   100,   103,
     104,   107,   110,   111,   114,   117,   120,   123,   126,   129,
     130,   133,   134,   137,   138,   141,   142,   145,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   195,   196,   199,   200,   203,   204,   207,   208,   211,
     214,   215,   216,   219,   220,   223,   224,   227,   228,   231,
     232,   233,   234,   237,   240,   241,   244,   245,   248,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   266,   269,   270,   273,   274,   277,   278,   281,
     282,   285,   286,   287,   288,   289,   290,   291,   292
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
  "'}'", "';'", "'('", "')'", "'='", "':'", "']'", "'_'", "$accept",
  "file", "file_section", "global_content", "main_definition",
  "import_statement", "export_statement", "struct_definition",
  "struct_field", "type", "moduled_name", "attribute_trail",
  "source_definition", "pipe_definition", "sink_definition",
  "local_definition", "global_var_definition", "optional_name",
  "possible_arg_list", "arg_list", "arg_element", "single_argument",
  "compilation_expression", "expression", "compilation_array_literal",
  "compilation_array_element", "array_literal", "array_element",
  "function_call", "call_list", "call_arg", "statement_list",
  "actual_statement_list", "general_statement", "if_section",
  "eif_section", "else_section", "while_section", "statement",
  "arrow_statement", "arrow_sender", "arrow_sender_item",
  "arrow_sender_list", "arrow_receiver", "arrow_receiver_item", YY_NULLPTR
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
      40,    41,    61,    58,    93,    95
};
# endif

#define YYPACT_NINF -141

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-141)))

#define YYTABLE_NINF -128

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-128)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     274,    -2,    -2,    -2,     9,   189,    50,    55,    48,  -141,
    -141,   189,   189,    96,   274,  -141,  -141,  -141,  -141,  -141,
      47,  -141,  -141,  -141,  -141,   452,  -141,  -141,    51,    53,
      54,    58,   561,    49,    72,   257,    78,    78,   512,    46,
      90,   365,  -141,  -141,   110,    97,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,    78,
      99,    99,    99,    99,  -141,  -141,   278,   278,   278,  -141,
    -141,   278,  -141,  -141,   278,   278,   111,  -141,  -141,   278,
    -141,    29,    17,   109,  -141,  -141,   472,  -141,    88,   257,
    -141,  -141,  -141,    89,  -141,   112,  -141,   113,  -141,   189,
    -141,    87,    29,   492,    27,  -141,    95,   -14,   103,   561,
     561,   581,   581,   241,   241,   311,    -3,    -3,    -3,   323,
      98,   106,   114,  -141,   107,   119,   120,    34,  -141,   385,
     407,   492,   532,   492,   492,    43,    78,    78,   492,   104,
     125,  -141,   345,   121,   126,   129,    43,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,    78,  -141,  -141,  -141,    13,  -141,    78,   512,
      78,   278,   110,  -141,  -141,  -141,    43,   148,    99,   189,
     152,   134,    99,  -141,   257,   257,  -141,  -141,  -141,  -141,
     278,   162,  -141,   172,  -141,    42,   141,   532,   532,   552,
     492,   588,   588,   501,   501,   592,    39,    39,    39,   303,
     151,  -141,  -141,   158,  -141,  -141,  -141,   190,  -141,   492,
    -141,    78,  -141,    43,  -141,   512,    43,   257,  -141,   168,
     170,    78,   492,    43,    78,   492,   174,  -141,    99,    78,
     191,    78,   -24,    78,    13,   180,   181,   182,  -141,  -141,
    -141,  -141,    78,   178,  -141,   202,  -141,   257,   257,  -141,
      66,   188,  -141,   192,   194,   278,   196,  -141,   257,  -141,
    -141,   429,   257,   215,   257,   216,  -141,   232,  -141,  -141
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
       0,    22,   138,   131,    22,    22,    22,   122,   129,    94,
      92,    40,    37,     0,    34,    36,     0,    95,    18,     0,
       0,    61,    88,     0,    62,   125,   128,    22,     0,   133,
       0,   134,   135,   137,     0,     0,     0,     0,   108,   104,
      79,   124,    60,     0,   132,     0,   130,    95,    95,    26,
     106,     0,   136,     0,     0,     0,     0,   103,    95,    24,
      25,     0,    95,     0,    95,     0,    27,     0,   107,   105
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -141,  -141,  -141,   222,  -141,  -141,  -141,  -141,  -141,  -140,
       0,    -6,    21,     1,     2,  -141,  -141,    86,  -141,  -141,
      59,   -54,   201,   -41,  -141,  -141,  -141,  -141,   142,  -141,
     118,   -94,  -141,   149,  -141,  -141,  -141,  -141,  -141,   -40,
    -141,   -71,  -141,  -141,    57
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    13,    14,    15,    16,    17,    18,    19,   127,   187,
      82,    36,    83,    84,    85,   216,    24,    28,   121,   122,
     123,   124,    25,    86,    39,    40,   139,   140,    87,   104,
     105,    88,    89,    90,    91,   260,   267,    92,    93,    94,
      95,    96,   145,   217,   218
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      20,    22,    23,   103,    37,    20,   194,   125,   126,   128,
     144,    20,    20,    47,    20,    22,    23,     2,     3,   210,
      59,    21,    97,    46,    47,   129,   130,   131,    27,   255,
     132,    58,     1,   133,   134,    21,   222,   146,   138,    31,
     142,   143,    58,     9,    71,   211,    20,    20,    20,    20,
      20,    20,    20,    20,    20,    20,    20,    20,    20,     9,
      77,   142,   143,   172,    78,   147,   148,    44,   212,   148,
     182,   136,   137,     9,    79,   186,   265,   266,   173,    81,
      33,   162,   183,   245,   161,    34,   246,   161,    29,    30,
     229,   230,   168,   250,   170,    35,    42,    44,    64,    20,
      98,    60,   141,    61,    62,    63,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,    65,   236,   141,    97,    71,    99,   106,   228,   120,
     219,   103,   135,   247,  -126,   188,   163,   166,   165,   171,
     101,    77,    26,   167,   174,    78,   188,    26,    58,   232,
     178,   176,   235,    26,    26,    79,    26,   177,   189,   179,
     102,   190,  -127,   263,   264,   193,   213,   214,   215,   221,
     180,   181,   192,   223,   273,     1,   188,   226,   275,    20,
     277,   227,   233,   231,   253,   234,   161,    71,    26,    26,
      26,    26,    26,    26,    26,    26,    26,    26,    26,    26,
      26,   238,     9,    77,     5,   239,    32,    78,   241,   242,
     243,   240,    38,    41,   141,   244,   248,    79,   249,     9,
      10,   254,   102,   188,   271,   251,   188,   257,   258,   261,
     259,   252,   262,   188,    11,   268,    43,   224,   164,    12,
     269,    26,   270,   272,   213,   214,   215,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
       1,     2,     3,   276,   278,    66,    67,    46,    47,    68,
      69,    70,    71,    72,    73,    74,    75,     1,     2,     3,
     279,     4,    54,    55,    56,    57,    58,    76,    77,     5,
     220,     0,    78,    71,     0,     0,     6,     7,     8,     0,
     169,   256,    79,     0,     9,    10,    80,    81,     9,    77,
       0,     0,     0,    78,     0,     0,     0,     0,     0,    11,
       0,    26,     0,    79,    12,     0,     0,     0,   102,   147,
     148,   149,   150,     0,     0,     0,   151,    46,    47,     0,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    46,
      47,    48,    49,    55,    56,    57,    58,   237,     0,     0,
      50,    51,    52,    53,    54,    55,    56,    57,    58,     0,
       0,   147,   148,   149,   150,     0,     0,   175,   151,     0,
     225,     0,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    46,    47,    48,    49,     0,   191,     0,     0,     0,
       0,     0,    50,    51,    52,    53,    54,    55,    56,    57,
      58,   147,   148,   149,   150,     0,   100,     0,   151,     0,
       0,     0,   153,   154,   155,   156,   157,   158,   159,   160,
     161,     0,   184,   147,   148,   149,   150,     0,     0,     0,
     151,     0,     0,     0,   153,   154,   155,   156,   157,   158,
     159,   160,   161,     0,   185,   147,   148,   149,   150,     0,
       0,     0,   151,     0,     0,     0,   153,   154,   155,   156,
     157,   158,   159,   160,   161,     0,   274,    45,    46,    47,
      48,    49,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,    53,    54,    55,    56,    57,    58,   147,   148,
     149,   150,     0,     0,     0,   151,   152,     0,     0,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   147,   148,
     149,   150,     0,     0,     0,   151,     0,   147,   148,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    46,    47,
      48,    49,   157,   158,   159,   160,   161,     0,     0,    50,
      51,    52,    53,    54,    55,    56,    57,    58,   147,   148,
       0,     0,     0,     0,     0,   151,     0,     0,     0,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   147,   148,
       0,     0,     0,     0,     0,  -128,     0,    46,    47,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    46,    47,     0,
       0,     0,     0,     0,   147,   148,     0,     0,   147,   148,
      52,    53,    54,    55,    56,    57,    58,   155,   156,   157,
     158,   159,   160,   161,   158,   159,   160,   161
};

static const yytype_int16 yycheck[] =
{
       0,     0,     0,    44,    10,     5,   146,    61,    62,    63,
      81,    11,    12,    27,    14,    14,    14,     4,     5,     6,
      26,     0,    46,    26,    27,    66,    67,    68,    30,    53,
      71,    45,     3,    74,    75,    14,   176,    20,    79,    30,
      81,    81,    45,    30,    15,    32,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    30,
      31,   102,   102,    36,    35,    26,    27,    50,    55,    27,
      36,    77,    78,    30,    45,    32,    10,    11,    51,    50,
      30,    87,    48,   223,    45,    30,   226,    45,     2,     3,
     184,   185,    98,   233,   100,    47,     0,    50,    49,    99,
      54,    50,    81,    50,    50,    47,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    49,   193,   102,    46,    15,    36,    30,   182,    30,
     171,   172,    21,   227,    25,   135,    48,    25,    49,    52,
      30,    31,     0,    30,    49,    35,   146,     5,    45,   190,
      36,    53,   193,    11,    12,    45,    14,    51,    54,    52,
      50,    36,    36,   257,   258,    36,   166,   166,   166,   175,
      51,    51,    51,    25,   268,     3,   176,    25,   272,   179,
     274,    47,    20,   189,   238,   191,    45,    15,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    50,    30,    31,    15,   211,     5,    35,   214,   215,
     216,    53,    11,    12,   193,    25,    48,    45,    48,    30,
      31,    30,    50,   223,   265,    51,   226,    47,    47,    51,
      48,   237,    30,   233,    45,    47,    14,   178,    89,    50,
      48,    99,    48,    47,   244,   244,   244,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
       3,     4,     5,    48,    48,     8,     9,    26,    27,    12,
      13,    14,    15,    16,    17,    18,    19,     3,     4,     5,
      48,     7,    41,    42,    43,    44,    45,    30,    31,    15,
     172,    -1,    35,    15,    -1,    -1,    22,    23,    24,    -1,
      99,   244,    45,    -1,    30,    31,    49,    50,    30,    31,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    45,
      -1,   179,    -1,    45,    50,    -1,    -1,    -1,    50,    26,
      27,    28,    29,    -1,    -1,    -1,    33,    26,    27,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    26,
      27,    28,    29,    42,    43,    44,    45,    54,    -1,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      -1,    26,    27,    28,    29,    -1,    -1,    54,    33,    -1,
     179,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    26,    27,    28,    29,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    26,    27,    28,    29,    -1,    51,    -1,    33,    -1,
      -1,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    26,    27,    28,    29,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    26,    27,    28,    29,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    25,    26,    27,
      28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    26,    27,
      28,    29,    -1,    -1,    -1,    33,    34,    -1,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    26,    27,
      28,    29,    -1,    -1,    -1,    33,    -1,    26,    27,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    26,    27,
      28,    29,    41,    42,    43,    44,    45,    -1,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    26,    27,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    26,    27,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    26,    27,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    26,    27,    -1,
      -1,    -1,    -1,    -1,    26,    27,    -1,    -1,    26,    27,
      39,    40,    41,    42,    43,    44,    45,    39,    40,    41,
      42,    43,    44,    45,    42,    43,    44,    45
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     7,    15,    22,    23,    24,    30,
      31,    45,    50,    57,    58,    59,    60,    61,    62,    63,
      66,    68,    69,    70,    72,    78,    84,    30,    73,    73,
      73,    30,    78,    30,    30,    47,    67,    67,    78,    80,
      81,    78,     0,    59,    50,    25,    26,    27,    28,    29,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    67,
      50,    50,    50,    47,    49,    49,     8,     9,    12,    13,
      14,    15,    16,    17,    18,    19,    30,    31,    35,    45,
      49,    50,    66,    68,    69,    70,    79,    84,    87,    88,
      89,    90,    93,    94,    95,    96,    97,    46,    54,    36,
      51,    30,    50,    79,    85,    86,    30,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      30,    74,    75,    76,    77,    77,    77,    64,    77,    79,
      79,    79,    79,    79,    79,    21,    67,    67,    79,    82,
      83,    68,    79,    95,    97,    98,    20,    26,    27,    28,
      29,    33,    34,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    67,    48,    89,    49,    25,    30,    67,    78,
      67,    52,    36,    51,    49,    54,    53,    51,    36,    52,
      51,    51,    36,    48,    47,    47,    32,    65,    66,    54,
      36,    51,    51,    36,    65,    79,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    79,    79,    79,
       6,    32,    55,    66,    69,    70,    71,    99,   100,    79,
      86,    67,    65,    25,    76,    78,    25,    47,    77,    87,
      87,    67,    79,    20,    67,    79,    97,    54,    50,    67,
      53,    67,    67,    67,    25,    65,    65,    87,    48,    48,
      65,    51,    67,    77,    30,    53,   100,    47,    47,    48,
      91,    51,    30,    87,    87,    10,    11,    92,    47,    48,
      48,    79,    47,    87,    47,    87,    48,    87,    48,    48
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    56,    57,    58,    58,    59,    59,    59,    59,    59,
      59,    59,    59,    60,    61,    62,    63,    64,    64,    65,
      65,    66,    67,    67,    68,    69,    70,    71,    72,    73,
      73,    74,    74,    75,    75,    76,    76,    77,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    79,    79,    79,
      79,    80,    80,    81,    81,    82,    82,    83,    83,    84,
      85,    85,    85,    86,    86,    87,    87,    88,    88,    89,
      89,    89,    89,    90,    91,    91,    92,    92,    93,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    95,    96,    96,    97,    97,    98,    98,    99,
      99,   100,   100,   100,   100,   100,   100,   100,   100
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
       3,     1,     3,     2,     2,     2,     4,     2,     1
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
      yyerror (&yylloc, programTree, scanner, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
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
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, programTree, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, astNodePtr *programTree, yyscan_t scanner)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (programTree);
  YYUSE (scanner);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, astNodePtr *programTree, yyscan_t scanner)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, programTree, scanner);
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, astNodePtr *programTree, yyscan_t scanner)
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
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , programTree, scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, programTree, scanner); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, astNodePtr *programTree, yyscan_t scanner)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (programTree);
  YYUSE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (astNodePtr *programTree, yyscan_t scanner)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

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

  yyes = yyesa;
  yyes_capacity = sizeof yyesa / sizeof *yyes;
  if (YYMAXDEPTH < yyes_capacity)
    yyes_capacity = YYMAXDEPTH;

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
      yychar = yylex (&yylval, &yylloc, scanner);
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
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  {
    int yychar_backup = yychar;
    switch (yyn)
      {
          case 2:
#line 70 "parser.y" /* yacc.c:1646  */
    {*programTree=(yyvsp[0].node);}
#line 1883 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 73 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1889 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 74 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = NODE(';',(yyvsp[-1].node),(yyvsp[0].node));}
#line 1895 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 77 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1901 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 78 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1907 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 79 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1913 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 80 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1919 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 81 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1925 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 82 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1931 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 83 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1937 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 84 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1943 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 87 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(MAIN,(yyvsp[-1].node));}
#line 1949 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 90 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(IMPORT,(yyvsp[-1].node));}
#line 1955 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 93 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(EXPORT,(yyvsp[-1].node));}
#line 1961 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 96 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(STRUCT,(yyvsp[-3].node),(yyvsp[-1].node));}
#line 1967 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 99 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1973 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 100 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(',',(yyvsp[-2].node),(yyvsp[0].node));}
#line 1979 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 103 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(TYPE); (yyval.node)->marker=(yyvsp[0].marker);}
#line 1985 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 104 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1991 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 107 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=RESOLVE_ATTRIBUTES((yyvsp[-1].node),(yyvsp[0].node));}
#line 1997 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 110 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NULL;}
#line 2003 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 111 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[-2].node)? NODE('.',(yyvsp[-2].node),(yyvsp[0].node)) : (yyvsp[0].node);}
#line 2009 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 114 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(SOURCE,(yyvsp[-8].node),(yyvsp[-6].node),(yyvsp[-3].node),(yyvsp[-1].node));}
#line 2015 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 117 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(PIPE,(yyvsp[-8].node),(yyvsp[-6].node),(yyvsp[-3].node),(yyvsp[-1].node));}
#line 2021 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 120 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(SINK,(yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-1].node));}
#line 2027 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 123 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(LOCAL,(yyvsp[-4].node),(yyvsp[-1].node));}
#line 2033 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 126 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(ARROW,(yyvsp[-3].node),(yyvsp[-1].node));}
#line 2039 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 129 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NULL;}
#line 2045 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 130 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2051 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 133 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NULL;}
#line 2057 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 134 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2063 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 137 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2069 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 138 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(',',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2075 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 141 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2081 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 142 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('=',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2087 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 145 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(':',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2093 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 148 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2099 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 149 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=RESOLVE_ATTRIBUTES(NODE(LITERAL,(yyvsp[-1].object)),(yyvsp[0].node));}
#line 2105 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 150 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=RESOLVE_ATTRIBUTES(NODE('[',(yyvsp[-4].node),(yyvsp[-2].node)),(yyvsp[0].node));}
#line 2111 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 151 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=RESOLVE_ATTRIBUTES(NODE('L',(yyvsp[-2].node)),(yyvsp[0].node));}
#line 2117 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 152 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=RESOLVE_ATTRIBUTES((yyvsp[-2].node),(yyvsp[0].node));}
#line 2123 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 153 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=RESOLVE_ATTRIBUTES((yyvsp[-1].node),(yyvsp[0].node));}
#line 2129 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 154 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(AND,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2135 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 155 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(OR,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2141 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 156 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(NOT,(yyvsp[0].node));}
#line 2147 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 157 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('+',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2153 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 158 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('-',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2159 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 159 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('*',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2165 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 160 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('/',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2171 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 161 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('%',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2177 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 162 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('^',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2183 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 163 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('|',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2189 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 164 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('&',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2195 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 165 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(LSHIFT,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2201 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 166 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(RSHIFT,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2207 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 169 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2213 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 170 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=RESOLVE_ATTRIBUTES(NODE(LITERAL,(yyvsp[-1].object)),(yyvsp[0].node));}
#line 2219 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 171 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(CONTEXT); (yyval.node)->marker=(yyvsp[-1].marker); if ( (yyvsp[0].node) ) {(yyval.node)=RESOLVE_ATTRIBUTES((yyval.node),(yyvsp[0].node));}}
#line 2225 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 172 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=RESOLVE_ATTRIBUTES(NODE('[',(yyvsp[-4].node),(yyvsp[-2].node)),(yyvsp[0].node));}
#line 2231 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 173 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=RESOLVE_ATTRIBUTES(NODE('L',(yyvsp[-2].node)),(yyvsp[0].node));}
#line 2237 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 174 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=RESOLVE_ATTRIBUTES((yyvsp[-2].node),(yyvsp[0].node));}
#line 2243 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 175 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=RESOLVE_ATTRIBUTES((yyvsp[-1].node),(yyvsp[0].node));}
#line 2249 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 176 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(AND,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2255 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 177 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(OR,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2261 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 178 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(NOT,(yyvsp[0].node));}
#line 2267 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 179 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(COMPARISON,(yyvsp[-2].node),(yyvsp[0].node)); (yyval.node)->marker=(yyvsp[-1].marker);}
#line 2273 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 180 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('+',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2279 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 181 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('-',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2285 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 182 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('*',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2291 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 183 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('/',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2297 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 184 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('%',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2303 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 185 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('^',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2309 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 186 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('|',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2315 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 187 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('&',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2321 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 188 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(LSHIFT,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2327 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 189 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(RSHIFT,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2333 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 190 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(IS,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2339 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 191 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(IS,(yyvsp[-3].node),(yyvsp[0].node));}
#line 2345 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 192 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[-1].node);}
#line 2351 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 195 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NULL;}
#line 2357 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 196 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2363 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 199 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2369 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 200 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(',',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2375 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 203 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NULL;}
#line 2381 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 204 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2387 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 207 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2393 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 208 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(',',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2399 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 211 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('C',(yyvsp[-3].node),(yyvsp[-1].node));}
#line 2405 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 214 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NULL;}
#line 2411 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 215 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2417 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 216 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(',',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2423 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 219 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2429 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 220 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('=',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2435 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 223 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NULL;}
#line 2441 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 224 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2447 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 227 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2453 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 228 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[-1].node)? NODE(';',(yyvsp[-1].node),(yyvsp[0].node)) : (yyvsp[0].node);}
#line 2459 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 231 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NULL;}
#line 2465 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 232 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[-1].node);}
#line 2471 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 233 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2477 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 234 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2483 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 237 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(IF,(yyvsp[-5].node),(yyvsp[-3].node),(yyvsp[-1].node),(yyvsp[0].node));}
#line 2489 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 240 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NULL;}
#line 2495 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 241 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(EIF,(yyvsp[-5].node),(yyvsp[-3].node),(yyvsp[-1].node));}
#line 2501 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 244 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NULL;}
#line 2507 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 245 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[-1].node);}
#line 2513 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 248 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(WHILE,(yyvsp[-3].node),(yyvsp[-1].node));}
#line 2519 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 251 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(PROD,(yyvsp[0].node));}
#line 2525 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 252 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(DROP);}
#line 2531 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 253 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(END);}
#line 2537 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 254 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(CONT);}
#line 2543 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 255 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(BREAK);}
#line 2549 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 256 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(ABORT,(yyvsp[0].node));}
#line 2555 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 257 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(VERIFY,(yyvsp[0].node));}
#line 2561 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 258 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(AS,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2567 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 259 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(OPERATOR_ASSIGNMENT,(yyvsp[-2].node),(yyvsp[0].node)); (yyval.node)->marker=(yyvsp[-1].marker);}
#line 2573 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 260 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2579 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 261 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2585 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 262 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2591 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 263 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2597 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 266 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(ARROW,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2603 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 269 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2609 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 270 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(',',(yyvsp[-3].node),(yyvsp[-1].node));}
#line 2615 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 273 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2621 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 274 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2627 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 277 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2633 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 278 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(',',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2639 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 281 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2645 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 282 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(ARROW,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2651 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 285 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2657 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 286 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(':',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2663 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 287 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(TYPE); (yyval.node)->marker=(yyvsp[-1].marker); (yyval.node)=RESOLVE_ATTRIBUTES((yyval.node),(yyvsp[0].node));}
#line 2669 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 288 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=RESOLVE_ATTRIBUTES((yyvsp[-1].node),(yyvsp[0].node));}
#line 2675 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 289 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=RESOLVE_ATTRIBUTES((yyvsp[-1].node),(yyvsp[0].node));}
#line 2681 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 290 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE(':',RESOLVE_ATTRIBUTES((yyvsp[-3].node),(yyvsp[-2].node)),(yyvsp[0].node));}
#line 2687 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 291 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=RESOLVE_ATTRIBUTES((yyvsp[-1].node),(yyvsp[0].node));}
#line 2693 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 292 "parser.y" /* yacc.c:1646  */
    {(yyval.node)=NODE('_');}
#line 2699 "parser.tab.c" /* yacc.c:1646  */
    break;


#line 2703 "parser.tab.c" /* yacc.c:1646  */
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
  *++yylsp = yyloc;

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
      yyerror (&yylloc, programTree, scanner, YY_("syntax error"));
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
        yyerror (&yylloc, programTree, scanner, yymsgp);
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
                      yytoken, &yylval, &yylloc, programTree, scanner);
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp, programTree, scanner);
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

#if 1
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, programTree, scanner, YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc, programTree, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, programTree, scanner);
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
#line 295 "parser.y" /* yacc.c:1906  */


void yyerror(YYLTYPE *yyllocp, yyscan_t scanner, const char *format, ...) {
    (void)scanner;
    va_list args;

    fprintf(stderr,"Syntax error beginning on line %i: ", yyllocp->first_line);
    va_start(args,format);
    vfprintf(stderr,format,args);
    va_end(args);
    fprintf(stderr,"\n");
}

static astNodePtr resolveAttributes(const YYLTYPE *yyllocp, astNodePtr object, astNodePtr attributes) {
    return attributes? createNode(yyllocp,'.',object,attributes) : object;
}
