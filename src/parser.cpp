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
#define YYERROR_VERBOSE

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
#define YYFINAL  61
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1297

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  86
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  64
/* YYNRULES -- Number of rules.  */
#define YYNRULES  213
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  355

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
       0,    74,    74,    77,    80,    83,    89,    92,    98,   101,
     107,   113,   119,   124,   132,   135,   144,   147,   152,   157,
     163,   168,   176,   179,   182,   185,   188,   191,   197,   200,
     209,   212,   218,   224,   233,   236,   242,   251,   254,   260,
     269,   272,   278,   284,   290,   299,   302,   308,   317,   320,
     329,   332,   341,   342,   351,   352,   361,   362,   371,   372,
     382,   383,   392,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   408,   409,   418,   422,   423,   432,   433,
     439,   440,   446,   447,   456,   457,   466,   467,   476,   477,
     478,   479,   480,   484,   485,   486,   487,   494,   495,   496,
     497,   498,   499,   500,   501,   505,   511,   516,   524,   525,
     529,   530,   539,   548,   554,   555,   561,   565,   566,   575,
     576,   577,   586,   591,   597,   605,   606,   615,   616,   625,
     626,   631,   637,   644,   647,   650,   658,   664,   672,   680,
     689,   692,   697,   702,   711,   714,   724,   727,   736,   739,
     748,   754,   760,   766,   769,   778,   781,   790,   793,   796,
     805,   808,   811,   814,   820,   828,   831,   834,   840,   851,
     854,   857,   864,   867,   876,   877,   878,   879,   880,   881,
     885,   891,   897,   906,   907,   908,   909,   918,   919,   928,
     929,   938,   939,   943,   949,   956,   965,   971,   977,   984,
     992,  1001,  1006,  1007,  1008,  1009,  1018,  1021,  1030,  1031,
    1035,  1043,  1050,  1057
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
  "\"end of file\"", "error", "\"invalid token\"", "IDENTIFIER",
  "CONSTANT", "STRING_LITERAL", "SIZEOF", "PTR_OP", "INC_OP", "DEC_OP",
  "LEFT_OP", "RIGHT_OP", "LE_OP", "GE_OP", "EQ_OP", "NE_OP", "AND_OP",
  "OR_OP", "MUL_ASSIGN", "DIV_ASSIGN", "MOD_ASSIGN", "ADD_ASSIGN",
  "SUB_ASSIGN", "LEFT_ASSIGN", "RIGHT_ASSIGN", "AND_ASSIGN", "XOR_ASSIGN",
  "OR_ASSIGN", "TYPE_NAME", "TYPEDEF", "EXTERN", "STATIC", "AUTO",
  "REGISTER", "CHAR", "SHORT", "INT", "LONG", "SIGNED", "UNSIGNED",
  "FLOAT", "DOUBLE", "CONST", "VOLATILE", "VOID", "STRUCT", "UNION",
  "ENUM", "ELLIPSIS", "CASE", "DEFAULT", "IF", "ELSE", "SWITCH", "WHILE",
  "DO", "FOR", "GOTO", "CONTINUE", "BREAK", "RETURN", "UNTIL", "';'",
  "'('", "')'", "'['", "']'", "'.'", "','", "'&'", "'*'", "'+'", "'-'",
  "'~'", "'!'", "'/'", "'%'", "'<'", "'>'", "'^'", "'|'", "'?'", "':'",
  "'='", "'{'", "'}'", "$accept", "primary_expression",
  "postfix_expression", "argument_expression_list", "unary_expression",
  "unary_operator", "cast_expression", "multiplicative_expression",
  "additive_expression", "shift_expression", "relational_expression",
  "equality_expression", "and_expression", "exclusive_or_expression",
  "inclusive_or_expression", "logical_and_expression",
  "logical_or_expression", "conditional_expression",
  "assignment_expression", "assignment_operator", "expression",
  "constant_expression", "declaration", "declaration_specifiers",
  "init_declarator_list", "init_declarator", "storage_class_specifier",
  "type_specifier", "struct_or_union_specifier", "struct_or_union",
  "struct_declaration_list", "struct_declaration",
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

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-209)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     974,  -209,  -209,  -209,  -209,  -209,  -209,  -209,  -209,  -209,
    -209,  -209,  -209,  -209,  -209,  -209,  -209,  -209,  -209,  -209,
    -209,     3,    44,   -19,  -209,    82,  1250,  1250,  -209,    10,
    -209,  1250,  1127,   156,    47,   884,  -209,  -209,   -77,    56,
     -36,  -209,  -209,   -19,  -209,    33,  -209,  1070,  -209,  -209,
     -30,  1102,  -209,   279,  -209,    82,  -209,  1127,  1019,   567,
     156,  -209,  -209,    56,   -37,   -16,  -209,  -209,  -209,  -209,
    -209,    44,   548,  -209,  1127,  1102,  1102,   405,  -209,    50,
    1102,   -12,  -209,  -209,   785,   809,   809,   836,    64,    12,
      75,   144,   532,   155,   127,   177,   183,   641,   185,  -209,
     620,  -209,  -209,  -209,  -209,  -209,  -209,  -209,  -209,   195,
     275,   836,  -209,   120,   112,   181,   111,   200,   187,   182,
     184,   247,     5,  -209,  -209,    43,  -209,  -209,  -209,   351,
     423,  -209,  -209,  -209,  -209,   188,  -209,  -209,  -209,  -209,
      77,   203,   206,  -209,     9,  -209,  -209,  -209,  -209,   209,
     -11,   836,    56,  -209,  -209,   548,  -209,  -209,  -209,  1050,
    -209,  -209,  -209,   836,    67,  -209,   196,  -209,   532,   620,
    -209,   836,  -209,  -209,   198,   532,   836,   836,   836,   223,
     665,   219,  -209,  -209,  -209,    87,   836,    48,    78,   222,
     286,  -209,  -209,   692,   836,   287,  -209,  -209,  -209,  -209,
    -209,  -209,  -209,  -209,  -209,  -209,  -209,   836,  -209,   836,
     836,   836,   836,   836,   836,   836,   836,   836,   836,   836,
     836,   836,   836,   836,   836,   836,   836,   836,  -209,   836,
    -209,   460,  -209,  -209,   929,   713,  -209,    13,  -209,   159,
    -209,  1229,  -209,   288,  -209,  -209,  -209,  -209,  -209,    -6,
    -209,  -209,  -209,    50,   836,  -209,   228,   532,  -209,    57,
      89,   129,   240,   665,  -209,  -209,   130,  -209,  1149,   186,
    -209,   836,  -209,  -209,   141,  -209,   189,  -209,  -209,  -209,
    -209,  -209,   120,   120,   112,   112,   181,   181,   181,   181,
     111,   111,   200,   187,   182,   184,   247,   -38,  -209,  -209,
    -209,   241,   263,  -209,   238,   159,  1192,   737,  -209,  -209,
    -209,   495,  -209,  -209,  -209,  -209,  -209,   532,   532,   532,
     836,   764,   532,  -209,  -209,   836,  -209,   836,  -209,  -209,
    -209,  -209,   267,  -209,   277,  -209,  -209,   292,  -209,  -209,
     148,   532,   149,  -209,  -209,  -209,  -209,  -209,   532,   283,
    -209,   532,  -209,  -209,  -209
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,   133,   104,    88,    89,    90,    91,    92,    94,    95,
      96,    97,   100,   101,    98,    99,   129,   130,    93,   108,
     109,     0,     0,   140,   209,     0,    78,    80,   102,     0,
     103,    82,     0,   132,     0,     0,   206,   208,   124,     0,
       0,   144,   142,   141,    76,     0,    84,    86,    79,    81,
     107,     0,    83,     0,   187,     0,   213,     0,     0,     0,
     131,     1,   207,     0,   127,     0,   125,   134,   145,   143,
      77,     0,     0,   211,     0,     0,   114,     0,   110,     0,
     116,     2,     3,     4,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   191,
       0,    22,    23,    24,    25,    26,    27,   183,     6,    16,
      28,     0,    30,    34,    37,    40,    45,    48,    50,    52,
      54,    56,    58,    60,    73,     0,   189,   174,   175,     0,
       0,   176,   177,   178,   179,    86,   188,   212,   153,   139,
     152,     0,   146,   148,     0,     2,   136,    28,    75,     0,
       0,     0,     0,   122,    85,     0,   169,    87,   210,     0,
     113,   106,   111,     0,     0,   117,   119,   115,     0,     0,
      20,     0,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,   202,   203,   204,     0,     0,     0,   155,     0,
       0,    12,    13,     0,     0,     0,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    62,     0,    19,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   192,     0,
     185,     0,   184,   190,     0,     0,   150,   157,   151,   158,
     137,     0,   138,     0,   135,   123,   128,   126,   172,     0,
     105,   120,   112,     0,     0,   180,     0,     0,   182,     0,
       0,     0,     0,     0,   201,   205,     0,     5,     0,   157,
     156,     0,    11,     8,     0,    14,     0,    10,    61,    31,
      32,    33,    35,    36,    38,    39,    43,    44,    41,    42,
      46,    47,    49,    51,    53,    55,    57,     0,    74,   186,
     165,     0,     0,   161,     0,   159,     0,     0,   147,   149,
     154,     0,   170,   118,   121,    21,   181,     0,     0,     0,
       0,     0,     0,    29,     9,     0,     7,     0,   166,   160,
     162,   167,     0,   163,     0,   171,   173,   193,   195,   196,
       0,     0,     0,   200,    15,    59,   168,   164,     0,     0,
     198,     0,   194,   197,   199
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -209,  -209,  -209,  -209,   -48,  -209,   -92,    17,    11,    19,
      49,   124,   138,   123,   137,   139,  -209,   -55,   -71,  -209,
     -88,   -54,    25,     0,  -209,   295,  -209,    51,  -209,  -209,
     293,   -67,   -35,  -209,   114,  -209,   306,   218,    -9,    -7,
     -29,    -3,  -209,   -56,  -209,   131,  -209,   202,  -132,  -208,
    -152,  -209,   -75,  -209,   154,    -4,   244,  -159,  -209,  -209,
    -209,  -209,   339,  -209
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,   108,   109,   274,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   207,
     125,   149,    54,    55,    45,    46,    26,    27,    28,    29,
      77,    78,    79,   164,   165,    30,    65,    66,    31,    32,
      33,    34,    43,   301,   142,   143,   144,   189,   302,   239,
     157,   249,   126,   127,   128,    57,   130,   131,   132,   133,
     134,    35,    36,    37
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      25,   156,   141,   248,   148,    60,    38,    63,   238,   185,
     162,   147,   187,    50,    41,    40,     1,   179,    47,   208,
      42,   263,   226,    16,    17,    24,    48,    49,    67,   305,
     229,    52,   148,   174,    68,    25,   170,   172,   173,   147,
      69,   160,    80,    74,   327,   167,   151,     1,   135,   129,
       1,    23,   152,     1,    75,   233,   270,   152,   140,    64,
      24,   305,   311,   147,   135,   188,    80,    80,    80,   153,
     168,    80,   166,   242,   245,   176,   234,   243,   235,   312,
       1,   187,   136,   187,   156,     1,   227,    39,   259,   260,
     261,    80,   162,   255,    51,    70,   148,   246,   266,   136,
     258,    71,    76,   147,   321,   228,   276,    22,   148,   251,
      22,   229,   267,    22,    23,   147,   229,   279,   280,   281,
      23,   317,   275,   216,   217,   229,    76,    76,    76,   252,
     181,    76,   163,   236,   188,   253,   278,   237,   177,   297,
     234,   268,   235,   235,    44,    22,   175,    23,    23,   265,
      80,    76,    23,   318,   136,   229,   233,   229,   298,   336,
      80,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   323,
     148,   304,   316,   212,   213,   269,    56,   147,   218,   219,
     209,   214,   215,   319,   322,   210,   211,   229,   229,   148,
     314,    73,   190,   191,   192,   324,   147,   178,    60,   325,
      76,   137,   349,   351,   220,   221,   229,   229,   180,    58,
      76,    59,   306,   147,   307,   284,   285,    40,   158,   282,
     283,   237,   340,   342,   140,   286,   287,   288,   289,   182,
     156,   140,   337,   338,   339,   183,   166,   343,   186,   268,
     332,   235,   148,   334,   344,   326,   222,   229,   193,   147,
     194,   223,   195,   225,   224,   269,   350,   240,   140,   290,
     291,    72,   345,   352,   241,   244,   354,   262,   254,   147,
     257,   264,    81,    82,    83,    84,   271,    85,    86,   272,
     277,   310,   315,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   320,   330,   328,   140,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,   329,    87,    88,
      89,   346,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   347,   348,   353,   292,   294,   101,   102,
     103,   104,   105,   106,    81,    82,    83,    84,   206,    85,
      86,   293,   295,    53,   107,   296,   154,   313,   159,   150,
     247,   256,   309,   231,    62,     0,     0,     0,     0,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,     0,
      87,    88,    89,     0,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,     0,     0,     0,     0,     0,
     101,   102,   103,   104,   105,   106,    81,    82,    83,    84,
       0,    85,    86,     2,     0,    53,   230,     0,     0,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    81,    82,    83,    84,     0,    85,    86,
       0,     0,    87,    88,    89,     0,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,     0,     0,     0,
     161,     0,   101,   102,   103,   104,   105,   106,   145,    82,
      83,    84,     0,    85,    86,     0,     0,    53,   232,    87,
      88,    89,     0,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,     0,     0,     0,     0,     0,   101,
     102,   103,   104,   105,   106,    81,    82,    83,    84,     0,
      85,    86,     0,     0,    53,   299,     0,     0,     0,     0,
       0,   145,    82,    83,    84,     0,    85,    86,   100,     0,
       0,     0,     0,     0,   101,   102,   103,   104,   105,   106,
     145,    82,    83,    84,     0,    85,    86,     0,     0,   155,
     335,    87,    88,    89,     0,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,     0,     0,     0,     0,
       0,   101,   102,   103,   104,   105,   106,     0,     0,     0,
       0,   100,     0,     0,     0,     0,    53,   101,   102,   103,
     104,   105,   106,   145,    82,    83,    84,     0,    85,    86,
     100,     0,   155,   146,     0,     0,   101,   102,   103,   104,
     105,   106,     0,     0,   145,    82,    83,    84,     2,    85,
      86,     0,     0,     0,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,   145,    82,
      83,    84,     0,    85,    86,     0,     0,     0,     0,     0,
       0,     0,     0,   100,     0,     0,     0,     0,     0,   101,
     102,   103,   104,   105,   106,   145,    82,    83,    84,     0,
      85,    86,     0,   184,   100,     0,     0,     0,     0,     0,
     101,   102,   103,   104,   105,   106,   145,    82,    83,    84,
       0,    85,    86,     0,     0,     0,     0,    99,   100,     0,
       0,     0,     0,     0,   101,   102,   103,   104,   105,   106,
     145,    82,    83,    84,     0,    85,    86,     0,     0,     0,
       0,     0,     0,     0,     0,   100,   273,     0,     0,     0,
       0,   101,   102,   103,   104,   105,   106,   145,    82,    83,
      84,     0,    85,    86,     0,     0,   100,     0,     0,   303,
       0,     0,   101,   102,   103,   104,   105,   106,   145,    82,
      83,    84,     0,    85,    86,     0,     0,     0,     0,     0,
     100,     0,     0,   333,     0,     0,   101,   102,   103,   104,
     105,   106,   145,    82,    83,    84,     0,    85,    86,     0,
       0,     0,     0,     0,     0,     0,     0,   100,   341,     0,
       0,     0,     0,   101,   102,   103,   104,   105,   106,   145,
      82,    83,    84,     0,    85,    86,     0,     0,   169,     0,
       0,     0,     0,     0,   101,   102,   103,   104,   105,   106,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   171,     0,     0,     0,     0,     0,   101,   102,
     103,   104,   105,   106,    61,     0,     0,     1,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,     0,     0,     0,   101,   102,   103,   104,   105,
     106,     0,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,     1,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,     0,     0,
       0,     0,     0,     0,    23,     0,     0,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,     1,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   234,   300,   235,     0,     0,     0,     0,    23,
       0,     0,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   138,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,     0,     0,
       0,     0,     0,     0,    23,     0,     0,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     2,     0,
       0,     0,     0,   139,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,     2,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       2,     0,     0,     0,     0,   250,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
       0,     0,     0,    72,    53,     2,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,     0,     0,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,   268,   300,   235,     0,     0,     0,     0,    23,
       2,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   331,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,   308,     2,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21
};

static const yytype_int16 yycheck[] =
{
       0,    72,    58,   155,    59,    34,     3,    84,   140,    97,
      77,    59,   100,     3,    23,    22,     3,    92,    25,   111,
      23,   180,    17,    42,    43,     0,    26,    27,    64,   237,
      68,    31,    87,    87,    43,    35,    84,    85,    86,    87,
      43,    76,    51,    47,    82,    80,    83,     3,    55,    53,
       3,    70,    68,     3,    84,   130,   188,    68,    58,     3,
      35,   269,    68,   111,    71,   100,    75,    76,    77,    85,
      82,    80,    79,    64,    85,    63,    63,    68,    65,    85,
       3,   169,    57,   171,   155,     3,    81,    84,   176,   177,
     178,   100,   159,   168,    84,    62,   151,   151,   186,    74,
     175,    68,    51,   151,   263,    62,   194,    63,   163,   163,
      63,    68,    64,    63,    70,   163,    68,   209,   210,   211,
      70,    64,   193,    12,    13,    68,    75,    76,    77,    62,
       3,    80,    82,   140,   169,    68,   207,   140,    63,   227,
      63,    63,    65,    65,    62,    63,    82,    70,    70,    62,
     159,   100,    70,    64,   129,    68,   231,    68,   229,   311,
     169,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   271,
     235,   235,   257,    71,    72,   188,    32,   235,    77,    78,
      70,    10,    11,    64,    64,    75,    76,    68,    68,   254,
     254,    47,     7,     8,     9,    64,   254,    63,   237,    68,
     159,    57,    64,    64,    14,    15,    68,    68,    63,    63,
     169,    65,    63,   271,    65,   214,   215,   234,    74,   212,
     213,   234,   320,   321,   234,   216,   217,   218,   219,    62,
     311,   241,   317,   318,   319,    62,   253,   322,    63,    63,
     306,    65,   307,   307,   325,    66,    69,    68,    63,   307,
      65,    79,    67,    16,    80,   268,   341,    64,   268,   220,
     221,    83,   327,   348,    68,    66,   351,    54,    82,   327,
      82,    62,     3,     4,     5,     6,    64,     8,     9,     3,
       3,     3,    64,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    63,    66,    64,   306,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    64,    49,    50,
      51,    64,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    66,    52,    62,   222,   224,    69,    70,
      71,    72,    73,    74,     3,     4,     5,     6,    83,     8,
       9,   223,   225,    84,    85,   226,    71,   253,    75,    63,
     152,   169,   241,   129,    35,    -1,    -1,    -1,    -1,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    -1,    -1,    -1,    -1,    -1,
      69,    70,    71,    72,    73,    74,     3,     4,     5,     6,
      -1,     8,     9,    28,    -1,    84,    85,    -1,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,     8,     9,
      -1,    -1,    49,    50,    51,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    -1,    -1,    -1,
      85,    -1,    69,    70,    71,    72,    73,    74,     3,     4,
       5,     6,    -1,     8,     9,    -1,    -1,    84,    85,    49,
      50,    51,    -1,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    -1,    -1,    -1,    -1,    -1,    69,
      70,    71,    72,    73,    74,     3,     4,     5,     6,    -1,
       8,     9,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,     8,     9,    63,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    73,    74,
       3,     4,     5,     6,    -1,     8,     9,    -1,    -1,    84,
      85,    49,    50,    51,    -1,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    -1,    -1,    -1,    -1,
      -1,    69,    70,    71,    72,    73,    74,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    84,    69,    70,    71,
      72,    73,    74,     3,     4,     5,     6,    -1,     8,     9,
      63,    -1,    84,    66,    -1,    -1,    69,    70,    71,    72,
      73,    74,    -1,    -1,     3,     4,     5,     6,    28,     8,
       9,    -1,    -1,    -1,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,     3,     4,
       5,     6,    -1,     8,     9,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    69,
      70,    71,    72,    73,    74,     3,     4,     5,     6,    -1,
       8,     9,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,
      69,    70,    71,    72,    73,    74,     3,     4,     5,     6,
      -1,     8,     9,    -1,    -1,    -1,    -1,    62,    63,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    73,    74,
       3,     4,     5,     6,    -1,     8,     9,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    69,    70,    71,    72,    73,    74,     3,     4,     5,
       6,    -1,     8,     9,    -1,    -1,    63,    -1,    -1,    66,
      -1,    -1,    69,    70,    71,    72,    73,    74,     3,     4,
       5,     6,    -1,     8,     9,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    66,    -1,    -1,    69,    70,    71,    72,
      73,    74,     3,     4,     5,     6,    -1,     8,     9,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,
      -1,    -1,    -1,    69,    70,    71,    72,    73,    74,     3,
       4,     5,     6,    -1,     8,     9,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    69,    70,
      71,    72,    73,    74,     0,    -1,    -1,     3,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,    73,
      74,    -1,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,     3,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    64,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    85,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    -1,    -1,    83,    84,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    -1,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    63,    64,    65,    -1,    -1,    -1,    -1,    70,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47
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
     124,     3,     4,     5,     6,     8,     9,    49,    50,    51,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    69,    70,    71,    72,    73,    74,    85,    87,    88,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   106,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   125,   108,   140,     3,    64,
     109,   129,   130,   131,   132,     3,    66,    90,   103,   107,
     122,    83,    68,    85,   111,    84,   104,   136,   140,   116,
     118,    85,   117,    82,   119,   120,   125,   118,    82,    63,
      90,    63,    90,    90,   107,    82,    63,    63,    63,   138,
      63,     3,    62,    62,    62,   106,    63,   106,   118,   133,
       7,     8,     9,    63,    65,    67,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    83,   105,    92,    70,
      75,    76,    71,    72,    10,    11,    12,    13,    77,    78,
      14,    15,    69,    79,    80,    16,    17,    81,    62,    68,
      85,   142,    85,   138,    63,    65,   125,   127,   134,   135,
      64,    68,    64,    68,    66,    85,   107,   123,   136,   137,
      85,   107,    62,    68,    82,   138,   133,    82,   138,   106,
     106,   106,    54,   143,    62,    62,   106,    64,    63,   127,
     134,    64,     3,    64,    89,   104,   106,     3,   104,    92,
      92,    92,    93,    93,    94,    94,    95,    95,    95,    95,
      96,    96,    97,    98,    99,   100,   101,   106,   104,    85,
      64,   129,   134,    66,   107,   135,    63,    65,    48,   131,
       3,    68,    85,   120,   107,    64,   138,    64,    64,    64,
      63,   143,    64,    92,    64,    68,    66,    82,    64,    64,
      66,    64,   129,    66,   107,    85,   136,   138,   138,   138,
     106,    64,   106,   138,   104,   103,    64,    66,    52,    64,
     138,    64,   138,    62,   138
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
     139,   139,   139,   140,   140,   140,   140,   141,   141,   142,
     142,   143,   143,   144,   144,   144,   145,   145,   145,   145,
     145,   146,   146,   146,   146,   146,   147,   147,   148,   148,
     149,   149,   149,   149
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
       3,     4,     3,     2,     3,     3,     4,     1,     2,     1,
       2,     1,     2,     5,     7,     5,     5,     7,     6,     7,
       5,     3,     2,     2,     2,     3,     1,     2,     1,     1,
       4,     3,     3,     2
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
  case 2: /* primary_expression: IDENTIFIER  */
#line 74 "parser.y"
                 {
    	(yyval.ptr) = createLeaf((yyvsp[0].str));
    }
#line 1708 "parser.cpp"
    break;

  case 3: /* primary_expression: CONSTANT  */
#line 77 "parser.y"
                        {
		(yyval.ptr) = createLeaf((yyvsp[0].str));
	}
#line 1716 "parser.cpp"
    break;

  case 4: /* primary_expression: STRING_LITERAL  */
#line 80 "parser.y"
                         {
		(yyval.ptr) = createLeaf((yyvsp[0].str));
	}
#line 1724 "parser.cpp"
    break;

  case 5: /* primary_expression: '(' expression ')'  */
#line 83 "parser.y"
                             {
		(yyval.ptr) = (yyvsp[-1].ptr);
	}
#line 1732 "parser.cpp"
    break;

  case 6: /* postfix_expression: primary_expression  */
#line 89 "parser.y"
                             {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 1740 "parser.cpp"
    break;

  case 7: /* postfix_expression: postfix_expression '[' expression ']'  */
#line 92 "parser.y"
                                                {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-3].ptr), "", 1);
		insertAttr(attr, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createNode("postfix_expression", attr);
	}
#line 1751 "parser.cpp"
    break;

  case 8: /* postfix_expression: postfix_expression '(' ')'  */
#line 98 "parser.y"
                                     {
		(yyval.ptr) = (yyvsp[-2].ptr);
	}
#line 1759 "parser.cpp"
    break;

  case 9: /* postfix_expression: postfix_expression '(' argument_expression_list ')'  */
#line 101 "parser.y"
                                                              {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-3].ptr), "", 1);
		insertAttr(attr, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createNode("postfix_expression", attr);
	}
#line 1770 "parser.cpp"
    break;

  case 10: /* postfix_expression: postfix_expression '.' IDENTIFIER  */
#line 107 "parser.y"
                                            {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, createLeaf((yyvsp[0].str)), "", 1);
		(yyval.ptr) = createNode("expression.id", attr);
	}
#line 1781 "parser.cpp"
    break;

  case 11: /* postfix_expression: postfix_expression PTR_OP IDENTIFIER  */
#line 113 "parser.y"
                                               {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, createLeaf((yyvsp[0].str)), "", 1);
		(yyval.ptr) = createNode((yyvsp[-1].str), attr);
	}
#line 1792 "parser.cpp"
    break;

  case 12: /* postfix_expression: postfix_expression INC_OP  */
#line 119 "parser.y"
                                    {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createNode((yyvsp[0].str), attr);
	}
#line 1802 "parser.cpp"
    break;

  case 13: /* postfix_expression: postfix_expression DEC_OP  */
#line 124 "parser.y"
                                    {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createNode((yyvsp[0].str), attr);
	}
#line 1812 "parser.cpp"
    break;

  case 14: /* argument_expression_list: assignment_expression  */
#line 132 "parser.y"
                                {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 1820 "parser.cpp"
    break;

  case 15: /* argument_expression_list: argument_expression_list ',' assignment_expression  */
#line 135 "parser.y"
                                                             {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("argument_list", attr);
	}
#line 1831 "parser.cpp"
    break;

  case 16: /* unary_expression: postfix_expression  */
#line 144 "parser.y"
                             {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 1839 "parser.cpp"
    break;

  case 17: /* unary_expression: INC_OP unary_expression  */
#line 147 "parser.y"
                                  {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode((yyvsp[-1].str),attr);
	}
#line 1849 "parser.cpp"
    break;

  case 18: /* unary_expression: DEC_OP unary_expression  */
#line 152 "parser.y"
                                  {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode((yyvsp[-1].str),attr);
	}
#line 1859 "parser.cpp"
    break;

  case 19: /* unary_expression: unary_operator cast_expression  */
#line 157 "parser.y"
                                         {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-1].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("unary_exp",attr);
	}
#line 1870 "parser.cpp"
    break;

  case 20: /* unary_expression: SIZEOF unary_expression  */
#line 163 "parser.y"
                                  {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode((yyvsp[-1].str),attr);
	}
#line 1880 "parser.cpp"
    break;

  case 21: /* unary_expression: SIZEOF '(' type_name ')'  */
#line 168 "parser.y"
                                   {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createNode((yyvsp[-3].str),attr);
	}
#line 1890 "parser.cpp"
    break;

  case 22: /* unary_operator: '&'  */
#line 176 "parser.y"
              {
		(yyval.ptr) = createLeaf("&");
	}
#line 1898 "parser.cpp"
    break;

  case 23: /* unary_operator: '*'  */
#line 179 "parser.y"
              {
		(yyval.ptr) = createLeaf("*");
	}
#line 1906 "parser.cpp"
    break;

  case 24: /* unary_operator: '+'  */
#line 182 "parser.y"
              {
		(yyval.ptr) = createLeaf("+");
	}
#line 1914 "parser.cpp"
    break;

  case 25: /* unary_operator: '-'  */
#line 185 "parser.y"
              {
		(yyval.ptr) = createLeaf("-");
	}
#line 1922 "parser.cpp"
    break;

  case 26: /* unary_operator: '~'  */
#line 188 "parser.y"
              {
		(yyval.ptr) = createLeaf("~");
	}
#line 1930 "parser.cpp"
    break;

  case 27: /* unary_operator: '!'  */
#line 191 "parser.y"
              {
		(yyval.ptr) = createLeaf("!");
	}
#line 1938 "parser.cpp"
    break;

  case 28: /* cast_expression: unary_expression  */
#line 197 "parser.y"
                           {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 1946 "parser.cpp"
    break;

  case 29: /* cast_expression: '(' type_name ')' cast_expression  */
#line 200 "parser.y"
                                            {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("cast_expression" ,attr);
	}
#line 1957 "parser.cpp"
    break;

  case 30: /* multiplicative_expression: cast_expression  */
#line 209 "parser.y"
                          {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 1965 "parser.cpp"
    break;

  case 31: /* multiplicative_expression: multiplicative_expression '*' cast_expression  */
#line 212 "parser.y"
                                                        {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("*" ,attr);
	}
#line 1976 "parser.cpp"
    break;

  case 32: /* multiplicative_expression: multiplicative_expression '/' cast_expression  */
#line 218 "parser.y"
                                                        {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("/" ,attr);
	}
#line 1987 "parser.cpp"
    break;

  case 33: /* multiplicative_expression: multiplicative_expression '%' cast_expression  */
#line 224 "parser.y"
                                                        {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("%" ,attr);
	}
#line 1998 "parser.cpp"
    break;

  case 34: /* additive_expression: multiplicative_expression  */
#line 233 "parser.y"
                                    {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 2006 "parser.cpp"
    break;

  case 35: /* additive_expression: additive_expression '+' multiplicative_expression  */
#line 236 "parser.y"
                                                            {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("+" ,attr);
	}
#line 2017 "parser.cpp"
    break;

  case 36: /* additive_expression: additive_expression '-' multiplicative_expression  */
#line 242 "parser.y"
                                                            {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("-" ,attr);
	}
#line 2028 "parser.cpp"
    break;

  case 37: /* shift_expression: additive_expression  */
#line 251 "parser.y"
                              {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 2036 "parser.cpp"
    break;

  case 38: /* shift_expression: shift_expression LEFT_OP additive_expression  */
#line 254 "parser.y"
                                                       {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode((yyvsp[-1].str) ,attr);
	}
#line 2047 "parser.cpp"
    break;

  case 39: /* shift_expression: shift_expression RIGHT_OP additive_expression  */
#line 260 "parser.y"
                                                        {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode((yyvsp[-1].str) ,attr);
	}
#line 2058 "parser.cpp"
    break;

  case 40: /* relational_expression: shift_expression  */
#line 269 "parser.y"
                           {
		 (yyval.ptr) = (yyvsp[0].ptr);
	}
#line 2066 "parser.cpp"
    break;

  case 41: /* relational_expression: relational_expression '<' shift_expression  */
#line 272 "parser.y"
                                                     {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("<" ,attr);
	}
#line 2077 "parser.cpp"
    break;

  case 42: /* relational_expression: relational_expression '>' shift_expression  */
#line 278 "parser.y"
                                                     {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode(">" ,attr);
	}
#line 2088 "parser.cpp"
    break;

  case 43: /* relational_expression: relational_expression LE_OP shift_expression  */
#line 284 "parser.y"
                                                       {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode((yyvsp[-1].str) ,attr);
	}
#line 2099 "parser.cpp"
    break;

  case 44: /* relational_expression: relational_expression GE_OP shift_expression  */
#line 290 "parser.y"
                                                       {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode((yyvsp[-1].str) ,attr);
	}
#line 2110 "parser.cpp"
    break;

  case 45: /* equality_expression: relational_expression  */
#line 299 "parser.y"
                                { 
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 2118 "parser.cpp"
    break;

  case 46: /* equality_expression: equality_expression EQ_OP relational_expression  */
#line 302 "parser.y"
                                                          {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode((yyvsp[-1].str) ,attr);
	}
#line 2129 "parser.cpp"
    break;

  case 47: /* equality_expression: equality_expression NE_OP relational_expression  */
#line 308 "parser.y"
                                                          {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode((yyvsp[-1].str) ,attr);
	}
#line 2140 "parser.cpp"
    break;

  case 48: /* and_expression: equality_expression  */
#line 317 "parser.y"
                              {	
		(yyval.ptr) = (yyvsp[0].ptr); 
	}
#line 2148 "parser.cpp"
    break;

  case 49: /* and_expression: and_expression '&' equality_expression  */
#line 320 "parser.y"
                                                {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("&",attr);
	}
#line 2159 "parser.cpp"
    break;

  case 50: /* exclusive_or_expression: and_expression  */
#line 329 "parser.y"
                         {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 2167 "parser.cpp"
    break;

  case 51: /* exclusive_or_expression: exclusive_or_expression '^' and_expression  */
#line 332 "parser.y"
                                                                                                {
																			std::vector<Data> attr;
																			insertAttr(attr, (yyvsp[-2].ptr), "", 1);
																			insertAttr(attr, (yyvsp[0].ptr), "", 1);
																			(yyval.ptr) = createNode("^",attr);
																		}
#line 2178 "parser.cpp"
    break;

  case 52: /* inclusive_or_expression: exclusive_or_expression  */
#line 341 "parser.y"
                                  {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2184 "parser.cpp"
    break;

  case 53: /* inclusive_or_expression: inclusive_or_expression '|' exclusive_or_expression  */
#line 342 "parser.y"
                                                             {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("|",attr);
	}
#line 2195 "parser.cpp"
    break;

  case 54: /* logical_and_expression: inclusive_or_expression  */
#line 351 "parser.y"
                                  {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2201 "parser.cpp"
    break;

  case 55: /* logical_and_expression: logical_and_expression AND_OP inclusive_or_expression  */
#line 352 "parser.y"
                                                               {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("&&",attr);
	}
#line 2212 "parser.cpp"
    break;

  case 56: /* logical_or_expression: logical_and_expression  */
#line 361 "parser.y"
                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2218 "parser.cpp"
    break;

  case 57: /* logical_or_expression: logical_or_expression OR_OP logical_and_expression  */
#line 362 "parser.y"
                                                            {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("||",attr);
	}
#line 2229 "parser.cpp"
    break;

  case 58: /* conditional_expression: logical_or_expression  */
#line 371 "parser.y"
                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2235 "parser.cpp"
    break;

  case 59: /* conditional_expression: logical_or_expression '?' expression ':' conditional_expression  */
#line 372 "parser.y"
                                                                         {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-4].ptr), "", 1);
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("ternary operator",attr);
	}
#line 2247 "parser.cpp"
    break;

  case 60: /* assignment_expression: conditional_expression  */
#line 382 "parser.y"
                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2253 "parser.cpp"
    break;

  case 61: /* assignment_expression: unary_expression assignment_operator assignment_expression  */
#line 383 "parser.y"
                                                                    {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode((yyvsp[-1].str),attr);
	}
#line 2264 "parser.cpp"
    break;

  case 62: /* assignment_operator: '='  */
#line 392 "parser.y"
                {
		(yyval.str) = strdup("=");
	}
#line 2272 "parser.cpp"
    break;

  case 63: /* assignment_operator: MUL_ASSIGN  */
#line 395 "parser.y"
                                {(yyval.str) = (yyvsp[0].str);}
#line 2278 "parser.cpp"
    break;

  case 64: /* assignment_operator: DIV_ASSIGN  */
#line 396 "parser.y"
                                {(yyval.str) = (yyvsp[0].str);}
#line 2284 "parser.cpp"
    break;

  case 65: /* assignment_operator: MOD_ASSIGN  */
#line 397 "parser.y"
                                {(yyval.str) = (yyvsp[0].str);}
#line 2290 "parser.cpp"
    break;

  case 66: /* assignment_operator: ADD_ASSIGN  */
#line 398 "parser.y"
                                {(yyval.str) = (yyvsp[0].str);}
#line 2296 "parser.cpp"
    break;

  case 67: /* assignment_operator: SUB_ASSIGN  */
#line 399 "parser.y"
                                {(yyval.str) = (yyvsp[0].str);}
#line 2302 "parser.cpp"
    break;

  case 68: /* assignment_operator: LEFT_ASSIGN  */
#line 400 "parser.y"
                                {(yyval.str) = (yyvsp[0].str);}
#line 2308 "parser.cpp"
    break;

  case 69: /* assignment_operator: RIGHT_ASSIGN  */
#line 401 "parser.y"
                                {(yyval.str) = (yyvsp[0].str);}
#line 2314 "parser.cpp"
    break;

  case 70: /* assignment_operator: AND_ASSIGN  */
#line 402 "parser.y"
                                {(yyval.str) = (yyvsp[0].str);}
#line 2320 "parser.cpp"
    break;

  case 71: /* assignment_operator: XOR_ASSIGN  */
#line 403 "parser.y"
                                {(yyval.str) = (yyvsp[0].str);}
#line 2326 "parser.cpp"
    break;

  case 72: /* assignment_operator: OR_ASSIGN  */
#line 404 "parser.y"
                                        {(yyval.str) = (yyvsp[0].str);}
#line 2332 "parser.cpp"
    break;

  case 73: /* expression: assignment_expression  */
#line 408 "parser.y"
                               {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2338 "parser.cpp"
    break;

  case 74: /* expression: expression ',' assignment_expression  */
#line 409 "parser.y"
                                              {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("expression",attr);
	}
#line 2349 "parser.cpp"
    break;

  case 75: /* constant_expression: conditional_expression  */
#line 418 "parser.y"
                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2355 "parser.cpp"
    break;

  case 76: /* declaration: declaration_specifiers ';'  */
#line 422 "parser.y"
                                    { (yyval.ptr) = (yyvsp[-1].ptr); }
#line 2361 "parser.cpp"
    break;

  case 77: /* declaration: declaration_specifiers init_declarator_list ';'  */
#line 423 "parser.y"
                                                         {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createNode("declaration",attr);
	}
#line 2372 "parser.cpp"
    break;

  case 78: /* declaration_specifiers: storage_class_specifier  */
#line 432 "parser.y"
                                  { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2378 "parser.cpp"
    break;

  case 79: /* declaration_specifiers: storage_class_specifier declaration_specifiers  */
#line 433 "parser.y"
                                                        {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-1].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("declaration_specifiers",attr);
	}
#line 2389 "parser.cpp"
    break;

  case 80: /* declaration_specifiers: type_specifier  */
#line 439 "parser.y"
                        { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2395 "parser.cpp"
    break;

  case 81: /* declaration_specifiers: type_specifier declaration_specifiers  */
#line 440 "parser.y"
                                               {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-1].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("declaration_specifiers",attr);
	}
#line 2406 "parser.cpp"
    break;

  case 82: /* declaration_specifiers: type_qualifier  */
#line 446 "parser.y"
                        { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2412 "parser.cpp"
    break;

  case 83: /* declaration_specifiers: type_qualifier declaration_specifiers  */
#line 447 "parser.y"
                                               {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-1].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("declaration_specifiers",attr);
	}
#line 2423 "parser.cpp"
    break;

  case 84: /* init_declarator_list: init_declarator  */
#line 456 "parser.y"
                         {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2429 "parser.cpp"
    break;

  case 85: /* init_declarator_list: init_declarator_list ',' init_declarator  */
#line 457 "parser.y"
                                                  {
		std::vector<Data> attr;
		insertAttr(attr, (yyvsp[-2].ptr), "", 1);
		insertAttr(attr, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("init_declarator_list",attr);
	}
#line 2440 "parser.cpp"
    break;

  case 86: /* init_declarator: declarator  */
#line 466 "parser.y"
                     {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2446 "parser.cpp"
    break;

  case 87: /* init_declarator: declarator '=' initializer  */
#line 467 "parser.y"
                                    {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("=", v);
	}
#line 2457 "parser.cpp"
    break;

  case 88: /* storage_class_specifier: TYPEDEF  */
#line 476 "parser.y"
                        { (yyval.ptr) = createLeaf((yyvsp[0].str));}
#line 2463 "parser.cpp"
    break;

  case 89: /* storage_class_specifier: EXTERN  */
#line 477 "parser.y"
                        { (yyval.ptr) = createLeaf((yyvsp[0].str));}
#line 2469 "parser.cpp"
    break;

  case 90: /* storage_class_specifier: STATIC  */
#line 478 "parser.y"
                        { (yyval.ptr) = createLeaf((yyvsp[0].str));}
#line 2475 "parser.cpp"
    break;

  case 91: /* storage_class_specifier: AUTO  */
#line 479 "parser.y"
                        { (yyval.ptr) = createLeaf((yyvsp[0].str));}
#line 2481 "parser.cpp"
    break;

  case 92: /* storage_class_specifier: REGISTER  */
#line 480 "parser.y"
                        { (yyval.ptr) = createLeaf((yyvsp[0].str));}
#line 2487 "parser.cpp"
    break;

  case 93: /* type_specifier: VOID  */
#line 484 "parser.y"
                                {(yyval.ptr) = createLeaf((yyvsp[0].str));}
#line 2493 "parser.cpp"
    break;

  case 94: /* type_specifier: CHAR  */
#line 485 "parser.y"
                                {(yyval.ptr) = createLeaf((yyvsp[0].str));}
#line 2499 "parser.cpp"
    break;

  case 95: /* type_specifier: SHORT  */
#line 486 "parser.y"
                                {(yyval.ptr) = createLeaf((yyvsp[0].str));}
#line 2505 "parser.cpp"
    break;

  case 96: /* type_specifier: INT  */
#line 487 "parser.y"
                                {
		if (!(yyvsp[0].str)) {
			yyerror("Parser error: NULL INT token received");
			YYABORT;
		}
		(yyval.ptr) = createLeaf((yyvsp[0].str));
	}
#line 2517 "parser.cpp"
    break;

  case 97: /* type_specifier: LONG  */
#line 494 "parser.y"
                                {(yyval.ptr) = createLeaf((yyvsp[0].str));}
#line 2523 "parser.cpp"
    break;

  case 98: /* type_specifier: FLOAT  */
#line 495 "parser.y"
                                {(yyval.ptr) = createLeaf((yyvsp[0].str));}
#line 2529 "parser.cpp"
    break;

  case 99: /* type_specifier: DOUBLE  */
#line 496 "parser.y"
                                {(yyval.ptr) = createLeaf((yyvsp[0].str));}
#line 2535 "parser.cpp"
    break;

  case 100: /* type_specifier: SIGNED  */
#line 497 "parser.y"
                                {(yyval.ptr) = createLeaf((yyvsp[0].str));}
#line 2541 "parser.cpp"
    break;

  case 101: /* type_specifier: UNSIGNED  */
#line 498 "parser.y"
                                {(yyval.ptr) = createLeaf((yyvsp[0].str));}
#line 2547 "parser.cpp"
    break;

  case 102: /* type_specifier: struct_or_union_specifier  */
#line 499 "parser.y"
                                        {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2553 "parser.cpp"
    break;

  case 103: /* type_specifier: enum_specifier  */
#line 500 "parser.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2559 "parser.cpp"
    break;

  case 104: /* type_specifier: TYPE_NAME  */
#line 501 "parser.y"
                                {(yyval.ptr) = createLeaf((yyvsp[0].str));}
#line 2565 "parser.cpp"
    break;

  case 105: /* struct_or_union_specifier: struct_or_union IDENTIFIER '{' struct_declaration_list '}'  */
#line 505 "parser.y"
                                                                        {
		std::vector<Data> v;
		insertAttr(v, createLeaf((yyvsp[-3].str)), "", 1);
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createNode((yyvsp[-4].str), v);
	}
#line 2576 "parser.cpp"
    break;

  case 106: /* struct_or_union_specifier: struct_or_union '{' struct_declaration_list '}'  */
#line 511 "parser.y"
                                                                        {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createNode((yyvsp[-3].str), v);
	}
#line 2586 "parser.cpp"
    break;

  case 107: /* struct_or_union_specifier: struct_or_union IDENTIFIER  */
#line 516 "parser.y"
                                        {
		std::vector<Data> v;
		insertAttr(v, createLeaf((yyvsp[0].str)), "", 1);
		(yyval.ptr) = createNode((yyvsp[-1].str), v);
	}
#line 2596 "parser.cpp"
    break;

  case 108: /* struct_or_union: STRUCT  */
#line 524 "parser.y"
                        {(yyval.str) = (yyvsp[0].str);}
#line 2602 "parser.cpp"
    break;

  case 109: /* struct_or_union: UNION  */
#line 525 "parser.y"
                        {(yyval.str) = (yyvsp[0].str);}
#line 2608 "parser.cpp"
    break;

  case 110: /* struct_declaration_list: struct_declaration  */
#line 529 "parser.y"
                                { (yyval.ptr) = (yyvsp[0].ptr) ;}
#line 2614 "parser.cpp"
    break;

  case 111: /* struct_declaration_list: struct_declaration_list struct_declaration  */
#line 530 "parser.y"
                                                        {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("struct_declaration_list", v);
	}
#line 2625 "parser.cpp"
    break;

  case 112: /* struct_declaration: specifier_qualifier_list struct_declarator_list ';'  */
#line 539 "parser.y"
                                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createNode("struct_declaration", v);
	}
#line 2636 "parser.cpp"
    break;

  case 113: /* specifier_qualifier_list: type_specifier specifier_qualifier_list  */
#line 548 "parser.y"
                                                        {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("specifier_qualifier_list", v);
	}
#line 2647 "parser.cpp"
    break;

  case 114: /* specifier_qualifier_list: type_specifier  */
#line 554 "parser.y"
                                { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2653 "parser.cpp"
    break;

  case 115: /* specifier_qualifier_list: type_qualifier specifier_qualifier_list  */
#line 555 "parser.y"
                                                        {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("specifier_qualifier_list", v);
	}
#line 2664 "parser.cpp"
    break;

  case 116: /* specifier_qualifier_list: type_qualifier  */
#line 561 "parser.y"
                                { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2670 "parser.cpp"
    break;

  case 117: /* struct_declarator_list: struct_declarator  */
#line 565 "parser.y"
                            { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2676 "parser.cpp"
    break;

  case 118: /* struct_declarator_list: struct_declarator_list ',' struct_declarator  */
#line 566 "parser.y"
                                                       {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("struct_declarator_list", v);
	}
#line 2687 "parser.cpp"
    break;

  case 119: /* struct_declarator: declarator  */
#line 575 "parser.y"
                        { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2693 "parser.cpp"
    break;

  case 120: /* struct_declarator: ':' constant_expression  */
#line 576 "parser.y"
                                        { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2699 "parser.cpp"
    break;

  case 121: /* struct_declarator: declarator ':' constant_expression  */
#line 577 "parser.y"
                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode(":", v);
	}
#line 2710 "parser.cpp"
    break;

  case 122: /* enum_specifier: ENUM '{' enumerator_list '}'  */
#line 586 "parser.y"
                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createNode((yyvsp[-3].str), v);
	}
#line 2720 "parser.cpp"
    break;

  case 123: /* enum_specifier: ENUM IDENTIFIER '{' enumerator_list '}'  */
#line 591 "parser.y"
                                                        {
		std::vector<Data> v;
		insertAttr(v, createLeaf((yyvsp[-3].str)), "", 1);
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createNode((yyvsp[-4].str), v);
	}
#line 2731 "parser.cpp"
    break;

  case 124: /* enum_specifier: ENUM IDENTIFIER  */
#line 597 "parser.y"
                          {
		std::vector<Data> v;
		insertAttr(v, createLeaf((yyvsp[0].str)), "", 1);
		(yyval.ptr) = createNode((yyvsp[-1].str), v);
	}
#line 2741 "parser.cpp"
    break;

  case 125: /* enumerator_list: enumerator  */
#line 605 "parser.y"
                        { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2747 "parser.cpp"
    break;

  case 126: /* enumerator_list: enumerator_list ',' enumerator  */
#line 606 "parser.y"
                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("enumerator_list", v);
	}
#line 2758 "parser.cpp"
    break;

  case 127: /* enumerator: IDENTIFIER  */
#line 615 "parser.y"
                        { (yyval.ptr) = createLeaf((yyvsp[0].str)); }
#line 2764 "parser.cpp"
    break;

  case 128: /* enumerator: IDENTIFIER '=' constant_expression  */
#line 616 "parser.y"
                                                {
		std::vector<Data> v;
		insertAttr(v, createLeaf((yyvsp[-2].str)), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("=", v);
	}
#line 2775 "parser.cpp"
    break;

  case 129: /* type_qualifier: CONST  */
#line 625 "parser.y"
                        { (yyval.ptr) = createLeaf((yyvsp[0].str)); }
#line 2781 "parser.cpp"
    break;

  case 130: /* type_qualifier: VOLATILE  */
#line 626 "parser.y"
                        { (yyval.ptr) = createLeaf((yyvsp[0].str)); }
#line 2787 "parser.cpp"
    break;

  case 131: /* declarator: pointer direct_declarator  */
#line 631 "parser.y"
                                   {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("declarator", v);
	}
#line 2798 "parser.cpp"
    break;

  case 132: /* declarator: direct_declarator  */
#line 637 "parser.y"
                            {
		(yyval.ptr) = (yyvsp[0].ptr) ;
	}
#line 2806 "parser.cpp"
    break;

  case 133: /* direct_declarator: IDENTIFIER  */
#line 644 "parser.y"
                     {
		(yyval.ptr) = createLeaf((yyvsp[0].str));
	}
#line 2814 "parser.cpp"
    break;

  case 134: /* direct_declarator: '(' declarator ')'  */
#line 647 "parser.y"
                              {
		(yyval.ptr) = (yyvsp[-1].ptr) ;
	}
#line 2822 "parser.cpp"
    break;

  case 135: /* direct_declarator: direct_declarator '[' constant_expression ']'  */
#line 650 "parser.y"
                                                       {
		std::vector<Data> v, v2;
		insertAttr(v2, (yyvsp[-1].ptr), "", 1);
		Node* node = createNode("[ ]", v2);
		insertAttr(v, (yyvsp[-3].ptr), "", 1);
		insertAttr(v, node, "", 1);
		(yyval.ptr) = createNode("direct_declarator", v);
	}
#line 2835 "parser.cpp"
    break;

  case 136: /* direct_declarator: direct_declarator '[' ']'  */
#line 658 "parser.y"
                                   {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, NULL, "[ ]", 0);
		(yyval.ptr) = createNode("direct_declarator", v);
	}
#line 2846 "parser.cpp"
    break;

  case 137: /* direct_declarator: direct_declarator '(' parameter_type_list ')'  */
#line 664 "parser.y"
                                                       {
		std::vector<Data> v, v2;
		insertAttr(v2, (yyvsp[-1].ptr), "", 1);
		Node* node = createNode("( )", v2);
		insertAttr(v, (yyvsp[-3].ptr), "", 1);
		insertAttr(v, node, "", 1);
		(yyval.ptr) = createNode("direct_declarator", v);
	}
#line 2859 "parser.cpp"
    break;

  case 138: /* direct_declarator: direct_declarator '(' identifier_list ')'  */
#line 672 "parser.y"
                                                   {
		std::vector<Data> v, v2;
		insertAttr(v2, (yyvsp[-1].ptr), "", 1);
		Node* node = createNode("( )", v2);
		insertAttr(v, (yyvsp[-3].ptr), "", 1);
		insertAttr(v, node, "", 1);
		(yyval.ptr) = createNode("direct_declarator", v);
	}
#line 2872 "parser.cpp"
    break;

  case 139: /* direct_declarator: direct_declarator '(' ')'  */
#line 680 "parser.y"
                                   {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, NULL, "( )", 0);
		(yyval.ptr) = createNode("direct_declarator", v);
	}
#line 2883 "parser.cpp"
    break;

  case 140: /* pointer: '*'  */
#line 689 "parser.y"
              {
		(yyval.ptr) = createLeaf("*(Pointer)");
	}
#line 2891 "parser.cpp"
    break;

  case 141: /* pointer: '*' type_qualifier_list  */
#line 692 "parser.y"
                                 {
		std::vector<Data> v;
		insertAttr(v,(yyvsp[0].ptr),"",1);
		(yyval.ptr) = createNode("*(Pointer)",v);
	}
#line 2901 "parser.cpp"
    break;

  case 142: /* pointer: '*' pointer  */
#line 697 "parser.y"
                     {
		std::vector<Data> v;
		insertAttr(v,(yyvsp[0].ptr),"",1);
		(yyval.ptr) = createNode("*(Pointer)",v);
	}
#line 2911 "parser.cpp"
    break;

  case 143: /* pointer: '*' type_qualifier_list pointer  */
#line 702 "parser.y"
                                         {
		std::vector<Data> v;
		insertAttr(v,(yyvsp[-1].ptr),"",1);
		insertAttr(v,(yyvsp[0].ptr),"",1);
		(yyval.ptr) = createNode("*(Pointer)",v);
	}
#line 2922 "parser.cpp"
    break;

  case 144: /* type_qualifier_list: type_qualifier  */
#line 711 "parser.y"
                         {
		(yyval.ptr) = (yyvsp[0].ptr) ;
	}
#line 2930 "parser.cpp"
    break;

  case 145: /* type_qualifier_list: type_qualifier_list type_qualifier  */
#line 714 "parser.y"
                                            {
		std::vector<Data> v;
		insertAttr(v,(yyvsp[-1].ptr),"",1);
		insertAttr(v,(yyvsp[0].ptr),"",1);
		(yyval.ptr) = createNode("type_qualifier_list",v);
	}
#line 2941 "parser.cpp"
    break;

  case 146: /* parameter_type_list: parameter_list  */
#line 724 "parser.y"
                         {
		(yyval.ptr) = (yyvsp[0].ptr) ;
	}
#line 2949 "parser.cpp"
    break;

  case 147: /* parameter_type_list: parameter_list ',' ELLIPSIS  */
#line 727 "parser.y"
                                     {
		std::vector<Data> v;
		insertAttr(v,(yyvsp[-2].ptr),"",1);
		insertAttr(v, createLeaf((yyvsp[0].str)), "", 1);
		(yyval.ptr) = createNode("parameter_type_list",v);
	}
#line 2960 "parser.cpp"
    break;

  case 148: /* parameter_list: parameter_declaration  */
#line 736 "parser.y"
                               {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 2968 "parser.cpp"
    break;

  case 149: /* parameter_list: parameter_list ',' parameter_declaration  */
#line 739 "parser.y"
                                                  {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("parameter_list",v);
	}
#line 2979 "parser.cpp"
    break;

  case 150: /* parameter_declaration: declaration_specifiers declarator  */
#line 748 "parser.y"
                                           {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("parameter_declaration",v);
	}
#line 2990 "parser.cpp"
    break;

  case 151: /* parameter_declaration: declaration_specifiers abstract_declarator  */
#line 754 "parser.y"
                                                    {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("parameter_declaration",v);
	}
#line 3001 "parser.cpp"
    break;

  case 152: /* parameter_declaration: declaration_specifiers  */
#line 760 "parser.y"
                                 {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 3009 "parser.cpp"
    break;

  case 153: /* identifier_list: IDENTIFIER  */
#line 766 "parser.y"
                     {
		(yyval.ptr) =createLeaf((yyvsp[0].str));
	}
#line 3017 "parser.cpp"
    break;

  case 154: /* identifier_list: identifier_list ',' IDENTIFIER  */
#line 769 "parser.y"
                                        {
		std::vector<Data> v;
		insertAttr(v,(yyvsp[-2].ptr),"",1);
		insertAttr(v,createLeaf((yyvsp[0].str)),"",1);
		(yyval.ptr) = createNode("identifier_list",v);
	}
#line 3028 "parser.cpp"
    break;

  case 155: /* type_name: specifier_qualifier_list  */
#line 778 "parser.y"
                                  {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 3036 "parser.cpp"
    break;

  case 156: /* type_name: specifier_qualifier_list abstract_declarator  */
#line 781 "parser.y"
                                                      {
		std::vector<Data> v;
		insertAttr(v,(yyvsp[-1].ptr),"",1);
		insertAttr(v,(yyvsp[0].ptr),"",1);
		(yyval.ptr) = createNode("type_name",v);
	}
#line 3047 "parser.cpp"
    break;

  case 157: /* abstract_declarator: pointer  */
#line 790 "parser.y"
                  {
		(yyval.ptr) =(yyvsp[0].ptr);
	}
#line 3055 "parser.cpp"
    break;

  case 158: /* abstract_declarator: direct_abstract_declarator  */
#line 793 "parser.y"
                                    {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 3063 "parser.cpp"
    break;

  case 159: /* abstract_declarator: pointer direct_abstract_declarator  */
#line 796 "parser.y"
                                            {
		std::vector<Data> v;
		insertAttr(v,(yyvsp[-1].ptr),"",1);
		insertAttr(v,(yyvsp[0].ptr),"",1);
		(yyval.ptr) = createNode("abstract_declarator",v);
	}
#line 3074 "parser.cpp"
    break;

  case 160: /* direct_abstract_declarator: '(' abstract_declarator ')'  */
#line 805 "parser.y"
                                      {
		(yyval.ptr) = (yyvsp[-1].ptr);
	}
#line 3082 "parser.cpp"
    break;

  case 161: /* direct_abstract_declarator: '[' ']'  */
#line 808 "parser.y"
                 {
		(yyval.ptr) = createLeaf("[ ]") ;
	}
#line 3090 "parser.cpp"
    break;

  case 162: /* direct_abstract_declarator: '[' constant_expression ']'  */
#line 811 "parser.y"
                                      {
		(yyval.ptr) = (yyvsp[-1].ptr);
	}
#line 3098 "parser.cpp"
    break;

  case 163: /* direct_abstract_declarator: direct_abstract_declarator '[' ']'  */
#line 814 "parser.y"
                                             {
		std::vector<Data> v;
		insertAttr(v,NULL,"[ ]",0);
		insertAttr(v,(yyvsp[-2].ptr),"",1);
		(yyval.ptr) = createNode("direct_abstract_declarator",v);
	}
#line 3109 "parser.cpp"
    break;

  case 164: /* direct_abstract_declarator: direct_abstract_declarator '[' constant_expression ']'  */
#line 820 "parser.y"
                                                                {
		std::vector<Data> v, v2;
		insertAttr(v2, (yyvsp[-1].ptr), NULL, 1);
		Node* node = createNode("[ ]", v2);
		insertAttr(v, (yyvsp[-3].ptr), "", 1);
		insertAttr(v, node, "", 1);
		(yyval.ptr) = createNode("direct_abstract_declarator", v);
	}
#line 3122 "parser.cpp"
    break;

  case 165: /* direct_abstract_declarator: '(' ')'  */
#line 828 "parser.y"
                 {
		(yyval.ptr) = createLeaf("( )") ;
	}
#line 3130 "parser.cpp"
    break;

  case 166: /* direct_abstract_declarator: '(' parameter_type_list ')'  */
#line 831 "parser.y"
                                     {
		(yyval.ptr) = (yyvsp[-1].ptr) ;
	}
#line 3138 "parser.cpp"
    break;

  case 167: /* direct_abstract_declarator: direct_abstract_declarator '(' ')'  */
#line 834 "parser.y"
                                            {
		std::vector<Data> v;
		insertAttr(v, NULL, "( )", 0);
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		(yyval.ptr) = createNode("direct_abstract_declarator",v);
	}
#line 3149 "parser.cpp"
    break;

  case 168: /* direct_abstract_declarator: direct_abstract_declarator '(' parameter_type_list ')'  */
#line 840 "parser.y"
                                                                {
		std::vector<Data> v, v2;
		insertAttr(v2, (yyvsp[-1].ptr), "", 1);
		Node* node = createNode("( )", v2);
		insertAttr(v, (yyvsp[-3].ptr), "", 1);
		insertAttr(v, node, "", 1);
		(yyval.ptr) = createNode("direct_abstract_declarator", v);
	}
#line 3162 "parser.cpp"
    break;

  case 169: /* initializer: assignment_expression  */
#line 851 "parser.y"
                               {
		(yyval.ptr) = (yyvsp[0].ptr) ;
	}
#line 3170 "parser.cpp"
    break;

  case 170: /* initializer: '{' initializer_list '}'  */
#line 854 "parser.y"
                                   {
		(yyval.ptr) = (yyvsp[-1].ptr) ;
	}
#line 3178 "parser.cpp"
    break;

  case 171: /* initializer: '{' initializer_list ',' '}'  */
#line 857 "parser.y"
                                      {
		(yyval.ptr) = (yyvsp[-2].ptr);
	}
#line 3186 "parser.cpp"
    break;

  case 172: /* initializer_list: initializer  */
#line 864 "parser.y"
                        {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 3194 "parser.cpp"
    break;

  case 173: /* initializer_list: initializer_list ',' initializer  */
#line 867 "parser.y"
                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("initializer_list", v);
	}
#line 3205 "parser.cpp"
    break;

  case 174: /* statement: labeled_statement  */
#line 876 "parser.y"
                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 3211 "parser.cpp"
    break;

  case 175: /* statement: compound_statement  */
#line 877 "parser.y"
                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 3217 "parser.cpp"
    break;

  case 176: /* statement: expression_statement  */
#line 878 "parser.y"
                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 3223 "parser.cpp"
    break;

  case 177: /* statement: selection_statement  */
#line 879 "parser.y"
                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 3229 "parser.cpp"
    break;

  case 178: /* statement: iteration_statement  */
#line 880 "parser.y"
                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 3235 "parser.cpp"
    break;

  case 179: /* statement: jump_statement  */
#line 881 "parser.y"
                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 3241 "parser.cpp"
    break;

  case 180: /* labeled_statement: IDENTIFIER ':' statement  */
#line 885 "parser.y"
                                        {
		std::vector<Data> v;
		insertAttr(v, createLeaf((yyvsp[-2].str)), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("labeled_statement", v);
	}
#line 3252 "parser.cpp"
    break;

  case 181: /* labeled_statement: CASE constant_expression ':' statement  */
#line 891 "parser.y"
                                                        {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("case", v);
	}
#line 3263 "parser.cpp"
    break;

  case 182: /* labeled_statement: DEFAULT ':' statement  */
#line 897 "parser.y"
                                {
		std::vector<Data> v;
		insertAttr(v, NULL, "default", 0);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("case", v);
	}
#line 3274 "parser.cpp"
    break;

  case 183: /* compound_statement: '{' '}'  */
#line 906 "parser.y"
                        {(yyval.ptr) = createLeaf("{ }");}
#line 3280 "parser.cpp"
    break;

  case 184: /* compound_statement: '{' statement_list '}'  */
#line 907 "parser.y"
                                        {(yyval.ptr) = (yyvsp[-1].ptr);}
#line 3286 "parser.cpp"
    break;

  case 185: /* compound_statement: '{' declaration_list '}'  */
#line 908 "parser.y"
                                        {(yyval.ptr) = (yyvsp[-1].ptr);}
#line 3292 "parser.cpp"
    break;

  case 186: /* compound_statement: '{' declaration_list statement_list '}'  */
#line 909 "parser.y"
                                                        {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createNode("compound_statement", v);
	}
#line 3303 "parser.cpp"
    break;

  case 187: /* declaration_list: declaration  */
#line 918 "parser.y"
                        {(yyval.ptr) = (yyvsp[0].ptr);}
#line 3309 "parser.cpp"
    break;

  case 188: /* declaration_list: declaration_list declaration  */
#line 919 "parser.y"
                                        {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("declaration_list", v);
	}
#line 3320 "parser.cpp"
    break;

  case 189: /* statement_list: statement  */
#line 928 "parser.y"
                        {(yyval.ptr) = (yyvsp[0].ptr);}
#line 3326 "parser.cpp"
    break;

  case 190: /* statement_list: statement_list statement  */
#line 929 "parser.y"
                                        {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("statement_list", v);
	}
#line 3337 "parser.cpp"
    break;

  case 191: /* expression_statement: ';'  */
#line 938 "parser.y"
                {(yyval.ptr) = createLeaf(";");}
#line 3343 "parser.cpp"
    break;

  case 192: /* expression_statement: expression ';'  */
#line 939 "parser.y"
                                {(yyval.ptr) = (yyvsp[-1].ptr);}
#line 3349 "parser.cpp"
    break;

  case 193: /* selection_statement: IF '(' expression ')' statement  */
#line 943 "parser.y"
                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("if", v);
	}
#line 3360 "parser.cpp"
    break;

  case 194: /* selection_statement: IF '(' expression ')' statement ELSE statement  */
#line 949 "parser.y"
                                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-4].ptr), "", 1);
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("if-else", v);
	}
#line 3372 "parser.cpp"
    break;

  case 195: /* selection_statement: SWITCH '(' expression ')' statement  */
#line 956 "parser.y"
                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("switch", v);
	}
#line 3383 "parser.cpp"
    break;

  case 196: /* iteration_statement: WHILE '(' expression ')' statement  */
#line 965 "parser.y"
                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("while-loop", v);
	}
#line 3394 "parser.cpp"
    break;

  case 197: /* iteration_statement: DO statement WHILE '(' expression ')' ';'  */
#line 971 "parser.y"
                                                        {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-5].ptr), "", 1);
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		(yyval.ptr) = createNode("do-while-loop", v);
	}
#line 3405 "parser.cpp"
    break;

  case 198: /* iteration_statement: FOR '(' expression_statement expression_statement ')' statement  */
#line 977 "parser.y"
                                                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-3].ptr), "", 1);
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("for-loop(w/o update stmt)", v);
	}
#line 3417 "parser.cpp"
    break;

  case 199: /* iteration_statement: FOR '(' expression_statement expression_statement expression ')' statement  */
#line 984 "parser.y"
                                                                                        {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-4].ptr), "", 1);
		insertAttr(v, (yyvsp[-3].ptr), "", 1);
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("for-loop", v);
	}
#line 3430 "parser.cpp"
    break;

  case 200: /* iteration_statement: UNTIL '(' expression ')' statement  */
#line 992 "parser.y"
                                         { /*** Added UNTIL grammar ***/
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("until-loop", v);
	}
#line 3441 "parser.cpp"
    break;

  case 201: /* jump_statement: GOTO IDENTIFIER ';'  */
#line 1001 "parser.y"
                                {
		std::string s;
		s = (std::string)(yyvsp[-2].str) + " : " + (std::string)(yyvsp[-1].str);
        (yyval.ptr) = createLeaf(s);
	}
#line 3451 "parser.cpp"
    break;

  case 202: /* jump_statement: CONTINUE ';'  */
#line 1006 "parser.y"
                        {(yyval.ptr) = createLeaf((yyvsp[-1].str));}
#line 3457 "parser.cpp"
    break;

  case 203: /* jump_statement: BREAK ';'  */
#line 1007 "parser.y"
                                {(yyval.ptr) = createLeaf((yyvsp[-1].str));}
#line 3463 "parser.cpp"
    break;

  case 204: /* jump_statement: RETURN ';'  */
#line 1008 "parser.y"
                        {(yyval.ptr) = createLeaf((yyvsp[-1].str));}
#line 3469 "parser.cpp"
    break;

  case 205: /* jump_statement: RETURN expression ';'  */
#line 1009 "parser.y"
                                {
		std::vector<Data> v;
		insertAttr(v, createLeaf((yyvsp[-2].str)), "", 1);
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		(yyval.ptr) = createNode("jump_stmt", v);
	}
#line 3480 "parser.cpp"
    break;

  case 206: /* translation_unit: external_declaration  */
#line 1018 "parser.y"
                                {
		(yyval.ptr) = (yyvsp[0].ptr);
	}
#line 3488 "parser.cpp"
    break;

  case 207: /* translation_unit: translation_unit external_declaration  */
#line 1021 "parser.y"
                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("program", v);
	}
#line 3499 "parser.cpp"
    break;

  case 208: /* external_declaration: function_definition  */
#line 1030 "parser.y"
                                { (yyval.ptr) = (yyvsp[0].ptr);}
#line 3505 "parser.cpp"
    break;

  case 209: /* external_declaration: declaration  */
#line 1031 "parser.y"
                        { (yyval.ptr) = (yyvsp[0].ptr);}
#line 3511 "parser.cpp"
    break;

  case 210: /* function_definition: declaration_specifiers declarator declaration_list compound_statement  */
#line 1035 "parser.y"
                                                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-3].ptr), "", 1);
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("function", v);
	}
#line 3524 "parser.cpp"
    break;

  case 211: /* function_definition: declaration_specifiers declarator compound_statement  */
#line 1043 "parser.y"
                                                                {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-2].ptr), "", 1);
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("function (w/o decl_list)", v);
	}
#line 3536 "parser.cpp"
    break;

  case 212: /* function_definition: declarator declaration_list compound_statement  */
#line 1050 "parser.y"
                                                                {
		std::vector<Data> v;
                insertAttr(v, (yyvsp[-2].ptr), "", 1);
                insertAttr(v, (yyvsp[-1].ptr), "", 1);
                insertAttr(v, (yyvsp[0].ptr), "", 1);
                (yyval.ptr) = createNode("function (w/o decl_specifiers)", v);
	}
#line 3548 "parser.cpp"
    break;

  case 213: /* function_definition: declarator compound_statement  */
#line 1057 "parser.y"
                                        {
		std::vector<Data> v;
		insertAttr(v, (yyvsp[-1].ptr), "", 1);
		insertAttr(v, (yyvsp[0].ptr), "", 1);
		(yyval.ptr) = createNode("function (w/o specifiers and decl_list)", v);
	}
#line 3559 "parser.cpp"
    break;


#line 3563 "parser.cpp"

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

#line 1065 "parser.y"


std::string lexerOutputFile;
std::string currentFilename; 

void print_error(const std::string& message) {	
    std::cerr << "\033[1;31mError: \033[0m" << message << "\n";
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
        print_error("Cannot open AST.dot for writing.");
        return;
    }

    beginAST();
    yyparse();
    endAST();

    fclose(dotfile);
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        print_error("No input files provided.");
        return -1;
    }

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-l") { // Lexer-only mode
            if (i + 1 >= argc) {
                print_error("Missing filename for -l option.");
                return -1;
            }
            onlyLexer = true;
            lexerOutputFile = argv[++i];
            continue;
        }

		currentFilename = argv[i];
        yyin = fopen(argv[i], "r");
        if (!yyin) {
            print_error("Cannot open file " + arg);
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
    std::cerr << "\033[1;31mError:\033[0m " << s << " at line " << line 
              << ", column " << column + 1 - (yytext ? strlen(yytext) : 0) 
              << std::endl;

    if (currentFilename.empty()) {
        print_error("Filename not set.");
        return -1;
    }

    std::ifstream file(currentFilename);  
    if (!file) {
        print_error("Cannot open source file: " + currentFilename);
        return -1;
    }

    std::string curr_line;
    int count = 1;
    
    while (std::getline(file, curr_line)) {
        if (count == line) {
            std::cerr << "\n" << line << " | " << curr_line << "\n";
            std::cerr << "    " << std::string(column - yyleng + 1, ' ') << "^\n";
            std::cerr << "\033[1;31mError: \033[0m" << s << "\n\n";
            break;
        }
        count++;
    }

    file.close();
    return -1;
}
