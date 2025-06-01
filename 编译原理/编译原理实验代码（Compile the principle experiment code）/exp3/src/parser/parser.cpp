/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

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

    #include "SyntaxTree.hpp"
    #include "SyntaxAnalyse.hpp"
    #include <iostream>

    int yylex();
    int yyparse();
    int yyrestart();
    
    extern FILE* yyin;
    extern char* yytext;
    extern int line_number;
    extern int column_end_number;
    extern int column_start_number;

    void yyerror(const char *s) {
        std::cerr << s << std::endl;
        std::cerr << "Error at line " << line_number << ": " << column_end_number << std::endl;
        std::cerr << "Error: " << yytext << std::endl;
        std::abort();
    }

    using namespace ast;

#line 96 "parser.cpp"

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

#include "parser.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INT = 3,                        /* INT  */
  YYSYMBOL_VOID = 4,                       /* VOID  */
  YYSYMBOL_IF = 5,                         /* IF  */
  YYSYMBOL_ELSE = 6,                       /* ELSE  */
  YYSYMBOL_RETURN = 7,                     /* RETURN  */
  YYSYMBOL_Ident = 8,                      /* Ident  */
  YYSYMBOL_ADD = 9,                        /* ADD  */
  YYSYMBOL_SUB = 10,                       /* SUB  */
  YYSYMBOL_MUL = 11,                       /* MUL  */
  YYSYMBOL_DIV = 12,                       /* DIV  */
  YYSYMBOL_MOD = 13,                       /* MOD  */
  YYSYMBOL_LPAREN = 14,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 15,                    /* RPAREN  */
  YYSYMBOL_LBRACKET = 16,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 17,                  /* RBRACKET  */
  YYSYMBOL_LBRACE = 18,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 19,                    /* RBRACE  */
  YYSYMBOL_IntConst = 20,                  /* IntConst  */
  YYSYMBOL_LESS = 21,                      /* LESS  */
  YYSYMBOL_GREATER = 22,                   /* GREATER  */
  YYSYMBOL_EQUAL = 23,                     /* EQUAL  */
  YYSYMBOL_NOT = 24,                       /* NOT  */
  YYSYMBOL_LESS_EQUAL = 25,                /* LESS_EQUAL  */
  YYSYMBOL_GREATER_EQUAL = 26,             /* GREATER_EQUAL  */
  YYSYMBOL_NOT_EQUAL = 27,                 /* NOT_EQUAL  */
  YYSYMBOL_AND = 28,                       /* AND  */
  YYSYMBOL_OR = 29,                        /* OR  */
  YYSYMBOL_ASSIGN = 30,                    /* ASSIGN  */
  YYSYMBOL_COMMA = 31,                     /* COMMA  */
  YYSYMBOL_SEMICOLON = 32,                 /* SEMICOLON  */
  YYSYMBOL_ERROR = 33,                     /* ERROR  */
  YYSYMBOL_YYACCEPT = 34,                  /* $accept  */
  YYSYMBOL_CompUnit = 35,                  /* CompUnit  */
  YYSYMBOL_FuncDef = 36,                   /* FuncDef  */
  YYSYMBOL_FuncType = 37,                  /* FuncType  */
  YYSYMBOL_Block = 38,                     /* Block  */
  YYSYMBOL_BlockItems = 39,                /* BlockItems  */
  YYSYMBOL_Stmt = 40,                      /* Stmt  */
  YYSYMBOL_PrimaryExp = 41,                /* PrimaryExp  */
  YYSYMBOL_Decl = 42,                      /* Decl  */
  YYSYMBOL_VarDecl = 43,                   /* VarDecl  */
  YYSYMBOL_VarDefGroup = 44,               /* VarDefGroup  */
  YYSYMBOL_VarDef = 45,                    /* VarDef  */
  YYSYMBOL_InitVal = 46,                   /* InitVal  */
  YYSYMBOL_AddExp = 47,                    /* AddExp  */
  YYSYMBOL_Exp = 48,                       /* Exp  */
  YYSYMBOL_MulExp = 49,                    /* MulExp  */
  YYSYMBOL_Lval = 50,                      /* Lval  */
  YYSYMBOL_Cond = 51,                      /* Cond  */
  YYSYMBOL_LOrExp = 52,                    /* LOrExp  */
  YYSYMBOL_LAndExp = 53,                   /* LAndExp  */
  YYSYMBOL_EqExp = 54,                     /* EqExp  */
  YYSYMBOL_RelExp = 55,                    /* RelExp  */
  YYSYMBOL_UnaryExp = 56,                  /* UnaryExp  */
  YYSYMBOL_UnaryOp = 57                    /* UnaryOp  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




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

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
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
typedef yytype_int8 yy_state_t;

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
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
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
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   80

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  34
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  52
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  89

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   288


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

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
      25,    26,    27,    28,    29,    30,    31,    32,    33
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    88,    88,    90,    94,    97,    98,   101,   104,   106,
     109,   115,   117,   120,   125,   130,   133,   139,   141,   144,
     150,   154,   158,   161,   165,   168,   171,   175,   178,   181,
     185,   191,   194,   197,   201,   207,   211,   214,   218,   221,
     225,   228,   231,   235,   238,   241,   244,   247,   254,   257,
     261,   264,   267
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "INT", "VOID", "IF",
  "ELSE", "RETURN", "Ident", "ADD", "SUB", "MUL", "DIV", "MOD", "LPAREN",
  "RPAREN", "LBRACKET", "RBRACKET", "LBRACE", "RBRACE", "IntConst", "LESS",
  "GREATER", "EQUAL", "NOT", "LESS_EQUAL", "GREATER_EQUAL", "NOT_EQUAL",
  "AND", "OR", "ASSIGN", "COMMA", "SEMICOLON", "ERROR", "$accept",
  "CompUnit", "FuncDef", "FuncType", "Block", "BlockItems", "Stmt",
  "PrimaryExp", "Decl", "VarDecl", "VarDefGroup", "VarDef", "InitVal",
  "AddExp", "Exp", "MulExp", "Lval", "Cond", "LOrExp", "LAndExp", "EqExp",
  "RelExp", "UnaryExp", "UnaryOp", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-62)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      35,   -62,   -62,    45,   -62,    -1,   -62,   -62,     6,    -2,
       5,   -62,   -62,     3,    24,    23,    -5,   -62,   -62,   -62,
     -62,   -62,   -62,    13,    17,    19,    20,   -62,   -62,   -62,
      20,   -62,   -62,   -62,   -62,    48,    36,    50,   -62,    20,
      20,    20,    24,    37,    48,    52,    41,    43,   -11,    34,
      57,    20,    20,   -62,    20,    20,   -62,    42,   -62,   -62,
      19,   -62,    28,    20,    20,    20,    20,    20,    20,    20,
      20,   -62,    50,    50,   -62,   -62,   -62,   -62,    67,    43,
     -11,    34,    34,    48,    48,    48,    48,    28,   -62
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     6,     5,     0,     3,     0,     1,     2,     0,     0,
       0,     9,     4,     0,     0,     0,     0,    34,     7,    12,
       8,    10,    20,     0,    24,    23,     0,    19,    50,    51,
       0,    17,    52,    13,    48,    30,     0,    27,    31,     0,
       0,     0,     0,     0,    43,     0,    35,    36,    38,    40,
       0,     0,     0,    11,     0,     0,    49,     0,    25,    26,
      23,    21,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    18,    28,    29,    32,    33,    14,    22,    15,    37,
      39,    41,    42,    44,    45,    46,    47,     0,    16
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -62,   -62,    72,   -62,    66,   -62,   -61,   -62,   -62,   -62,
      18,    38,   -62,   -16,     1,    12,   -62,   -62,   -62,    14,
      15,     0,   -37,   -62
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     3,     4,     5,    19,    13,    20,    34,    21,    22,
      43,    25,    58,    44,    36,    37,    23,    45,    46,    47,
      48,    49,    38,    39
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      35,    78,    56,    27,    28,    29,    14,     8,    15,    30,
      16,    17,    65,    10,    35,    31,    66,    74,    75,    32,
       9,    11,    18,    11,    35,    35,    88,    33,    27,    28,
      29,    50,    24,    15,    30,    16,    17,    26,     1,     2,
      31,    57,    59,    40,    32,     6,    11,    41,     1,     2,
      42,    83,    84,    85,    86,    67,    68,    51,    52,    69,
      70,    54,    55,    72,    73,    81,    82,    62,    53,    61,
      63,    64,    71,    87,    76,     7,    12,    79,    77,    80,
      60
};

static const yytype_int8 yycheck[] =
{
      16,    62,    39,     8,     9,    10,     3,     8,     5,    14,
       7,     8,    23,    15,    30,    20,    27,    54,    55,    24,
      14,    18,    19,    18,    40,    41,    87,    32,     8,     9,
      10,    30,     8,     5,    14,     7,     8,    14,     3,     4,
      20,    40,    41,    30,    24,     0,    18,    30,     3,     4,
      31,    67,    68,    69,    70,    21,    22,     9,    10,    25,
      26,    11,    12,    51,    52,    65,    66,    15,    32,    32,
      29,    28,    15,     6,    32,     3,    10,    63,    60,    64,
      42
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,    35,    36,    37,     0,    36,     8,    14,
      15,    18,    38,    39,     3,     5,     7,     8,    19,    38,
      40,    42,    43,    50,     8,    45,    14,     8,     9,    10,
      14,    20,    24,    32,    41,    47,    48,    49,    56,    57,
      30,    30,    31,    44,    47,    51,    52,    53,    54,    55,
      48,     9,    10,    32,    11,    12,    56,    48,    46,    48,
      45,    32,    15,    29,    28,    23,    27,    21,    22,    25,
      26,    15,    49,    49,    56,    56,    32,    44,    40,    53,
      54,    55,    55,    47,    47,    47,    47,     6,    40
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    34,    35,    35,    36,    37,    37,    38,    39,    39,
      39,    40,    40,    40,    40,    40,    40,    41,    41,    41,
      42,    43,    44,    44,    45,    45,    46,    47,    47,    47,
      48,    49,    49,    49,    50,    51,    52,    52,    53,    53,
      54,    54,    54,    55,    55,    55,    55,    55,    56,    56,
      57,    57,    57
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     5,     1,     1,     3,     2,     0,
       2,     3,     1,     2,     4,     5,     7,     1,     3,     1,
       1,     4,     3,     0,     1,     3,     1,     1,     3,     3,
       1,     1,     3,     3,     1,     1,     1,     3,     1,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     1,     2,
       1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
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
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
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
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
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
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

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
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
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

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* CompUnit: CompUnit FuncDef  */
#line 88 "parser.y"
                      { SyntaxAnalyseCompUnit((yyval.compunit),(yyvsp[-1].compunit),(yyvsp[0].func_def));
    }
#line 1196 "parser.cpp"
    break;

  case 3: /* CompUnit: FuncDef  */
#line 90 "parser.y"
             { SyntaxAnalyseCompUnit((yyval.compunit),nullptr,(yyvsp[0].func_def)); 
    }
#line 1203 "parser.cpp"
    break;

  case 4: /* FuncDef: FuncType Ident LPAREN RPAREN Block  */
#line 94 "parser.y"
                                        { SyntaxAnalyseFuncDef((yyval.func_def),(yyvsp[-4].var_type),(yyvsp[-3].current_symbol),(yyvsp[0].block));}
#line 1209 "parser.cpp"
    break;

  case 5: /* FuncType: VOID  */
#line 97 "parser.y"
          { SynataxAnalyseFuncType((yyval.var_type),(yyvsp[0].current_symbol));}
#line 1215 "parser.cpp"
    break;

  case 6: /* FuncType: INT  */
#line 98 "parser.y"
         { SynataxAnalyseFuncType((yyval.var_type),(yyvsp[0].current_symbol));}
#line 1221 "parser.cpp"
    break;

  case 7: /* Block: LBRACE BlockItems RBRACE  */
#line 101 "parser.y"
                               { SynataxAnalyseBlock((yyval.block),(yyvsp[-1].block));}
#line 1227 "parser.cpp"
    break;

  case 8: /* BlockItems: BlockItems Stmt  */
#line 104 "parser.y"
                      { SynataxAnalyseBlockItems((yyval.block),(yyvsp[-1].block),(yyvsp[0].stmt));
    }
#line 1234 "parser.cpp"
    break;

  case 9: /* BlockItems: %empty  */
#line 106 "parser.y"
      { SynataxAnalyseBlockItems((yyval.block),nullptr,nullptr);
    }
#line 1241 "parser.cpp"
    break;

  case 10: /* BlockItems: BlockItems Decl  */
#line 109 "parser.y"
                     {
        SynataxAnalyseBlockItems((yyval.block),(yyvsp[-1].block),(yyvsp[0].stmt));
    }
#line 1249 "parser.cpp"
    break;

  case 11: /* Stmt: RETURN Exp SEMICOLON  */
#line 115 "parser.y"
                           { SynataxAnalyseStmtReturn((yyval.stmt),(yyvsp[-1].expr));}
#line 1255 "parser.cpp"
    break;

  case 12: /* Stmt: Block  */
#line 117 "parser.y"
           {
        SynataxAnalyseStmtBlock((yyval.stmt),(yyvsp[0].block));
    }
#line 1263 "parser.cpp"
    break;

  case 13: /* Stmt: RETURN SEMICOLON  */
#line 120 "parser.y"
                     {
        SynataxAnalyseStmtReturn((yyval.stmt),nullptr);
    }
#line 1271 "parser.cpp"
    break;

  case 14: /* Stmt: Lval ASSIGN Exp SEMICOLON  */
#line 125 "parser.y"
                               {
        SynataxAnalyseStmtAssign((yyval.stmt),(yyvsp[-3].lval),(yyvsp[-1].expr));
    }
#line 1279 "parser.cpp"
    break;

  case 15: /* Stmt: IF LPAREN Cond RPAREN Stmt  */
#line 130 "parser.y"
                                 {
        SynataxAnalyseStmtIf((yyval.stmt),(yyvsp[-2].expr),(yyvsp[0].stmt),nullptr);
    }
#line 1287 "parser.cpp"
    break;

  case 16: /* Stmt: IF LPAREN Cond RPAREN Stmt ELSE Stmt  */
#line 133 "parser.y"
                                          {
        SynataxAnalyseStmtIf((yyval.stmt),(yyvsp[-4].expr),(yyvsp[-2].stmt),(yyvsp[0].stmt));
    }
#line 1295 "parser.cpp"
    break;

  case 17: /* PrimaryExp: IntConst  */
#line 139 "parser.y"
               { SynataxAnalysePrimaryExpIntConst((yyval.expr),(yyvsp[0].current_symbol)); }
#line 1301 "parser.cpp"
    break;

  case 18: /* PrimaryExp: LPAREN Exp RPAREN  */
#line 141 "parser.y"
                       {
        (yyval.expr)=(yyvsp[-1].expr);
    }
#line 1309 "parser.cpp"
    break;

  case 19: /* PrimaryExp: Ident  */
#line 144 "parser.y"
           {
        SynataxAnalysePrimaryExpVar((yyval.expr),(yyvsp[0].current_symbol));
    }
#line 1317 "parser.cpp"
    break;

  case 20: /* Decl: VarDecl  */
#line 150 "parser.y"
                 {
        (yyval.stmt)=(yyvsp[0].stmt);
    }
#line 1325 "parser.cpp"
    break;

  case 21: /* VarDecl: INT VarDef VarDefGroup SEMICOLON  */
#line 154 "parser.y"
                                             {
        SynataxAnalyseVarDecl((yyval.stmt),(yyvsp[-2].var_def_stmt),(yyvsp[-1].var_decl_stmt));
    }
#line 1333 "parser.cpp"
    break;

  case 22: /* VarDefGroup: COMMA VarDef VarDefGroup  */
#line 158 "parser.y"
                                          {
        SynataxAnalyseVarDefGroup((yyval.var_decl_stmt),(yyvsp[-1].var_def_stmt),(yyvsp[0].var_decl_stmt));
    }
#line 1341 "parser.cpp"
    break;

  case 23: /* VarDefGroup: %empty  */
#line 161 "parser.y"
     {
        (yyval.var_decl_stmt)=nullptr;
    }
#line 1349 "parser.cpp"
    break;

  case 24: /* VarDef: Ident  */
#line 165 "parser.y"
                  {
         SynataxAnalyseVarDef((yyval.var_def_stmt),(yyvsp[0].current_symbol),nullptr);
    }
#line 1357 "parser.cpp"
    break;

  case 25: /* VarDef: Ident ASSIGN InitVal  */
#line 168 "parser.y"
                          {
        SynataxAnalyseVarDef((yyval.var_def_stmt),(yyvsp[-2].current_symbol),(yyvsp[0].expr));
    }
#line 1365 "parser.cpp"
    break;

  case 26: /* InitVal: Exp  */
#line 171 "parser.y"
                {
        (yyval.expr)=(yyvsp[0].expr);
    }
#line 1373 "parser.cpp"
    break;

  case 27: /* AddExp: MulExp  */
#line 175 "parser.y"
                  {
        (yyval.expr)=(yyvsp[0].expr);
    }
#line 1381 "parser.cpp"
    break;

  case 28: /* AddExp: AddExp ADD MulExp  */
#line 178 "parser.y"
                       {
        SynataxAnalyseAddExp((yyval.expr),(yyvsp[-2].expr),(yyvsp[-1].current_symbol),(yyvsp[0].expr));
    }
#line 1389 "parser.cpp"
    break;

  case 29: /* AddExp: AddExp SUB MulExp  */
#line 181 "parser.y"
                       {
        SynataxAnalyseAddExp((yyval.expr),(yyvsp[-2].expr),(yyvsp[-1].current_symbol),(yyvsp[0].expr));
    }
#line 1397 "parser.cpp"
    break;

  case 30: /* Exp: AddExp  */
#line 185 "parser.y"
               {
        (yyval.expr)=(yyvsp[0].expr);
    }
#line 1405 "parser.cpp"
    break;

  case 31: /* MulExp: UnaryExp  */
#line 191 "parser.y"
                    {
        (yyval.expr)=(yyvsp[0].expr);
    }
#line 1413 "parser.cpp"
    break;

  case 32: /* MulExp: MulExp MUL UnaryExp  */
#line 194 "parser.y"
                          {
         SynataxAnalyseMulExp((yyval.expr),(yyvsp[-2].expr),(yyvsp[-1].current_symbol),(yyvsp[0].expr));
    }
#line 1421 "parser.cpp"
    break;

  case 33: /* MulExp: MulExp DIV UnaryExp  */
#line 197 "parser.y"
                          {
         SynataxAnalyseMulExp((yyval.expr),(yyvsp[-2].expr),(yyvsp[-1].current_symbol),(yyvsp[0].expr));
    }
#line 1429 "parser.cpp"
    break;

  case 34: /* Lval: Ident  */
#line 201 "parser.y"
               {
        SynataxAnalyseLval((yyval.lval),(yyvsp[0].current_symbol));
    }
#line 1437 "parser.cpp"
    break;

  case 35: /* Cond: LOrExp  */
#line 207 "parser.y"
               {
    (yyval.expr)=(yyvsp[0].expr);
   }
#line 1445 "parser.cpp"
    break;

  case 36: /* LOrExp: LAndExp  */
#line 211 "parser.y"
                  {
    (yyval.expr)=(yyvsp[0].expr);
   }
#line 1453 "parser.cpp"
    break;

  case 37: /* LOrExp: LOrExp OR LAndExp  */
#line 214 "parser.y"
                     {
    SynataxAnalyseLOrExp((yyval.expr),(yyvsp[-2].expr),(yyvsp[0].expr));
   }
#line 1461 "parser.cpp"
    break;

  case 38: /* LAndExp: EqExp  */
#line 218 "parser.y"
                  {
        (yyval.expr)=(yyvsp[0].expr);
    }
#line 1469 "parser.cpp"
    break;

  case 39: /* LAndExp: LAndExp AND EqExp  */
#line 221 "parser.y"
                        {
        SynataxAnalyseLAndExp((yyval.expr),(yyvsp[-2].expr),(yyvsp[0].expr));
    }
#line 1477 "parser.cpp"
    break;

  case 40: /* EqExp: RelExp  */
#line 225 "parser.y"
                 {
        (yyval.expr)=(yyvsp[0].expr);
    }
#line 1485 "parser.cpp"
    break;

  case 41: /* EqExp: EqExp EQUAL RelExp  */
#line 228 "parser.y"
                       {
        SynataxAnalyseEqExp((yyval.expr),(yyvsp[-2].expr),(yyvsp[-1].current_symbol),(yyvsp[0].expr));
    }
#line 1493 "parser.cpp"
    break;

  case 42: /* EqExp: EqExp NOT_EQUAL RelExp  */
#line 231 "parser.y"
                           {
        SynataxAnalyseEqExp((yyval.expr),(yyvsp[-2].expr),(yyvsp[-1].current_symbol),(yyvsp[0].expr));
    }
#line 1501 "parser.cpp"
    break;

  case 43: /* RelExp: AddExp  */
#line 235 "parser.y"
                  {
        (yyval.expr)=(yyvsp[0].expr);
    }
#line 1509 "parser.cpp"
    break;

  case 44: /* RelExp: RelExp LESS AddExp  */
#line 238 "parser.y"
                         {
        SynataxAnalyseRelExp((yyval.expr),(yyvsp[-2].expr),(yyvsp[-1].current_symbol),(yyvsp[0].expr));
    }
#line 1517 "parser.cpp"
    break;

  case 45: /* RelExp: RelExp GREATER AddExp  */
#line 241 "parser.y"
                            {
        SynataxAnalyseRelExp((yyval.expr),(yyvsp[-2].expr),(yyvsp[-1].current_symbol),(yyvsp[0].expr));
    }
#line 1525 "parser.cpp"
    break;

  case 46: /* RelExp: RelExp LESS_EQUAL AddExp  */
#line 244 "parser.y"
                               {
        SynataxAnalyseRelExp((yyval.expr),(yyvsp[-2].expr),(yyvsp[-1].current_symbol),(yyvsp[0].expr));
    }
#line 1533 "parser.cpp"
    break;

  case 47: /* RelExp: RelExp GREATER_EQUAL AddExp  */
#line 247 "parser.y"
                                  {
        SynataxAnalyseRelExp((yyval.expr),(yyvsp[-2].expr),(yyvsp[-1].current_symbol),(yyvsp[0].expr));
    }
#line 1541 "parser.cpp"
    break;

  case 48: /* UnaryExp: PrimaryExp  */
#line 254 "parser.y"
                         {
        (yyval.expr)=(yyvsp[0].expr);
    }
#line 1549 "parser.cpp"
    break;

  case 49: /* UnaryExp: UnaryOp UnaryExp  */
#line 257 "parser.y"
                      {
        SynataxAnalyseUnaryExp((yyval.expr),(yyvsp[-1].current_symbol),(yyvsp[0].expr));
    }
#line 1557 "parser.cpp"
    break;

  case 50: /* UnaryOp: ADD  */
#line 261 "parser.y"
               {
        (yyval.current_symbol)=(yyvsp[0].current_symbol);
    }
#line 1565 "parser.cpp"
    break;

  case 51: /* UnaryOp: SUB  */
#line 264 "parser.y"
         {
        (yyval.current_symbol)=(yyvsp[0].current_symbol);
    }
#line 1573 "parser.cpp"
    break;

  case 52: /* UnaryOp: NOT  */
#line 267 "parser.y"
         {
        (yyval.current_symbol)=(yyvsp[0].current_symbol);
    }
#line 1581 "parser.cpp"
    break;


#line 1585 "parser.cpp"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

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
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
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
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 272 "parser.y"


