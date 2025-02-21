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

#include "AST.hpp"
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <cstring>

typedef struct token_data
{
	int line;
	int column;
	std::string token_type;
	std::string lexeme;
} TOKEN;


extern char* yytext;
extern int column;
extern int line;
extern std::string current_token_type;

int yyerror(const char*);
int yylex();
int onlyLexer = 0;
FILE* dotfile;


extern int yyleng;
extern int yylex();
extern int yyrestart(FILE*);
extern FILE* yyin;

#line 107 "parser.cpp"

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
  YYSYMBOL_IDENTIFIER = 3,                 /* IDENTIFIER  */
  YYSYMBOL_CONSTANT = 4,                   /* CONSTANT  */
  YYSYMBOL_STRING_LITERAL = 5,             /* STRING_LITERAL  */
  YYSYMBOL_SIZEOF = 6,                     /* SIZEOF  */
  YYSYMBOL_PTR_OP = 7,                     /* PTR_OP  */
  YYSYMBOL_INC_OP = 8,                     /* INC_OP  */
  YYSYMBOL_DEC_OP = 9,                     /* DEC_OP  */
  YYSYMBOL_LEFT_OP = 10,                   /* LEFT_OP  */
  YYSYMBOL_RIGHT_OP = 11,                  /* RIGHT_OP  */
  YYSYMBOL_LE_OP = 12,                     /* LE_OP  */
  YYSYMBOL_GE_OP = 13,                     /* GE_OP  */
  YYSYMBOL_EQ_OP = 14,                     /* EQ_OP  */
  YYSYMBOL_NE_OP = 15,                     /* NE_OP  */
  YYSYMBOL_AND_OP = 16,                    /* AND_OP  */
  YYSYMBOL_OR_OP = 17,                     /* OR_OP  */
  YYSYMBOL_MUL_ASSIGN = 18,                /* MUL_ASSIGN  */
  YYSYMBOL_DIV_ASSIGN = 19,                /* DIV_ASSIGN  */
  YYSYMBOL_MOD_ASSIGN = 20,                /* MOD_ASSIGN  */
  YYSYMBOL_ADD_ASSIGN = 21,                /* ADD_ASSIGN  */
  YYSYMBOL_SUB_ASSIGN = 22,                /* SUB_ASSIGN  */
  YYSYMBOL_LEFT_ASSIGN = 23,               /* LEFT_ASSIGN  */
  YYSYMBOL_RIGHT_ASSIGN = 24,              /* RIGHT_ASSIGN  */
  YYSYMBOL_AND_ASSIGN = 25,                /* AND_ASSIGN  */
  YYSYMBOL_XOR_ASSIGN = 26,                /* XOR_ASSIGN  */
  YYSYMBOL_OR_ASSIGN = 27,                 /* OR_ASSIGN  */
  YYSYMBOL_TYPE_NAME = 28,                 /* TYPE_NAME  */
  YYSYMBOL_TYPEDEF = 29,                   /* TYPEDEF  */
  YYSYMBOL_EXTERN = 30,                    /* EXTERN  */
  YYSYMBOL_STATIC = 31,                    /* STATIC  */
  YYSYMBOL_AUTO = 32,                      /* AUTO  */
  YYSYMBOL_REGISTER = 33,                  /* REGISTER  */
  YYSYMBOL_CHAR = 34,                      /* CHAR  */
  YYSYMBOL_SHORT = 35,                     /* SHORT  */
  YYSYMBOL_INT = 36,                       /* INT  */
  YYSYMBOL_LONG = 37,                      /* LONG  */
  YYSYMBOL_SIGNED = 38,                    /* SIGNED  */
  YYSYMBOL_UNSIGNED = 39,                  /* UNSIGNED  */
  YYSYMBOL_FLOAT = 40,                     /* FLOAT  */
  YYSYMBOL_DOUBLE = 41,                    /* DOUBLE  */
  YYSYMBOL_CONST = 42,                     /* CONST  */
  YYSYMBOL_VOLATILE = 43,                  /* VOLATILE  */
  YYSYMBOL_VOID = 44,                      /* VOID  */
  YYSYMBOL_STRUCT = 45,                    /* STRUCT  */
  YYSYMBOL_UNION = 46,                     /* UNION  */
  YYSYMBOL_ENUM = 47,                      /* ENUM  */
  YYSYMBOL_ELLIPSIS = 48,                  /* ELLIPSIS  */
  YYSYMBOL_CASE = 49,                      /* CASE  */
  YYSYMBOL_DEFAULT = 50,                   /* DEFAULT  */
  YYSYMBOL_IF = 51,                        /* IF  */
  YYSYMBOL_ELSE = 52,                      /* ELSE  */
  YYSYMBOL_SWITCH = 53,                    /* SWITCH  */
  YYSYMBOL_WHILE = 54,                     /* WHILE  */
  YYSYMBOL_DO = 55,                        /* DO  */
  YYSYMBOL_FOR = 56,                       /* FOR  */
  YYSYMBOL_GOTO = 57,                      /* GOTO  */
  YYSYMBOL_CONTINUE = 58,                  /* CONTINUE  */
  YYSYMBOL_BREAK = 59,                     /* BREAK  */
  YYSYMBOL_RETURN = 60,                    /* RETURN  */
  YYSYMBOL_UNTIL = 61,                     /* UNTIL  */
  YYSYMBOL_62_ = 62,                       /* ';'  */
  YYSYMBOL_63_ = 63,                       /* '('  */
  YYSYMBOL_64_ = 64,                       /* ')'  */
  YYSYMBOL_65_ = 65,                       /* '['  */
  YYSYMBOL_66_ = 66,                       /* ']'  */
  YYSYMBOL_67_ = 67,                       /* '.'  */
  YYSYMBOL_68_ = 68,                       /* ','  */
  YYSYMBOL_69_ = 69,                       /* '&'  */
  YYSYMBOL_70_ = 70,                       /* '*'  */
  YYSYMBOL_71_ = 71,                       /* '+'  */
  YYSYMBOL_72_ = 72,                       /* '-'  */
  YYSYMBOL_73_ = 73,                       /* '~'  */
  YYSYMBOL_74_ = 74,                       /* '!'  */
  YYSYMBOL_75_ = 75,                       /* '/'  */
  YYSYMBOL_76_ = 76,                       /* '%'  */
  YYSYMBOL_77_ = 77,                       /* '<'  */
  YYSYMBOL_78_ = 78,                       /* '>'  */
  YYSYMBOL_79_ = 79,                       /* '^'  */
  YYSYMBOL_80_ = 80,                       /* '|'  */
  YYSYMBOL_81_ = 81,                       /* '?'  */
  YYSYMBOL_82_ = 82,                       /* ':'  */
  YYSYMBOL_83_ = 83,                       /* '='  */
  YYSYMBOL_84_ = 84,                       /* '{'  */
  YYSYMBOL_85_ = 85,                       /* '}'  */
  YYSYMBOL_YYACCEPT = 86,                  /* $accept  */
  YYSYMBOL_primary_expression = 87,        /* primary_expression  */
  YYSYMBOL_postfix_expression = 88,        /* postfix_expression  */
  YYSYMBOL_argument_expression_list = 89,  /* argument_expression_list  */
  YYSYMBOL_unary_expression = 90,          /* unary_expression  */
  YYSYMBOL_unary_operator = 91,            /* unary_operator  */
  YYSYMBOL_cast_expression = 92,           /* cast_expression  */
  YYSYMBOL_multiplicative_expression = 93, /* multiplicative_expression  */
  YYSYMBOL_additive_expression = 94,       /* additive_expression  */
  YYSYMBOL_shift_expression = 95,          /* shift_expression  */
  YYSYMBOL_relational_expression = 96,     /* relational_expression  */
  YYSYMBOL_equality_expression = 97,       /* equality_expression  */
  YYSYMBOL_and_expression = 98,            /* and_expression  */
  YYSYMBOL_exclusive_or_expression = 99,   /* exclusive_or_expression  */
  YYSYMBOL_inclusive_or_expression = 100,  /* inclusive_or_expression  */
  YYSYMBOL_logical_and_expression = 101,   /* logical_and_expression  */
  YYSYMBOL_logical_or_expression = 102,    /* logical_or_expression  */
  YYSYMBOL_conditional_expression = 103,   /* conditional_expression  */
  YYSYMBOL_assignment_expression = 104,    /* assignment_expression  */
  YYSYMBOL_assignment_operator = 105,      /* assignment_operator  */
  YYSYMBOL_expression = 106,               /* expression  */
  YYSYMBOL_constant_expression = 107,      /* constant_expression  */
  YYSYMBOL_declaration = 108,              /* declaration  */
  YYSYMBOL_declaration_specifiers = 109,   /* declaration_specifiers  */
  YYSYMBOL_init_declarator_list = 110,     /* init_declarator_list  */
  YYSYMBOL_init_declarator = 111,          /* init_declarator  */
  YYSYMBOL_storage_class_specifier = 112,  /* storage_class_specifier  */
  YYSYMBOL_type_specifier = 113,           /* type_specifier  */
  YYSYMBOL_struct_or_union_specifier = 114, /* struct_or_union_specifier  */
  YYSYMBOL_struct_or_union = 115,          /* struct_or_union  */
  YYSYMBOL_struct_declaration_list = 116,  /* struct_declaration_list  */
  YYSYMBOL_struct_declaration = 117,       /* struct_declaration  */
  YYSYMBOL_specifier_qualifier_list = 118, /* specifier_qualifier_list  */
  YYSYMBOL_struct_declarator_list = 119,   /* struct_declarator_list  */
  YYSYMBOL_struct_declarator = 120,        /* struct_declarator  */
  YYSYMBOL_enum_specifier = 121,           /* enum_specifier  */
  YYSYMBOL_enumerator_list = 122,          /* enumerator_list  */
  YYSYMBOL_enumerator = 123,               /* enumerator  */
  YYSYMBOL_type_qualifier = 124,           /* type_qualifier  */
  YYSYMBOL_declarator = 125,               /* declarator  */
  YYSYMBOL_direct_declarator = 126,        /* direct_declarator  */
  YYSYMBOL_pointer = 127,                  /* pointer  */
  YYSYMBOL_type_qualifier_list = 128,      /* type_qualifier_list  */
  YYSYMBOL_parameter_type_list = 129,      /* parameter_type_list  */
  YYSYMBOL_parameter_list = 130,           /* parameter_list  */
  YYSYMBOL_parameter_declaration = 131,    /* parameter_declaration  */
  YYSYMBOL_identifier_list = 132,          /* identifier_list  */
  YYSYMBOL_type_name = 133,                /* type_name  */
  YYSYMBOL_abstract_declarator = 134,      /* abstract_declarator  */
  YYSYMBOL_direct_abstract_declarator = 135, /* direct_abstract_declarator  */
  YYSYMBOL_initializer = 136,              /* initializer  */
  YYSYMBOL_initializer_list = 137,         /* initializer_list  */
  YYSYMBOL_statement = 138,                /* statement  */
  YYSYMBOL_labeled_statement = 139,        /* labeled_statement  */
  YYSYMBOL_compound_statement = 140,       /* compound_statement  */
  YYSYMBOL_declaration_list = 141,         /* declaration_list  */
  YYSYMBOL_statement_list = 142,           /* statement_list  */
  YYSYMBOL_expression_statement = 143,     /* expression_statement  */
  YYSYMBOL_selection_statement = 144,      /* selection_statement  */
  YYSYMBOL_iteration_statement = 145,      /* iteration_statement  */
  YYSYMBOL_jump_statement = 146,           /* jump_statement  */
  YYSYMBOL_translation_unit = 147,         /* translation_unit  */
  YYSYMBOL_external_declaration = 148,     /* external_declaration  */
  YYSYMBOL_function_definition = 149       /* function_definition  */
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
#endif /* 1 */

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
#define YYFINAL  61
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1354

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  86
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  64
/* YYNRULES -- Number of rules.  */
#define YYNRULES  215
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  358

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   316


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
       2,     2,     2,    74,     2,     2,     2,    76,    69,     2,
      63,    64,    70,    71,    68,    72,    67,    75,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    82,    62,
      77,    83,    78,    81,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    65,     2,    66,    79,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    84,    80,    85,    73,     2,     2,     2,
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
      55,    56,    57,    58,    59,    60,    61
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    77,    77,    80,    83,    86,    92,    95,   101,   104,
     110,   116,   122,   127,   135,   138,   147,   150,   155,   160,
     166,   171,   179,   182,   185,   188,   191,   194,   200,   203,
     212,   215,   221,   227,   236,   239,   245,   254,   257,   263,
     272,   275,   281,   287,   293,   302,   305,   311,   320,   323,
     332,   335,   344,   345,   354,   355,   364,   365,   374,   375,
     385,   386,   395,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   411,   412,   421,   425,   426,   435,   436,
     442,   443,   449,   450,   459,   460,   469,   470,   479,   480,
     481,   482,   483,   487,   488,   489,   490,   491,   492,   493,
     494,   495,   496,   497,   498,   502,   508,   513,   521,   522,
     526,   527,   536,   545,   551,   552,   558,   562,   563,   572,
     573,   574,   583,   588,   594,   602,   603,   612,   613,   622,
     623,   628,   634,   641,   644,   647,   655,   661,   669,   677,
     686,   689,   694,   699,   708,   711,   721,   724,   733,   736,
     745,   751,   757,   763,   766,   775,   778,   787,   790,   793,
     802,   805,   808,   811,   817,   825,   828,   831,   837,   848,
     851,   854,   861,   864,   873,   874,   875,   876,   877,   878,
     879,   884,   892,   898,   904,   913,   914,   915,   916,   925,
     926,   935,   936,   945,   946,   950,   956,   963,   972,   978,
     984,   991,   999,  1008,  1013,  1014,  1015,  1016,  1025,  1028,
    1037,  1038,  1042,  1050,  1057,  1064
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  static const char *const yy_sname[] =
  {
  "end of file", "error", "invalid token", "IDENTIFIER", "CONSTANT",
  "STRING_LITERAL", "SIZEOF", "PTR_OP", "INC_OP", "DEC_OP", "LEFT_OP",
  "RIGHT_OP", "LE_OP", "GE_OP", "EQ_OP", "NE_OP", "AND_OP", "OR_OP",
  "MUL_ASSIGN", "DIV_ASSIGN", "MOD_ASSIGN", "ADD_ASSIGN", "SUB_ASSIGN",
  "LEFT_ASSIGN", "RIGHT_ASSIGN", "AND_ASSIGN", "XOR_ASSIGN", "OR_ASSIGN",
  "TYPE_NAME", "TYPEDEF", "EXTERN", "STATIC", "AUTO", "REGISTER", "CHAR",
  "SHORT", "INT", "LONG", "SIGNED", "UNSIGNED", "FLOAT", "DOUBLE", "CONST",
  "VOLATILE", "VOID", "STRUCT", "UNION", "ENUM", "ELLIPSIS", "CASE",
  "DEFAULT", "IF", "ELSE", "SWITCH", "WHILE", "DO", "FOR", "GOTO",
  "CONTINUE", "BREAK", "RETURN", "UNTIL", "';'", "'('", "')'", "'['",
  "']'", "'.'", "','", "'&'", "'*'", "'+'", "'-'", "'~'", "'!'", "'/'",
  "'%'", "'<'", "'>'", "'^'", "'|'", "'?'", "':'", "'='", "'{'", "'}'",
  "$accept", "primary_expression", "postfix_expression",
  "argument_expression_list", "unary_expression", "unary_operator",
  "cast_expression", "multiplicative_expression", "additive_expression",
  "shift_expression", "relational_expression", "equality_expression",
  "and_expression", "exclusive_or_expression", "inclusive_or_expression",
  "logical_and_expression", "logical_or_expression",
  "conditional_expression", "assignment_expression", "assignment_operator",
  "expression", "constant_expression", "declaration",
  "declaration_specifiers", "init_declarator_list", "init_declarator",
  "storage_class_specifier", "type_specifier", "struct_or_union_specifier",
  "struct_or_union", "struct_declaration_list", "struct_declaration",
  "specifier_qualifier_list", "struct_declarator_list",
  "struct_declarator", "enum_specifier", "enumerator_list", "enumerator",
  "type_qualifier", "declarator", "direct_declarator", "pointer",
  "type_qualifier_list", "parameter_type_list", "parameter_list",
  "parameter_declaration", "identifier_list", "type_name",
  "abstract_declarator", "direct_abstract_declarator", "initializer",
  "initializer_list", "statement", "labeled_statement",
  "compound_statement", "declaration_list", "statement_list",
  "expression_statement", "selection_statement", "iteration_statement",
  "jump_statement", "translation_unit", "external_declaration",
  "function_definition", YY_NULLPTR
  };
  return yy_sname[yysymbol];
}
#endif

#define YYPACT_NINF (-217)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     975,  -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,
    -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,
    -217,     6,    25,    83,  -217,    12,  1307,  1307,  -217,     9,
    -217,  1307,  1149,     2,    20,   885,  -217,  -217,   -59,    54,
      16,  -217,  -217,    83,  -217,    52,  -217,  1129,  -217,  -217,
      -8,  1103,  -217,   288,  -217,    12,  -217,  1149,  1020,   609,
       2,  -217,  -217,    54,    47,   -19,  -217,  -217,  -217,  -217,
    -217,    25,   143,  -217,  1149,  1103,  1103,  1051,  -217,    15,
    1103,   -12,    60,  -217,  -217,   798,   810,   810,   837,    63,
     135,   163,   169,   525,   182,   248,   191,   195,   642,   199,
    -217,   597,  -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,
     127,   370,   837,  -217,   -15,    30,   175,    41,   225,   200,
     185,   190,   257,    17,  -217,  -217,    71,  -217,  -217,  -217,
     377,   451,  -217,  -217,  -217,  -217,   197,  -217,  -217,  -217,
    -217,    14,   221,   218,  -217,    23,  -217,  -217,  -217,  -217,
     222,   -17,   837,    54,  -217,  -217,   143,  -217,  -217,  -217,
    1071,  -217,  -217,  -217,   837,    82,  -217,   208,  -217,  -217,
    -217,   525,   597,  -217,   837,  -217,  -217,   213,   525,   837,
     837,   837,   244,   681,   237,  -217,  -217,  -217,   125,   837,
     131,    91,   236,   299,  -217,  -217,   693,   837,   300,  -217,
    -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,  -217,
     837,  -217,   837,   837,   837,   837,   837,   837,   837,   837,
     837,   837,   837,   837,   837,   837,   837,   837,   837,   837,
     837,  -217,   837,  -217,   488,  -217,  -217,   930,   714,  -217,
      44,  -217,    94,  -217,  1286,  -217,   301,  -217,  -217,  -217,
    -217,  -217,    42,  -217,  -217,  -217,    15,   837,  -217,   241,
     525,  -217,   136,   139,   141,   243,   681,  -217,  -217,   154,
    -217,  1206,   158,  -217,   837,  -217,  -217,   156,  -217,   170,
    -217,  -217,  -217,  -217,  -217,   -15,   -15,    30,    30,   190,
     190,   190,   190,    41,    41,   175,   200,   185,   225,   257,
       4,  -217,  -217,  -217,   246,   247,  -217,   270,    94,  1249,
     726,  -217,  -217,  -217,   395,  -217,  -217,  -217,  -217,  -217,
     525,   525,   525,   837,   765,   525,  -217,  -217,   837,  -217,
     837,  -217,  -217,  -217,  -217,   249,  -217,   274,  -217,  -217,
     255,  -217,  -217,   165,   525,   167,  -217,  -217,  -217,  -217,
    -217,   525,   250,  -217,   525,  -217,  -217,  -217
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,   133,   104,    88,    89,    90,    91,    92,    94,    95,
      96,    97,   100,   101,    98,    99,   129,   130,    93,   108,
     109,     0,     0,   140,   211,     0,    78,    80,   102,     0,
     103,    82,     0,   132,     0,     0,   208,   210,   124,     0,
       0,   144,   142,   141,    76,     0,    84,    86,    79,    81,
     107,     0,    83,     0,   189,     0,   215,     0,     0,     0,
     131,     1,   209,     0,   127,     0,   125,   134,   145,   143,
      77,     0,     0,   213,     0,     0,   114,     0,   110,     0,
     116,     0,     2,     3,     4,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     193,     0,    22,    23,    24,    25,    26,    27,   185,     6,
      16,    28,     0,    30,    34,    37,    48,    45,    54,    50,
      52,    40,    56,    58,    60,    73,     0,   191,   174,   175,
       0,     0,   176,   177,   178,   179,    86,   190,   214,   153,
     139,   152,     0,   146,   148,     0,     2,   136,    28,    75,
       0,     0,     0,     0,   122,    85,     0,   169,    87,   212,
       0,   113,   106,   111,     0,     0,   117,   119,   115,   180,
     181,     0,     0,    20,     0,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,   204,   205,   206,     0,     0,
       0,   155,     0,     0,    12,    13,     0,     0,     0,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    62,
       0,    19,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   194,     0,   187,     0,   186,   192,     0,     0,   150,
     157,   151,   158,   137,     0,   138,     0,   135,   123,   128,
     126,   172,     0,   105,   120,   112,     0,     0,   182,     0,
       0,   184,     0,     0,     0,     0,     0,   203,   207,     0,
       5,     0,   157,   156,     0,    11,     8,     0,    14,     0,
      10,    61,    31,    32,    33,    35,    36,    38,    39,    43,
      44,    41,    42,    46,    47,    49,    51,    53,    55,    57,
       0,    74,   188,   165,     0,     0,   161,     0,   159,     0,
       0,   147,   149,   154,     0,   170,   118,   121,    21,   183,
       0,     0,     0,     0,     0,     0,    29,     9,     0,     7,
       0,   166,   160,   162,   167,     0,   163,     0,   171,   173,
     195,   197,   198,     0,     0,     0,   202,    15,    59,   168,
     164,     0,     0,   200,     0,   196,   199,   201
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -217,  -217,  -217,  -217,   -49,  -217,   -90,    33,    49,    89,
      36,   124,   128,   126,    57,   134,  -217,   -56,   -13,  -217,
     -68,   -47,     7,     0,  -217,   284,  -217,   -32,  -217,  -217,
     281,   -71,   -72,  -217,   108,  -217,   302,   215,   207,    -9,
     -29,    -3,  -217,   -57,  -217,   122,  -217,   198,  -120,  -216,
    -154,  -217,   -79,  -217,    84,   149,   239,  -172,  -217,  -217,
    -217,  -217,   336,  -217
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,   109,   110,   277,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   210,
     126,   150,    54,    55,    45,    46,    26,    27,    28,    29,
      77,    78,    79,   165,   166,    30,    65,    66,    31,    32,
      33,    34,    43,   304,   143,   144,   145,   192,   305,   242,
     158,   252,   127,   128,   129,    57,   131,   132,   133,   134,
     135,    35,    36,    37
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      25,   142,   251,   149,   161,    60,   163,    24,   168,    38,
     148,   266,    50,    40,   182,     1,    47,     1,     1,    76,
      42,   241,   211,     1,   308,    63,    48,    49,     1,   191,
     188,    52,   149,   190,   229,    25,   173,   175,   176,   148,
      69,   177,    24,    76,    76,    76,   136,     1,    76,   153,
     169,   153,   236,   219,   220,   212,   308,    64,   141,   157,
     213,   214,   136,   148,   137,    58,   154,    59,   248,    76,
     167,   273,   232,   170,    44,    22,    75,   237,    22,   238,
      67,   137,    23,    22,    23,    23,   330,   245,    22,   163,
      39,   246,   258,    51,   324,    23,   149,   164,   230,   261,
     191,   215,   216,   148,   190,   249,   190,   237,   149,   238,
     314,   262,   263,   264,    70,   148,    56,   254,   221,   222,
      71,   269,   282,   283,   284,    16,    17,   315,    76,   279,
     152,    73,   239,   231,   193,   194,   195,   137,   240,   232,
      76,   138,   171,   157,   255,   178,   146,    83,    84,    85,
     256,    86,    87,    23,   271,   236,   238,   309,   159,   310,
     339,    23,   300,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   319,   149,   278,   326,   217,   218,   268,   272,   148,
     196,   307,   197,   232,   198,   270,    74,   281,   179,   232,
     320,   149,   130,   321,   232,   322,   101,   232,   148,   232,
     317,    60,   102,   103,   104,   105,   106,   107,   325,   301,
     327,   271,   232,   238,   328,   148,   180,   156,    40,   352,
      41,   354,   181,   232,   240,   232,   329,   141,   232,   223,
     224,   340,   341,   342,   141,   183,   346,   167,   285,   286,
      68,   184,   335,   185,   149,   343,   345,   186,    80,   293,
     294,   148,   189,   337,   226,   353,   287,   288,   272,   225,
     227,   141,   355,   228,   348,   357,   289,   290,   291,   292,
      72,   148,    80,    80,    80,   243,   244,    80,   247,    81,
     257,    82,    83,    84,    85,   260,    86,    87,   265,   267,
     274,   157,   275,   280,   313,   318,   323,   351,    80,   141,
     331,   332,   356,   349,   295,   347,     2,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,   333,    88,    89,    90,
     350,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   298,   297,   296,   155,   160,   102,   103,   104,
     105,   106,   107,   299,   316,   151,   312,    80,   250,   234,
     259,    62,    53,   108,     0,     0,     0,     0,    81,    80,
      82,    83,    84,    85,     0,    86,    87,     0,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   146,    83,
      84,    85,     0,    86,    87,     2,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,     0,    88,    89,    90,     0,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,     0,     0,     0,     0,     0,   102,   103,   104,   105,
     106,   107,    81,   209,    82,    83,    84,    85,   101,    86,
      87,    53,   233,     0,   102,   103,   104,   105,   106,   107,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   156,
     338,     0,     0,     0,     0,     0,     0,     0,     0,    81,
       0,    82,    83,    84,    85,     0,    86,    87,     0,     0,
      88,    89,    90,     0,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,     0,     0,     0,     0,     0,
     102,   103,   104,   105,   106,   107,    81,     0,    82,    83,
      84,    85,     0,    86,    87,    53,   235,    88,    89,    90,
       0,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,     0,     0,     0,     0,     0,   102,   103,   104,
     105,   106,   107,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,   302,    88,    89,    90,     0,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,     0,
       0,     0,     0,     0,   102,   103,   104,   105,   106,   107,
     146,    83,    84,    85,     0,    86,    87,     0,     0,    53,
       0,     0,   146,    83,    84,    85,     0,    86,    87,     0,
       0,     0,     0,     0,     0,     2,     0,     0,     0,     0,
       0,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,   146,    83,    84,    85,     0,
      86,    87,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,     0,     0,     0,   102,   103,   104,   105,
     106,   107,   101,     0,     0,   147,     0,     0,   102,   103,
     104,   105,   106,   107,   146,    83,    84,    85,     0,    86,
      87,     0,     0,     0,     0,     0,   146,    83,    84,    85,
       0,    86,    87,     0,   187,   101,     0,     0,     0,     0,
       0,   102,   103,   104,   105,   106,   107,   146,    83,    84,
      85,     0,    86,    87,     0,     0,     0,     0,     0,   146,
      83,    84,    85,     0,    86,    87,     0,     0,     0,     0,
       0,     0,     0,   100,   101,     0,     0,     0,     0,     0,
     102,   103,   104,   105,   106,   107,   101,   276,     0,     0,
       0,     0,   102,   103,   104,   105,   106,   107,   146,    83,
      84,    85,     0,    86,    87,     0,     0,   101,     0,     0,
     306,     0,     0,   102,   103,   104,   105,   106,   107,   101,
       0,     0,   336,     0,     0,   102,   103,   104,   105,   106,
     107,   146,    83,    84,    85,     0,    86,    87,     0,     0,
       0,     0,     0,   146,    83,    84,    85,     0,    86,    87,
       0,     0,     0,     0,     0,     0,     0,     0,   101,   344,
       0,     0,     0,     0,   102,   103,   104,   105,   106,   107,
     146,    83,    84,    85,     0,    86,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   172,     0,     0,     0,     0,     0,   102,   103,   104,
     105,   106,   107,   174,     0,     0,     0,     0,     0,   102,
     103,   104,   105,   106,   107,    61,     0,     0,     1,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,     0,     0,     0,   102,   103,   104,   105,
     106,   107,     0,     2,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,     1,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,     0,    23,     0,     0,     2,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,     1,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   237,   303,   238,     0,     0,     0,     0,
      23,     0,     0,     2,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,   139,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,     0,    23,     0,     0,     2,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     2,
       0,     0,     0,     0,   140,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,     2,
       0,     0,     0,     0,     0,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     2,     0,     0,     0,     0,   162,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,     0,     0,     0,     0,     0,   253,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    72,    53,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    53,     2,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   271,
     303,   238,     0,     0,     0,     0,    23,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   334,     2,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,   311,     2,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21
};

static const yytype_int16 yycheck[] =
{
       0,    58,   156,    59,    76,    34,    77,     0,    80,     3,
      59,   183,     3,    22,    93,     3,    25,     3,     3,    51,
      23,   141,   112,     3,   240,    84,    26,    27,     3,   101,
      98,    31,    88,   101,    17,    35,    85,    86,    87,    88,
      43,    88,    35,    75,    76,    77,    55,     3,    80,    68,
      62,    68,   131,    12,    13,    70,   272,     3,    58,    72,
      75,    76,    71,   112,    57,    63,    85,    65,    85,   101,
      79,   191,    68,    85,    62,    63,    84,    63,    63,    65,
      64,    74,    70,    63,    70,    70,    82,    64,    63,   160,
      84,    68,   171,    84,   266,    70,   152,    82,    81,   178,
     172,    71,    72,   152,   172,   152,   174,    63,   164,    65,
      68,   179,   180,   181,    62,   164,    32,   164,    77,    78,
      68,   189,   212,   213,   214,    42,    43,    85,   160,   197,
      83,    47,   141,    62,     7,     8,     9,   130,   141,    68,
     172,    57,    82,   156,    62,    82,     3,     4,     5,     6,
      68,     8,     9,    70,    63,   234,    65,    63,    74,    65,
     314,    70,   230,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   260,   238,   196,   274,    10,    11,    62,   191,   238,
      63,   238,    65,    68,    67,    64,    47,   210,    63,    68,
      64,   257,    53,    64,    68,    64,    63,    68,   257,    68,
     257,   240,    69,    70,    71,    72,    73,    74,    64,   232,
      64,    63,    68,    65,    68,   274,    63,    84,   237,    64,
      23,    64,    63,    68,   237,    68,    66,   237,    68,    14,
      15,   320,   321,   322,   244,    63,   325,   256,   215,   216,
      43,     3,   309,    62,   310,   323,   324,    62,    51,   223,
     224,   310,    63,   310,    79,   344,   217,   218,   271,    69,
      80,   271,   351,    16,   330,   354,   219,   220,   221,   222,
      83,   330,    75,    76,    77,    64,    68,    80,    66,     1,
      82,     3,     4,     5,     6,    82,     8,     9,    54,    62,
      64,   314,     3,     3,     3,    64,    63,    52,   101,   309,
      64,    64,    62,    64,   225,   328,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    66,    49,    50,    51,
      66,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,   228,   227,   226,    71,    75,    69,    70,    71,
      72,    73,    74,   229,   256,    63,   244,   160,   153,   130,
     172,    35,    84,    85,    -1,    -1,    -1,    -1,     1,   172,
       3,     4,     5,     6,    -1,     8,     9,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     3,     4,
       5,     6,    -1,     8,     9,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,
      73,    74,     1,    83,     3,     4,     5,     6,    63,     8,
       9,    84,    85,    -1,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
      -1,     3,     4,     5,     6,    -1,     8,     9,    -1,    -1,
      49,    50,    51,    -1,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    -1,    -1,    -1,    -1,    -1,
      69,    70,    71,    72,    73,    74,     1,    -1,     3,     4,
       5,     6,    -1,     8,     9,    84,    85,    49,    50,    51,
      -1,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    85,    49,    50,    51,    -1,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    73,    74,
       3,     4,     5,     6,    -1,     8,     9,    -1,    -1,    84,
      -1,    -1,     3,     4,     5,     6,    -1,     8,     9,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,     3,     4,     5,     6,    -1,
       8,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,
      73,    74,    63,    -1,    -1,    66,    -1,    -1,    69,    70,
      71,    72,    73,    74,     3,     4,     5,     6,    -1,     8,
       9,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,     8,     9,    -1,    62,    63,    -1,    -1,    -1,    -1,
      -1,    69,    70,    71,    72,    73,    74,     3,     4,     5,
       6,    -1,     8,     9,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,     8,     9,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,
      69,    70,    71,    72,    73,    74,    63,    64,    -1,    -1,
      -1,    -1,    69,    70,    71,    72,    73,    74,     3,     4,
       5,     6,    -1,     8,     9,    -1,    -1,    63,    -1,    -1,
      66,    -1,    -1,    69,    70,    71,    72,    73,    74,    63,
      -1,    -1,    66,    -1,    -1,    69,    70,    71,    72,    73,
      74,     3,     4,     5,     6,    -1,     8,     9,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,     8,     9,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    73,    74,
       3,     4,     5,     6,    -1,     8,     9,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,
      72,    73,    74,    63,    -1,    -1,    -1,    -1,    -1,    69,
      70,    71,    72,    73,    74,     0,    -1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,
      73,    74,    -1,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,     3,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    64,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    28,
      -1,    -1,    -1,    -1,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    85,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    -1,    -1,    -1,    -1,    85,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    65,    -1,    -1,    -1,    -1,    70,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    63,    70,   108,   109,   112,   113,   114,   115,
     121,   124,   125,   126,   127,   147,   148,   149,     3,    84,
     125,   124,   127,   128,    62,   110,   111,   125,   109,   109,
       3,    84,   109,    84,   108,   109,   140,   141,    63,    65,
     126,     0,   148,    84,     3,   122,   123,    64,   124,   127,
      62,    68,    83,   140,   141,    84,   113,   116,   117,   118,
     124,     1,     3,     4,     5,     6,     8,     9,    49,    50,
      51,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    69,    70,    71,    72,    73,    74,    85,    87,
      88,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   106,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   125,   108,   140,     3,
      64,   109,   129,   130,   131,   132,     3,    66,    90,   103,
     107,   122,    83,    68,    85,   111,    84,   104,   136,   140,
     116,   118,    85,   117,    82,   119,   120,   125,   118,    62,
      85,    82,    63,    90,    63,    90,    90,   107,    82,    63,
      63,    63,   138,    63,     3,    62,    62,    62,   106,    63,
     106,   118,   133,     7,     8,     9,    63,    65,    67,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    83,
     105,    92,    70,    75,    76,    71,    72,    10,    11,    12,
      13,    77,    78,    14,    15,    69,    79,    80,    16,    17,
      81,    62,    68,    85,   142,    85,   138,    63,    65,   125,
     127,   134,   135,    64,    68,    64,    68,    66,    85,   107,
     123,   136,   137,    85,   107,    62,    68,    82,   138,   133,
      82,   138,   106,   106,   106,    54,   143,    62,    62,   106,
      64,    63,   127,   134,    64,     3,    64,    89,   104,   106,
       3,   104,    92,    92,    92,    93,    93,    94,    94,   100,
     100,   100,   100,    96,    96,    95,    98,    99,    97,   101,
     106,   104,    85,    64,   129,   134,    66,   107,   135,    63,
      65,    48,   131,     3,    68,    85,   120,   107,    64,   138,
      64,    64,    64,    63,   143,    64,    92,    64,    68,    66,
      82,    64,    64,    66,    64,   129,    66,   107,    85,   136,
     138,   138,   138,   106,    64,   106,   138,   104,   103,    64,
      66,    52,    64,   138,    64,   138,    62,   138
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    86,    87,    87,    87,    87,    88,    88,    88,    88,
      88,    88,    88,    88,    89,    89,    90,    90,    90,    90,
      90,    90,    91,    91,    91,    91,    91,    91,    92,    92,
      93,    93,    93,    93,    94,    94,    94,    95,    95,    95,
      96,    96,    96,    96,    96,    97,    97,    97,    98,    98,
      99,    99,   100,   100,   101,   101,   102,   102,   103,   103,
     104,   104,   105,   105,   105,   105,   105,   105,   105,   105,
     105,   105,   105,   106,   106,   107,   108,   108,   109,   109,
     109,   109,   109,   109,   110,   110,   111,   111,   112,   112,
     112,   112,   112,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   114,   114,   114,   115,   115,
     116,   116,   117,   118,   118,   118,   118,   119,   119,   120,
     120,   120,   121,   121,   121,   122,   122,   123,   123,   124,
     124,   125,   125,   126,   126,   126,   126,   126,   126,   126,
     127,   127,   127,   127,   128,   128,   129,   129,   130,   130,
     131,   131,   131,   132,   132,   133,   133,   134,   134,   134,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   136,
     136,   136,   137,   137,   138,   138,   138,   138,   138,   138,
     138,   138,   139,   139,   139,   140,   140,   140,   140,   141,
     141,   142,   142,   143,   143,   144,   144,   144,   145,   145,
     145,   145,   145,   146,   146,   146,   146,   146,   147,   147,
     148,   148,   149,   149,   149,   149
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     3,     1,     4,     3,     4,
       3,     3,     2,     2,     1,     3,     1,     2,     2,     2,
       2,     4,     1,     1,     1,     1,     1,     1,     1,     4,
       1,     3,     3,     3,     1,     3,     3,     1,     3,     3,
       1,     3,     3,     3,     3,     1,     3,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     5,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     1,     2,     3,     1,     2,
       1,     2,     1,     2,     1,     3,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     5,     4,     2,     1,     1,
       1,     2,     3,     2,     1,     2,     1,     1,     3,     1,
       2,     3,     4,     5,     2,     1,     3,     1,     3,     1,
       1,     2,     1,     1,     3,     4,     3,     4,     4,     3,
       1,     2,     2,     3,     1,     2,     1,     3,     1,     3,
       2,     2,     1,     1,     3,     1,     2,     1,     1,     2,
       3,     2,     3,     3,     4,     2,     3,     3,     4,     1,
       3,     4,     1,     3,     1,     1,     1,     1,     1,     1,
       2,     2,     3,     4,     3,     2,     3,     3,     4,     1,
       2,     1,     2,     1,     2,     5,     7,     5,     5,     7,
       6,     7,     5,     3,     2,     2,     2,     3,     1,     2,
       1,     1,     4,     3,     3,     2
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
        YY_LAC_DISCARD ("YYBACKUP");                              \
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


/* Given a state stack such that *YYBOTTOM is its bottom, such that
   *YYTOP is either its top or is YYTOP_EMPTY to indicate an empty
   stack, and such that *YYCAPACITY is the maximum number of elements it
   can hold without a reallocation, make sure there is enough room to
   store YYADD more elements.  If not, allocate a new stack using
   YYSTACK_ALLOC, copy the existing elements, and adjust *YYBOTTOM,
   *YYTOP, and *YYCAPACITY to reflect the new capacity and memory
   location.  If *YYBOTTOM != YYBOTTOM_NO_FREE, then free the old stack
   using YYSTACK_FREE.  Return 0 if successful or if no reallocation is
   required.  Return YYENOMEM if memory is exhausted.  */
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
          return YYENOMEM;
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
          return YYENOMEM;
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
#define YY_LAC_ESTABLISH                                                \
do {                                                                    \
  if (!yy_lac_established)                                              \
    {                                                                   \
      YYDPRINTF ((stderr,                                               \
                  "LAC: initial context established for %s\n",          \
                  yysymbol_name (yytoken)));                            \
      yy_lac_established = 1;                                           \
      switch (yy_lac (yyesa, &yyes, &yyes_capacity, yyssp, yytoken))    \
        {                                                               \
        case YYENOMEM:                                                  \
          YYNOMEM;                                                      \
        case 1:                                                         \
          goto yyerrlab;                                                \
        }                                                               \
    }                                                                   \
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
      YYDPRINTF ((stderr, "LAC: initial context discarded due to "       \
                  Event "\n"));                                          \
      yy_lac_established = 0;                                            \
    }                                                                    \
} while (0)
#else
# define YY_LAC_DISCARD(Event) yy_lac_established = 0
#endif

/* Given the stack whose top is *YYSSP, return 0 iff YYTOKEN can
   eventually (after perhaps some reductions) be shifted, return 1 if
   not, or return YYENOMEM if memory is exhausted.  As preconditions and
   postconditions: *YYES_CAPACITY is the allocated size of the array to
   which *YYES points, and either *YYES = YYESA or *YYES points to an
   array allocated with YYSTACK_ALLOC.  yy_lac may overwrite the
   contents of either array, alter *YYES and *YYES_CAPACITY, and free
   any old *YYES other than YYESA.  */
static int
yy_lac (yy_state_t *yyesa, yy_state_t **yyes,
        YYPTRDIFF_T *yyes_capacity, yy_state_t *yyssp, yysymbol_kind_t yytoken)
{
  yy_state_t *yyes_prev = yyssp;
  yy_state_t *yyesp = yyes_prev;
  /* Reduce until we encounter a shift and thereby accept the token.  */
  YYDPRINTF ((stderr, "LAC: checking lookahead %s:", yysymbol_name (yytoken)));
  if (yytoken == YYSYMBOL_YYUNDEF)
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
          /* Use the default action.  */
          yyrule = yydefact[+*yyesp];
          if (yyrule == 0)
            {
              YYDPRINTF ((stderr, " Err\n"));
              return 1;
            }
        }
      else
        {
          /* Use the action from yytable.  */
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
      /* By now we know we have to simulate a reduce.  */
      YYDPRINTF ((stderr, " R%d", yyrule - 1));
      {
        /* Pop the corresponding number of values from the stack.  */
        YYPTRDIFF_T yylen = yyr2[yyrule];
        /* First pop from the LAC stack as many tokens as possible.  */
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
                yyesp = yyes_prev;
                yylen -= yysize;
              }
          }
        /* Only afterwards look at the main stack.  */
        if (yylen)
          yyesp = yyes_prev -= yylen;
      }
      /* Push the resulting state of the reduction.  */
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
                return YYENOMEM;
              }
            YY_IGNORE_USELESS_CAST_BEGIN
            *++yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_END
          }
        YYDPRINTF ((stderr, " G%d", yystate));
      }
    }
}

/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yy_state_t *yyesa;
  yy_state_t **yyes;
  YYPTRDIFF_T *yyes_capacity;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;

  int yyx;
  for (yyx = 0; yyx < YYNTOKENS; ++yyx)
    {
      yysymbol_kind_t yysym = YY_CAST (yysymbol_kind_t, yyx);
      if (yysym != YYSYMBOL_YYerror && yysym != YYSYMBOL_YYUNDEF)
        switch (yy_lac (yyctx->yyesa, yyctx->yyes, yyctx->yyes_capacity, yyctx->yyssp, yysym))
          {
          case YYENOMEM:
            return YYENOMEM;
          case 1:
            continue;
          default:
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = yysym;
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
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
# endif
#endif



static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
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
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      YYDPRINTF ((stderr, "Constructing syntax error message\n"));
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else if (yyn == 0)
        YYDPRINTF ((stderr, "No expected tokens.\n"));
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.  In order to see if a particular token T is a
   valid looakhead, invoke yy_lac (YYESA, YYES, YYES_CAPACITY, YYSSP, T).

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store or if
   yy_lac returned YYENOMEM.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
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
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yystrlen (yysymbol_name (yyarg[yyi]));
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
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
          yyp = yystpcpy (yyp, yysymbol_name (yyarg[yyi++]));
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

    yy_state_t yyesa[20];
    yy_state_t *yyes = yyesa;
    YYPTRDIFF_T yyes_capacity = 20 < YYMAXDEPTH ? 20 : YYMAXDEPTH;

  /* Whether LAC context is established.  A Boolean.  */
  int yy_lac_established = 0;
  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

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
    {
      YY_LAC_ESTABLISH;
      goto yydefault;
    }
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      YY_LAC_ESTABLISH;
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


  YY_REDUCE_PRINT (yyn);
  {
    int yychar_backup = yychar;
    switch (yyn)
      {
  case 2: /* primary_expression: IDENTIFIER  */
#line 77 "parser.y"
                 {
    	(yyval.ptr) = createASTNode((yyvsp[0].str));
    }
#line 2174 "parser.cpp"
    break;

  case 3: /* primary_expression: CONSTANT  */
#line 80 "parser.y"
                        {
		(yyval.ptr) = createASTNode((yyvsp[0].str));
	}
#line 2182 "parser.cpp"
    break;

  case 4: /* primary_expression: STRING_LITERAL  */
#line 83 "parser.y"
                         {
		(yyval.ptr) = createASTNode((yyvsp[0].str));
	}
#line 2190 "parser.cpp"
    break;

  case 5: /* primary_expression: '(' expression ')'  */
#line 86 "parser.y"
                             {
		(yyval.ptr) = (yyvsp[-1].ptr);
	}
#line 2198 "parser.cpp"
    break;

  case 6: /* postfix_expression: primary_expression  */
#line 92 "parser.y"
                             {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 2206 "parser.cpp"
    break;

  case 7: /* postfix_expression: postfix_expression '[' expression ']'  */
#line 95 "parser.y"
                                                {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-3].ptr), "", 1);
		insertAttr(attr, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createASTNode("postfix_expression", &attr);
	}
#line 2217 "parser.cpp"
    break;

  case 8: /* postfix_expression: postfix_expression '(' ')'  */
#line 101 "parser.y"
                                     {
		(yyval.ptr) = (yyvsp[-2].ptr);
	}
#line 2225 "parser.cpp"
    break;

  case 9: /* postfix_expression: postfix_expression '(' argument_expression_list ')'  */
#line 104 "parser.y"
                                                              {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-3].ptr), "", 1);
		insertAttr(attr, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createASTNode("postfix_expression", &attr);
	}
#line 2236 "parser.cpp"
    break;

  case 10: /* postfix_expression: postfix_expression '.' IDENTIFIER  */
#line 110 "parser.y"
                                            {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, createASTNode((yyvsp[0].str)), "", 1);
		(yyval.ptr) = createASTNode("expression.id", &attr);
	}
#line 2247 "parser.cpp"
    break;

  case 11: /* postfix_expression: postfix_expression PTR_OP IDENTIFIER  */
#line 116 "parser.y"
                                               {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, createASTNode((yyvsp[0].str)), "", 1);
		(yyval.ptr) = createASTNode((yyvsp[-1].str), &attr);
	}
#line 2258 "parser.cpp"
    break;

  case 12: /* postfix_expression: postfix_expression INC_OP  */
#line 122 "parser.y"
                                    {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createASTNode((yyvsp[0].str), &attr);
	}
#line 2268 "parser.cpp"
    break;

  case 13: /* postfix_expression: postfix_expression DEC_OP  */
#line 127 "parser.y"
                                    {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createASTNode((yyvsp[0].str), &attr);
	}
#line 2278 "parser.cpp"
    break;

  case 14: /* argument_expression_list: assignment_expression  */
#line 135 "parser.y"
                                {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 2286 "parser.cpp"
    break;

  case 15: /* argument_expression_list: argument_expression_list ',' assignment_expression  */
#line 138 "parser.y"
                                                             {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("argument_list", &attr);
	}
#line 2297 "parser.cpp"
    break;

  case 16: /* unary_expression: postfix_expression  */
#line 147 "parser.y"
                             {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 2305 "parser.cpp"
    break;

  case 17: /* unary_expression: INC_OP unary_expression  */
#line 150 "parser.y"
                                  {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode((yyvsp[-1].str), &attr);
	}
#line 2315 "parser.cpp"
    break;

  case 18: /* unary_expression: DEC_OP unary_expression  */
#line 155 "parser.y"
                                  {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode((yyvsp[-1].str), &attr);
	}
#line 2325 "parser.cpp"
    break;

  case 19: /* unary_expression: unary_operator cast_expression  */
#line 160 "parser.y"
                                         {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-1].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("unary_exp", &attr);
	}
#line 2336 "parser.cpp"
    break;

  case 20: /* unary_expression: SIZEOF unary_expression  */
#line 166 "parser.y"
                                  {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode((yyvsp[-1].str), &attr);
	}
#line 2346 "parser.cpp"
    break;

  case 21: /* unary_expression: SIZEOF '(' type_name ')'  */
#line 171 "parser.y"
                                   {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createASTNode((yyvsp[-3].str), &attr);
	}
#line 2356 "parser.cpp"
    break;

  case 22: /* unary_operator: '&'  */
#line 179 "parser.y"
              {
		(yyval.ptr) = createASTNode("&");
	}
#line 2364 "parser.cpp"
    break;

  case 23: /* unary_operator: '*'  */
#line 182 "parser.y"
              {
		(yyval.ptr) = createASTNode("*");
	}
#line 2372 "parser.cpp"
    break;

  case 24: /* unary_operator: '+'  */
#line 185 "parser.y"
              {
		(yyval.ptr) = createASTNode("+");
	}
#line 2380 "parser.cpp"
    break;

  case 25: /* unary_operator: '-'  */
#line 188 "parser.y"
              {
		(yyval.ptr) = createASTNode("-");
	}
#line 2388 "parser.cpp"
    break;

  case 26: /* unary_operator: '~'  */
#line 191 "parser.y"
              {
		(yyval.ptr) = createASTNode("~");
	}
#line 2396 "parser.cpp"
    break;

  case 27: /* unary_operator: '!'  */
#line 194 "parser.y"
              {
		(yyval.ptr) = createASTNode("!");
	}
#line 2404 "parser.cpp"
    break;

  case 28: /* cast_expression: unary_expression  */
#line 200 "parser.y"
                           {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 2412 "parser.cpp"
    break;

  case 29: /* cast_expression: '(' type_name ')' cast_expression  */
#line 203 "parser.y"
                                            {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("cast_expression", &attr);
	}
#line 2423 "parser.cpp"
    break;

  case 30: /* multiplicative_expression: cast_expression  */
#line 212 "parser.y"
                          {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 2431 "parser.cpp"
    break;

  case 31: /* multiplicative_expression: multiplicative_expression '*' cast_expression  */
#line 215 "parser.y"
                                                        {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("*", &attr);
	}
#line 2442 "parser.cpp"
    break;

  case 32: /* multiplicative_expression: multiplicative_expression '/' cast_expression  */
#line 221 "parser.y"
                                                        {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("/", &attr);
	}
#line 2453 "parser.cpp"
    break;

  case 33: /* multiplicative_expression: multiplicative_expression '%' cast_expression  */
#line 227 "parser.y"
                                                        {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("%", &attr);
	}
#line 2464 "parser.cpp"
    break;

  case 34: /* additive_expression: multiplicative_expression  */
#line 236 "parser.y"
                                    {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 2472 "parser.cpp"
    break;

  case 35: /* additive_expression: additive_expression '+' multiplicative_expression  */
#line 239 "parser.y"
                                                            {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("+", &attr);
	}
#line 2483 "parser.cpp"
    break;

  case 36: /* additive_expression: additive_expression '-' multiplicative_expression  */
#line 245 "parser.y"
                                                            {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("-", &attr);
	}
#line 2494 "parser.cpp"
    break;

  case 37: /* shift_expression: additive_expression  */
#line 254 "parser.y"
                              {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 2502 "parser.cpp"
    break;

  case 38: /* shift_expression: shift_expression LEFT_OP additive_expression  */
#line 257 "parser.y"
                                                       {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode((yyvsp[-1].str), &attr);
	}
#line 2513 "parser.cpp"
    break;

  case 39: /* shift_expression: shift_expression RIGHT_OP additive_expression  */
#line 263 "parser.y"
                                                        {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode((yyvsp[-1].str), &attr);
	}
#line 2524 "parser.cpp"
    break;

  case 40: /* relational_expression: inclusive_or_expression  */
#line 272 "parser.y"
                                  {
		 (yyval.ptr) = (yyvsp[0].ptr);
	}
#line 2532 "parser.cpp"
    break;

  case 41: /* relational_expression: relational_expression '<' inclusive_or_expression  */
#line 275 "parser.y"
                                                            {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("<", &attr);
	}
#line 2543 "parser.cpp"
    break;

  case 42: /* relational_expression: relational_expression '>' inclusive_or_expression  */
#line 281 "parser.y"
                                                            {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode(">", &attr);
	}
#line 2554 "parser.cpp"
    break;

  case 43: /* relational_expression: relational_expression LE_OP inclusive_or_expression  */
#line 287 "parser.y"
                                                              {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode((yyvsp[-1].str), &attr);
	}
#line 2565 "parser.cpp"
    break;

  case 44: /* relational_expression: relational_expression GE_OP inclusive_or_expression  */
#line 293 "parser.y"
                                                              {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode((yyvsp[-1].str), &attr);
	}
#line 2576 "parser.cpp"
    break;

  case 45: /* equality_expression: relational_expression  */
#line 302 "parser.y"
                                { 
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 2584 "parser.cpp"
    break;

  case 46: /* equality_expression: equality_expression EQ_OP relational_expression  */
#line 305 "parser.y"
                                                          {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode((yyvsp[-1].str), &attr);
	}
#line 2595 "parser.cpp"
    break;

  case 47: /* equality_expression: equality_expression NE_OP relational_expression  */
#line 311 "parser.y"
                                                          {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode((yyvsp[-1].str), &attr);
	}
#line 2606 "parser.cpp"
    break;

  case 48: /* and_expression: shift_expression  */
#line 320 "parser.y"
                           {	
		(yyval.ptr) = (yyvsp[0].ptr); 
	}
#line 2614 "parser.cpp"
    break;

  case 49: /* and_expression: and_expression '&' shift_expression  */
#line 323 "parser.y"
                                             {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("&", &attr);
	}
#line 2625 "parser.cpp"
    break;

  case 50: /* exclusive_or_expression: and_expression  */
#line 332 "parser.y"
                         {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 2633 "parser.cpp"
    break;

  case 51: /* exclusive_or_expression: exclusive_or_expression '^' and_expression  */
#line 335 "parser.y"
                                                    {
        std::vector<Data> attr;
        insertAttr(attr, (yyvsp[-2].ptr), "", 1);
        insertAttr(attr, (yyvsp[0].ptr), "", 1);
        (yyval.ptr) = createASTNode("^", &attr);
    }
#line 2644 "parser.cpp"
    break;

  case 52: /* inclusive_or_expression: exclusive_or_expression  */
#line 344 "parser.y"
                                  {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2650 "parser.cpp"
    break;

  case 53: /* inclusive_or_expression: inclusive_or_expression '|' exclusive_or_expression  */
#line 345 "parser.y"
                                                             {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("|", &attr);
	}
#line 2661 "parser.cpp"
    break;

  case 54: /* logical_and_expression: equality_expression  */
#line 354 "parser.y"
                              {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2667 "parser.cpp"
    break;

  case 55: /* logical_and_expression: logical_and_expression AND_OP equality_expression  */
#line 355 "parser.y"
                                                           {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("&&", &attr);
	}
#line 2678 "parser.cpp"
    break;

  case 56: /* logical_or_expression: logical_and_expression  */
#line 364 "parser.y"
                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2684 "parser.cpp"
    break;

  case 57: /* logical_or_expression: logical_or_expression OR_OP logical_and_expression  */
#line 365 "parser.y"
                                                            {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("||", &attr);
	}
#line 2695 "parser.cpp"
    break;

  case 58: /* conditional_expression: logical_or_expression  */
#line 374 "parser.y"
                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2701 "parser.cpp"
    break;

  case 59: /* conditional_expression: logical_or_expression '?' expression ':' conditional_expression  */
#line 375 "parser.y"
                                                                         {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-4].ptr), "", 1);
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("ternary operator", &attr);
	}
#line 2713 "parser.cpp"
    break;

  case 60: /* assignment_expression: conditional_expression  */
#line 385 "parser.y"
                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2719 "parser.cpp"
    break;

  case 61: /* assignment_expression: unary_expression assignment_operator assignment_expression  */
#line 386 "parser.y"
                                                                    {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode((yyvsp[-1].str), &attr);
	}
#line 2730 "parser.cpp"
    break;

  case 62: /* assignment_operator: '='  */
#line 395 "parser.y"
                {
		(yyval.str) = strdup("=");
	}
#line 2738 "parser.cpp"
    break;

  case 63: /* assignment_operator: MUL_ASSIGN  */
#line 398 "parser.y"
                                {(yyval.str) = (yyvsp[0].str);}
#line 2744 "parser.cpp"
    break;

  case 64: /* assignment_operator: DIV_ASSIGN  */
#line 399 "parser.y"
                                {(yyval.str) = (yyvsp[0].str);}
#line 2750 "parser.cpp"
    break;

  case 65: /* assignment_operator: MOD_ASSIGN  */
#line 400 "parser.y"
                                {(yyval.str) = (yyvsp[0].str);}
#line 2756 "parser.cpp"
    break;

  case 66: /* assignment_operator: ADD_ASSIGN  */
#line 401 "parser.y"
                                {(yyval.str) = (yyvsp[0].str);}
#line 2762 "parser.cpp"
    break;

  case 67: /* assignment_operator: SUB_ASSIGN  */
#line 402 "parser.y"
                                {(yyval.str) = (yyvsp[0].str);}
#line 2768 "parser.cpp"
    break;

  case 68: /* assignment_operator: LEFT_ASSIGN  */
#line 403 "parser.y"
                                {(yyval.str) = (yyvsp[0].str);}
#line 2774 "parser.cpp"
    break;

  case 69: /* assignment_operator: RIGHT_ASSIGN  */
#line 404 "parser.y"
                                {(yyval.str) = (yyvsp[0].str);}
#line 2780 "parser.cpp"
    break;

  case 70: /* assignment_operator: AND_ASSIGN  */
#line 405 "parser.y"
                                {(yyval.str) = (yyvsp[0].str);}
#line 2786 "parser.cpp"
    break;

  case 71: /* assignment_operator: XOR_ASSIGN  */
#line 406 "parser.y"
                                {(yyval.str) = (yyvsp[0].str);}
#line 2792 "parser.cpp"
    break;

  case 72: /* assignment_operator: OR_ASSIGN  */
#line 407 "parser.y"
                                        {(yyval.str) = (yyvsp[0].str);}
#line 2798 "parser.cpp"
    break;

  case 73: /* expression: assignment_expression  */
#line 411 "parser.y"
                               {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2804 "parser.cpp"
    break;

  case 74: /* expression: expression ',' assignment_expression  */
#line 412 "parser.y"
                                              {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("expression", &attr);
	}
#line 2815 "parser.cpp"
    break;

  case 75: /* constant_expression: conditional_expression  */
#line 421 "parser.y"
                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2821 "parser.cpp"
    break;

  case 76: /* declaration: declaration_specifiers ';'  */
#line 425 "parser.y"
                                    { (yyval.ptr) = (yyvsp[-1].ptr); }
#line 2827 "parser.cpp"
    break;

  case 77: /* declaration: declaration_specifiers init_declarator_list ';'  */
#line 426 "parser.y"
                                                         {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createASTNode("declaration", &attr);
	}
#line 2838 "parser.cpp"
    break;

  case 78: /* declaration_specifiers: storage_class_specifier  */
#line 435 "parser.y"
                                  { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2844 "parser.cpp"
    break;

  case 79: /* declaration_specifiers: storage_class_specifier declaration_specifiers  */
#line 436 "parser.y"
                                                        {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-1].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("declaration_specifiers", &attr);
	}
#line 2855 "parser.cpp"
    break;

  case 80: /* declaration_specifiers: type_specifier  */
#line 442 "parser.y"
                        { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2861 "parser.cpp"
    break;

  case 81: /* declaration_specifiers: type_specifier declaration_specifiers  */
#line 443 "parser.y"
                                               {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-1].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("declaration_specifiers", &attr);
	}
#line 2872 "parser.cpp"
    break;

  case 82: /* declaration_specifiers: type_qualifier  */
#line 449 "parser.y"
                        { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2878 "parser.cpp"
    break;

  case 83: /* declaration_specifiers: type_qualifier declaration_specifiers  */
#line 450 "parser.y"
                                               {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-1].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("declaration_specifiers", &attr);
	}
#line 2889 "parser.cpp"
    break;

  case 84: /* init_declarator_list: init_declarator  */
#line 459 "parser.y"
                         {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2895 "parser.cpp"
    break;

  case 85: /* init_declarator_list: init_declarator_list ',' init_declarator  */
#line 460 "parser.y"
                                                  {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("init_declarator_list", &attr);
	}
#line 2906 "parser.cpp"
    break;

  case 86: /* init_declarator: declarator  */
#line 469 "parser.y"
                     {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2912 "parser.cpp"
    break;

  case 87: /* init_declarator: declarator '=' initializer  */
#line 470 "parser.y"
                                    {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("=", &v);
	}
#line 2923 "parser.cpp"
    break;

  case 88: /* storage_class_specifier: TYPEDEF  */
#line 479 "parser.y"
                        { (yyval.ptr) = createASTNode((yyvsp[0].str));}
#line 2929 "parser.cpp"
    break;

  case 89: /* storage_class_specifier: EXTERN  */
#line 480 "parser.y"
                        { (yyval.ptr) = createASTNode((yyvsp[0].str));}
#line 2935 "parser.cpp"
    break;

  case 90: /* storage_class_specifier: STATIC  */
#line 481 "parser.y"
                        { (yyval.ptr) = createASTNode((yyvsp[0].str));}
#line 2941 "parser.cpp"
    break;

  case 91: /* storage_class_specifier: AUTO  */
#line 482 "parser.y"
                        { (yyval.ptr) = createASTNode((yyvsp[0].str));}
#line 2947 "parser.cpp"
    break;

  case 92: /* storage_class_specifier: REGISTER  */
#line 483 "parser.y"
                        { (yyval.ptr) = createASTNode((yyvsp[0].str));}
#line 2953 "parser.cpp"
    break;

  case 93: /* type_specifier: VOID  */
#line 487 "parser.y"
                                {(yyval.ptr) = createASTNode((yyvsp[0].str));}
#line 2959 "parser.cpp"
    break;

  case 94: /* type_specifier: CHAR  */
#line 488 "parser.y"
                                {(yyval.ptr) = createASTNode((yyvsp[0].str));}
#line 2965 "parser.cpp"
    break;

  case 95: /* type_specifier: SHORT  */
#line 489 "parser.y"
                                {(yyval.ptr) = createASTNode((yyvsp[0].str));}
#line 2971 "parser.cpp"
    break;

  case 96: /* type_specifier: INT  */
#line 490 "parser.y"
                                {(yyval.ptr) = createASTNode((yyvsp[0].str));}
#line 2977 "parser.cpp"
    break;

  case 97: /* type_specifier: LONG  */
#line 491 "parser.y"
                                {(yyval.ptr) = createASTNode((yyvsp[0].str));}
#line 2983 "parser.cpp"
    break;

  case 98: /* type_specifier: FLOAT  */
#line 492 "parser.y"
                                {(yyval.ptr) = createASTNode((yyvsp[0].str));}
#line 2989 "parser.cpp"
    break;

  case 99: /* type_specifier: DOUBLE  */
#line 493 "parser.y"
                                {(yyval.ptr) = createASTNode((yyvsp[0].str));}
#line 2995 "parser.cpp"
    break;

  case 100: /* type_specifier: SIGNED  */
#line 494 "parser.y"
                                {(yyval.ptr) = createASTNode((yyvsp[0].str));}
#line 3001 "parser.cpp"
    break;

  case 101: /* type_specifier: UNSIGNED  */
#line 495 "parser.y"
                                {(yyval.ptr) = createASTNode((yyvsp[0].str));}
#line 3007 "parser.cpp"
    break;

  case 102: /* type_specifier: struct_or_union_specifier  */
#line 496 "parser.y"
                                        {(yyval.ptr) = (yyvsp[0].ptr);}
#line 3013 "parser.cpp"
    break;

  case 103: /* type_specifier: enum_specifier  */
#line 497 "parser.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 3019 "parser.cpp"
    break;

  case 104: /* type_specifier: TYPE_NAME  */
#line 498 "parser.y"
                                {(yyval.ptr) = createASTNode((yyvsp[0].str));}
#line 3025 "parser.cpp"
    break;

  case 105: /* struct_or_union_specifier: struct_or_union IDENTIFIER '{' struct_declaration_list '}'  */
#line 502 "parser.y"
                                                                        {
		std::vector<Data> v;
		insertAttr(v, createASTNode((yyvsp[-3].str)), "", 1);
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createASTNode((yyvsp[-4].str), &v);
	}
#line 3036 "parser.cpp"
    break;

  case 106: /* struct_or_union_specifier: struct_or_union '{' struct_declaration_list '}'  */
#line 508 "parser.y"
                                                                        {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createASTNode((yyvsp[-3].str), &v);
	}
#line 3046 "parser.cpp"
    break;

  case 107: /* struct_or_union_specifier: struct_or_union IDENTIFIER  */
#line 513 "parser.y"
                                        {
		std::vector<Data> v;
		insertAttr(v, createASTNode((yyvsp[0].str)), "", 1);
		(yyval.ptr) = createASTNode((yyvsp[-1].str), &v);
	}
#line 3056 "parser.cpp"
    break;

  case 108: /* struct_or_union: STRUCT  */
#line 521 "parser.y"
                        {(yyval.str) = (yyvsp[0].str);}
#line 3062 "parser.cpp"
    break;

  case 109: /* struct_or_union: UNION  */
#line 522 "parser.y"
                        {(yyval.str) = (yyvsp[0].str);}
#line 3068 "parser.cpp"
    break;

  case 110: /* struct_declaration_list: struct_declaration  */
#line 526 "parser.y"
                                { (yyval.ptr) = (yyvsp[0].ptr) ;}
#line 3074 "parser.cpp"
    break;

  case 111: /* struct_declaration_list: struct_declaration_list struct_declaration  */
#line 527 "parser.y"
                                                        {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("struct_declaration_list", &v);
	}
#line 3085 "parser.cpp"
    break;

  case 112: /* struct_declaration: specifier_qualifier_list struct_declarator_list ';'  */
#line 536 "parser.y"
                                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createASTNode("struct_declaration", &v);
	}
#line 3096 "parser.cpp"
    break;

  case 113: /* specifier_qualifier_list: type_specifier specifier_qualifier_list  */
#line 545 "parser.y"
                                                        {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("specifier_qualifier_list", &v);
	}
#line 3107 "parser.cpp"
    break;

  case 114: /* specifier_qualifier_list: type_specifier  */
#line 551 "parser.y"
                                { (yyval.ptr) = (yyvsp[0].ptr); }
#line 3113 "parser.cpp"
    break;

  case 115: /* specifier_qualifier_list: type_qualifier specifier_qualifier_list  */
#line 552 "parser.y"
                                                        {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("specifier_qualifier_list", &v);
	}
#line 3124 "parser.cpp"
    break;

  case 116: /* specifier_qualifier_list: type_qualifier  */
#line 558 "parser.y"
                                { (yyval.ptr) = (yyvsp[0].ptr); }
#line 3130 "parser.cpp"
    break;

  case 117: /* struct_declarator_list: struct_declarator  */
#line 562 "parser.y"
                            { (yyval.ptr) = (yyvsp[0].ptr); }
#line 3136 "parser.cpp"
    break;

  case 118: /* struct_declarator_list: struct_declarator_list ',' struct_declarator  */
#line 563 "parser.y"
                                                       {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("struct_declarator_list", &v);
	}
#line 3147 "parser.cpp"
    break;

  case 119: /* struct_declarator: declarator  */
#line 572 "parser.y"
                        { (yyval.ptr) = (yyvsp[0].ptr); }
#line 3153 "parser.cpp"
    break;

  case 120: /* struct_declarator: ':' constant_expression  */
#line 573 "parser.y"
                                        { (yyval.ptr) = (yyvsp[0].ptr); }
#line 3159 "parser.cpp"
    break;

  case 121: /* struct_declarator: declarator ':' constant_expression  */
#line 574 "parser.y"
                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode(":", &v);
	}
#line 3170 "parser.cpp"
    break;

  case 122: /* enum_specifier: ENUM '{' enumerator_list '}'  */
#line 583 "parser.y"
                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createASTNode((yyvsp[-3].str), &v);
	}
#line 3180 "parser.cpp"
    break;

  case 123: /* enum_specifier: ENUM IDENTIFIER '{' enumerator_list '}'  */
#line 588 "parser.y"
                                                        {
		std::vector<Data> v;
		insertAttr(v, createASTNode((yyvsp[-3].str)), "", 1);
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createASTNode((yyvsp[-4].str), &v);
	}
#line 3191 "parser.cpp"
    break;

  case 124: /* enum_specifier: ENUM IDENTIFIER  */
#line 594 "parser.y"
                          {
		std::vector<Data> v;
		insertAttr(v, createASTNode((yyvsp[0].str)), "", 1);
		(yyval.ptr) = createASTNode((yyvsp[-1].str), &v);
	}
#line 3201 "parser.cpp"
    break;

  case 125: /* enumerator_list: enumerator  */
#line 602 "parser.y"
                        { (yyval.ptr) = (yyvsp[0].ptr); }
#line 3207 "parser.cpp"
    break;

  case 126: /* enumerator_list: enumerator_list ',' enumerator  */
#line 603 "parser.y"
                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("enumerator_list", &v);
	}
#line 3218 "parser.cpp"
    break;

  case 127: /* enumerator: IDENTIFIER  */
#line 612 "parser.y"
                        { (yyval.ptr) = createASTNode((yyvsp[0].str)); }
#line 3224 "parser.cpp"
    break;

  case 128: /* enumerator: IDENTIFIER '=' constant_expression  */
#line 613 "parser.y"
                                                {
		std::vector<Data> v;
		insertAttr(v, createASTNode((yyvsp[-2].str)), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("=", &v);
	}
#line 3235 "parser.cpp"
    break;

  case 129: /* type_qualifier: CONST  */
#line 622 "parser.y"
                        { (yyval.ptr) = createASTNode((yyvsp[0].str)); }
#line 3241 "parser.cpp"
    break;

  case 130: /* type_qualifier: VOLATILE  */
#line 623 "parser.y"
                        { (yyval.ptr) = createASTNode((yyvsp[0].str)); }
#line 3247 "parser.cpp"
    break;

  case 131: /* declarator: pointer direct_declarator  */
#line 628 "parser.y"
                                   {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("declarator", &v);
	}
#line 3258 "parser.cpp"
    break;

  case 132: /* declarator: direct_declarator  */
#line 634 "parser.y"
                            {
		(yyval.ptr) = (yyvsp[0].ptr) ;
	}
#line 3266 "parser.cpp"
    break;

  case 133: /* direct_declarator: IDENTIFIER  */
#line 641 "parser.y"
                     {
		(yyval.ptr) = createASTNode((yyvsp[0].str));
	}
#line 3274 "parser.cpp"
    break;

  case 134: /* direct_declarator: '(' declarator ')'  */
#line 644 "parser.y"
                              {
		(yyval.ptr) = (yyvsp[-1].ptr) ;
	}
#line 3282 "parser.cpp"
    break;

  case 135: /* direct_declarator: direct_declarator '[' constant_expression ']'  */
#line 647 "parser.y"
                                                       {
		std::vector<Data> v, v2;
		insertAttr(v2, (yyvsp[-1].ptr), "", 1);
		Node* node = createASTNode("[ ]", &v2);
		insertAttr(v, (yyvsp[-3].ptr), "", 1);
		insertAttr(v, node, "", 1);
		(yyval.ptr) = createASTNode("direct_declarator", &v);
	}
#line 3295 "parser.cpp"
    break;

  case 136: /* direct_declarator: direct_declarator '[' ']'  */
#line 655 "parser.y"
                                   {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, NULL, "[ ]", 0);
		(yyval.ptr) = createASTNode("direct_declarator", &v);
	}
#line 3306 "parser.cpp"
    break;

  case 137: /* direct_declarator: direct_declarator '(' parameter_type_list ')'  */
#line 661 "parser.y"
                                                       {
		std::vector<Data> v, v2;
		insertAttr(v2, (yyvsp[-1].ptr), "", 1);
		Node* node = createASTNode("( )", &v2);
		insertAttr(v, (yyvsp[-3].ptr), "", 1);
		insertAttr(v, node, "", 1);
		(yyval.ptr) = createASTNode("direct_declarator", &v);
	}
#line 3319 "parser.cpp"
    break;

  case 138: /* direct_declarator: direct_declarator '(' identifier_list ')'  */
#line 669 "parser.y"
                                                   {
		std::vector<Data> v, v2;
		insertAttr(v2, (yyvsp[-1].ptr), "", 1);
		Node* node = createASTNode("( )", &v2);
		insertAttr(v, (yyvsp[-3].ptr), "", 1);
		insertAttr(v, node, "", 1);
		(yyval.ptr) = createASTNode("direct_declarator", &v);
	}
#line 3332 "parser.cpp"
    break;

  case 139: /* direct_declarator: direct_declarator '(' ')'  */
#line 677 "parser.y"
                                   {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, NULL, "( )", 0);
		(yyval.ptr) = createASTNode("direct_declarator", &v);
	}
#line 3343 "parser.cpp"
    break;

  case 140: /* pointer: '*'  */
#line 686 "parser.y"
              {
		(yyval.ptr) = createASTNode("*(Pointer)");
	}
#line 3351 "parser.cpp"
    break;

  case 141: /* pointer: '*' type_qualifier_list  */
#line 689 "parser.y"
                                 {
		std::vector<Data> v;
		insertAttr(v,(yyvsp[0].ptr),"",1);
		(yyval.ptr) = createASTNode("*(Pointer)",&v);
	}
#line 3361 "parser.cpp"
    break;

  case 142: /* pointer: '*' pointer  */
#line 694 "parser.y"
                     {
		std::vector<Data> v;
		insertAttr(v,(yyvsp[0].ptr),"",1);
		(yyval.ptr) = createASTNode("*(Pointer)",&v);
	}
#line 3371 "parser.cpp"
    break;

  case 143: /* pointer: '*' type_qualifier_list pointer  */
#line 699 "parser.y"
                                         {
		std::vector<Data> v;
		insertAttr(v,(yyvsp[-1].ptr),"",1);
		insertAttr(v,(yyvsp[0].ptr),"",1);
		(yyval.ptr) = createASTNode("*(Pointer)",&v);
	}
#line 3382 "parser.cpp"
    break;

  case 144: /* type_qualifier_list: type_qualifier  */
#line 708 "parser.y"
                         {
		(yyval.ptr) = (yyvsp[0].ptr) ;
	}
#line 3390 "parser.cpp"
    break;

  case 145: /* type_qualifier_list: type_qualifier_list type_qualifier  */
#line 711 "parser.y"
                                            {
		std::vector<Data> v;
		insertAttr(v,(yyvsp[-1].ptr),"",1);
		insertAttr(v,(yyvsp[0].ptr),"",1);
		(yyval.ptr) = createASTNode("type_qualifier_list",&v);
	}
#line 3401 "parser.cpp"
    break;

  case 146: /* parameter_type_list: parameter_list  */
#line 721 "parser.y"
                         {
		(yyval.ptr) = (yyvsp[0].ptr) ;
	}
#line 3409 "parser.cpp"
    break;

  case 147: /* parameter_type_list: parameter_list ',' ELLIPSIS  */
#line 724 "parser.y"
                                     {
		std::vector<Data> v;
		insertAttr(v,(yyvsp[-2].ptr),"",1);
		insertAttr(v, createASTNode((yyvsp[0].str)), "", 1);
		(yyval.ptr) = createASTNode("parameter_type_list",&v);
	}
#line 3420 "parser.cpp"
    break;

  case 148: /* parameter_list: parameter_declaration  */
#line 733 "parser.y"
                               {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 3428 "parser.cpp"
    break;

  case 149: /* parameter_list: parameter_list ',' parameter_declaration  */
#line 736 "parser.y"
                                                  {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("parameter_list",&v);
	}
#line 3439 "parser.cpp"
    break;

  case 150: /* parameter_declaration: declaration_specifiers declarator  */
#line 745 "parser.y"
                                           {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("parameter_declaration",&v);
	}
#line 3450 "parser.cpp"
    break;

  case 151: /* parameter_declaration: declaration_specifiers abstract_declarator  */
#line 751 "parser.y"
                                                    {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("parameter_declaration",&v);
	}
#line 3461 "parser.cpp"
    break;

  case 152: /* parameter_declaration: declaration_specifiers  */
#line 757 "parser.y"
                                 {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 3469 "parser.cpp"
    break;

  case 153: /* identifier_list: IDENTIFIER  */
#line 763 "parser.y"
                     {
		(yyval.ptr) =createASTNode((yyvsp[0].str));
	}
#line 3477 "parser.cpp"
    break;

  case 154: /* identifier_list: identifier_list ',' IDENTIFIER  */
#line 766 "parser.y"
                                        {
		std::vector<Data> v;
		insertAttr(v,(yyvsp[-2].ptr),"",1);
		insertAttr(v,createASTNode((yyvsp[0].str)),"",1);
		(yyval.ptr) = createASTNode("identifier_list",&v);
	}
#line 3488 "parser.cpp"
    break;

  case 155: /* type_name: specifier_qualifier_list  */
#line 775 "parser.y"
                                  {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 3496 "parser.cpp"
    break;

  case 156: /* type_name: specifier_qualifier_list abstract_declarator  */
#line 778 "parser.y"
                                                      {
		std::vector<Data> v;
		insertAttr(v,(yyvsp[-1].ptr),"",1);
		insertAttr(v,(yyvsp[0].ptr),"",1);
		(yyval.ptr) = createASTNode("type_name",&v);
	}
#line 3507 "parser.cpp"
    break;

  case 157: /* abstract_declarator: pointer  */
#line 787 "parser.y"
                  {
		(yyval.ptr) =(yyvsp[0].ptr);
	}
#line 3515 "parser.cpp"
    break;

  case 158: /* abstract_declarator: direct_abstract_declarator  */
#line 790 "parser.y"
                                    {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 3523 "parser.cpp"
    break;

  case 159: /* abstract_declarator: pointer direct_abstract_declarator  */
#line 793 "parser.y"
                                            {
		std::vector<Data> v;
		insertAttr(v,(yyvsp[-1].ptr),"",1);
		insertAttr(v,(yyvsp[0].ptr),"",1);
		(yyval.ptr) = createASTNode("abstract_declarator",&v);
	}
#line 3534 "parser.cpp"
    break;

  case 160: /* direct_abstract_declarator: '(' abstract_declarator ')'  */
#line 802 "parser.y"
                                      {
		(yyval.ptr) = (yyvsp[-1].ptr);
	}
#line 3542 "parser.cpp"
    break;

  case 161: /* direct_abstract_declarator: '[' ']'  */
#line 805 "parser.y"
                 {
		(yyval.ptr) = createASTNode("[ ]") ;
	}
#line 3550 "parser.cpp"
    break;

  case 162: /* direct_abstract_declarator: '[' constant_expression ']'  */
#line 808 "parser.y"
                                      {
		(yyval.ptr) = (yyvsp[-1].ptr);
	}
#line 3558 "parser.cpp"
    break;

  case 163: /* direct_abstract_declarator: direct_abstract_declarator '[' ']'  */
#line 811 "parser.y"
                                             {
		std::vector<Data> v;
		insertAttr(v,NULL,"[ ]",0);
		insertAttr(v,(yyvsp[-2].ptr),"",1);
		(yyval.ptr) = createASTNode("direct_abstract_declarator",&v);
	}
#line 3569 "parser.cpp"
    break;

  case 164: /* direct_abstract_declarator: direct_abstract_declarator '[' constant_expression ']'  */
#line 817 "parser.y"
                                                                {
		std::vector<Data> v, v2;
		insertAttr(v2, (yyvsp[-1].ptr), NULL, 1);
		Node* node = createASTNode("[ ]", &v2);
		insertAttr(v, (yyvsp[-3].ptr), "", 1);
		insertAttr(v, node, "", 1);
		(yyval.ptr) = createASTNode("direct_abstract_declarator", &v);
	}
#line 3582 "parser.cpp"
    break;

  case 165: /* direct_abstract_declarator: '(' ')'  */
#line 825 "parser.y"
                 {
		(yyval.ptr) = createASTNode("( )") ;
	}
#line 3590 "parser.cpp"
    break;

  case 166: /* direct_abstract_declarator: '(' parameter_type_list ')'  */
#line 828 "parser.y"
                                     {
		(yyval.ptr) = (yyvsp[-1].ptr) ;
	}
#line 3598 "parser.cpp"
    break;

  case 167: /* direct_abstract_declarator: direct_abstract_declarator '(' ')'  */
#line 831 "parser.y"
                                            {
		std::vector<Data> v;
		insertAttr(v, NULL, "( )", 0);
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		(yyval.ptr) = createASTNode("direct_abstract_declarator",&v);
	}
#line 3609 "parser.cpp"
    break;

  case 168: /* direct_abstract_declarator: direct_abstract_declarator '(' parameter_type_list ')'  */
#line 837 "parser.y"
                                                                {
		std::vector<Data> v, v2;
		insertAttr(v2, (yyvsp[-1].ptr), "", 1);
		Node* node = createASTNode("( )", &v2);
		insertAttr(v, (yyvsp[-3].ptr), "", 1);
		insertAttr(v, node, "", 1);
		(yyval.ptr) = createASTNode("direct_abstract_declarator", &v);
	}
#line 3622 "parser.cpp"
    break;

  case 169: /* initializer: assignment_expression  */
#line 848 "parser.y"
                               {
		(yyval.ptr) = (yyvsp[0].ptr) ;
	}
#line 3630 "parser.cpp"
    break;

  case 170: /* initializer: '{' initializer_list '}'  */
#line 851 "parser.y"
                                   {
		(yyval.ptr) = (yyvsp[-1].ptr) ;
	}
#line 3638 "parser.cpp"
    break;

  case 171: /* initializer: '{' initializer_list ',' '}'  */
#line 854 "parser.y"
                                      {
		(yyval.ptr) = (yyvsp[-2].ptr);
	}
#line 3646 "parser.cpp"
    break;

  case 172: /* initializer_list: initializer  */
#line 861 "parser.y"
                        {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 3654 "parser.cpp"
    break;

  case 173: /* initializer_list: initializer_list ',' initializer  */
#line 864 "parser.y"
                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("initializer_list", &v);
	}
#line 3665 "parser.cpp"
    break;

  case 174: /* statement: labeled_statement  */
#line 873 "parser.y"
                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 3671 "parser.cpp"
    break;

  case 175: /* statement: compound_statement  */
#line 874 "parser.y"
                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 3677 "parser.cpp"
    break;

  case 176: /* statement: expression_statement  */
#line 875 "parser.y"
                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 3683 "parser.cpp"
    break;

  case 177: /* statement: selection_statement  */
#line 876 "parser.y"
                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 3689 "parser.cpp"
    break;

  case 178: /* statement: iteration_statement  */
#line 877 "parser.y"
                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 3695 "parser.cpp"
    break;

  case 179: /* statement: jump_statement  */
#line 878 "parser.y"
                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 3701 "parser.cpp"
    break;

  case 180: /* statement: error ';'  */
#line 879 "parser.y"
                    {  
        std::cerr << "Syntax error detected, skipping invalid statement.\n";
        yyclearin;   // Discard only the incorrect token
        yyerrok;     // Reset error tracking
    }
#line 3711 "parser.cpp"
    break;

  case 181: /* statement: error '}'  */
#line 884 "parser.y"
                {  
        std::cerr << "Syntax error detected, skipping invalid block.\n";
        yyclearin;
        yyerrok;
    }
#line 3721 "parser.cpp"
    break;

  case 182: /* labeled_statement: IDENTIFIER ':' statement  */
#line 892 "parser.y"
                                        {
		std::vector<Data> v;
		insertAttr(v, createASTNode((yyvsp[-2].str)), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("labeled_statement", &v);
	}
#line 3732 "parser.cpp"
    break;

  case 183: /* labeled_statement: CASE constant_expression ':' statement  */
#line 898 "parser.y"
                                                        {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("case", &v);
	}
#line 3743 "parser.cpp"
    break;

  case 184: /* labeled_statement: DEFAULT ':' statement  */
#line 904 "parser.y"
                                {
		std::vector<Data> v;
		insertAttr(v, NULL, "default", 0);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("case", &v);
	}
#line 3754 "parser.cpp"
    break;

  case 185: /* compound_statement: '{' '}'  */
#line 913 "parser.y"
                        {(yyval.ptr) = createASTNode("{ }");}
#line 3760 "parser.cpp"
    break;

  case 186: /* compound_statement: '{' statement_list '}'  */
#line 914 "parser.y"
                                        {(yyval.ptr) = (yyvsp[-1].ptr);}
#line 3766 "parser.cpp"
    break;

  case 187: /* compound_statement: '{' declaration_list '}'  */
#line 915 "parser.y"
                                        {(yyval.ptr) = (yyvsp[-1].ptr);}
#line 3772 "parser.cpp"
    break;

  case 188: /* compound_statement: '{' declaration_list statement_list '}'  */
#line 916 "parser.y"
                                                        {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createASTNode("compound_statement", &v);
	}
#line 3783 "parser.cpp"
    break;

  case 189: /* declaration_list: declaration  */
#line 925 "parser.y"
                        {(yyval.ptr) = (yyvsp[0].ptr);}
#line 3789 "parser.cpp"
    break;

  case 190: /* declaration_list: declaration_list declaration  */
#line 926 "parser.y"
                                        {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("declaration_list", &v);
	}
#line 3800 "parser.cpp"
    break;

  case 191: /* statement_list: statement  */
#line 935 "parser.y"
                        {(yyval.ptr) = (yyvsp[0].ptr);}
#line 3806 "parser.cpp"
    break;

  case 192: /* statement_list: statement_list statement  */
#line 936 "parser.y"
                                        {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("statement_list", &v);
	}
#line 3817 "parser.cpp"
    break;

  case 193: /* expression_statement: ';'  */
#line 945 "parser.y"
                {(yyval.ptr) = createASTNode(";");}
#line 3823 "parser.cpp"
    break;

  case 194: /* expression_statement: expression ';'  */
#line 946 "parser.y"
                                {(yyval.ptr) = (yyvsp[-1].ptr);}
#line 3829 "parser.cpp"
    break;

  case 195: /* selection_statement: IF '(' expression ')' statement  */
#line 950 "parser.y"
                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("if", &v);
	}
#line 3840 "parser.cpp"
    break;

  case 196: /* selection_statement: IF '(' expression ')' statement ELSE statement  */
#line 956 "parser.y"
                                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-4].ptr), "", 1);
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("if-else", &v);
	}
#line 3852 "parser.cpp"
    break;

  case 197: /* selection_statement: SWITCH '(' expression ')' statement  */
#line 963 "parser.y"
                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("switch", &v);
	}
#line 3863 "parser.cpp"
    break;

  case 198: /* iteration_statement: WHILE '(' expression ')' statement  */
#line 972 "parser.y"
                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("while-loop", &v);
	}
#line 3874 "parser.cpp"
    break;

  case 199: /* iteration_statement: DO statement WHILE '(' expression ')' ';'  */
#line 978 "parser.y"
                                                        {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-5].ptr), "", 1);
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		(yyval.ptr) = createASTNode("do-while-loop", &v);
	}
#line 3885 "parser.cpp"
    break;

  case 200: /* iteration_statement: FOR '(' expression_statement expression_statement ')' statement  */
#line 984 "parser.y"
                                                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-3].ptr), "", 1);
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("for-loop(w/o update stmt)", &v);
	}
#line 3897 "parser.cpp"
    break;

  case 201: /* iteration_statement: FOR '(' expression_statement expression_statement expression ')' statement  */
#line 991 "parser.y"
                                                                                        {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-4].ptr), "", 1);
		insertAttr(v, (yyvsp[-3].ptr), "", 1);
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("for-loop", &v);
	}
#line 3910 "parser.cpp"
    break;

  case 202: /* iteration_statement: UNTIL '(' expression ')' statement  */
#line 999 "parser.y"
                                         { /*** Added UNTIL grammar ***/
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("until-loop", &v);
	}
#line 3921 "parser.cpp"
    break;

  case 203: /* jump_statement: GOTO IDENTIFIER ';'  */
#line 1008 "parser.y"
                                {
		std::string s;
		s = (std::string)(yyvsp[-2].str) + " : " + (std::string)(yyvsp[-1].str);
        (yyval.ptr) = createASTNode(s);
	}
#line 3931 "parser.cpp"
    break;

  case 204: /* jump_statement: CONTINUE ';'  */
#line 1013 "parser.y"
                        {(yyval.ptr) = createASTNode((yyvsp[-1].str));}
#line 3937 "parser.cpp"
    break;

  case 205: /* jump_statement: BREAK ';'  */
#line 1014 "parser.y"
                                {(yyval.ptr) = createASTNode((yyvsp[-1].str));}
#line 3943 "parser.cpp"
    break;

  case 206: /* jump_statement: RETURN ';'  */
#line 1015 "parser.y"
                        {(yyval.ptr) = createASTNode((yyvsp[-1].str));}
#line 3949 "parser.cpp"
    break;

  case 207: /* jump_statement: RETURN expression ';'  */
#line 1016 "parser.y"
                                {
		std::vector<Data> v;
		insertAttr(v, createASTNode((yyvsp[-2].str)), "", 1);
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createASTNode("jump_stmt", &v);
	}
#line 3960 "parser.cpp"
    break;

  case 208: /* translation_unit: external_declaration  */
#line 1025 "parser.y"
                                {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 3968 "parser.cpp"
    break;

  case 209: /* translation_unit: translation_unit external_declaration  */
#line 1028 "parser.y"
                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("program", &v);
	}
#line 3979 "parser.cpp"
    break;

  case 210: /* external_declaration: function_definition  */
#line 1037 "parser.y"
                                { (yyval.ptr) = (yyvsp[0].ptr);}
#line 3985 "parser.cpp"
    break;

  case 211: /* external_declaration: declaration  */
#line 1038 "parser.y"
                        { (yyval.ptr) = (yyvsp[0].ptr);}
#line 3991 "parser.cpp"
    break;

  case 212: /* function_definition: declaration_specifiers declarator declaration_list compound_statement  */
#line 1042 "parser.y"
                                                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-3].ptr), "", 1);
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("function", &v);
	}
#line 4004 "parser.cpp"
    break;

  case 213: /* function_definition: declaration_specifiers declarator compound_statement  */
#line 1050 "parser.y"
                                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("function (w/o decl_list)", &v);
	}
#line 4016 "parser.cpp"
    break;

  case 214: /* function_definition: declarator declaration_list compound_statement  */
#line 1057 "parser.y"
                                                                {
		std::vector<Data> v;
                insertAttr(v, (yyvsp[-2].ptr), "", 1);
                insertAttr(v, (yyvsp[-1].ptr), "", 1);
                insertAttr(v, (yyvsp[0].ptr), "", 1);
                (yyval.ptr) = createASTNode("function (w/o decl_specifiers)", &v);
	}
#line 4028 "parser.cpp"
    break;

  case 215: /* function_definition: declarator compound_statement  */
#line 1064 "parser.y"
                                        {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createASTNode("function (w/o specifiers and decl_list)", &v);
	}
#line 4039 "parser.cpp"
    break;


#line 4043 "parser.cpp"

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
      {
        yypcontext_t yyctx
          = {yyssp, yyesa, &yyes, &yyes_capacity, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        if (yychar != YYEMPTY)
          YY_LAC_ESTABLISH;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
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

  /* If the stack popping above didn't lose the initial context for the
     current lookahead token, the shift below will for sure.  */
  YY_LAC_DISCARD ("error recovery");

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
  if (yyes != yyesa)
    YYSTACK_FREE (yyes);
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 1072 "parser.y"


std::string lexerOutputFile;
std::string currentFilename; 

void print_error(const std::string& message) {	
    std::cerr << "\033[1;31merror: \033[0m" << message << "\n";
}

void print_options() {
	//Later
}

void performLexicalAnalysis(const std::string& filename) {
    std::ofstream out(lexerOutputFile);
    if (!out) {
        print_error("Cannot open output file " + lexerOutputFile);
        return;
    }

    std::unordered_map<std::string, TOKEN> symbol_table;
    std::vector<TOKEN> tokens;
    
    int token_id;
    while ((token_id = yylex()) != 0) {
        TOKEN token;
        token.token_type = current_token_type;
        token.lexeme = std::string(yytext);
        token.line = line;
        token.column = column - yyleng;

        if (symbol_table.find(token.lexeme) == symbol_table.end()) {
            symbol_table[token.lexeme] = token;
        }
        tokens.push_back(token);
    }

    out << "Lexical Analysis for " << filename << "\n\n";
    out << std::left 
        << std::setw(20) << "Token" 
        << std::setw(40) << "Lexeme" 
        << std::setw(10) << "Line" 
        << std::setw(10) << "Column" << "\n"
        << std::string(80, '-') << "\n";

    for (auto& token : tokens) {
        out << std::left 
            << std::setw(20) << token.token_type
            << std::setw(40) << token.lexeme
            << std::setw(10) << token.line
            << std::setw(10) << token.column << "\n";
    }

    out.close();
}

void performParsing() {
    dotfile = fopen("src/AST.dot", "w");
    if (!dotfile) {
        print_error("cannot open AST.dot for writing.");
        return;
    }

    beginAST();
    yyparse();
    endAST();

    fclose(dotfile);
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        print_error("no input files provided.");
        return -1;
    }

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-l") { // Lexer-only mode
            if (i + 1 >= argc) {
                print_error("missing filename for -l option.");
                return -1;
            }
            onlyLexer = true;
            lexerOutputFile = argv[++i];
            continue;
        }

		currentFilename = argv[i];
        yyin = fopen(argv[i], "r");
        if (!yyin) {
            print_error("cannot open file " + arg);
            continue;
        }

        line = 1;
        column = 0;
        yyrestart(yyin);

        if (onlyLexer) {
            performLexicalAnalysis(arg);
        } else {
            performParsing();
        }

        fclose(yyin);
    }

    return 0;
}

// Error handling function
int yyerror(const char* s) {
    if (currentFilename.empty()) {
        std::cerr << "error: filename not set." << std::endl;
        return -1;
    }

    std::ifstream file(currentFilename);
    if (!file) {
        std::cerr << "error: cannot open file: " << currentFilename << std::endl;
        return -1;
    }

    std::string curr_line;
    int count = 1;
    std::string heading = "syntax error";
    std::string error_line(s);

    size_t pos = error_line.find(heading);
    if (pos != std::string::npos) {
        error_line.erase(pos, heading.length() + 2);  // Remove "syntax error: "
    }

    while (std::getline(file, curr_line)) {
        if (count == line) {
            int error_column = std::max(column - yyleng, 1);

            std::cerr << "\033[1;31merror: \033[0m" << heading << " at line " << line << ", column " << error_column << ": " << error_line << "\n\n";
            std::cerr << line << " | " << curr_line << "\n";
            std::cerr << std::string(error_column + 3, ' ') << "^\n";
            
            // SKIP ERRONEOUS INPUT TO PREVENT STUCK PARSING
            yyclearin; // Discards the current token

            break;
        }
        count++;
    }

    file.close();
    
    return -1;
}
