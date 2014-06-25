/* A Bison parser, made by GNU Bison 2.7.1.  */

/* Bison interface for Yacc-like parsers in C
   
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

#ifndef YY_TDR_D_GITHUB_TDR_SOURCE_GEN_INCLUDE_PARSER_Y_H_INCLUDED
# define YY_TDR_D_GITHUB_TDR_SOURCE_GEN_INCLUDE_PARSER_Y_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int tdrdebug;
#endif
/* "%code requires" blocks.  */
/* Line 2053 of yacc.c  */
#line 30 "D:/GitHub/TDR/source/parser_y.yy"

#include "parse/parser.h"
#include "parse/scanner.h"

#include "parse/check.h"
#include "parse/reduce.h"
#include "symbols.h"


/* Line 2053 of yacc.c  */
#line 56 "D:/GitHub/TDR/source/gen/include/parser_y.h"

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
     tok_true = 272,
     tok_false = 273,
     tok_bool = 274,
     tok_identifier = 275,
     tok_const = 276,
     tok_enum = 277,
     tok_equal = 278,
     tok_unequal = 279,
     tok_count = 280,
     tok_unixcomment = 281,
     tok_typedef = 282,
     tok_t_int8 = 283,
     tok_t_int16 = 284,
     tok_t_int32 = 285,
     tok_t_int64 = 286,
     tok_t_uint8 = 287,
     tok_t_uint16 = 288,
     tok_t_uint32 = 289,
     tok_t_uint64 = 290,
     tok_t_char = 291,
     tok_t_double = 292,
     tok_t_string = 293,
     tok_t_vector = 294,
     tok_t_bool = 295
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
int tdrparse (void *YYPARSE_PARAM);
#else
int tdrparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int tdrparse (scanner_t *self);
#else
int tdrparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_TDR_D_GITHUB_TDR_SOURCE_GEN_INCLUDE_PARSER_Y_H_INCLUDED  */
