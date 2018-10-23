// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.
// //                    "%code top" blocks.
#line 29 "parser/impossible.ypp" // lalr1.cc:397

  #define YYDEBUG 1
  
  #include <iostream>

  #include "Values.h"
  #include "TypeTraits.h"
  #include "instruction.h"
  #include "defines.h"
  
  #include "CompositeValue.h" //TODO: for value_hash
  
  #include "lexer.h"
  #include "compiler.h"

	//extern "C"
	//{
    int yyparse(void);
    extern int yylex(void);  
    //int cyywrap();
    /*{
     return 1;
     }*/
    void yyerror(const char *s);

    Code *assemble(const char *string);
    Code *code;
	//}
  
  
  static compiler::Parser::symbol_type yylex(compiler::Lexer& scanner, compiler::Compiler& compiler)
  {
    return scanner.get_next_token();
  }
  
  using namespace compiler;

#line 72 "parser/parser.cpp" // lalr1.cc:397


// First part of user declarations.

#line 77 "parser/parser.cpp" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "parser.hpp"

// User implementation prologue.

#line 91 "parser/parser.cpp" // lalr1.cc:412


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 5 "parser/impossible.ypp" // lalr1.cc:479
namespace compiler {
#line 177 "parser/parser.cpp" // lalr1.cc:479

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
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
              // Fall through.
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  Parser::Parser (compiler::Lexer &lexer_yyarg, compiler::Compiler &compiler_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      lexer (lexer_yyarg),
      compiler (compiler_yyarg)
  {}

  Parser::~Parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  inline
  Parser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  Parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  Parser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  Parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  Parser::symbol_number_type
  Parser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  Parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  Parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
      switch (that.type_get ())
    {
      case 78: // start
        value.move< Code* > (that.value);
        break;

      case 80: // instruction
        value.move< Instruction* > (that.value);
        break;

      case 11: // "$"
      case 12: // "%"
      case 13: // ";"
      case 14: // ","
      case 15: // "(>"
      case 16: // "<)"
      case 17: // "+"
      case 18: // "-"
      case 19: // "/"
      case 20: // "*"
      case 21: // "//"
      case 22: // "&"
      case 23: // "|"
      case 24: // "~"
      case 25: // ">>"
      case 26: // "<<"
      case 27: // "_"
      case 28: // "+."
      case 29: // "+:"
      case 30: // "-."
      case 31: // "-:"
      case 32: // "*."
      case 33: // "*:"
      case 34: // "/."
      case 35: // "/:"
      case 36: // "@>"
      case 37: // "#>"
      case 38: // "$>"
      case 39: // "@"
      case 40: // "#"
      case 41: // "<@"
      case 42: // ">@"
      case 43: // "<#"
      case 44: // ">#"
      case 45: // ".>."
      case 46: // ":>."
      case 47: // ":>:"
      case 48: // ".>"
      case 49: // ".>:"
      case 50: // ":*."
      case 51: // ".?"
      case 52: // ":?"
      case 53: // "="
      case 54: // "<"
      case 55: // ">"
      case 56: // "."
      case 57: // "!"
      case 58: // "!!"
      case 59: // "?"
      case 60: // "??"
      case 61: // "^"
      case 62: // "[#]"
      case 63: // "{#}"
      case 64: // "<>"
      case 65: // "i"
      case 66: // "s"
      case 67: // OPCODE
        value.move< Opcode > (that.value);
        break;

      case 68: // "integer"
      case 69: // "float"
      case 70: // "bool"
      case 71: // "char"
      case 72: // "string"
      case 73: // "range"
      case 74: // "lambda"
      case 75: // "larray"
      case 84: // lazy_array
      case 86: // value
      case 87: // lambda
        value.move< Value > (that.value);
        break;

      case 81: // values
        value.move< std::list<Value> > (that.value);
        break;

      case 82: // value_pairs
        value.move< std::list<std::pair<Value, Value>> > (that.value);
        break;

      case 83: // lazy_pairs
        value.move< std::list<std::pair<integral_t, Lambda*>> > (that.value);
        break;

      case 76: // "optional_value"
      case 85: // opt_value
        value.move< std::pair<bool, Value> > (that.value);
        break;

      case 79: // instructions
        value.move< std::vector<Instruction*> > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 78: // start
        value.copy< Code* > (that.value);
        break;

      case 80: // instruction
        value.copy< Instruction* > (that.value);
        break;

      case 11: // "$"
      case 12: // "%"
      case 13: // ";"
      case 14: // ","
      case 15: // "(>"
      case 16: // "<)"
      case 17: // "+"
      case 18: // "-"
      case 19: // "/"
      case 20: // "*"
      case 21: // "//"
      case 22: // "&"
      case 23: // "|"
      case 24: // "~"
      case 25: // ">>"
      case 26: // "<<"
      case 27: // "_"
      case 28: // "+."
      case 29: // "+:"
      case 30: // "-."
      case 31: // "-:"
      case 32: // "*."
      case 33: // "*:"
      case 34: // "/."
      case 35: // "/:"
      case 36: // "@>"
      case 37: // "#>"
      case 38: // "$>"
      case 39: // "@"
      case 40: // "#"
      case 41: // "<@"
      case 42: // ">@"
      case 43: // "<#"
      case 44: // ">#"
      case 45: // ".>."
      case 46: // ":>."
      case 47: // ":>:"
      case 48: // ".>"
      case 49: // ".>:"
      case 50: // ":*."
      case 51: // ".?"
      case 52: // ":?"
      case 53: // "="
      case 54: // "<"
      case 55: // ">"
      case 56: // "."
      case 57: // "!"
      case 58: // "!!"
      case 59: // "?"
      case 60: // "??"
      case 61: // "^"
      case 62: // "[#]"
      case 63: // "{#}"
      case 64: // "<>"
      case 65: // "i"
      case 66: // "s"
      case 67: // OPCODE
        value.copy< Opcode > (that.value);
        break;

      case 68: // "integer"
      case 69: // "float"
      case 70: // "bool"
      case 71: // "char"
      case 72: // "string"
      case 73: // "range"
      case 74: // "lambda"
      case 75: // "larray"
      case 84: // lazy_array
      case 86: // value
      case 87: // lambda
        value.copy< Value > (that.value);
        break;

      case 81: // values
        value.copy< std::list<Value> > (that.value);
        break;

      case 82: // value_pairs
        value.copy< std::list<std::pair<Value, Value>> > (that.value);
        break;

      case 83: // lazy_pairs
        value.copy< std::list<std::pair<integral_t, Lambda*>> > (that.value);
        break;

      case 76: // "optional_value"
      case 85: // opt_value
        value.copy< std::pair<bool, Value> > (that.value);
        break;

      case 79: // instructions
        value.copy< std::vector<Instruction*> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  Parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  Parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  Parser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  Parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  Parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            symbol_type yylookahead (yylex (lexer, compiler));
            yyla.move (yylookahead);
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
        switch (yyr1_[yyn])
    {
      case 78: // start
        yylhs.value.build< Code* > ();
        break;

      case 80: // instruction
        yylhs.value.build< Instruction* > ();
        break;

      case 11: // "$"
      case 12: // "%"
      case 13: // ";"
      case 14: // ","
      case 15: // "(>"
      case 16: // "<)"
      case 17: // "+"
      case 18: // "-"
      case 19: // "/"
      case 20: // "*"
      case 21: // "//"
      case 22: // "&"
      case 23: // "|"
      case 24: // "~"
      case 25: // ">>"
      case 26: // "<<"
      case 27: // "_"
      case 28: // "+."
      case 29: // "+:"
      case 30: // "-."
      case 31: // "-:"
      case 32: // "*."
      case 33: // "*:"
      case 34: // "/."
      case 35: // "/:"
      case 36: // "@>"
      case 37: // "#>"
      case 38: // "$>"
      case 39: // "@"
      case 40: // "#"
      case 41: // "<@"
      case 42: // ">@"
      case 43: // "<#"
      case 44: // ">#"
      case 45: // ".>."
      case 46: // ":>."
      case 47: // ":>:"
      case 48: // ".>"
      case 49: // ".>:"
      case 50: // ":*."
      case 51: // ".?"
      case 52: // ":?"
      case 53: // "="
      case 54: // "<"
      case 55: // ">"
      case 56: // "."
      case 57: // "!"
      case 58: // "!!"
      case 59: // "?"
      case 60: // "??"
      case 61: // "^"
      case 62: // "[#]"
      case 63: // "{#}"
      case 64: // "<>"
      case 65: // "i"
      case 66: // "s"
      case 67: // OPCODE
        yylhs.value.build< Opcode > ();
        break;

      case 68: // "integer"
      case 69: // "float"
      case 70: // "bool"
      case 71: // "char"
      case 72: // "string"
      case 73: // "range"
      case 74: // "lambda"
      case 75: // "larray"
      case 84: // lazy_array
      case 86: // value
      case 87: // lambda
        yylhs.value.build< Value > ();
        break;

      case 81: // values
        yylhs.value.build< std::list<Value> > ();
        break;

      case 82: // value_pairs
        yylhs.value.build< std::list<std::pair<Value, Value>> > ();
        break;

      case 83: // lazy_pairs
        yylhs.value.build< std::list<std::pair<integral_t, Lambda*>> > ();
        break;

      case 76: // "optional_value"
      case 85: // opt_value
        yylhs.value.build< std::pair<bool, Value> > ();
        break;

      case 79: // instructions
        yylhs.value.build< std::vector<Instruction*> > ();
        break;

      default:
        break;
    }


      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 190 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< Code* > () = new CodeStandard(yystack_[0].value.as< std::vector<Instruction*> > ()); }
#line 862 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 3:
#line 194 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< std::vector<Instruction*> > () = std::vector<Instruction*>(); }
#line 868 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 4:
#line 195 "parser/impossible.ypp" // lalr1.cc:859
    { yystack_[1].value.as< std::vector<Instruction*> > ().push_back(yystack_[0].value.as< Instruction* > ()); yylhs.value.as< std::vector<Instruction*> > () = yystack_[1].value.as< std::vector<Instruction*> > (); }
#line 874 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 5:
#line 199 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< Instruction* > () = new PushInstruction(yystack_[0].value.as< Value > ()); }
#line 880 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 6:
#line 200 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< Instruction* > () = new OpcodeInstruction(yystack_[0].value.as< Opcode > ()); }
#line 886 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 7:
#line 204 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< std::list<Value> > () = std::list<Value>(); yylhs.value.as< std::list<Value> > ().push_front(yystack_[0].value.as< Value > ()); }
#line 892 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 8:
#line 205 "parser/impossible.ypp" // lalr1.cc:859
    { yystack_[1].value.as< std::list<Value> > ().push_back(yystack_[0].value.as< Value > ()); yylhs.value.as< std::list<Value> > () = yystack_[1].value.as< std::list<Value> > (); }
#line 898 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 9:
#line 209 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< std::list<std::pair<Value, Value>> > () = std::list<std::pair<Value, Value>>(); yylhs.value.as< std::list<std::pair<Value, Value>> > ().push_front(std::make_pair(yystack_[2].value.as< Value > (), yystack_[0].value.as< Value > ())); }
#line 904 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 10:
#line 210 "parser/impossible.ypp" // lalr1.cc:859
    { yystack_[3].value.as< std::list<std::pair<Value, Value>> > ().push_back(std::make_pair(yystack_[2].value.as< Value > (), yystack_[0].value.as< Value > ())); yylhs.value.as< std::list<std::pair<Value, Value>> > () = yystack_[3].value.as< std::list<std::pair<Value, Value>> > (); }
#line 910 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 11:
#line 214 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< std::list<std::pair<integral_t, Lambda*>> > () = std::list<std::pair<integral_t, Lambda*>>(); }
#line 916 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 12:
#line 215 "parser/impossible.ypp" // lalr1.cc:859
    { yystack_[3].value.as< std::list<std::pair<integral_t, Lambda*>> > ().push_back(std::make_pair(yystack_[2].value.as< Value > ().integral(), yystack_[0].value.as< Value > ().lambda())); yylhs.value.as< std::list<std::pair<integral_t, Lambda*>> > () = yystack_[3].value.as< std::list<std::pair<integral_t, Lambda*>> > (); }
#line 922 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 13:
#line 219 "parser/impossible.ypp" // lalr1.cc:859
    {
    LazyArrayHolder holder(yystack_[1].value.as< Value > ().lambda(), false);
    
    for (const auto& pair : yystack_[2].value.as< std::list<std::pair<integral_t, Lambda*>> > ())
      holder.addMap(pair.first, pair.second);

    yylhs.value.as< Value > () = new LazyArray(holder);
  }
#line 935 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 14:
#line 228 "parser/impossible.ypp" // lalr1.cc:859
    {
    LazyArrayHolder holder(yystack_[1].value.as< Value > ().lambda(), true);
  
    for (const auto& pair : yystack_[2].value.as< std::list<std::pair<integral_t, Lambda*>> > ())
      holder.addMap(pair.first, pair.second);
  
    yylhs.value.as< Value > () = new LazyArray(holder);
  }
#line 948 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 15:
#line 238 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< std::pair<bool, Value> > () = std::make_pair(false, Value()); }
#line 954 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 16:
#line 239 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< std::pair<bool, Value> > () = std::make_pair(false, yystack_[0].value.as< Value > ()); }
#line 960 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 17:
#line 243 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< Value > () = yystack_[0].value.as< Value > (); }
#line 966 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 18:
#line 244 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< Value > () = yystack_[0].value.as< Value > (); }
#line 972 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 19:
#line 245 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< Value > () = yystack_[0].value.as< Value > (); }
#line 978 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 246 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< Value > () = yystack_[0].value.as< Value > (); }
#line 984 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 247 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< Value > () = yystack_[0].value.as< Value > (); }
#line 990 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 22:
#line 248 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< Value > () = yystack_[0].value.as< Value > (); }
#line 996 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 23:
#line 250 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< Value > () = new List(yystack_[1].value.as< std::list<Value> > ()); }
#line 1002 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 24:
#line 251 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< Value > () = new List(); }
#line 1008 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 25:
#line 253 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< Value > () = new Stack(yystack_[1].value.as< std::list<Value> > ()); }
#line 1014 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 254 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< Value > () = new Stack(); }
#line 1020 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 256 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< Value > () = new Queue(yystack_[1].value.as< std::list<Value> > ()); }
#line 1026 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 28:
#line 257 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< Value > () = new Queue(); }
#line 1032 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 259 "parser/impossible.ypp" // lalr1.cc:859
    {
      yylhs.value.as< Value > () = new Set(Set::set_t(yystack_[1].value.as< std::list<Value> > ().begin(), yystack_[1].value.as< std::list<Value> > ().end()));
    }
#line 1040 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 30:
#line 262 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< Value > () = new Set(); }
#line 1046 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 31:
#line 264 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< Value > () = new Map(Map::map_t(yystack_[1].value.as< std::list<std::pair<Value, Value>> > ().begin(), yystack_[1].value.as< std::list<std::pair<Value, Value>> > ().end())); }
#line 1052 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 32:
#line 265 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< Value > () = new Map(); }
#line 1058 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 33:
#line 268 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< Value > () = new Array(Array::array_t(yystack_[1].value.as< std::list<Value> > ().begin(), yystack_[1].value.as< std::list<Value> > ().end())); }
#line 1064 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 34:
#line 269 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< Value > () = yystack_[1].value.as< std::pair<bool, Value> > ().first ? new Array(yystack_[2].value.as< Value > ().integral(), yystack_[1].value.as< std::pair<bool, Value> > ().second) : new Array(yystack_[2].value.as< Value > ().integral()); }
#line 1070 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 35:
#line 270 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< Value > () = new Array(); }
#line 1076 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 272 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< Value > () = yystack_[0].value.as< Value > (); }
#line 1082 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 273 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< Value > () = yystack_[0].value.as< Value > (); }
#line 1088 "parser/parser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 277 "parser/impossible.ypp" // lalr1.cc:859
    { yylhs.value.as< Value > () = new CodeStandard(yystack_[1].value.as< std::vector<Instruction*> > ()); }
#line 1094 "parser/parser.cpp" // lalr1.cc:859
    break;


#line 1098 "parser/parser.cpp" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char Parser::yypact_ninf_ = -59;

  const signed char Parser::yytable_ninf_ = -1;

  const short int
  Parser::yypact_[] =
  {
     -59,    12,    17,   -59,     3,    -4,   -59,   -59,   -59,   -59,
     -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -58,
     -50,    23,   -59,   -59,    13,    29,    35,    41,    49,   119,
       7,    10,   143,   -59,    -7,   -59,   -59,   -59,   -59,   125,
     -59,   131,   -59,   137,   -59,   -59,    14,   143,   -59,    21,
     -59,    -7,    20,    39,   -59,   -59,   -59,   143,   -59,   -59,
      58,    26,   -59,   -59,   -59,   -59
  };

  const unsigned char
  Parser::yydefact_[] =
  {
       3,     0,     2,     1,     0,     0,     3,     6,    17,    18,
      19,    20,    21,    22,     4,    36,     5,    37,    35,     0,
      11,     0,     7,    24,     0,     0,     0,     0,     0,     0,
       7,     0,    15,    11,     0,    33,     8,    32,    28,     0,
      26,     0,    30,     0,    23,    31,     0,     0,    38,     0,
      16,     0,     0,     0,    27,    25,    29,     0,     9,    34,
       0,     0,    13,    10,    14,    12
  };

  const short int
  Parser::yypgoto_[] =
  {
     -59,   -59,    34,   -59,    33,   -59,    37,   -59,   -59,   215,
     -29
  };

  const signed char
  Parser::yydefgoto_[] =
  {
      -1,     1,     2,    14,    21,    29,    34,    15,    49,    36,
      17
  };

  const unsigned char
  Parser::yytable_[] =
  {
       4,     6,     5,    23,     6,    53,    24,     4,    18,     5,
      32,     6,     3,    19,     4,    33,     5,    47,     6,    48,
      37,     4,    60,     5,    57,     6,    59,     4,    35,     5,
      61,     6,    65,     4,     6,     5,    38,     6,    28,     4,
      31,     5,    40,     6,    62,     4,     0,     5,    42,     6,
      25,    26,    27,     4,     0,     5,    44,     6,    39,    41,
      43,    52,    20,    64,     8,     9,    10,    11,    12,    13,
      51,     8,     9,    10,    11,    12,    13,     7,     8,     9,
      10,    11,    12,    13,     7,     8,     9,    10,    11,    12,
      13,     8,     9,    10,    11,    12,    13,     8,     9,    10,
      11,    12,    13,     8,     9,    10,    11,    12,    13,     8,
       9,    10,    11,    12,    13,     0,     0,     8,     9,    10,
      11,    12,    13,     4,     0,     5,    45,     6,     0,     4,
       0,     5,    54,     6,     0,     4,     0,     5,    55,     6,
       0,     4,     0,     5,    56,     6,     0,     4,     0,     5,
       0,     6,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     8,     9,    10,
      11,    12,    13,     8,     9,    10,    11,    12,    13,     8,
       9,    10,    11,    12,    13,     8,     9,    10,    11,    12,
      13,     8,     9,    10,    11,    12,    13,    16,     0,    22,
      30,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,    22,    22,     0,    46,     0,    16,    50,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63
  };

  const signed char
  Parser::yycheck_[] =
  {
       4,     8,     6,     7,     8,    34,    10,     4,     5,     6,
      68,     8,     0,    10,     4,    65,     6,    10,     8,     9,
       7,     4,    51,     6,    10,     8,     5,     4,     5,     6,
      10,     8,    61,     4,     8,     6,     7,     8,     5,     4,
       6,     6,     7,     8,     5,     4,    -1,     6,     7,     8,
      54,    55,    56,     4,    -1,     6,     7,     8,    25,    26,
      27,    68,    59,     5,    68,    69,    70,    71,    72,    73,
      33,    68,    69,    70,    71,    72,    73,    67,    68,    69,
      70,    71,    72,    73,    67,    68,    69,    70,    71,    72,
      73,    68,    69,    70,    71,    72,    73,    68,    69,    70,
      71,    72,    73,    68,    69,    70,    71,    72,    73,    68,
      69,    70,    71,    72,    73,    -1,    -1,    68,    69,    70,
      71,    72,    73,     4,    -1,     6,     7,     8,    -1,     4,
      -1,     6,     7,     8,    -1,     4,    -1,     6,     7,     8,
      -1,     4,    -1,     6,     7,     8,    -1,     4,    -1,     6,
      -1,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    69,    70,
      71,    72,    73,    68,    69,    70,    71,    72,    73,    68,
      69,    70,    71,    72,    73,    68,    69,    70,    71,    72,
      73,    68,    69,    70,    71,    72,    73,     2,    -1,     4,
       5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    -1,    29,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    57
  };

  const unsigned char
  Parser::yystos_[] =
  {
       0,    78,    79,     0,     4,     6,     8,    67,    68,    69,
      70,    71,    72,    73,    80,    84,    86,    87,     5,    10,
      59,    81,    86,     7,    10,    54,    55,    56,    81,    82,
      86,    79,    68,    65,    83,     5,    86,     7,     7,    81,
       7,    81,     7,    81,     7,     7,    86,    10,     9,    85,
      86,    83,    68,    87,     7,     7,     7,    10,    86,     5,
      87,    10,     5,    86,     5,    87
  };

  const unsigned char
  Parser::yyr1_[] =
  {
       0,    77,    78,    79,    79,    80,    80,    81,    81,    82,
      82,    83,    83,    84,    84,    85,    85,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    87
  };

  const unsigned char
  Parser::yyr2_[] =
  {
       0,     2,     1,     0,     2,     1,     1,     1,     2,     3,
       4,     0,     4,     5,     6,     0,     1,     1,     1,     1,
       1,     1,     1,     3,     2,     4,     3,     4,     3,     4,
       3,     3,     3,     3,     5,     2,     1,     1,     3
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\"\\n\"", "\"(\"", "\")\"",
  "\"{\"", "\"}\"", "\"[\"", "\"]\"", "\":\"", "\"$\"", "\"%\"", "\";\"",
  "\",\"", "\"(>\"", "\"<)\"", "\"+\"", "\"-\"", "\"/\"", "\"*\"",
  "\"//\"", "\"&\"", "\"|\"", "\"~\"", "\">>\"", "\"<<\"", "\"_\"",
  "\"+.\"", "\"+:\"", "\"-.\"", "\"-:\"", "\"*.\"", "\"*:\"", "\"/.\"",
  "\"/:\"", "\"@>\"", "\"#>\"", "\"$>\"", "\"@\"", "\"#\"", "\"<@\"",
  "\">@\"", "\"<#\"", "\">#\"", "\".>.\"", "\":>.\"", "\":>:\"", "\".>\"",
  "\".>:\"", "\":*.\"", "\".?\"", "\":?\"", "\"=\"", "\"<\"", "\">\"",
  "\".\"", "\"!\"", "\"!!\"", "\"?\"", "\"??\"", "\"^\"", "\"[#]\"",
  "\"{#}\"", "\"<>\"", "\"i\"", "\"s\"", "OPCODE", "\"integer\"",
  "\"float\"", "\"bool\"", "\"char\"", "\"string\"", "\"range\"",
  "\"lambda\"", "\"larray\"", "\"optional_value\"", "$accept", "start",
  "instructions", "instruction", "values", "value_pairs", "lazy_pairs",
  "lazy_array", "opt_value", "value", "lambda", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  Parser::yyrline_[] =
  {
       0,   190,   190,   194,   195,   199,   200,   204,   205,   209,
     210,   214,   215,   219,   228,   238,   239,   243,   244,   245,
     246,   247,   248,   250,   251,   253,   254,   256,   257,   259,
     262,   264,   265,   268,   269,   270,   272,   273,   277
  };

  // Print the state stack on the debug stream.
  void
  Parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


#line 5 "parser/impossible.ypp" // lalr1.cc:1167
} // compiler
#line 1556 "parser/parser.cpp" // lalr1.cc:1167
#line 280 "parser/impossible.ypp" // lalr1.cc:1168


void compiler::Parser::error(const location_type& l, const std::string& m)
{
  compiler.error(l,m);
}
