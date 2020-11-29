/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "ast.h"
#include "marker.h"
#include "nameTable.h"
#include "plObject.h"
#include "parserWrapper.h"

int yylex(YYSTYPE *yylvalp, YYLTYPE *yyllocp, yyscan_t scanner);

void yyerror(YYLTYPE *yyllocp, yyscan_t yyscanner, const char *format, ...);

static astNodePtr resolveAttributes(const YYLTYPE *yyllocp, astNodePtr object, astNodePtr attributes);
#define RESOLVE_ATTRIBUTES(object,attributes) resolveAttributes(&yylloc,object,attributes)

#define NODE(...) createNode(&yylloc, __VA_ARGS__)


#line 92 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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
#line 30 "parser.y"

    astNodePtr node;
    plObject *object;
    int marker;

#line 186 "parser.tab.c"

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



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

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
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
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
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
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
#define YYLAST   663

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  46
/* YYNRULES -- Number of rules.  */
#define YYNRULES  140
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  287

#define YYUNDEFTOK  2
#define YYMAXUTOK   290


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    41,    44,     2,
      51,    52,    39,    37,    36,    38,    46,    40,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    50,    49,
       2,    53,     2,     2,     2,     2,     2,     2,     2,     2,
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
static const yytype_int16 yyrline[] =
{
       0,    71,    71,    74,    75,    78,    79,    80,    81,    82,
      83,    84,    85,    88,    91,    94,    97,   100,   101,   104,
     105,   108,   109,   112,   115,   116,   119,   122,   125,   128,
     131,   134,   135,   138,   139,   142,   143,   146,   147,   150,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   200,   201,   204,   205,   208,   209,   212,
     213,   216,   219,   220,   221,   224,   225,   228,   229,   232,
     233,   236,   237,   238,   239,   242,   245,   246,   249,   250,
     253,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   271,   274,   275,   278,   279,   282,
     283,   286,   287,   290,   291,   292,   293,   294,   295,   296,
     297
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
  "'}'", "';'", "':'", "'('", "')'", "'='", "']'", "'_'", "$accept",
  "file", "file_section", "global_content", "main_definition",
  "import_statement", "export_statement", "struct_definition",
  "struct_field_list", "struct_field", "type", "moduled_name",
  "attribute_trail", "source_definition", "pipe_definition",
  "sink_definition", "local_definition", "global_var_definition",
  "optional_name", "possible_arg_list", "arg_list", "arg_element",
  "single_argument", "compilation_expression", "expression",
  "compilation_array_literal", "compilation_array_element",
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
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,    44,    43,    45,    42,
      47,    37,    94,   124,    38,    91,    46,   123,   125,    59,
      58,    40,    41,    61,    93,    95
};
# endif

#define YYPACT_NINF (-132)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-130)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     282,    -1,    -1,    -1,     6,   194,    44,    48,   -15,  -132,
    -132,   194,   194,    88,   282,  -132,  -132,  -132,  -132,  -132,
      40,  -132,  -132,  -132,  -132,   462,  -132,  -132,    49,    73,
      80,    57,   571,    77,    87,   265,    92,    92,   522,    85,
     104,   375,  -132,  -132,    45,   111,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,    92,
     112,   112,   112,   113,  -132,  -132,    52,    52,    52,  -132,
    -132,    52,  -132,  -132,    52,    52,   123,  -132,  -132,    52,
    -132,   287,     8,   124,  -132,  -132,   482,  -132,   103,   265,
    -132,  -132,  -132,   105,  -132,   128,  -132,   136,  -132,   194,
    -132,   114,   287,   502,    27,  -132,   109,   -14,   127,   571,
     571,   591,   591,   511,   511,   120,    -8,    -8,    -8,   335,
     125,   122,   140,  -132,   129,   131,   133,   130,    29,  -132,
    -132,   395,   417,   502,   542,   502,   502,    75,    92,    92,
     502,   150,   151,  -132,   355,   138,   169,   170,    75,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    92,  -132,  -132,  -132,    38,  -132,
      92,   522,    92,    52,    45,  -132,  -132,  -132,    75,   182,
     112,   194,   185,   168,    32,   113,  -132,   265,   265,  -132,
    -132,  -132,  -132,    52,   196,  -132,   300,  -132,    39,   173,
     542,   542,   562,   502,   598,   598,   618,   618,   249,    -4,
      -4,    -4,   315,   171,  -132,  -132,   177,  -132,  -132,  -132,
     198,  -132,   502,  -132,    92,  -132,    75,  -132,   522,    75,
     265,   172,  -132,   180,   184,    92,   502,    75,    92,   502,
     178,  -132,   112,    92,   203,    92,    23,    92,    38,   187,
     189,   190,   113,  -132,  -132,  -132,  -132,    92,   188,  -132,
     211,  -132,   265,   265,  -132,    53,   119,   195,  -132,   199,
     204,  -132,    52,   206,  -132,   265,  -132,  -132,   439,   265,
     223,   265,   224,  -132,   240,  -132,  -132
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    31,    31,    31,     0,     0,     0,     0,     0,    24,
      24,    83,     0,     0,     2,     3,     5,     6,     7,     8,
      40,     9,    10,    11,    12,     0,    24,    32,     0,     0,
       0,     0,    48,     0,     0,    97,    23,    41,    85,     0,
      84,     0,     1,     4,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    45,
      33,     0,     0,     0,    14,    15,     0,     0,     0,   112,
     113,     0,   114,   115,     0,     0,    24,    24,    24,    87,
     101,     0,    59,   121,   122,   123,   127,    24,     0,    98,
      99,   103,   104,     0,   120,     0,   125,     0,    24,     0,
      24,    24,     0,    95,     0,    93,     0,    57,    58,    46,
      47,    49,    50,    51,    52,    53,    54,    55,    56,     0,
       0,     0,    34,    35,    37,     0,     0,     0,     0,    17,
      19,     0,     0,   111,    68,   117,   116,     0,    60,    61,
      89,     0,    88,   128,   127,     0,   125,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    65,    13,   100,   102,     0,    25,
      43,    86,    44,     0,     0,    91,    30,    24,     0,     0,
       0,     0,     0,     0,     0,     0,    16,    97,    97,    21,
     118,    22,    24,     0,    24,    82,     0,    80,    78,    79,
      66,    67,    69,   119,    70,    71,    72,    73,    74,    75,
      76,    77,     0,     0,    24,   140,   133,    24,    24,    24,
     124,   131,    96,    94,    42,    39,     0,    36,    38,     0,
      97,     0,    18,     0,     0,    63,    90,     0,    64,   127,
     130,    24,     0,   135,     0,   136,   137,   139,     0,     0,
       0,     0,     0,   110,   106,    81,   126,    62,     0,   134,
       0,   132,    97,    97,    28,     0,   108,     0,   138,     0,
       0,    20,     0,     0,   105,    97,    26,    27,     0,    97,
       0,    97,     0,    29,     0,   109,   107
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -132,  -132,  -132,   229,  -132,  -132,  -132,  -132,    46,    61,
    -131,     0,    -6,    21,     1,     2,  -132,  -132,   132,  -132,
    -132,    71,   -53,   209,   -41,  -132,  -132,  -132,  -132,   145,
    -132,   135,  -102,  -132,   165,  -132,  -132,  -132,  -132,  -132,
     -57,  -132,   -71,  -132,  -132,    51
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    13,    14,    15,    16,    17,    18,    19,   128,   129,
     225,    82,    36,    83,    84,    85,   219,    24,    28,   121,
     122,   123,   130,    25,    86,    39,    40,   141,   142,    87,
     104,   105,    88,    89,    90,    91,   266,   274,    92,    93,
      94,    95,    96,   147,   220,   221
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      20,    22,    23,   103,    37,    20,   190,   124,   125,   126,
     146,    20,    20,    47,    20,    22,    23,   197,    46,    47,
      59,    21,   149,   150,   145,   131,   132,   133,   148,    27,
     134,    58,    35,   135,   136,    21,    31,    58,   140,   231,
     144,   163,     2,     3,   213,   145,    20,    20,    20,    20,
      20,    20,    20,    20,    20,    20,    20,    20,    20,    44,
      71,   144,     9,   174,   189,   185,   150,    71,     9,    97,
     214,   138,   139,   260,    33,   101,    77,   186,    34,   175,
      78,   164,     9,    77,   163,   233,   234,    78,    42,   185,
      79,    44,   170,   215,   172,   249,   102,    79,   250,    20,
      60,   271,   143,   102,    63,     9,   255,   189,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   143,    61,   240,    64,   124,   251,   272,
     273,    62,   222,   103,    29,    30,    65,   191,    97,    98,
      99,   106,   120,   127,   137,    26,    46,    47,   191,  -128,
      26,   165,   236,   168,   167,   239,    26,    26,   176,    26,
     269,   270,    55,    56,    57,    58,   169,   173,   216,   217,
     218,   224,    58,   280,   179,   178,   180,   282,   191,   284,
     184,    20,   181,   182,   191,   183,   235,   193,   238,   258,
     195,    26,    26,    26,    26,    26,    26,    26,    26,    26,
      26,    26,    26,    26,   192,  -129,   196,   226,   243,     5,
     229,   245,   246,   247,    32,   230,   237,   143,   163,   252,
      38,    41,   242,   248,     9,    10,   191,   244,   253,   191,
     256,   278,   254,   259,   262,   257,   263,   191,   264,    11,
     267,   268,   275,    43,    26,    12,   232,   276,   216,   217,
     218,   227,   277,   279,   166,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,     1,     2,
       3,   283,   285,    66,    67,   149,   150,    68,    69,    70,
      71,    72,    73,    74,    75,     1,     2,     3,   286,     4,
       1,   160,   161,   162,   163,    76,    77,     5,   265,   261,
      78,     0,    71,     1,     6,     7,     8,     0,   171,   223,
      79,     0,     9,    10,    80,    71,    81,     9,    77,     0,
       0,     0,    78,     0,     0,     0,    26,    11,     0,     0,
       9,    77,    79,    12,     0,    78,     0,     0,    81,     0,
       0,   149,   150,   151,   152,    79,     0,     0,   153,     0,
       0,   102,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    46,    47,    48,    49,     0,     0,     0,     0,   241,
       0,     0,    50,    51,    52,    53,    54,    55,    56,    57,
      58,   149,   150,   151,   152,     0,     0,     0,   153,   177,
     228,     0,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    46,    47,    48,    49,     0,     0,   194,     0,     0,
       0,     0,    50,    51,    52,    53,    54,    55,    56,    57,
      58,   149,   150,   151,   152,     0,     0,   100,   153,     0,
       0,     0,   155,   156,   157,   158,   159,   160,   161,   162,
     163,     0,   187,   149,   150,   151,   152,     0,     0,     0,
     153,     0,     0,     0,   155,   156,   157,   158,   159,   160,
     161,   162,   163,     0,   188,   149,   150,   151,   152,     0,
       0,     0,   153,     0,     0,     0,   155,   156,   157,   158,
     159,   160,   161,   162,   163,     0,   281,    45,    46,    47,
      48,    49,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,    53,    54,    55,    56,    57,    58,   149,   150,
     151,   152,     0,     0,     0,   153,   154,     0,     0,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   149,   150,
     151,   152,     0,     0,     0,   153,     0,    46,    47,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    46,    47,
      48,    49,    54,    55,    56,    57,    58,     0,     0,    50,
      51,    52,    53,    54,    55,    56,    57,    58,   149,   150,
       0,     0,     0,     0,     0,   153,     0,     0,     0,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   149,   150,
       0,     0,     0,     0,     0,  -130,     0,    46,    47,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    46,    47,     0,
       0,     0,     0,     0,   149,   150,     0,     0,     0,     0,
      52,    53,    54,    55,    56,    57,    58,   157,   158,   159,
     160,   161,   162,   163,   149,   150,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   159,
     160,   161,   162,   163
};

static const yytype_int16 yycheck[] =
{
       0,     0,     0,    44,    10,     5,   137,    60,    61,    62,
      81,    11,    12,    27,    14,    14,    14,   148,    26,    27,
      26,     0,    26,    27,    81,    66,    67,    68,    20,    30,
      71,    45,    47,    74,    75,    14,    30,    45,    79,     7,
      81,    45,     4,     5,     6,   102,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    51,
      15,   102,    30,    36,    32,    36,    27,    15,    30,    46,
      32,    77,    78,    50,    30,    30,    31,    48,    30,    52,
      35,    87,    30,    31,    45,   187,   188,    35,     0,    36,
      45,    51,    98,    55,   100,   226,    51,    45,   229,    99,
      51,    48,    81,    51,    47,    30,   237,    32,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   102,    51,   196,    49,   180,   230,    10,
      11,    51,   173,   174,     2,     3,    49,   137,    46,    54,
      36,    30,    30,    30,    21,     0,    26,    27,   148,    25,
       5,    48,   193,    25,    49,   196,    11,    12,    49,    14,
     262,   263,    42,    43,    44,    45,    30,    53,   168,   168,
     168,   177,    45,   275,    52,    50,    36,   279,   178,   281,
      50,   181,    53,    52,   184,    52,   192,    36,   194,   242,
      52,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    54,    36,    36,    25,   214,    15,
      25,   217,   218,   219,     5,    47,    20,   196,    45,    47,
      11,    12,    51,    25,    30,    31,   226,    50,    48,   229,
      52,   272,    48,    30,    47,   241,    47,   237,    48,    45,
      52,    30,    47,    14,    99,    51,   185,    48,   248,   248,
     248,   180,    48,    47,    89,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,     3,     4,
       5,    48,    48,     8,     9,    26,    27,    12,    13,    14,
      15,    16,    17,    18,    19,     3,     4,     5,    48,     7,
       3,    42,    43,    44,    45,    30,    31,    15,   252,   248,
      35,    -1,    15,     3,    22,    23,    24,    -1,    99,   174,
      45,    -1,    30,    31,    49,    15,    51,    30,    31,    -1,
      -1,    -1,    35,    -1,    -1,    -1,   181,    45,    -1,    -1,
      30,    31,    45,    51,    -1,    35,    -1,    -1,    51,    -1,
      -1,    26,    27,    28,    29,    45,    -1,    -1,    33,    -1,
      -1,    51,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    26,    27,    28,    29,    -1,    -1,    -1,    -1,    54,
      -1,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    26,    27,    28,    29,    -1,    -1,    -1,    33,    54,
     181,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    26,    27,    28,    29,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    26,    27,    28,    29,    -1,    -1,    52,    33,    -1,
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
      -1,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,
      39,    40,    41,    42,    43,    44,    45,    39,    40,    41,
      42,    43,    44,    45,    26,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,
      42,    43,    44,    45
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     7,    15,    22,    23,    24,    30,
      31,    45,    51,    57,    58,    59,    60,    61,    62,    63,
      67,    69,    70,    71,    73,    79,    85,    30,    74,    74,
      74,    30,    79,    30,    30,    47,    68,    68,    79,    81,
      82,    79,     0,    59,    51,    25,    26,    27,    28,    29,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    68,
      51,    51,    51,    47,    49,    49,     8,     9,    12,    13,
      14,    15,    16,    17,    18,    19,    30,    31,    35,    45,
      49,    51,    67,    69,    70,    71,    80,    85,    88,    89,
      90,    91,    94,    95,    96,    97,    98,    46,    54,    36,
      52,    30,    51,    80,    86,    87,    30,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    79,    79,    79,
      30,    75,    76,    77,    78,    78,    78,    30,    64,    65,
      78,    80,    80,    80,    80,    80,    80,    21,    68,    68,
      80,    83,    84,    69,    80,    96,    98,    99,    20,    26,
      27,    28,    29,    33,    34,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    68,    48,    90,    49,    25,    30,
      68,    79,    68,    53,    36,    52,    49,    54,    50,    52,
      36,    53,    52,    52,    50,    36,    48,    47,    47,    32,
      66,    67,    54,    36,    52,    52,    36,    66,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    80,     6,    32,    55,    67,    70,    71,    72,
     100,   101,    80,    87,    68,    66,    25,    77,    79,    25,
      47,     7,    65,    88,    88,    68,    80,    20,    68,    80,
      98,    54,    51,    68,    50,    68,    68,    68,    25,    66,
      66,    88,    47,    48,    48,    66,    52,    68,    78,    30,
      50,   101,    47,    47,    48,    64,    92,    52,    30,    88,
      88,    48,    10,    11,    93,    47,    48,    48,    80,    47,
      88,    47,    88,    48,    88,    48,    48
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    56,    57,    58,    58,    59,    59,    59,    59,    59,
      59,    59,    59,    60,    61,    62,    63,    64,    64,    65,
      65,    66,    66,    67,    68,    68,    69,    70,    71,    72,
      73,    74,    74,    75,    75,    76,    76,    77,    77,    78,
      79,    79,    79,    79,    79,    79,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    79,    79,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    80,    81,    81,    82,    82,    83,    83,    84,
      84,    85,    86,    86,    86,    87,    87,    88,    88,    89,
      89,    90,    90,    90,    90,    91,    92,    92,    93,    93,
      94,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    96,    97,    97,    98,    98,    99,
      99,   100,   100,   101,   101,   101,   101,   101,   101,   101,
     101
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     3,     3,     5,     1,     3,     1,
       6,     1,     1,     2,     0,     3,    10,    10,     8,     7,
       4,     0,     1,     0,     1,     1,     3,     1,     3,     3,
       1,     2,     5,     4,     4,     2,     3,     3,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       2,     2,     5,     4,     4,     2,     3,     3,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     5,     3,     0,     1,     1,     3,     0,     1,     1,
       3,     4,     0,     1,     3,     1,     3,     0,     1,     1,
       2,     1,     2,     1,     1,     7,     0,     6,     0,     4,
       5,     2,     1,     1,     1,     1,     2,     2,     3,     3,
       1,     1,     1,     1,     3,     1,     5,     1,     1,     1,
       3,     1,     3,     1,     3,     2,     2,     2,     4,     2,
       1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
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


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, astNodePtr *programTree, yyscan_t scanner)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  YYUSE (programTree);
  YYUSE (scanner);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, astNodePtr *programTree, yyscan_t scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yytype, yyvaluep, yylocationp, programTree, scanner);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, astNodePtr *programTree, yyscan_t scanner)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
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
yy_lac_stack_realloc (YYPTRDIFF_T *yycapacity, YYPTRDIFF_T yyadd,
#if YYDEBUG
                      char const *yydebug_prefix,
                      char const *yydebug_suffix,
#endif
                      yy_state_t **yybottom,
                      yy_state_t *yybottom_no_free,
                      yy_state_t **yytop, yy_state_t *yytop_empty)
{
  YYPTRDIFF_T yysize_old =
    *yytop == yytop_empty ? 0 : *yytop - *yybottom + 1;
  YYPTRDIFF_T yysize_new = yysize_old + yyadd;
  if (*yycapacity < yysize_new)
    {
      YYPTRDIFF_T yyalloc = 2 * yysize_new;
      yy_state_t *yybottom_new;
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
        YY_CAST (yy_state_t *,
                 YYSTACK_ALLOC (YY_CAST (YYSIZE_T,
                                         yyalloc * YYSIZEOF (*yybottom_new))));
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
yy_lac (yy_state_t *yyesa, yy_state_t **yyes,
        YYPTRDIFF_T *yyes_capacity, yy_state_t *yyssp, int yytoken)
{
  yy_state_t *yyes_prev = yyssp;
  yy_state_t *yyesp = yyes_prev;
  YYDPRINTF ((stderr, "LAC: checking lookahead %s:", yytname[yytoken]));
  if (yytoken == YYUNDEFTOK)
    {
      YYDPRINTF ((stderr, " Always Err\n"));
      return 1;
    }
  while (1)
    {
      int yyrule = yypact[+*yyesp];
      if (yypact_value_is_default (yyrule)
          || (yyrule += yytoken) < 0 || YYLAST < yyrule
          || yycheck[yyrule] != yytoken)
        {
          yyrule = yydefact[+*yyesp];
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
        YYPTRDIFF_T yylen = yyr2[yyrule];
        YYDPRINTF ((stderr, " R%d", yyrule - 1));
        if (yyesp != yyes_prev)
          {
            YYPTRDIFF_T yysize = yyesp - *yyes + 1;
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
        yy_state_fast_t yystate;
        {
          const int yylhs = yyr1[yyrule] - YYNTOKENS;
          const int yyi = yypgoto[yylhs] + *yyesp;
          yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyesp
                     ? yytable[yyi]
                     : yydefgoto[yylhs]);
        }
        if (yyesp == yyes_prev)
          {
            yyesp = *yyes;
            YY_IGNORE_USELESS_CAST_BEGIN
            *yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_END
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
            YY_IGNORE_USELESS_CAST_BEGIN
            *++yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_END
          }
        YYDPRINTF ((stderr, " G%d", yystate));
      }
    }
}


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
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
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
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
            else
              goto append;

          append:
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

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
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
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyesa, yy_state_t **yyes,
                YYPTRDIFF_T *yyes_capacity, yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

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
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
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
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
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
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
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
          ++yyp;
          ++yyformat;
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

    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

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

    YYPTRDIFF_T yystacksize;

    yy_state_t yyesa[20];
    yy_state_t *yyes;
    YYPTRDIFF_T yyes_capacity;

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
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
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
  yyes_capacity = 20;
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
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  YY_LAC_DISCARD ("shift");
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
| yyreduce -- do a reduction.  |
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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  {
    int yychar_backup = yychar;
    switch (yyn)
      {
  case 2:
#line 71 "parser.y"
                   {*programTree=(yyvsp[0].node);}
#line 2000 "parser.tab.c"
    break;

  case 3:
#line 74 "parser.y"
                             {(yyval.node)=(yyvsp[0].node);}
#line 2006 "parser.tab.c"
    break;

  case 4:
#line 75 "parser.y"
                                  {(yyval.node) = NODE(';',(yyvsp[-1].node),(yyvsp[0].node));}
#line 2012 "parser.tab.c"
    break;

  case 5:
#line 78 "parser.y"
                                {(yyval.node)=(yyvsp[0].node);}
#line 2018 "parser.tab.c"
    break;

  case 6:
#line 79 "parser.y"
                       {(yyval.node)=(yyvsp[0].node);}
#line 2024 "parser.tab.c"
    break;

  case 7:
#line 80 "parser.y"
                       {(yyval.node)=(yyvsp[0].node);}
#line 2030 "parser.tab.c"
    break;

  case 8:
#line 81 "parser.y"
                        {(yyval.node)=(yyvsp[0].node);}
#line 2036 "parser.tab.c"
    break;

  case 9:
#line 82 "parser.y"
                        {(yyval.node)=(yyvsp[0].node);}
#line 2042 "parser.tab.c"
    break;

  case 10:
#line 83 "parser.y"
                      {(yyval.node)=(yyvsp[0].node);}
#line 2048 "parser.tab.c"
    break;

  case 11:
#line 84 "parser.y"
                      {(yyval.node)=(yyvsp[0].node);}
#line 2054 "parser.tab.c"
    break;

  case 12:
#line 85 "parser.y"
                            {(yyval.node)=(yyvsp[0].node);}
#line 2060 "parser.tab.c"
    break;

  case 13:
#line 88 "parser.y"
                                             {(yyval.node)=NODE(MAIN,(yyvsp[-1].node));}
#line 2066 "parser.tab.c"
    break;

  case 14:
#line 91 "parser.y"
                                  {(yyval.node)=NODE(IMPORT,(yyvsp[-1].node));}
#line 2072 "parser.tab.c"
    break;

  case 15:
#line 94 "parser.y"
                                  {(yyval.node)=NODE(EXPORT,(yyvsp[-1].node));}
#line 2078 "parser.tab.c"
    break;

  case 16:
#line 97 "parser.y"
                                                         {(yyval.node)=NODE(STRUCT,(yyvsp[-3].node),(yyvsp[-1].node));}
#line 2084 "parser.tab.c"
    break;

  case 17:
#line 100 "parser.y"
                                {(yyval.node)=(yyvsp[0].node);}
#line 2090 "parser.tab.c"
    break;

  case 18:
#line 101 "parser.y"
                                         {(yyval.node)=NODE(',',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2096 "parser.tab.c"
    break;

  case 19:
#line 104 "parser.y"
                              {(yyval.node)=(yyvsp[0].node);}
#line 2102 "parser.tab.c"
    break;

  case 20:
#line 105 "parser.y"
                                                {(yyval.node)=NODE(STRUCT,(yyvsp[-5].node),(yyvsp[-1].node));}
#line 2108 "parser.tab.c"
    break;

  case 21:
#line 108 "parser.y"
           {(yyval.node)=NODE(TYPE); (yyval.node)->marker=(yyvsp[0].marker);}
#line 2114 "parser.tab.c"
    break;

  case 22:
#line 109 "parser.y"
                   {(yyval.node)=(yyvsp[0].node);}
#line 2120 "parser.tab.c"
    break;

  case 23:
#line 112 "parser.y"
                                   {(yyval.node)=RESOLVE_ATTRIBUTES((yyvsp[-1].node),(yyvsp[0].node));}
#line 2126 "parser.tab.c"
    break;

  case 24:
#line 115 "parser.y"
                 {(yyval.node)=NULL;}
#line 2132 "parser.tab.c"
    break;

  case 25:
#line 116 "parser.y"
                               {(yyval.node)=(yyvsp[-2].node)? NODE('.',(yyvsp[-2].node),(yyvsp[0].node)) : (yyvsp[0].node);}
#line 2138 "parser.tab.c"
    break;

  case 26:
#line 119 "parser.y"
                                                                                                    {(yyval.node)=NODE(SOURCE,(yyvsp[-8].node),(yyvsp[-6].node),(yyvsp[-3].node),(yyvsp[-1].node));}
#line 2144 "parser.tab.c"
    break;

  case 27:
#line 122 "parser.y"
                                                                                              {(yyval.node)=NODE(PIPE,(yyvsp[-8].node),(yyvsp[-6].node),(yyvsp[-3].node),(yyvsp[-1].node));}
#line 2150 "parser.tab.c"
    break;

  case 28:
#line 125 "parser.y"
                                                                                   {(yyval.node)=NODE(SINK,(yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-1].node));}
#line 2156 "parser.tab.c"
    break;

  case 29:
#line 128 "parser.y"
                                                                       {(yyval.node)=NODE(LOCAL,(yyvsp[-4].node),(yyvsp[-1].node));}
#line 2162 "parser.tab.c"
    break;

  case 30:
#line 131 "parser.y"
                                                             {(yyval.node)=NODE(ARROW,(yyvsp[-3].node),(yyvsp[-1].node));}
#line 2168 "parser.tab.c"
    break;

  case 31:
#line 134 "parser.y"
               {(yyval.node)=NULL;}
#line 2174 "parser.tab.c"
    break;

  case 32:
#line 135 "parser.y"
           {(yyval.node)=(yyvsp[0].node);}
#line 2180 "parser.tab.c"
    break;

  case 33:
#line 138 "parser.y"
                   {(yyval.node)=NULL;}
#line 2186 "parser.tab.c"
    break;

  case 34:
#line 139 "parser.y"
               {(yyval.node)=(yyvsp[0].node);}
#line 2192 "parser.tab.c"
    break;

  case 35:
#line 142 "parser.y"
                      {(yyval.node)=(yyvsp[0].node);}
#line 2198 "parser.tab.c"
    break;

  case 36:
#line 143 "parser.y"
                               {(yyval.node)=NODE(',',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2204 "parser.tab.c"
    break;

  case 37:
#line 146 "parser.y"
                             {(yyval.node)=(yyvsp[0].node);}
#line 2210 "parser.tab.c"
    break;

  case 38:
#line 147 "parser.y"
                                                 {(yyval.node)=NODE('=',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2216 "parser.tab.c"
    break;

  case 39:
#line 150 "parser.y"
                               {(yyval.node)=NODE(':',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2222 "parser.tab.c"
    break;

  case 40:
#line 153 "parser.y"
                                     {(yyval.node)=(yyvsp[0].node);}
#line 2228 "parser.tab.c"
    break;

  case 41:
#line 154 "parser.y"
                              {(yyval.node)=RESOLVE_ATTRIBUTES(NODE(LITERAL,(yyvsp[-1].object)),(yyvsp[0].node));}
#line 2234 "parser.tab.c"
    break;

  case 42:
#line 155 "parser.y"
                                                                            {(yyval.node)=RESOLVE_ATTRIBUTES(NODE('[',(yyvsp[-4].node),(yyvsp[-2].node)),(yyvsp[0].node));}
#line 2240 "parser.tab.c"
    break;

  case 43:
#line 156 "parser.y"
                                                        {(yyval.node)=RESOLVE_ATTRIBUTES(NODE('L',(yyvsp[-2].node)),(yyvsp[0].node));}
#line 2246 "parser.tab.c"
    break;

  case 44:
#line 157 "parser.y"
                                                     {(yyval.node)=RESOLVE_ATTRIBUTES((yyvsp[-2].node),(yyvsp[0].node));}
#line 2252 "parser.tab.c"
    break;

  case 45:
#line 158 "parser.y"
                                    {(yyval.node)=RESOLVE_ATTRIBUTES((yyvsp[-1].node),(yyvsp[0].node));}
#line 2258 "parser.tab.c"
    break;

  case 46:
#line 159 "parser.y"
                                                        {(yyval.node)=NODE(AND,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2264 "parser.tab.c"
    break;

  case 47:
#line 160 "parser.y"
                                                       {(yyval.node)=NODE(OR,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2270 "parser.tab.c"
    break;

  case 48:
#line 161 "parser.y"
                                 {(yyval.node)=NODE(NOT,(yyvsp[0].node));}
#line 2276 "parser.tab.c"
    break;

  case 49:
#line 162 "parser.y"
                                                        {(yyval.node)=NODE('+',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2282 "parser.tab.c"
    break;

  case 50:
#line 163 "parser.y"
                                                        {(yyval.node)=NODE('-',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2288 "parser.tab.c"
    break;

  case 51:
#line 164 "parser.y"
                                                        {(yyval.node)=NODE('*',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2294 "parser.tab.c"
    break;

  case 52:
#line 165 "parser.y"
                                                        {(yyval.node)=NODE('/',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2300 "parser.tab.c"
    break;

  case 53:
#line 166 "parser.y"
                                                        {(yyval.node)=NODE('%',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2306 "parser.tab.c"
    break;

  case 54:
#line 167 "parser.y"
                                                        {(yyval.node)=NODE('^',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2312 "parser.tab.c"
    break;

  case 55:
#line 168 "parser.y"
                                                        {(yyval.node)=NODE('|',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2318 "parser.tab.c"
    break;

  case 56:
#line 169 "parser.y"
                                                        {(yyval.node)=NODE('&',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2324 "parser.tab.c"
    break;

  case 57:
#line 170 "parser.y"
                                                           {(yyval.node)=NODE(LSHIFT,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2330 "parser.tab.c"
    break;

  case 58:
#line 171 "parser.y"
                                                           {(yyval.node)=NODE(RSHIFT,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2336 "parser.tab.c"
    break;

  case 59:
#line 174 "parser.y"
                         {(yyval.node)=(yyvsp[0].node);}
#line 2342 "parser.tab.c"
    break;

  case 60:
#line 175 "parser.y"
                              {(yyval.node)=RESOLVE_ATTRIBUTES(NODE(LITERAL,(yyvsp[-1].object)),(yyvsp[0].node));}
#line 2348 "parser.tab.c"
    break;

  case 61:
#line 176 "parser.y"
                              {(yyval.node)=NODE(CONTEXT); (yyval.node)->marker=(yyvsp[-1].marker); if ( (yyvsp[0].node) ) {(yyval.node)=RESOLVE_ATTRIBUTES((yyval.node),(yyvsp[0].node));}}
#line 2354 "parser.tab.c"
    break;

  case 62:
#line 177 "parser.y"
                                                    {(yyval.node)=RESOLVE_ATTRIBUTES(NODE('[',(yyvsp[-4].node),(yyvsp[-2].node)),(yyvsp[0].node));}
#line 2360 "parser.tab.c"
    break;

  case 63:
#line 178 "parser.y"
                                            {(yyval.node)=RESOLVE_ATTRIBUTES(NODE('L',(yyvsp[-2].node)),(yyvsp[0].node));}
#line 2366 "parser.tab.c"
    break;

  case 64:
#line 179 "parser.y"
                                         {(yyval.node)=RESOLVE_ATTRIBUTES((yyvsp[-2].node),(yyvsp[0].node));}
#line 2372 "parser.tab.c"
    break;

  case 65:
#line 180 "parser.y"
                                    {(yyval.node)=RESOLVE_ATTRIBUTES((yyvsp[-1].node),(yyvsp[0].node));}
#line 2378 "parser.tab.c"
    break;

  case 66:
#line 181 "parser.y"
                                {(yyval.node)=NODE(AND,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2384 "parser.tab.c"
    break;

  case 67:
#line 182 "parser.y"
                               {(yyval.node)=NODE(OR,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2390 "parser.tab.c"
    break;

  case 68:
#line 183 "parser.y"
                     {(yyval.node)=NODE(NOT,(yyvsp[0].node));}
#line 2396 "parser.tab.c"
    break;

  case 69:
#line 184 "parser.y"
                                       {(yyval.node)=NODE(COMPARISON,(yyvsp[-2].node),(yyvsp[0].node)); (yyval.node)->marker=(yyvsp[-1].marker);}
#line 2402 "parser.tab.c"
    break;

  case 70:
#line 185 "parser.y"
                                {(yyval.node)=NODE('+',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2408 "parser.tab.c"
    break;

  case 71:
#line 186 "parser.y"
                                {(yyval.node)=NODE('-',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2414 "parser.tab.c"
    break;

  case 72:
#line 187 "parser.y"
                                {(yyval.node)=NODE('*',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2420 "parser.tab.c"
    break;

  case 73:
#line 188 "parser.y"
                                {(yyval.node)=NODE('/',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2426 "parser.tab.c"
    break;

  case 74:
#line 189 "parser.y"
                                {(yyval.node)=NODE('%',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2432 "parser.tab.c"
    break;

  case 75:
#line 190 "parser.y"
                                {(yyval.node)=NODE('^',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2438 "parser.tab.c"
    break;

  case 76:
#line 191 "parser.y"
                                {(yyval.node)=NODE('|',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2444 "parser.tab.c"
    break;

  case 77:
#line 192 "parser.y"
                                {(yyval.node)=NODE('&',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2450 "parser.tab.c"
    break;

  case 78:
#line 193 "parser.y"
                                   {(yyval.node)=NODE(LSHIFT,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2456 "parser.tab.c"
    break;

  case 79:
#line 194 "parser.y"
                                   {(yyval.node)=NODE(RSHIFT,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2462 "parser.tab.c"
    break;

  case 80:
#line 195 "parser.y"
                           {(yyval.node)=NODE(IS,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2468 "parser.tab.c"
    break;

  case 81:
#line 196 "parser.y"
                                 {(yyval.node)=NODE(IS,(yyvsp[-3].node),(yyvsp[0].node));}
#line 2474 "parser.tab.c"
    break;

  case 82:
#line 197 "parser.y"
                              {(yyval.node)=(yyvsp[-1].node);}
#line 2480 "parser.tab.c"
    break;

  case 83:
#line 200 "parser.y"
                           {(yyval.node)=NULL;}
#line 2486 "parser.tab.c"
    break;

  case 84:
#line 201 "parser.y"
                                {(yyval.node)=(yyvsp[0].node);}
#line 2492 "parser.tab.c"
    break;

  case 85:
#line 204 "parser.y"
                                                  {(yyval.node)=(yyvsp[0].node);}
#line 2498 "parser.tab.c"
    break;

  case 86:
#line 205 "parser.y"
                                                           {(yyval.node)=NODE(',',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2504 "parser.tab.c"
    break;

  case 87:
#line 208 "parser.y"
               {(yyval.node)=NULL;}
#line 2510 "parser.tab.c"
    break;

  case 88:
#line 209 "parser.y"
                    {(yyval.node)=(yyvsp[0].node);}
#line 2516 "parser.tab.c"
    break;

  case 89:
#line 212 "parser.y"
                          {(yyval.node)=(yyvsp[0].node);}
#line 2522 "parser.tab.c"
    break;

  case 90:
#line 213 "parser.y"
                                   {(yyval.node)=NODE(',',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2528 "parser.tab.c"
    break;

  case 91:
#line 216 "parser.y"
                                              {(yyval.node)=NODE('C',(yyvsp[-3].node),(yyvsp[-1].node));}
#line 2534 "parser.tab.c"
    break;

  case 92:
#line 219 "parser.y"
           {(yyval.node)=NULL;}
#line 2540 "parser.tab.c"
    break;

  case 93:
#line 220 "parser.y"
               {(yyval.node)=(yyvsp[0].node);}
#line 2546 "parser.tab.c"
    break;

  case 94:
#line 221 "parser.y"
                             {(yyval.node)=NODE(',',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2552 "parser.tab.c"
    break;

  case 95:
#line 224 "parser.y"
                     {(yyval.node)=(yyvsp[0].node);}
#line 2558 "parser.tab.c"
    break;

  case 96:
#line 225 "parser.y"
                          {(yyval.node)=NODE('=',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2564 "parser.tab.c"
    break;

  case 97:
#line 228 "parser.y"
                {(yyval.node)=NULL;}
#line 2570 "parser.tab.c"
    break;

  case 98:
#line 229 "parser.y"
                            {(yyval.node)=(yyvsp[0].node);}
#line 2576 "parser.tab.c"
    break;

  case 99:
#line 232 "parser.y"
                                         {(yyval.node)=(yyvsp[0].node);}
#line 2582 "parser.tab.c"
    break;

  case 100:
#line 233 "parser.y"
                                              {(yyval.node)=(yyvsp[-1].node)? NODE(';',(yyvsp[-1].node),(yyvsp[0].node)) : (yyvsp[0].node);}
#line 2588 "parser.tab.c"
    break;

  case 101:
#line 236 "parser.y"
                       {(yyval.node)=NULL;}
#line 2594 "parser.tab.c"
    break;

  case 102:
#line 237 "parser.y"
                    {(yyval.node)=(yyvsp[-1].node);}
#line 2600 "parser.tab.c"
    break;

  case 103:
#line 238 "parser.y"
                 {(yyval.node)=(yyvsp[0].node);}
#line 2606 "parser.tab.c"
    break;

  case 104:
#line 239 "parser.y"
                    {(yyval.node)=(yyvsp[0].node);}
#line 2612 "parser.tab.c"
    break;

  case 105:
#line 242 "parser.y"
                                                                          {(yyval.node)=NODE(IF,(yyvsp[-5].node),(yyvsp[-3].node),(yyvsp[-1].node),(yyvsp[0].node));}
#line 2618 "parser.tab.c"
    break;

  case 106:
#line 245 "parser.y"
             {(yyval.node)=NULL;}
#line 2624 "parser.tab.c"
    break;

  case 107:
#line 246 "parser.y"
                                                        {(yyval.node)=NODE(EIF,(yyvsp[-5].node),(yyvsp[-3].node),(yyvsp[-1].node));}
#line 2630 "parser.tab.c"
    break;

  case 108:
#line 249 "parser.y"
              {(yyval.node)=NULL;}
#line 2636 "parser.tab.c"
    break;

  case 109:
#line 250 "parser.y"
                                  {(yyval.node)=(yyvsp[-1].node);}
#line 2642 "parser.tab.c"
    break;

  case 110:
#line 253 "parser.y"
                                                       {(yyval.node)=NODE(WHILE,(yyvsp[-3].node),(yyvsp[-1].node));}
#line 2648 "parser.tab.c"
    break;

  case 111:
#line 256 "parser.y"
                           {(yyval.node)=NODE(PROD,(yyvsp[0].node));}
#line 2654 "parser.tab.c"
    break;

  case 112:
#line 257 "parser.y"
           {(yyval.node)=NODE(DROP);}
#line 2660 "parser.tab.c"
    break;

  case 113:
#line 258 "parser.y"
          {(yyval.node)=NODE(END);}
#line 2666 "parser.tab.c"
    break;

  case 114:
#line 259 "parser.y"
           {(yyval.node)=NODE(CONT);}
#line 2672 "parser.tab.c"
    break;

  case 115:
#line 260 "parser.y"
            {(yyval.node)=NODE(BREAK);}
#line 2678 "parser.tab.c"
    break;

  case 116:
#line 261 "parser.y"
                       {(yyval.node)=NODE(ABORT,(yyvsp[0].node));}
#line 2684 "parser.tab.c"
    break;

  case 117:
#line 262 "parser.y"
                        {(yyval.node)=NODE(VERIFY,(yyvsp[0].node));}
#line 2690 "parser.tab.c"
    break;

  case 118:
#line 263 "parser.y"
                   {(yyval.node)=NODE(AS,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2696 "parser.tab.c"
    break;

  case 119:
#line 264 "parser.y"
                                                {(yyval.node)=NODE(OPERATOR_ASSIGNMENT,(yyvsp[-2].node),(yyvsp[0].node)); (yyval.node)->marker=(yyvsp[-1].marker);}
#line 2702 "parser.tab.c"
    break;

  case 120:
#line 265 "parser.y"
                      {(yyval.node)=(yyvsp[0].node);}
#line 2708 "parser.tab.c"
    break;

  case 121:
#line 266 "parser.y"
                        {(yyval.node)=(yyvsp[0].node);}
#line 2714 "parser.tab.c"
    break;

  case 122:
#line 267 "parser.y"
                      {(yyval.node)=(yyvsp[0].node);}
#line 2720 "parser.tab.c"
    break;

  case 123:
#line 268 "parser.y"
                      {(yyval.node)=(yyvsp[0].node);}
#line 2726 "parser.tab.c"
    break;

  case 124:
#line 271 "parser.y"
                                                   {(yyval.node)=NODE(ARROW,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2732 "parser.tab.c"
    break;

  case 125:
#line 274 "parser.y"
                                {(yyval.node)=(yyvsp[0].node);}
#line 2738 "parser.tab.c"
    break;

  case 126:
#line 275 "parser.y"
                                                      {(yyval.node)=NODE(',',(yyvsp[-3].node),(yyvsp[-1].node));}
#line 2744 "parser.tab.c"
    break;

  case 127:
#line 278 "parser.y"
                              {(yyval.node)=(yyvsp[0].node);}
#line 2750 "parser.tab.c"
    break;

  case 128:
#line 279 "parser.y"
                        {(yyval.node)=(yyvsp[0].node);}
#line 2756 "parser.tab.c"
    break;

  case 129:
#line 282 "parser.y"
                                     {(yyval.node)=(yyvsp[0].node);}
#line 2762 "parser.tab.c"
    break;

  case 130:
#line 283 "parser.y"
                                              {(yyval.node)=NODE(',',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2768 "parser.tab.c"
    break;

  case 131:
#line 286 "parser.y"
                                    {(yyval.node)=(yyvsp[0].node);}
#line 2774 "parser.tab.c"
    break;

  case 132:
#line 287 "parser.y"
                                               {(yyval.node)=NODE(ARROW,(yyvsp[-2].node),(yyvsp[0].node));}
#line 2780 "parser.tab.c"
    break;

  case 133:
#line 290 "parser.y"
                                  {(yyval.node)=(yyvsp[0].node);}
#line 2786 "parser.tab.c"
    break;

  case 134:
#line 291 "parser.y"
                            {(yyval.node)=NODE(':',(yyvsp[-2].node),(yyvsp[0].node));}
#line 2792 "parser.tab.c"
    break;

  case 135:
#line 292 "parser.y"
                           {(yyval.node)=NODE(TYPE); (yyval.node)->marker=(yyvsp[-1].marker); (yyval.node)=RESOLVE_ATTRIBUTES((yyval.node),(yyvsp[0].node));}
#line 2798 "parser.tab.c"
    break;

  case 136:
#line 293 "parser.y"
                                      {(yyval.node)=RESOLVE_ATTRIBUTES((yyvsp[-1].node),(yyvsp[0].node));}
#line 2804 "parser.tab.c"
    break;

  case 137:
#line 294 "parser.y"
                                      {(yyval.node)=RESOLVE_ATTRIBUTES((yyvsp[-1].node),(yyvsp[0].node));}
#line 2810 "parser.tab.c"
    break;

  case 138:
#line 295 "parser.y"
                                               {(yyval.node)=NODE(':',RESOLVE_ATTRIBUTES((yyvsp[-3].node),(yyvsp[-2].node)),(yyvsp[0].node));}
#line 2816 "parser.tab.c"
    break;

  case 139:
#line 296 "parser.y"
                                       {(yyval.node)=RESOLVE_ATTRIBUTES((yyvsp[-1].node),(yyvsp[0].node));}
#line 2822 "parser.tab.c"
    break;

  case 140:
#line 297 "parser.y"
          {(yyval.node)=NULL;}
#line 2828 "parser.tab.c"
    break;


#line 2832 "parser.tab.c"

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
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

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
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

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


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
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
                  yystos[+*yyssp], yyvsp, yylsp, programTree, scanner);
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
#line 300 "parser.y"


void yyerror(YYLTYPE *yyllocp, yyscan_t yyscanner, const char *format, ...) {
    SETUP_SCANNER;
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
