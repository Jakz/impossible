// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton interface for Bison LALR(1) parsers in C++

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

/**
 ** \file parser/parser.hpp
 ** Define the compiler::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

#ifndef YY_YY_PARSER_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_PARSER_HPP_INCLUDED
// //                    "%code requires" blocks.
#line 11 "parser/impossible.ypp" // lalr1.cc:392

  namespace compiler
  {
    class Compiler;
    class Lexer;
  }

#line 52 "parser/parser.hpp" // lalr1.cc:392


# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>
# include "stack.hh"
# include "location.hh"

#ifndef YYASSERT
# include <cassert>
# define YYASSERT assert
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

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

#line 5 "parser/impossible.ypp" // lalr1.cc:392
namespace compiler {
#line 129 "parser/parser.hpp" // lalr1.cc:392



  /// A char[S] buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current state.
  template <size_t S>
  struct variant
  {
    /// Type of *this.
    typedef variant<S> self_type;

    /// Empty construction.
    variant ()
    {}

    /// Construct and fill.
    template <typename T>
    variant (const T& t)
    {
      YYASSERT (sizeof (T) <= S);
      new (yyas_<T> ()) T (t);
    }

    /// Destruction, allowed only if empty.
    ~variant ()
    {}

    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    build ()
    {
      return *new (yyas_<T> ()) T;
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    build (const T& t)
    {
      return *new (yyas_<T> ()) T (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as ()
    {
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const
    {
      return *yyas_<T> ();
    }

    /// Swap the content with \a other, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsability.
    /// Swapping between built and (possibly) non-built is done with
    /// variant::move ().
    template <typename T>
    void
    swap (self_type& other)
    {
      std::swap (as<T> (), other.as<T> ());
    }

    /// Move the content of \a other to this.
    ///
    /// Destroys \a other.
    template <typename T>
    void
    move (self_type& other)
    {
      build<T> ();
      swap<T> (other);
      other.destroy<T> ();
    }

    /// Copy the content of \a other to this.
    template <typename T>
    void
    copy (const self_type& other)
    {
      build<T> (other.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
    }

  private:
    /// Prohibit blind copies.
    self_type& operator=(const self_type&);
    variant (const self_type&);

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ ()
    {
      void *yyp = yybuffer_.yyraw;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const
    {
      const void *yyp = yybuffer_.yyraw;
      return static_cast<const T*> (yyp);
     }

    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me;
      /// A buffer large enough to store any of the semantic values.
      char yyraw[S];
    } yybuffer_;
  };


  /// A Bison parser.
  class Parser
  {
  public:
#ifndef YYSTYPE
    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // start
      char dummy1[sizeof(Code*)];

      // instruction
      char dummy2[sizeof(Instruction*)];

      // "$"
      // "%"
      // ";"
      // ","
      // "(>"
      // "<)"
      // "+"
      // "-"
      // "/"
      // "*"
      // "//"
      // "&"
      // "|"
      // "~"
      // ">>"
      // "<<"
      // "_"
      // "+."
      // "+:"
      // "-."
      // "-:"
      // "*."
      // "*:"
      // "/."
      // "/:"
      // "@>"
      // "#>"
      // "$>"
      // "@"
      // "#"
      // "<@"
      // ">@"
      // "<#"
      // ">#"
      // ".>."
      // ":>."
      // ":>:"
      // ".>"
      // ".>:"
      // ":*."
      // ".?"
      // ":?"
      // "="
      // "<"
      // ">"
      // "."
      // "!"
      // "!!"
      // "?"
      // "??"
      // "^"
      // "[#]"
      // "{#}"
      // "<>"
      // "i"
      // "s"
      // OPCODE
      char dummy3[sizeof(Opcode)];

      // "integer"
      // "float"
      // "bool"
      // "char"
      // "string"
      // "range"
      // "lambda"
      // "larray"
      // lazy_array
      // value
      // lambda
      char dummy4[sizeof(Value)];

      // values
      char dummy5[sizeof(std::list<Value>)];

      // value_pairs
      char dummy6[sizeof(std::list<std::pair<Value, Value>>)];

      // lazy_pairs
      char dummy7[sizeof(std::list<std::pair<integral_t, Lambda*>>)];

      // "optional_value"
      // opt_value
      char dummy8[sizeof(std::pair<bool, Value>)];

      // instructions
      char dummy9[sizeof(std::vector<Instruction*>)];
};

    /// Symbol semantic values.
    typedef variant<sizeof(union_type)> semantic_type;
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m);
      location_type location;
    };

    /// Tokens.
    struct token
    {
      enum yytokentype
      {
        T_END = 0,
        T_EOL = 258,
        T_LPAREN = 259,
        T_RPAREN = 260,
        T_LBRACE = 261,
        T_RBRACE = 262,
        T_LBRACK = 263,
        T_RBRACK = 264,
        T_COLON = 265,
        T_DOLLAR = 266,
        T_PERCENT = 267,
        T_SEMICOL = 268,
        T_COMMA = 269,
        T_RISE = 270,
        T_SINK = 271,
        T_PLUS = 272,
        T_MINUS = 273,
        T_DIVIDE = 274,
        T_TIMES = 275,
        T_DDIVIDE = 276,
        T_AND = 277,
        T_OR = 278,
        T_NOT = 279,
        T_RSHIFT = 280,
        T_LSHIFT = 281,
        T_UNDERSCORE = 282,
        T_PLUS_MON = 283,
        T_PLUS_DIA = 284,
        T_MINUS_MON = 285,
        T_MINUS_DIA = 286,
        T_TIMES_MON = 287,
        T_TIMES_DIA = 288,
        T_DIVIDE_MON = 289,
        T_DIVIDE_DIA = 290,
        T_RAND = 291,
        T_SORT = 292,
        T_SHUFFLE = 293,
        T_AT = 294,
        T_HASH = 295,
        T_AT_FRONT = 296,
        T_AT_BACK = 297,
        T_HASH_FRONT = 298,
        T_HASH_BACK = 299,
        T_MAP = 300,
        T_FOLD = 301,
        T_DMAP = 302,
        T_ITER = 303,
        T_ITERI = 304,
        T_CARTESIAN = 305,
        T_ANY = 306,
        T_EVERY = 307,
        T_EQUAL = 308,
        T_LESSER = 309,
        T_GREATER = 310,
        T_PERIOD = 311,
        T_BANG = 312,
        T_DBANG = 313,
        T_QUESTION = 314,
        T_DQUESTION = 315,
        T_PEEK = 316,
        T_RECUR = 317,
        T_RECURV = 318,
        T_WHILE = 319,
        T_I = 320,
        T_S = 321,
        T_OPCODE = 322,
        T_INTEGER = 323,
        T_FLOAT = 324,
        T_BOOLEAN = 325,
        T_CHARACTER = 326,
        T_STRING = 327,
        T_RANGE = 328
      };
    };

    /// (External) token type, as returned by yylex.
    typedef token::yytokentype token_type;

    /// Symbol type: an internal symbol number.
    typedef int symbol_number_type;

    /// The symbol type number to denote an empty symbol.
    enum { empty_symbol = -2 };

    /// Internal symbol number for tokens (subsumed by symbol_number_type).
    typedef unsigned char token_number_type;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol type
    /// via type_get().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol ();

      /// Copy constructor.
      basic_symbol (const basic_symbol& other);

      /// Constructor for valueless symbols, and symbols from each type.

  basic_symbol (typename Base::kind_type t, const location_type& l);

  basic_symbol (typename Base::kind_type t, const Code* v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const Instruction* v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const Opcode v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const Value v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::list<Value> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::list<std::pair<Value, Value>> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::list<std::pair<integral_t, Lambda*>> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::pair<bool, Value> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::vector<Instruction*> v, const location_type& l);


      /// Constructor for symbols with semantic value.
      basic_symbol (typename Base::kind_type t,
                    const semantic_type& v,
                    const location_type& l);

      /// Destroy the symbol.
      ~basic_symbol ();

      /// Destroy contents, and record that is empty.
      void clear ();

      /// Whether empty.
      bool empty () const;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      semantic_type value;

      /// The location.
      location_type location;

    private:
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& other);
    };

    /// Type access provider for token (enum) based symbols.
    struct by_type
    {
      /// Default constructor.
      by_type ();

      /// Copy constructor.
      by_type (const by_type& other);

      /// The symbol type as needed by the constructor.
      typedef token_type kind_type;

      /// Constructor from (external) token numbers.
      by_type (kind_type t);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_type& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_number_type type_get () const;

      /// The token.
      token_type token () const;

      /// The symbol type.
      /// \a empty_symbol when empty.
      /// An int, not token_number_type, to be able to store empty_symbol.
      int type;
    };

    /// "External" symbols: returned by the scanner.
    typedef basic_symbol<by_type> symbol_type;

    // Symbol constructors declarations.
    static inline
    symbol_type
    make_END (const location_type& l);

    static inline
    symbol_type
    make_EOL (const location_type& l);

    static inline
    symbol_type
    make_LPAREN (const location_type& l);

    static inline
    symbol_type
    make_RPAREN (const location_type& l);

    static inline
    symbol_type
    make_LBRACE (const location_type& l);

    static inline
    symbol_type
    make_RBRACE (const location_type& l);

    static inline
    symbol_type
    make_LBRACK (const location_type& l);

    static inline
    symbol_type
    make_RBRACK (const location_type& l);

    static inline
    symbol_type
    make_COLON (const location_type& l);

    static inline
    symbol_type
    make_DOLLAR (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_PERCENT (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_SEMICOL (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_COMMA (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_RISE (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_SINK (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_PLUS (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_MINUS (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_DIVIDE (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_TIMES (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_DDIVIDE (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_AND (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_OR (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_NOT (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_RSHIFT (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_LSHIFT (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_UNDERSCORE (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_PLUS_MON (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_PLUS_DIA (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_MINUS_MON (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_MINUS_DIA (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_TIMES_MON (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_TIMES_DIA (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_DIVIDE_MON (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_DIVIDE_DIA (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_RAND (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_SORT (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_SHUFFLE (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_AT (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_HASH (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_AT_FRONT (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_AT_BACK (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_HASH_FRONT (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_HASH_BACK (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_MAP (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_FOLD (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_DMAP (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_ITER (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_ITERI (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_CARTESIAN (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_ANY (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_EVERY (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_EQUAL (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_LESSER (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_GREATER (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_PERIOD (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_BANG (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_DBANG (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_QUESTION (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_DQUESTION (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_PEEK (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_RECUR (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_RECURV (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_WHILE (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_I (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_S (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_OPCODE (const Opcode& v, const location_type& l);

    static inline
    symbol_type
    make_INTEGER (const Value& v, const location_type& l);

    static inline
    symbol_type
    make_FLOAT (const Value& v, const location_type& l);

    static inline
    symbol_type
    make_BOOLEAN (const Value& v, const location_type& l);

    static inline
    symbol_type
    make_CHARACTER (const Value& v, const location_type& l);

    static inline
    symbol_type
    make_STRING (const Value& v, const location_type& l);

    static inline
    symbol_type
    make_RANGE (const Value& v, const location_type& l);


    /// Build a parser object.
    Parser (compiler::Lexer &lexer_yyarg, compiler::Compiler &compiler_yyarg);
    virtual ~Parser ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

  private:
    /// This class is not copyable.
    Parser (const Parser&);
    Parser& operator= (const Parser&);

    /// State numbers.
    typedef int state_type;

    /// Generate an error message.
    /// \param yystate   the state where the error occurred.
    /// \param yyla      the lookahead token.
    virtual std::string yysyntax_error_ (state_type yystate,
                                         const symbol_type& yyla) const;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    static const signed char yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token number \a t to a symbol number.
    static token_number_type yytranslate_ (token_type t);

    // Tables.
  // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
  // STATE-NUM.
  static const short int yypact_[];

  // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
  // Performed when YYTABLE does not specify something else to do.  Zero
  // means the default is an error.
  static const unsigned char yydefact_[];

  // YYPGOTO[NTERM-NUM].
  static const short int yypgoto_[];

  // YYDEFGOTO[NTERM-NUM].
  static const signed char yydefgoto_[];

  // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
  // positive, shift that token.  If negative, reduce the rule whose
  // number is the opposite.  If YYTABLE_NINF, syntax error.
  static const unsigned char yytable_[];

  static const signed char yycheck_[];

  // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
  // symbol of state STATE-NUM.
  static const unsigned char yystos_[];

  // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
  static const unsigned char yyr1_[];

  // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
  static const unsigned char yyr2_[];


    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *n);


    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#if YYDEBUG
  // YYRLINE[YYN] -- Source line where rule number YYN was defined.
  static const unsigned short int yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    // Debugging.
    int yydebug_;
    std::ostream* yycdebug_;

    /// \brief Display a symbol type, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state ();

      /// The symbol type as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s);

      /// Copy constructor.
      by_state (const by_state& other);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_state& that);

      /// The (internal) type number (corresponding to \a state).
      /// \a empty_symbol when empty.
      symbol_number_type type_get () const;

      /// The state number used to denote an empty symbol.
      enum { empty_state = -1 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, symbol_type& sym);
      /// Assignment, needed by push_back.
      stack_symbol_type& operator= (const stack_symbol_type& that);
    };

    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, stack_symbol_type& s);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, state_type s, symbol_type& sym);

    /// Pop \a n symbols the three stacks.
    void yypop_ (unsigned int n = 1);

    /// Constants.
    enum
    {
      yyeof_ = 0,
      yylast_ = 272,     ///< Last index in yytable_.
      yynnts_ = 11,  ///< Number of nonterminal symbols.
      yyfinal_ = 3, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 77  ///< Number of tokens.
    };


    // User arguments.
    compiler::Lexer &lexer;
    compiler::Compiler &compiler;
  };

  // Symbol number corresponding to token number t.
  inline
  Parser::token_number_type
  Parser::yytranslate_ (token_type t)
  {
    static
    const token_number_type
    translate_table[] =
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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76
    };
    const unsigned int user_token_number_max_ = 331;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

  inline
  Parser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
      switch (other.type_get ())
    {
      case 78: // start
        value.copy< Code* > (other.value);
        break;

      case 80: // instruction
        value.copy< Instruction* > (other.value);
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
        value.copy< Opcode > (other.value);
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
        value.copy< Value > (other.value);
        break;

      case 81: // values
        value.copy< std::list<Value> > (other.value);
        break;

      case 82: // value_pairs
        value.copy< std::list<std::pair<Value, Value>> > (other.value);
        break;

      case 83: // lazy_pairs
        value.copy< std::list<std::pair<integral_t, Lambda*>> > (other.value);
        break;

      case 76: // "optional_value"
      case 85: // opt_value
        value.copy< std::pair<bool, Value> > (other.value);
        break;

      case 79: // instructions
        value.copy< std::vector<Instruction*> > (other.value);
        break;

      default:
        break;
    }

  }


  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {
    (void) v;
      switch (this->type_get ())
    {
      case 78: // start
        value.copy< Code* > (v);
        break;

      case 80: // instruction
        value.copy< Instruction* > (v);
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
        value.copy< Opcode > (v);
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
        value.copy< Value > (v);
        break;

      case 81: // values
        value.copy< std::list<Value> > (v);
        break;

      case 82: // value_pairs
        value.copy< std::list<std::pair<Value, Value>> > (v);
        break;

      case 83: // lazy_pairs
        value.copy< std::list<std::pair<integral_t, Lambda*>> > (v);
        break;

      case 76: // "optional_value"
      case 85: // opt_value
        value.copy< std::pair<bool, Value> > (v);
        break;

      case 79: // instructions
        value.copy< std::vector<Instruction*> > (v);
        break;

      default:
        break;
    }
}


  // Implementation of basic_symbol constructor for each type.

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const Code* v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const Instruction* v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const Opcode v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const Value v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::list<Value> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::list<std::pair<Value, Value>> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::list<std::pair<integral_t, Lambda*>> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::pair<bool, Value> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::vector<Instruction*> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  template <typename Base>
  inline
  Parser::basic_symbol<Base>::~basic_symbol ()
  {
    clear ();
  }

  template <typename Base>
  inline
  void
  Parser::basic_symbol<Base>::clear ()
  {
    // User destructor.
    symbol_number_type yytype = this->type_get ();
    basic_symbol<Base>& yysym = *this;
    (void) yysym;
    switch (yytype)
    {
   default:
      break;
    }

    // Type destructor.
    switch (yytype)
    {
      case 78: // start
        value.template destroy< Code* > ();
        break;

      case 80: // instruction
        value.template destroy< Instruction* > ();
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
        value.template destroy< Opcode > ();
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
        value.template destroy< Value > ();
        break;

      case 81: // values
        value.template destroy< std::list<Value> > ();
        break;

      case 82: // value_pairs
        value.template destroy< std::list<std::pair<Value, Value>> > ();
        break;

      case 83: // lazy_pairs
        value.template destroy< std::list<std::pair<integral_t, Lambda*>> > ();
        break;

      case 76: // "optional_value"
      case 85: // opt_value
        value.template destroy< std::pair<bool, Value> > ();
        break;

      case 79: // instructions
        value.template destroy< std::vector<Instruction*> > ();
        break;

      default:
        break;
    }

    Base::clear ();
  }

  template <typename Base>
  inline
  bool
  Parser::basic_symbol<Base>::empty () const
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  inline
  void
  Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
      switch (this->type_get ())
    {
      case 78: // start
        value.move< Code* > (s.value);
        break;

      case 80: // instruction
        value.move< Instruction* > (s.value);
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
        value.move< Opcode > (s.value);
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
        value.move< Value > (s.value);
        break;

      case 81: // values
        value.move< std::list<Value> > (s.value);
        break;

      case 82: // value_pairs
        value.move< std::list<std::pair<Value, Value>> > (s.value);
        break;

      case 83: // lazy_pairs
        value.move< std::list<std::pair<integral_t, Lambda*>> > (s.value);
        break;

      case 76: // "optional_value"
      case 85: // opt_value
        value.move< std::pair<bool, Value> > (s.value);
        break;

      case 79: // instructions
        value.move< std::vector<Instruction*> > (s.value);
        break;

      default:
        break;
    }

    location = s.location;
  }

  // by_type.
  inline
  Parser::by_type::by_type ()
    : type (empty_symbol)
  {}

  inline
  Parser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  Parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  Parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
  Parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
  Parser::by_type::type_get () const
  {
    return type;
  }

  inline
  Parser::token_type
  Parser::by_type::token () const
  {
    // YYTOKNUM[NUM] -- (External) token number corresponding to the
    // (internal) symbol number NUM (which must be that of a token).  */
    static
    const unsigned short int
    yytoken_number_[] =
    {
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331
    };
    return static_cast<token_type> (yytoken_number_[type]);
  }
  // Implementation of make_symbol for each symbol type.
  Parser::symbol_type
  Parser::make_END (const location_type& l)
  {
    return symbol_type (token::T_END, l);
  }

  Parser::symbol_type
  Parser::make_EOL (const location_type& l)
  {
    return symbol_type (token::T_EOL, l);
  }

  Parser::symbol_type
  Parser::make_LPAREN (const location_type& l)
  {
    return symbol_type (token::T_LPAREN, l);
  }

  Parser::symbol_type
  Parser::make_RPAREN (const location_type& l)
  {
    return symbol_type (token::T_RPAREN, l);
  }

  Parser::symbol_type
  Parser::make_LBRACE (const location_type& l)
  {
    return symbol_type (token::T_LBRACE, l);
  }

  Parser::symbol_type
  Parser::make_RBRACE (const location_type& l)
  {
    return symbol_type (token::T_RBRACE, l);
  }

  Parser::symbol_type
  Parser::make_LBRACK (const location_type& l)
  {
    return symbol_type (token::T_LBRACK, l);
  }

  Parser::symbol_type
  Parser::make_RBRACK (const location_type& l)
  {
    return symbol_type (token::T_RBRACK, l);
  }

  Parser::symbol_type
  Parser::make_COLON (const location_type& l)
  {
    return symbol_type (token::T_COLON, l);
  }

  Parser::symbol_type
  Parser::make_DOLLAR (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_DOLLAR, v, l);
  }

  Parser::symbol_type
  Parser::make_PERCENT (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_PERCENT, v, l);
  }

  Parser::symbol_type
  Parser::make_SEMICOL (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_SEMICOL, v, l);
  }

  Parser::symbol_type
  Parser::make_COMMA (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_COMMA, v, l);
  }

  Parser::symbol_type
  Parser::make_RISE (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_RISE, v, l);
  }

  Parser::symbol_type
  Parser::make_SINK (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_SINK, v, l);
  }

  Parser::symbol_type
  Parser::make_PLUS (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_PLUS, v, l);
  }

  Parser::symbol_type
  Parser::make_MINUS (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_MINUS, v, l);
  }

  Parser::symbol_type
  Parser::make_DIVIDE (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_DIVIDE, v, l);
  }

  Parser::symbol_type
  Parser::make_TIMES (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_TIMES, v, l);
  }

  Parser::symbol_type
  Parser::make_DDIVIDE (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_DDIVIDE, v, l);
  }

  Parser::symbol_type
  Parser::make_AND (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_AND, v, l);
  }

  Parser::symbol_type
  Parser::make_OR (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_OR, v, l);
  }

  Parser::symbol_type
  Parser::make_NOT (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_NOT, v, l);
  }

  Parser::symbol_type
  Parser::make_RSHIFT (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_RSHIFT, v, l);
  }

  Parser::symbol_type
  Parser::make_LSHIFT (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_LSHIFT, v, l);
  }

  Parser::symbol_type
  Parser::make_UNDERSCORE (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_UNDERSCORE, v, l);
  }

  Parser::symbol_type
  Parser::make_PLUS_MON (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_PLUS_MON, v, l);
  }

  Parser::symbol_type
  Parser::make_PLUS_DIA (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_PLUS_DIA, v, l);
  }

  Parser::symbol_type
  Parser::make_MINUS_MON (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_MINUS_MON, v, l);
  }

  Parser::symbol_type
  Parser::make_MINUS_DIA (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_MINUS_DIA, v, l);
  }

  Parser::symbol_type
  Parser::make_TIMES_MON (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_TIMES_MON, v, l);
  }

  Parser::symbol_type
  Parser::make_TIMES_DIA (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_TIMES_DIA, v, l);
  }

  Parser::symbol_type
  Parser::make_DIVIDE_MON (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_DIVIDE_MON, v, l);
  }

  Parser::symbol_type
  Parser::make_DIVIDE_DIA (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_DIVIDE_DIA, v, l);
  }

  Parser::symbol_type
  Parser::make_RAND (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_RAND, v, l);
  }

  Parser::symbol_type
  Parser::make_SORT (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_SORT, v, l);
  }

  Parser::symbol_type
  Parser::make_SHUFFLE (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_SHUFFLE, v, l);
  }

  Parser::symbol_type
  Parser::make_AT (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_AT, v, l);
  }

  Parser::symbol_type
  Parser::make_HASH (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_HASH, v, l);
  }

  Parser::symbol_type
  Parser::make_AT_FRONT (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_AT_FRONT, v, l);
  }

  Parser::symbol_type
  Parser::make_AT_BACK (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_AT_BACK, v, l);
  }

  Parser::symbol_type
  Parser::make_HASH_FRONT (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_HASH_FRONT, v, l);
  }

  Parser::symbol_type
  Parser::make_HASH_BACK (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_HASH_BACK, v, l);
  }

  Parser::symbol_type
  Parser::make_MAP (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_MAP, v, l);
  }

  Parser::symbol_type
  Parser::make_FOLD (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_FOLD, v, l);
  }

  Parser::symbol_type
  Parser::make_DMAP (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_DMAP, v, l);
  }

  Parser::symbol_type
  Parser::make_ITER (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_ITER, v, l);
  }

  Parser::symbol_type
  Parser::make_ITERI (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_ITERI, v, l);
  }

  Parser::symbol_type
  Parser::make_CARTESIAN (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_CARTESIAN, v, l);
  }

  Parser::symbol_type
  Parser::make_ANY (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_ANY, v, l);
  }

  Parser::symbol_type
  Parser::make_EVERY (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_EVERY, v, l);
  }

  Parser::symbol_type
  Parser::make_EQUAL (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_EQUAL, v, l);
  }

  Parser::symbol_type
  Parser::make_LESSER (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_LESSER, v, l);
  }

  Parser::symbol_type
  Parser::make_GREATER (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_GREATER, v, l);
  }

  Parser::symbol_type
  Parser::make_PERIOD (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_PERIOD, v, l);
  }

  Parser::symbol_type
  Parser::make_BANG (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_BANG, v, l);
  }

  Parser::symbol_type
  Parser::make_DBANG (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_DBANG, v, l);
  }

  Parser::symbol_type
  Parser::make_QUESTION (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_QUESTION, v, l);
  }

  Parser::symbol_type
  Parser::make_DQUESTION (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_DQUESTION, v, l);
  }

  Parser::symbol_type
  Parser::make_PEEK (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_PEEK, v, l);
  }

  Parser::symbol_type
  Parser::make_RECUR (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_RECUR, v, l);
  }

  Parser::symbol_type
  Parser::make_RECURV (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_RECURV, v, l);
  }

  Parser::symbol_type
  Parser::make_WHILE (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_WHILE, v, l);
  }

  Parser::symbol_type
  Parser::make_I (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_I, v, l);
  }

  Parser::symbol_type
  Parser::make_S (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_S, v, l);
  }

  Parser::symbol_type
  Parser::make_OPCODE (const Opcode& v, const location_type& l)
  {
    return symbol_type (token::T_OPCODE, v, l);
  }

  Parser::symbol_type
  Parser::make_INTEGER (const Value& v, const location_type& l)
  {
    return symbol_type (token::T_INTEGER, v, l);
  }

  Parser::symbol_type
  Parser::make_FLOAT (const Value& v, const location_type& l)
  {
    return symbol_type (token::T_FLOAT, v, l);
  }

  Parser::symbol_type
  Parser::make_BOOLEAN (const Value& v, const location_type& l)
  {
    return symbol_type (token::T_BOOLEAN, v, l);
  }

  Parser::symbol_type
  Parser::make_CHARACTER (const Value& v, const location_type& l)
  {
    return symbol_type (token::T_CHARACTER, v, l);
  }

  Parser::symbol_type
  Parser::make_STRING (const Value& v, const location_type& l)
  {
    return symbol_type (token::T_STRING, v, l);
  }

  Parser::symbol_type
  Parser::make_RANGE (const Value& v, const location_type& l)
  {
    return symbol_type (token::T_RANGE, v, l);
  }


#line 5 "parser/impossible.ypp" // lalr1.cc:392
} // compiler
#line 2227 "parser/parser.hpp" // lalr1.cc:392




#endif // !YY_YY_PARSER_PARSER_HPP_INCLUDED
