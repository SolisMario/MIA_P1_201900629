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
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <string.h>

#include "scanner.h"
#include "mkdisk.h"
#include "rmdisk.h"
#include "fdisk.h"
#include "mount.h"
#include "rep.h"
#include "mkfs.h"
#include "touch.h"
#include "mkdir.h"
#include "exec.h"
#include "cat.h"
#include "ren.h"
#include "mv.h"
#include "rm.h"
#include "edit.h"
#include "cp.h"
#include "find.h"
#include "users.h"
#include "simulate.h"

using namespace std;
extern int yylex(void);
extern char *yytext;
extern FILE *yyin;
extern void yyerror(const char *s);

MKDISK * mkdiskV;
RMDISK * rmdiskV;
FDISK * fdiskV;
MOUNT * mountV;
REP * repV;
MKFS * mkfsV;
TOUCH * touchV;
MKDIR * mkdirV;
EXEC * execV;
CAT * catV;
REN * renV;
MV * moveV;
RM * rmV;
EDIT * editV;
CP * cpV;
FIND * findV;
USERS * usersV;
SIMULATE * simulateV;

#line 123 "parser.cpp"

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
#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
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
    numero = 258,
    comentarioSS = 259,
    cadena = 260,
    guion = 261,
    igual = 262,
    mkdisk = 263,
    rfind = 264,
    size = 265,
    fit = 266,
    unit = 267,
    rp = 268,
    rr = 269,
    path = 270,
    rmdisk = 271,
    fdisk = 272,
    rtype = 273,
    rdelete = 274,
    rname = 275,
    radd = 276,
    rmount = 277,
    rumount = 278,
    rmkfs = 279,
    id = 280,
    rfs = 281,
    ruta = 282,
    rep = 283,
    rid = 284,
    retruta = 285,
    root = 286,
    id_particion = 287,
    rmkdir = 288,
    touch = 289,
    rcont = 290,
    rstdin = 291,
    rexec = 292,
    rcat = 293,
    rren = 294,
    rmv = 295,
    rdest = 296,
    rrm = 297,
    redit = 298,
    rpause = 299,
    rcp = 300,
    rpwd = 301,
    ruser = 302,
    rlogin = 303,
    rlogout = 304,
    rmkgrp = 305,
    rrmgrp = 306,
    rmkusr = 307,
    rusr = 308,
    rgrp = 309,
    rrmusr = 310,
    rloss = 311,
    rrecovery = 312,
    rchgrp = 313
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 162 "parser.y"

    char*STRING;
    char*NUM;

#line 239 "parser.cpp"

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


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */



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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


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
#define YYFINAL  56
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   362

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  72
/* YYNRULES -- Number of rules.  */
#define YYNRULES  190
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  348

#define YYUNDEFTOK  2
#define YYMAXUTOK   313


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
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   170,   170,   174,   175,   179,   179,   180,   180,   181,
     181,   182,   182,   183,   183,   184,   184,   185,   185,   186,
     186,   187,   187,   188,   188,   189,   189,   190,   190,   191,
     191,   192,   192,   193,   193,   194,   194,   195,   196,   196,
     197,   197,   198,   199,   199,   200,   200,   201,   201,   202,
     202,   203,   203,   204,   204,   205,   205,   209,   210,   214,
     215,   216,   217,   218,   222,   223,   227,   228,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   246,
     247,   251,   252,   253,   254,   258,   262,   263,   267,   268,
     269,   270,   271,   272,   273,   277,   278,   282,   283,   284,
     288,   289,   293,   294,   295,   296,   297,   298,   299,   303,
     304,   308,   309,   310,   314,   315,   319,   320,   324,   325,
     329,   330,   333,   334,   335,   339,   340,   343,   344,   345,
     346,   350,   351,   355,   356,   360,   361,   362,   363,   364,
     368,   369,   372,   373,   374,   375,   379,   380,   384,   385,
     386,   390,   391,   395,   396,   397,   398,   399,   400,   401,
     405,   406,   410,   411,   415,   416,   420,   421,   425,   426,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   442,
     443,   447,   448,   452,   456,   457,   461,   462,   463,   464,
     465
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "numero", "comentarioSS", "cadena",
  "guion", "igual", "mkdisk", "rfind", "size", "fit", "unit", "rp", "rr",
  "path", "rmdisk", "fdisk", "rtype", "rdelete", "rname", "radd", "rmount",
  "rumount", "rmkfs", "id", "rfs", "ruta", "rep", "rid", "retruta", "root",
  "id_particion", "rmkdir", "touch", "rcont", "rstdin", "rexec", "rcat",
  "rren", "rmv", "rdest", "rrm", "redit", "rpause", "rcp", "rpwd", "ruser",
  "rlogin", "rlogout", "rmkgrp", "rrmgrp", "rmkusr", "rusr", "rgrp",
  "rrmusr", "rloss", "rrecovery", "rchgrp", "$accept", "INI",
  "INSTRUCCIONES", "INSTRUCCION", "$@1", "$@2", "$@3", "$@4", "$@5", "$@6",
  "$@7", "$@8", "$@9", "$@10", "$@11", "$@12", "$@13", "$@14", "$@15",
  "$@16", "$@17", "$@18", "$@19", "$@20", "$@21", "$@22", "$@23", "$@24",
  "$@25", "MKDISKPARAMS", "MKDISKPAR", "RMDISKPARAMS", "FDISKPARAMS",
  "FDISKPAR", "MOUNTPARAMS", "MOUNTPARAM", "UMOUNTPARAM", "REPPARAMS",
  "REPPARAM", "MKFSPARAMS", "MKFSPARAM", "TOUCHPARAMS", "TOUCHPARAM",
  "MKDIRPARAMS", "MKDIRPARAM", "EXECPARAM", "CATPARAMS", "CATPARAM",
  "RENPARAMS", "RENPARAM", "MVPARAMS", "MVPARAM", "RMPARAM", "EDITPARAMS",
  "EDITPARAM", "CPPARAMS", "CPPARAM", "FINDPARAMS", "FINDPARAM",
  "LOGINPARAMS", "LOGINPARAM", "MKGRPPARAMS", "MKGRPPARAM", "RMGRPPARAMS",
  "RMGRPPARAM", "MKUSERPARAMS", "MKUSERPARAM", "RMUSERPARAMS",
  "RMUSERPARAM", "SIMULATEPARAM", "CHGRPPARAMS", "CHGRPPARAM", YY_NULLPTR
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
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313
};
# endif

#define YYPACT_NINF (-43)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -8,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,
     -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,
     -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   114,    -8,
     -43,     0,    50,   125,   141,   144,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   163,   164,   -43,   -43,   134,     0,
     -43,   -10,    50,   -43,   165,   -43,   118,   141,   -43,    92,
     144,   -43,    26,   -43,   122,   147,   -43,   112,   148,   -43,
       6,   149,   -43,     3,   150,   -43,   166,   -43,   167,   152,
     -43,    93,   153,   -43,    36,   154,   -43,   168,   -43,   -12,
     156,   -43,    37,   157,   -43,    71,   158,   -43,   169,   159,
     -43,   170,   160,   -43,   -42,   161,   -43,   119,   162,   -43,
     142,   -43,   -43,    81,   164,   -43,   171,   172,   175,   177,
     -43,   178,   179,   -43,   180,   181,   184,   186,   187,   188,
     189,   190,   191,   -43,   192,   193,   -43,   194,   195,   196,
     197,   -43,   198,   199,   200,   201,   202,   -43,   -43,   203,
     -43,   204,   -43,   205,   206,   -43,   -43,   207,   208,   -43,
     209,   210,   -43,   211,   212,   -43,   213,   214,   215,   -43,
     -43,   216,   217,   -43,   218,   219,   220,   -43,   221,   -43,
     222,   -43,   223,   224,   225,   -43,   226,   -43,   227,   228,
     229,   -43,   173,   230,   231,    48,    49,   232,    52,   174,
     233,   234,    53,    86,   235,    96,   236,    54,    97,   143,
     237,   238,   239,    55,   240,   241,    56,   242,    57,   243,
      58,    59,    62,    63,    64,   244,    65,    68,    69,    82,
      83,    88,    89,   245,    94,    -3,    98,    99,    95,     2,
      40,   100,   246,    41,   101,   -43,   -43,   -43,   -43,   -43,
     -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,
     -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,
     -43,   -43,   248,   -43,   -43,   -43,   -43,   -43,   -43,   -43,
     -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,
     -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,
     -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,
     247,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   250,
     -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,
     -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     5,    38,     7,     9,    11,    13,    17,    15,    21,
      19,    23,    25,    27,    29,    31,    33,    37,    35,    40,
      42,    43,    45,    49,    47,    53,    55,    51,     0,     2,
       4,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     1,     3,     0,     6,
      58,     0,    39,   147,     0,     8,     0,    10,    67,     0,
      12,    80,     0,    14,     0,    18,    96,     0,    16,    87,
       0,    22,   110,     0,    20,   101,     0,    24,     0,    26,
     117,     0,    28,   121,     0,    30,   126,     0,    32,     0,
      34,   134,     0,    36,   141,     0,    41,   152,     0,    44,
     161,     0,    46,   165,     0,    50,   169,     0,    48,   180,
       0,    54,    56,     0,    52,   185,     0,     0,     0,     0,
      57,     0,     0,   146,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,     0,    79,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,    86,   113,     0,
     109,     0,   104,     0,     0,   108,   100,     0,     0,   116,
       0,     0,   120,     0,     0,   125,     0,     0,     0,   139,
     133,     0,     0,   140,     0,     0,     0,   151,     0,   160,
       0,   164,     0,     0,     0,   168,     0,   179,     0,     0,
       0,   184,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,    60,    61,    63,    62,
     149,   148,   150,    65,    64,    68,    74,    69,    71,    70,
      73,    72,    75,    77,    76,    78,    82,    81,    84,    83,
      85,    98,     0,    97,    90,    89,    88,    91,    93,    92,
      94,   112,   111,   105,   103,   102,   107,   106,   115,   114,
     119,   118,   123,   122,   124,   128,   127,   130,   129,   132,
     131,   136,   135,   138,   137,   143,   142,   145,   144,   159,
     158,   156,   155,   153,   154,   163,   162,   167,   166,   178,
     176,   175,   173,   174,   172,   170,   171,   182,   181,   183,
     190,   188,   189,   187,   186,    99,   157,   177
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -43,   -43,   -43,   145,   -43,   -43,   -43,   -43,   -43,   -43,
     -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,
     -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,   -43,
     183,   -43,   -43,   106,   -43,   182,   -43,   -43,   176,   -43,
     249,   -43,   251,   -43,   185,   -43,   -43,   252,   -43,   253,
     -43,   254,   -43,   -43,   138,   -43,   137,   -43,   255,   -43,
     256,   -43,   135,   -43,   131,   -43,   132,   -43,   130,   257,
     -43,   126
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    28,    29,    30,    31,    33,    34,    35,    36,    38,
      37,    40,    39,    41,    42,    43,    44,    45,    46,    47,
      32,    48,    49,    50,    52,    51,    55,    53,    54,    59,
      60,    65,    67,    68,    70,    71,    73,    78,    79,    75,
      76,    84,    85,    81,    82,    87,    89,    90,    92,    93,
      95,    96,    98,   100,   101,   103,   104,    62,    63,   106,
     107,   109,   110,   112,   113,   115,   116,   118,   119,   121,
     124,   125
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
       1,     2,   322,   177,   192,   131,    58,   331,     3,     4,
     132,   193,   194,   161,     5,     6,     7,   162,   163,   158,
       8,   159,   323,   178,   179,     9,    10,   332,   324,    11,
      12,    13,    14,   333,    15,    16,    17,    18,   164,   165,
      19,    20,    21,    22,    23,   334,   340,    24,    25,    26,
      27,   173,   181,   258,   260,   147,    61,   263,   268,   276,
     284,   288,   291,   294,   296,   335,   341,   298,   300,   302,
     305,   336,   342,   307,   309,   259,   261,   174,   182,   264,
     269,   277,   285,   289,   292,   295,   297,   311,   313,   299,
     301,   303,   306,   315,   317,   308,   310,   320,   329,   270,
     184,   273,   278,   325,   327,   337,   343,   144,   170,   312,
     314,   271,   145,   171,    56,   316,   318,   185,   186,   321,
     330,   274,   279,   326,   328,   338,   344,   152,   135,   136,
     137,    64,   153,   138,   199,   200,   139,   140,   141,   142,
     148,   154,   155,   156,   126,   127,   128,    66,   149,   129,
      69,   150,    72,    74,    77,    80,    83,    86,    88,    91,
      94,    97,    99,   102,   105,   108,   111,   114,   117,   120,
     123,   198,   196,   143,    57,   280,   255,   265,   202,   203,
     134,   167,   204,   176,   205,   206,   207,   208,   209,   188,
     190,   210,   168,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   262,   180,   275,
     183,   282,   130,   191,   189,   290,   293,   195,   197,     0,
     201,     0,   146,     0,   157,   256,   257,     0,   266,   267,
     272,     0,   281,     0,     0,   286,   160,     0,     0,   304,
       0,   283,   346,   287,   345,   347,     0,   319,   339,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   122,     0,     0,     0,     0,     0,   133,     0,     0,
       0,     0,     0,     0,   151,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   166,     0,     0,     0,     0,
       0,   169,     0,     0,     0,   172,     0,     0,     0,   175,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   187
};

static const yytype_int8 yycheck[] =
{
       8,     9,     5,    15,    46,    15,     6,     5,    16,    17,
      20,    53,    54,    10,    22,    23,    24,    14,    15,    13,
      28,    15,    25,    35,    36,    33,    34,    25,    31,    37,
      38,    39,    40,    31,    42,    43,    44,    45,    35,    36,
      48,    49,    50,    51,    52,     5,     5,    55,    56,    57,
      58,    15,    15,     5,     5,    29,     6,     5,     5,     5,
       5,     5,     5,     5,     5,    25,    25,     5,     5,     5,
       5,    31,    31,     5,     5,    27,    27,    41,    41,    27,
      27,    27,    27,    27,    27,    27,    27,     5,     5,    27,
      27,    27,    27,     5,     5,    27,    27,     3,     3,    13,
      29,     5,     5,     5,     5,     5,     5,    15,    15,    27,
      27,    25,    20,    20,     0,    27,    27,    46,    47,    25,
      25,    25,    25,    25,    25,    25,    25,    15,    10,    11,
      12,     6,    20,    15,    53,    54,    18,    19,    20,    21,
      18,    29,    30,    31,    10,    11,    12,     6,    26,    15,
       6,    29,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,    29,    53,    67,    29,    32,     3,     3,     7,     7,
      15,    15,     7,    15,     7,     7,     7,     7,     7,    20,
      20,     7,    25,     7,     7,     7,     7,     7,     7,     7,
       7,     7,     7,     7,     7,     7,     7,     7,     7,     7,
       7,     7,     7,     7,     7,     7,     7,     7,     7,     7,
       7,     7,     7,     7,     7,     7,     7,     7,     7,     7,
       7,     7,     7,     7,     7,     7,     7,     5,   100,     3,
     103,     3,    59,   112,   109,     3,     3,   115,   118,    -1,
     124,    -1,    70,    -1,    78,    25,    25,    -1,    25,    25,
      25,    -1,    25,    -1,    -1,    25,    81,    -1,    -1,    25,
      -1,    32,    25,    32,    26,    25,    -1,    32,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    92,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     8,     9,    16,    17,    22,    23,    24,    28,    33,
      34,    37,    38,    39,    40,    42,    43,    44,    45,    48,
      49,    50,    51,    52,    55,    56,    57,    58,    60,    61,
      62,    63,    79,    64,    65,    66,    67,    69,    68,    71,
      70,    72,    73,    74,    75,    76,    77,    78,    80,    81,
      82,    84,    83,    86,    87,    85,     0,    62,     6,    88,
      89,     6,   116,   117,     6,    90,     6,    91,    92,     6,
      93,    94,     6,    95,     6,    98,    99,     6,    96,    97,
       6,   102,   103,     6,   100,   101,     6,   104,     6,   105,
     106,     6,   107,   108,     6,   109,   110,     6,   111,     6,
     112,   113,     6,   114,   115,     6,   118,   119,     6,   120,
     121,     6,   122,   123,     6,   124,   125,     6,   126,   127,
       6,   128,   128,     6,   129,   130,    10,    11,    12,    15,
      89,    15,    20,   117,    15,    10,    11,    12,    15,    18,
      19,    20,    21,    92,    15,    20,    94,    29,    18,    26,
      29,    99,    15,    20,    29,    30,    31,    97,    13,    15,
     103,    10,    14,    15,    35,    36,   101,    15,    25,   106,
      15,    20,   108,    15,    41,   110,    15,    15,    35,    36,
     113,    15,    41,   115,    29,    46,    47,   119,    20,   121,
      20,   123,    46,    53,    54,   125,    53,   127,    29,    53,
      54,   130,     7,     7,     7,     7,     7,     7,     7,     7,
       7,     7,     7,     7,     7,     7,     7,     7,     7,     7,
       7,     7,     7,     7,     7,     7,     7,     7,     7,     7,
       7,     7,     7,     7,     7,     7,     7,     7,     7,     7,
       7,     7,     7,     7,     7,     7,     7,     7,     7,     7,
       7,     7,     7,     7,     7,     3,    25,    25,     5,    27,
       5,    27,     5,     5,    27,     3,    25,    25,     5,    27,
      13,    25,    25,     5,    25,     3,     5,    27,     5,    25,
      32,    25,     3,    32,     5,    27,    25,    32,     5,    27,
       3,     5,    27,     3,     5,    27,     5,    27,     5,    27,
       5,    27,     5,    27,    25,     5,    27,     5,    27,     5,
      27,     5,    27,     5,    27,     5,    27,     5,    27,    32,
       3,    25,     5,    25,    31,     5,    25,     5,    25,     3,
      25,     5,    25,    31,     5,    25,    31,     5,    25,    32,
       5,    25,    31,     5,    25,    26,    25,    25
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    59,    60,    61,    61,    63,    62,    64,    62,    65,
      62,    66,    62,    67,    62,    68,    62,    69,    62,    70,
      62,    71,    62,    72,    62,    73,    62,    74,    62,    75,
      62,    76,    62,    77,    62,    78,    62,    62,    79,    62,
      80,    62,    62,    81,    62,    82,    62,    83,    62,    84,
      62,    85,    62,    86,    62,    87,    62,    88,    88,    89,
      89,    89,    89,    89,    90,    90,    91,    91,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    93,
      93,    94,    94,    94,    94,    95,    96,    96,    97,    97,
      97,    97,    97,    97,    97,    98,    98,    99,    99,    99,
     100,   100,   101,   101,   101,   101,   101,   101,   101,   102,
     102,   103,   103,   103,   104,   104,   105,   105,   106,   106,
     107,   107,   108,   108,   108,   109,   109,   110,   110,   110,
     110,   111,   111,   112,   112,   113,   113,   113,   113,   113,
     114,   114,   115,   115,   115,   115,   116,   116,   117,   117,
     117,   118,   118,   119,   119,   119,   119,   119,   119,   119,
     120,   120,   121,   121,   122,   122,   123,   123,   124,   124,
     125,   125,   125,   125,   125,   125,   125,   125,   125,   126,
     126,   127,   127,   128,   129,   129,   130,   130,   130,   130,
     130
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     1,     0,     3,
       0,     3,     1,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     2,     1,     4,
       4,     4,     4,     4,     4,     4,     2,     1,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     2,
       1,     4,     4,     4,     4,     4,     2,     1,     4,     4,
       4,     4,     4,     4,     4,     2,     1,     4,     4,     5,
       2,     1,     4,     4,     2,     4,     4,     4,     2,     2,
       1,     4,     4,     2,     4,     4,     2,     1,     4,     4,
       2,     1,     4,     4,     4,     2,     1,     4,     4,     4,
       4,     4,     4,     2,     1,     4,     4,     4,     4,     2,
       2,     1,     4,     4,     4,     4,     2,     1,     4,     4,
       4,     2,     1,     4,     4,     4,     4,     5,     4,     4,
       2,     1,     4,     4,     2,     1,     4,     4,     2,     1,
       4,     4,     4,     4,     4,     4,     4,     5,     4,     2,
       1,     4,     4,     4,     2,     1,     4,     4,     4,     4,
       4
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
                  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
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
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yytype, yyvaluep, yylocationp);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
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
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
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
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
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
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
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
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
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
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
  switch (yyn)
    {
  case 5:
#line 179 "parser.y"
               {mkdiskV = new MKDISK();}
#line 1787 "parser.cpp"
    break;

  case 6:
#line 179 "parser.y"
                                                      {mkdiskV->mkdisk();/*realiza la creación del disco*/}
#line 1793 "parser.cpp"
    break;

  case 7:
#line 180 "parser.y"
                 {rmdiskV = new RMDISK();}
#line 1799 "parser.cpp"
    break;

  case 8:
#line 180 "parser.y"
                                                        {rmdiskV->rmdisk();}
#line 1805 "parser.cpp"
    break;

  case 9:
#line 181 "parser.y"
                {fdiskV = new FDISK();}
#line 1811 "parser.cpp"
    break;

  case 10:
#line 181 "parser.y"
                                                    {fdiskV->fdisk();}
#line 1817 "parser.cpp"
    break;

  case 11:
#line 182 "parser.y"
                 {mountV = new MOUNT();}
#line 1823 "parser.cpp"
    break;

  case 12:
#line 182 "parser.y"
                                                     {mountV->mount();}
#line 1829 "parser.cpp"
    break;

  case 13:
#line 183 "parser.y"
                  {mountV = new MOUNT();}
#line 1835 "parser.cpp"
    break;

  case 14:
#line 183 "parser.y"
                                                      {mountV->umount();}
#line 1841 "parser.cpp"
    break;

  case 15:
#line 184 "parser.y"
              {repV = new REP();}
#line 1847 "parser.cpp"
    break;

  case 16:
#line 184 "parser.y"
                                            {repV->rep();}
#line 1853 "parser.cpp"
    break;

  case 17:
#line 185 "parser.y"
                {mkfsV = new MKFS();}
#line 1859 "parser.cpp"
    break;

  case 18:
#line 185 "parser.y"
                                                 {mkfsV->mkfs();}
#line 1865 "parser.cpp"
    break;

  case 19:
#line 186 "parser.y"
                {touchV = new TOUCH();}
#line 1871 "parser.cpp"
    break;

  case 20:
#line 186 "parser.y"
                                                    {touchV->touch();}
#line 1877 "parser.cpp"
    break;

  case 21:
#line 187 "parser.y"
                 {mkdirV = new MKDIR();}
#line 1883 "parser.cpp"
    break;

  case 22:
#line 187 "parser.y"
                                                     {mkdirV->mkdir();}
#line 1889 "parser.cpp"
    break;

  case 23:
#line 188 "parser.y"
                {execV = new EXEC();}
#line 1895 "parser.cpp"
    break;

  case 24:
#line 188 "parser.y"
                                                {execV->exec();}
#line 1901 "parser.cpp"
    break;

  case 25:
#line 189 "parser.y"
               {catV = new CAT();}
#line 1907 "parser.cpp"
    break;

  case 26:
#line 189 "parser.y"
                                             {catV->cat();}
#line 1913 "parser.cpp"
    break;

  case 27:
#line 190 "parser.y"
               {renV = new REN();}
#line 1919 "parser.cpp"
    break;

  case 28:
#line 190 "parser.y"
                                             {renV->ren();}
#line 1925 "parser.cpp"
    break;

  case 29:
#line 191 "parser.y"
              {moveV = new MV();}
#line 1931 "parser.cpp"
    break;

  case 30:
#line 191 "parser.y"
                                           {moveV->mv();}
#line 1937 "parser.cpp"
    break;

  case 31:
#line 192 "parser.y"
              {rmV = new RM();}
#line 1943 "parser.cpp"
    break;

  case 32:
#line 192 "parser.y"
                                        {rmV->rm();}
#line 1949 "parser.cpp"
    break;

  case 33:
#line 193 "parser.y"
                {editV = new EDIT();}
#line 1955 "parser.cpp"
    break;

  case 34:
#line 193 "parser.y"
                                                 {editV->edit();}
#line 1961 "parser.cpp"
    break;

  case 35:
#line 194 "parser.y"
              {cpV = new CP();}
#line 1967 "parser.cpp"
    break;

  case 36:
#line 194 "parser.y"
                                         {cpV->cp();}
#line 1973 "parser.cpp"
    break;

  case 37:
#line 195 "parser.y"
                 {system("read -p 'Presione Enter para continuar...' var");}
#line 1979 "parser.cpp"
    break;

  case 38:
#line 196 "parser.y"
                {findV = new FIND();}
#line 1985 "parser.cpp"
    break;

  case 39:
#line 196 "parser.y"
                                                 {findV->find();}
#line 1991 "parser.cpp"
    break;

  case 40:
#line 197 "parser.y"
                 {usersV = new USERS();}
#line 1997 "parser.cpp"
    break;

  case 41:
#line 197 "parser.y"
                                                     {usersV->login();}
#line 2003 "parser.cpp"
    break;

  case 42:
#line 198 "parser.y"
                  {usersV = new USERS(); usersV->logout();}
#line 2009 "parser.cpp"
    break;

  case 43:
#line 199 "parser.y"
                 {usersV = new USERS();}
#line 2015 "parser.cpp"
    break;

  case 44:
#line 199 "parser.y"
                                                     {usersV->mkgrp();}
#line 2021 "parser.cpp"
    break;

  case 45:
#line 200 "parser.y"
                 {usersV = new USERS();}
#line 2027 "parser.cpp"
    break;

  case 46:
#line 200 "parser.y"
                                                     {usersV->rmgrp();}
#line 2033 "parser.cpp"
    break;

  case 47:
#line 201 "parser.y"
                 {usersV = new USERS();}
#line 2039 "parser.cpp"
    break;

  case 48:
#line 201 "parser.y"
                                                      {usersV->rmusr();}
#line 2045 "parser.cpp"
    break;

  case 49:
#line 202 "parser.y"
                 {usersV = new USERS();}
#line 2051 "parser.cpp"
    break;

  case 50:
#line 202 "parser.y"
                                                      {usersV->mkusr();}
#line 2057 "parser.cpp"
    break;

  case 51:
#line 203 "parser.y"
                 {usersV = new USERS();}
#line 2063 "parser.cpp"
    break;

  case 52:
#line 203 "parser.y"
                                                     {usersV->chgrp();}
#line 2069 "parser.cpp"
    break;

  case 53:
#line 204 "parser.y"
                {simulateV = new SIMULATE();}
#line 2075 "parser.cpp"
    break;

  case 54:
#line 204 "parser.y"
                                                            {simulateV->loss();}
#line 2081 "parser.cpp"
    break;

  case 55:
#line 205 "parser.y"
                    {simulateV = new SIMULATE();}
#line 2087 "parser.cpp"
    break;

  case 56:
#line 205 "parser.y"
                                                                {simulateV->recovery();}
#line 2093 "parser.cpp"
    break;

  case 59:
#line 214 "parser.y"
                                {mkdiskV->setSize(atoi((yyvsp[0].NUM)));}
#line 2099 "parser.cpp"
    break;

  case 60:
#line 215 "parser.y"
                             {mkdiskV->setFit((yyvsp[0].STRING));}
#line 2105 "parser.cpp"
    break;

  case 61:
#line 216 "parser.y"
                              {mkdiskV->setUnit((yyvsp[0].STRING));}
#line 2111 "parser.cpp"
    break;

  case 62:
#line 217 "parser.y"
                                {mkdiskV->setPath(false, (yyvsp[0].STRING));}
#line 2117 "parser.cpp"
    break;

  case 63:
#line 218 "parser.y"
                                  {mkdiskV->setPath(true, (yyvsp[0].STRING));}
#line 2123 "parser.cpp"
    break;

  case 64:
#line 222 "parser.y"
                              {rmdiskV->setPath(false, (yyvsp[0].STRING));}
#line 2129 "parser.cpp"
    break;

  case 65:
#line 223 "parser.y"
                                  {rmdiskV->setPath(true, (yyvsp[0].STRING));}
#line 2135 "parser.cpp"
    break;

  case 68:
#line 232 "parser.y"
                                {fdiskV->setSize(atoi((yyvsp[0].NUM)));}
#line 2141 "parser.cpp"
    break;

  case 69:
#line 233 "parser.y"
                              {fdiskV->setUnit((yyvsp[0].STRING));}
#line 2147 "parser.cpp"
    break;

  case 70:
#line 234 "parser.y"
                                {fdiskV->setPath(false, (yyvsp[0].STRING));}
#line 2153 "parser.cpp"
    break;

  case 71:
#line 235 "parser.y"
                                  {fdiskV->setPath(true, (yyvsp[0].STRING));}
#line 2159 "parser.cpp"
    break;

  case 72:
#line 236 "parser.y"
                               {fdiskV->setType((yyvsp[0].STRING));}
#line 2165 "parser.cpp"
    break;

  case 73:
#line 237 "parser.y"
                               {fdiskV->setType((yyvsp[0].STRING));}
#line 2171 "parser.cpp"
    break;

  case 74:
#line 238 "parser.y"
                             {fdiskV->setFit((yyvsp[0].STRING));}
#line 2177 "parser.cpp"
    break;

  case 75:
#line 239 "parser.y"
                                 {fdiskV->setDelete((yyvsp[0].STRING));}
#line 2183 "parser.cpp"
    break;

  case 76:
#line 240 "parser.y"
                               {fdiskV->setName(false, (yyvsp[0].STRING));}
#line 2189 "parser.cpp"
    break;

  case 77:
#line 241 "parser.y"
                                   {fdiskV->setName(true, (yyvsp[0].STRING));}
#line 2195 "parser.cpp"
    break;

  case 78:
#line 242 "parser.y"
                                  {fdiskV->setAdd(atoi((yyvsp[0].NUM)));}
#line 2201 "parser.cpp"
    break;

  case 81:
#line 251 "parser.y"
                              {mountV->setPath(false, (yyvsp[0].STRING));}
#line 2207 "parser.cpp"
    break;

  case 82:
#line 252 "parser.y"
                                  {mountV->setPath(true, (yyvsp[0].STRING));}
#line 2213 "parser.cpp"
    break;

  case 83:
#line 253 "parser.y"
                               {mountV->setName(false, (yyvsp[0].STRING));}
#line 2219 "parser.cpp"
    break;

  case 84:
#line 254 "parser.y"
                                   {mountV->setName(true, (yyvsp[0].STRING));}
#line 2225 "parser.cpp"
    break;

  case 85:
#line 258 "parser.y"
                                     {mountV->setID((yyvsp[0].STRING));}
#line 2231 "parser.cpp"
    break;

  case 88:
#line 267 "parser.y"
                             {repV->setName((yyvsp[0].STRING));}
#line 2237 "parser.cpp"
    break;

  case 89:
#line 268 "parser.y"
                                {repV->setPath(false, (yyvsp[0].STRING));}
#line 2243 "parser.cpp"
    break;

  case 90:
#line 269 "parser.y"
                                  {repV->setPath(true, (yyvsp[0].STRING));}
#line 2249 "parser.cpp"
    break;

  case 91:
#line 270 "parser.y"
                                       {repV->setID((yyvsp[0].STRING));}
#line 2255 "parser.cpp"
    break;

  case 92:
#line 271 "parser.y"
                                   {repV->setRuta(false, (yyvsp[0].STRING));}
#line 2261 "parser.cpp"
    break;

  case 93:
#line 272 "parser.y"
                                     {repV->setRuta(true, (yyvsp[0].STRING));}
#line 2267 "parser.cpp"
    break;

  case 94:
#line 273 "parser.y"
                                  {repV->setRoot(atoi((yyvsp[0].NUM)));}
#line 2273 "parser.cpp"
    break;

  case 97:
#line 282 "parser.y"
                                     {mkfsV->set_id((yyvsp[0].STRING));}
#line 2279 "parser.cpp"
    break;

  case 98:
#line 283 "parser.y"
                               {mkfsV->set_type((yyvsp[0].STRING));}
#line 2285 "parser.cpp"
    break;

  case 99:
#line 284 "parser.y"
                                     {mkfsV->set_fs(atoi((yyvsp[-1].NUM)));}
#line 2291 "parser.cpp"
    break;

  case 102:
#line 293 "parser.y"
                              {touchV->set_path(false, (yyvsp[0].STRING));}
#line 2297 "parser.cpp"
    break;

  case 103:
#line 294 "parser.y"
                                  {touchV->set_path(true, (yyvsp[0].STRING));}
#line 2303 "parser.cpp"
    break;

  case 104:
#line 295 "parser.y"
                   {touchV->set_r(true);}
#line 2309 "parser.cpp"
    break;

  case 105:
#line 296 "parser.y"
                                  {touchV->set_size(atoi((yyvsp[0].NUM)));}
#line 2315 "parser.cpp"
    break;

  case 106:
#line 297 "parser.y"
                                 {touchV->set_cont(false, (yyvsp[0].STRING));}
#line 2321 "parser.cpp"
    break;

  case 107:
#line 298 "parser.y"
                                   {touchV->set_cont(true, (yyvsp[0].STRING));}
#line 2327 "parser.cpp"
    break;

  case 108:
#line 299 "parser.y"
                       {touchV->set_stdin(true);}
#line 2333 "parser.cpp"
    break;

  case 111:
#line 308 "parser.y"
                              {mkdirV->set_path(false, (yyvsp[0].STRING));}
#line 2339 "parser.cpp"
    break;

  case 112:
#line 309 "parser.y"
                                  {mkdirV->set_path(true, (yyvsp[0].STRING));}
#line 2345 "parser.cpp"
    break;

  case 113:
#line 310 "parser.y"
                   {mkdirV->set_p(true);}
#line 2351 "parser.cpp"
    break;

  case 114:
#line 314 "parser.y"
                              {execV->set_path(false, (yyvsp[0].STRING));}
#line 2357 "parser.cpp"
    break;

  case 115:
#line 315 "parser.y"
                                  {execV->set_path(true, (yyvsp[0].STRING));}
#line 2363 "parser.cpp"
    break;

  case 118:
#line 324 "parser.y"
                            {catV->add_file(false, (yyvsp[0].STRING));}
#line 2369 "parser.cpp"
    break;

  case 119:
#line 325 "parser.y"
                                {catV->add_file(true, (yyvsp[0].STRING));}
#line 2375 "parser.cpp"
    break;

  case 122:
#line 333 "parser.y"
                              {renV->set_path(false, (yyvsp[0].STRING));}
#line 2381 "parser.cpp"
    break;

  case 123:
#line 334 "parser.y"
                                  {renV->set_path(true, (yyvsp[0].STRING));}
#line 2387 "parser.cpp"
    break;

  case 124:
#line 335 "parser.y"
                               {renV->set_name((yyvsp[0].STRING));}
#line 2393 "parser.cpp"
    break;

  case 127:
#line 343 "parser.y"
                              {moveV->set_path(false, (yyvsp[0].STRING));}
#line 2399 "parser.cpp"
    break;

  case 128:
#line 344 "parser.y"
                                  {moveV->set_path(true, (yyvsp[0].STRING));}
#line 2405 "parser.cpp"
    break;

  case 129:
#line 345 "parser.y"
                                 {moveV->set_dest(false, (yyvsp[0].STRING));}
#line 2411 "parser.cpp"
    break;

  case 130:
#line 346 "parser.y"
                                   {moveV->set_dest(true, (yyvsp[0].STRING));}
#line 2417 "parser.cpp"
    break;

  case 131:
#line 350 "parser.y"
                              {rmV->set_path(false, (yyvsp[0].STRING));}
#line 2423 "parser.cpp"
    break;

  case 132:
#line 351 "parser.y"
                                  {rmV->set_path(true, (yyvsp[0].STRING));}
#line 2429 "parser.cpp"
    break;

  case 135:
#line 360 "parser.y"
                              {editV->set_path(false, (yyvsp[0].STRING));}
#line 2435 "parser.cpp"
    break;

  case 136:
#line 361 "parser.y"
                                  {editV->set_path(true, (yyvsp[0].STRING));}
#line 2441 "parser.cpp"
    break;

  case 137:
#line 362 "parser.y"
                                 {editV->set_cont(false, (yyvsp[0].STRING));}
#line 2447 "parser.cpp"
    break;

  case 138:
#line 363 "parser.y"
                                   {editV->set_cont(true, (yyvsp[0].STRING));}
#line 2453 "parser.cpp"
    break;

  case 139:
#line 364 "parser.y"
                       {editV->set_stdin(true);}
#line 2459 "parser.cpp"
    break;

  case 142:
#line 372 "parser.y"
                              {cpV->set_path(false, (yyvsp[0].STRING));}
#line 2465 "parser.cpp"
    break;

  case 143:
#line 373 "parser.y"
                                  {cpV->set_path(true, (yyvsp[0].STRING));}
#line 2471 "parser.cpp"
    break;

  case 144:
#line 374 "parser.y"
                                 {cpV->set_dest(false, (yyvsp[0].STRING));}
#line 2477 "parser.cpp"
    break;

  case 145:
#line 375 "parser.y"
                                   {cpV->set_dest(true, (yyvsp[0].STRING));}
#line 2483 "parser.cpp"
    break;

  case 148:
#line 384 "parser.y"
                              {findV->set_path(false, (yyvsp[0].STRING));}
#line 2489 "parser.cpp"
    break;

  case 149:
#line 385 "parser.y"
                                  {findV->set_path(true, (yyvsp[0].STRING));}
#line 2495 "parser.cpp"
    break;

  case 150:
#line 386 "parser.y"
                                   {findV->set_name((yyvsp[0].STRING));}
#line 2501 "parser.cpp"
    break;

  case 153:
#line 395 "parser.y"
                             {usersV->set_user(false, (yyvsp[0].STRING));}
#line 2507 "parser.cpp"
    break;

  case 154:
#line 396 "parser.y"
                                 {usersV->set_user(false, (yyvsp[0].STRING));}
#line 2513 "parser.cpp"
    break;

  case 155:
#line 397 "parser.y"
                                   {usersV->set_user(true, (yyvsp[0].STRING));}
#line 2519 "parser.cpp"
    break;

  case 156:
#line 398 "parser.y"
                              {usersV->set_pwd((yyvsp[0].STRING), "");}
#line 2525 "parser.cpp"
    break;

  case 157:
#line 399 "parser.y"
                                     {usersV->set_pwd((yyvsp[-1].NUM), (yyvsp[0].STRING));}
#line 2531 "parser.cpp"
    break;

  case 158:
#line 400 "parser.y"
                                  {usersV->set_pwd((yyvsp[0].NUM), "");}
#line 2537 "parser.cpp"
    break;

  case 159:
#line 401 "parser.y"
                                       {usersV->set_id((yyvsp[0].STRING));}
#line 2543 "parser.cpp"
    break;

  case 162:
#line 410 "parser.y"
                             {usersV->set_name(false, (yyvsp[0].STRING));}
#line 2549 "parser.cpp"
    break;

  case 163:
#line 411 "parser.y"
                                   {usersV->set_name(true, (yyvsp[0].STRING));}
#line 2555 "parser.cpp"
    break;

  case 166:
#line 420 "parser.y"
                             {usersV->set_name(false, (yyvsp[0].STRING));}
#line 2561 "parser.cpp"
    break;

  case 167:
#line 421 "parser.y"
                                   {usersV->set_name(true, (yyvsp[0].STRING));}
#line 2567 "parser.cpp"
    break;

  case 170:
#line 430 "parser.y"
                            {usersV->set_name(false, (yyvsp[0].STRING));}
#line 2573 "parser.cpp"
    break;

  case 171:
#line 431 "parser.y"
                               {usersV->set_name(false, (yyvsp[0].STRING));}
#line 2579 "parser.cpp"
    break;

  case 172:
#line 432 "parser.y"
                                  {usersV->set_name(true, (yyvsp[0].STRING));}
#line 2585 "parser.cpp"
    break;

  case 173:
#line 433 "parser.y"
                              {usersV->set_user(false, (yyvsp[0].STRING));}
#line 2591 "parser.cpp"
    break;

  case 174:
#line 434 "parser.y"
                                {usersV->set_user(false, (yyvsp[0].STRING));}
#line 2597 "parser.cpp"
    break;

  case 175:
#line 435 "parser.y"
                                  {usersV->set_user(true, (yyvsp[0].STRING));}
#line 2603 "parser.cpp"
    break;

  case 176:
#line 436 "parser.y"
                              {usersV->set_pwd((yyvsp[0].STRING), "");}
#line 2609 "parser.cpp"
    break;

  case 177:
#line 437 "parser.y"
                                     {usersV->set_pwd((yyvsp[-1].NUM), (yyvsp[0].STRING));}
#line 2615 "parser.cpp"
    break;

  case 178:
#line 438 "parser.y"
                                  {usersV->set_pwd((yyvsp[0].NUM), "");}
#line 2621 "parser.cpp"
    break;

  case 181:
#line 447 "parser.y"
                            {usersV->set_name(false, (yyvsp[0].STRING));}
#line 2627 "parser.cpp"
    break;

  case 182:
#line 448 "parser.y"
                                  {usersV->set_name(true, (yyvsp[0].STRING));}
#line 2633 "parser.cpp"
    break;

  case 183:
#line 452 "parser.y"
                                     {simulateV->set_id((yyvsp[0].STRING));}
#line 2639 "parser.cpp"
    break;

  case 186:
#line 461 "parser.y"
                            {usersV->set_name(false, (yyvsp[0].STRING));}
#line 2645 "parser.cpp"
    break;

  case 187:
#line 462 "parser.y"
                                  {usersV->set_name(true, (yyvsp[0].STRING));}
#line 2651 "parser.cpp"
    break;

  case 188:
#line 463 "parser.y"
                              {usersV->set_user(false, (yyvsp[0].STRING));}
#line 2657 "parser.cpp"
    break;

  case 189:
#line 464 "parser.y"
                                {usersV->set_user(false, (yyvsp[0].STRING));}
#line 2663 "parser.cpp"
    break;

  case 190:
#line 465 "parser.y"
                                  {usersV->set_user(true, (yyvsp[0].STRING));}
#line 2669 "parser.cpp"
    break;


#line 2673 "parser.cpp"

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
      yyerror (YY_("syntax error"));
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
        yyerror (yymsgp);
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
                      yytoken, &yylval, &yylloc);
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
                  yystos[yystate], yyvsp, yylsp);
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
  yyerror (YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp, yylsp);
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
  return yyresult;
}
#line 467 "parser.y"

void yyerror(const char *s)
{
    printf("Error sintactico en la linea %i: %s\n", 0, s);
}
